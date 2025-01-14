import ctypes
import numpy as np
import matplotlib.pyplot as plt

lib = ctypes.CDLL('/home/sheshaoya/stl/robust-fpop/build/liball.so')
# 定义 C 函数的参数和返回类型
lib.rob_fpop_RtoC.argtypes = [
    ctypes.POINTER(ctypes.c_double),  # signal
    ctypes.POINTER(ctypes.c_int),     # n
    ctypes.POINTER(ctypes.c_double),  # lambda_
    ctypes.POINTER(ctypes.c_double),  # lthreshold
    ctypes.POINTER(ctypes.c_double),  # rthreshold
    ctypes.POINTER(ctypes.c_double),  # lslope
    ctypes.POINTER(ctypes.c_double),  # rslope
    ctypes.POINTER(ctypes.c_double),  # mini
    ctypes.POINTER(ctypes.c_double),  # maxi
    ctypes.POINTER(ctypes.c_int),     # path
    ctypes.POINTER(ctypes.c_double),  # cost
    ctypes.POINTER(ctypes.c_double)   # mean
]
lib.rob_fpop_RtoC.restype = None

def multi_bin_seg(geno, Kmax):
    # 检查输入类型
    if isinstance(geno, np.ndarray) and geno.ndim == 2:
        nRow, nCol = geno.shape
    else:
        nRow = len(geno)
        nCol = 1
        geno = np.array(geno, dtype=np.double).reshape(-1, 1)
    
    # 准备参数
    x_i = geno.astype(np.double).flatten()
    K = ctypes.c_int(Kmax)
    n = ctypes.c_int(nRow)
    P = ctypes.c_int(nCol)
    t_est = (ctypes.c_int * Kmax)()
    J_est = (ctypes.c_double * Kmax)()
    
    # 调用 C 函数
    lib.Call_BinSeg(
        x_i.ctypes.data_as(ctypes.POINTER(ctypes.c_double)),
        ctypes.byref(K),
        ctypes.byref(n),
        ctypes.byref(P),
        t_est,
        J_est
    )
    # 返回结果
    return np.array(t_est)

def rob_seg_std(x, loss="L1", lambda_=1.0, lthreshold=1.345):
    if loss == "L1":
        res = rob_seg(x, lambda_=lambda_, lthreshold=0, lslope=-1)
    elif loss == "L2":
        res = rob_seg(x, lambda_=lambda_, lthreshold=np.diff(np.ptp(x)))
    elif loss == "Huber":
        res = rob_seg(x, lambda_=lambda_, lthreshold=lthreshold, lslope=-2*lthreshold)
    elif loss == "Outlier":
        res = rob_seg(x, lambda_=lambda_, lthreshold=lthreshold)
    else:
        raise ValueError("Unknown loss function")
    return res

def getPath(path, n):

    chaine = [len(path)]
    j = 1
    while chaine[j-1] > 0:
        chaine.append(path[chaine[j-1] - 1])  # C arrays are 0-indexed
        j += 1
    return chaine[::-1][1:]

def rob_seg(x, lambda_, lthreshold, rthreshold=None, lslope=0, rslope=None, mini=None, maxi=None):
    
    n = len(x)
    if rthreshold is None:
        rthreshold = lthreshold
    if rslope is None:
        rslope = -lslope
    if mini is None:
        mini = min(x)
    if maxi is None:
        maxi = max(x)

    # Convert to ctypes arrays
    signal = (ctypes.c_double * n)(*x)
    n_ = ctypes.c_int(n)
    lambda_ = ctypes.c_double(lambda_)
    lthreshold = ctypes.c_double(lthreshold)
    rthreshold = ctypes.c_double(rthreshold)
    lslope = ctypes.c_double(lslope)
    rslope = ctypes.c_double(rslope)
    mini = ctypes.c_double(mini)
    maxi = ctypes.c_double(maxi)
    path = (ctypes.c_int * n)()
    cost = (ctypes.c_double * n)()
    mean = (ctypes.c_double * n)()

    # Call the C function
    lib.rob_fpop_RtoC(signal, ctypes.byref(n_), ctypes.byref(lambda_), 
                       ctypes.byref(lthreshold), ctypes.byref(rthreshold), 
                       ctypes.byref(lslope), ctypes.byref(rslope), 
                       ctypes.byref(mini), ctypes.byref(maxi), 
                       path, cost, mean)

    # Convert ctypes arrays to numpy arrays
    path_np = np.array(path)
    cost_np = np.array(cost)
    mean_np = np.array(mean)

    # Get the change points
    t_est = getPath(path_np, n)
    smt = np.repeat(mean_np[np.array(t_est) - 1], np.diff(np.concatenate(([0], t_est))))
    rthreshold_val = rthreshold.value
    lthreshold_val = lthreshold.value
    out = ((x - smt) > rthreshold_val) | ((x - smt) < -lthreshold_val)

    result = {
        't.est': t_est,
        'smt': smt,
        'out': out,
        'K': len(t_est)
    }
    return result

def fpop_intern(x, test_stat="Normal", pen_value=None, lthreshold=None):
    if lthreshold is None:
        lthreshold = {
            "Laplace": None,
            "Normal": None,
            "Huber": 1.345,
            "Outlier": 3
        }.get(test_stat, None)

    tmp_res = {
        "Laplace": lambda: rob_seg(x, lambda_=pen_value, lthreshold=0, lslope=-1),
        "Normal": lambda: rob_seg(x, lambda_=pen_value, lthreshold=np.ptp(x)),
        "Huber": lambda: rob_seg(x, lambda_=pen_value, lthreshold=lthreshold, lslope=-2*lthreshold),
        "Outlier": lambda: rob_seg(x, lambda_=pen_value, lthreshold=lthreshold)
    }.get(test_stat, lambda: None)()

    tmp_res['method'] = "rob.fpop"
    tmp_res['test_stat'] = test_stat
    tmp_res['minseglen'] = 1

    link_param = [
        ("signal", "data", "keep"),
        ("n", "n", "discard"),
        ("lambda", "pen_value", "keep"),
        ("lthreshold", "lthreshold", "keep"),
        ("rthreshold", "rthreshold", "keep"),
        ("lslope", "lslope", "keep"),
        ("rslope", "rslope", "keep"),
        ("min", "min", "discard"),
        ("max", "max", "discard"),
        ("path", "path", "discard"),
        ("cost", "cost", "keep"),
        ("mean", "mean_path", "discard"),
        ("t_est", "cpts", "keep"),
        ("smt", "smt_signal", "keep"),
        ("out", "out_position", "keep"),
        ("K", "nb_seg", "keep"),
        ("method", "method", "keep"),
        ("test_stat", "test_stat", "keep"),
        ("minseglen", "minseglen", "keep")
    ]

    to_keep = [param[1] for param in link_param if param[2] == "keep"]
    tmp_res = {param[1]: tmp_res[param[0]] for param in link_param if param[2] == "keep"}

    return tmp_res

### Variance estimation using MAD or HALL
def var_diff(x, method='MAD'):
    n = len(x)
    if method == "MAD":
        return np.median(np.abs(np.diff(x) / np.sqrt(2)))
    elif method == "HALL":
        wei = np.array([0.1942, 0.2809, 0.3832, -0.8582])
        mat = np.outer(wei, x)
        mat[1, :-1] = mat[1, 1:]
        mat[2, :-2] = mat[2, 2:]
        mat[3, :-3] = mat[3, 3:]
        return np.sqrt(np.sum(np.sum(mat[:, :-3], axis=0) ** 2) / (n - 3))
    else:
        raise ValueError("Unknown method. Use 'MAD' or 'HALL'.")

# Example usage
if __name__ == "__main__":
    np.random.seed(0)
    x = np.concatenate([np.random.normal(0, 1, 100), np.random.normal(2, 1, 100)])
    std_dev = np.median(np.abs(np.diff(x) / np.sqrt(2)))
    x_ = x / std_dev
    lambda_ = np.log(len(x))

    res_l1 = fpop_intern(x_, test_stat="Laplace", pen_value=lambda_)
    res_l2 = fpop_intern(x_, test_stat="Normal", pen_value=2 * lambda_)
    res_hu = fpop_intern(x_, test_stat="Huber", pen_value=1.4 * lambda_, lthreshold=1.345)
    res_ou = fpop_intern(x_, test_stat="Outlier", pen_value=2 * lambda_, lthreshold=3)


    plt.plot(x_, marker='o', linestyle='-', markersize=3)
    plt.plot(res_l1['smt.signal'], label='Laplace', linestyle='--')
    plt.plot(res_l2['smt.signal'], label='Normal', linestyle='--')
    plt.plot(res_hu['smt.signal'], label='Huber', linestyle='--')
    plt.plot(res_ou['smt.signal'], label='Outlier', linestyle='--')
    plt.legend()
    plt.show()

    res_l1 = rob_seg_std(x_, "L1", lambda_=lambda_)
    res_l2 = rob_seg_std(x_, "L2", lambda_=2*lambda_)
    res_hu = rob_seg_std(x_, "Huber", lambda_=1.4*lambda_, lthreshold=1.345)
    res_ou = rob_seg_std(x_, "Outlier", lambda_=2*lambda_, lthreshold=3)

    plt.plot(x_, 'o', label='Data')
    plt.plot(res_l1['smt'], '--', label='L1')
    plt.plot(res_l2['smt'], '--', label='L2')
    plt.plot(res_hu['smt'], '--', label='Huber')
    plt.plot(res_ou['smt'], '--', label='Outlier')
    plt.legend()
    plt.show()