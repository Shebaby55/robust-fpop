import numpy as np
import pickle
import matplotlib.pyplot as plt
from scipy.stats import t
import os
from functions import rob_seg_std, var_diff

# import debugpy

# # Allow other computers to attach to ptvsd at this IP address and port.
# debugpy.listen(('localhost', 45678))

# # Pause the program until a remote debugger is attached
# print("Waiting for debugger attach...")
# debugpy.wait_for_client()
# print("Debugger attached.")
# Load the Simu list from the Simulation.py file
with open(os.path.join(os.getcwd(), 'py', 'Simu.pkl'), 'rb') as f:
    Simu = pickle.load(f)

# Simulate one profile
i = 1  # there are 6 scenarios, we take the first one
dfree = 6  # degree of freedom of the Student noise

# We recover the info of the first scenario
Ktrue = Simu[i-1]['Ktrue']
bkptrue = Simu[i-1]['bkpPage29'][1:Ktrue+1] - 1  # Convert to 0-based index
signaltrue = Simu[i-1]['signal']
sigmatrue = Simu[i-1]['sigma']

# We simulate one profile
np.random.seed(1)
x_data = signaltrue + t.rvs(dfree, size=len(signaltrue)) * sigmatrue

# Estimate the variance using MAD
est_sd = var_diff(x_data)

# Robust Fpop (Outlier)
# Run dynamic programming
res_ou = rob_seg_std(x=x_data / est_sd, loss="Outlier", lambda_=2 * np.log(len(x_data)), lthreshold=3)

# Estimated changepoints
cpt_ou = res_ou['t.est'][:-1]

# Simple plotting of changes and smoothed profile
plt.figure(figsize=(12, 8))
plt.plot(x_data / est_sd, marker='o', linestyle='-', color='black', label='Data')
plt.plot(res_ou['smt'], color='red', linewidth=2, label='Smoothed Profile (Outlier)')
plt.vlines(cpt_ou, ymin=min(x_data / est_sd), ymax=max(x_data / est_sd), colors='blue', linestyles='dashed', label='Change Points (Outlier)')
plt.title('Robust Fpop (Outlier) - Change Points Detection')
plt.xlabel('Index')
plt.ylabel('Normalized Signal')
plt.legend()
plt.savefig('robust_fpop_outlier.png')
plt.show()

# Robust Fpop (Huber)
# Run dynamic programming
res_hu = rob_seg_std(x=x_data / est_sd, loss="Huber", lambda_=1.4 * np.log(len(x_data)), lthreshold=1.345)

# Estimated changepoints
cpt_hu = res_hu['t.est'][:-1]

# Simple plotting of changes and smoothed profile
plt.figure(figsize=(12, 8))
plt.plot(x_data / est_sd, marker='o', linestyle='-', color='black', label='Data')
plt.plot(res_hu['smt'], color='red', linewidth=2, label='Smoothed Profile (Huber)')
plt.vlines(cpt_hu, ymin=min(x_data / est_sd), ymax=max(x_data / est_sd), colors='blue', linestyles='dashed', label='Change Points (Huber)')
plt.title('Robust Fpop (Huber) - Change Points Detection')
plt.xlabel('Index')
plt.ylabel('Normalized Signal')
plt.legend()
plt.savefig('robust_fpop_huber.png')
plt.show()