#include <iostream>
#include <dlfcn.h>  // 包含动态链接库操作的头文件
#include <cstring>  // 包含 C 字符串操作的头文件
#include <list>

int main() {
    std::list<int> nums = {1, 2, 3, 4, 5};
    auto it = nums.insert(nums.begin(), 0);
    std::cout << *it << std::endl;
}

// typedef void (*Call_BinSeg)(
//     double*, int*, int*, int*, int*, double*
// );

// int main() {
//     // 动态库路径
//     const char* libPath = "build/liball.so";

//     // 加载动态库
//     void* handle = dlopen(libPath, RTLD_LAZY);
//     if (!handle) {
//         std::cerr << "Cannot open library: " << dlerror() << std::endl;
//         return 1;
//     }

//     // 清除之前的错误
//     dlerror();

//     // 获取函数指针
//     Call_BinSeg call_BinSeg = (Call_BinSeg)dlsym(handle, "Call_BinSeg");
//     const char* dlsym_error = dlerror();
//     if (dlsym_error) {
//         std::cerr << "Cannot load symbol 'rob_fpop_RtoC': " << dlsym_error << std::endl;
//         dlclose(handle);
//         return 1;
//     }

//     int n = 100;
//     int P = 10;
//     int Kmax = 5;
//     double dataVec[n * P];
//     int Ruptures[Kmax];
//     double RupturesCost[Kmax];

//     // 初始化数据
//     for (int i = 0; i < n * P; ++i) {
//         dataVec[i] = static_cast<double>(i);
//     }

//     call_BinSeg(dataVec, &Kmax, &n, &P, Ruptures, RupturesCost);

//     // 打印结果
//     std::cout << "Ruptures: ";
//     for (int i = 0; i < Kmax; ++i) {
//         std::cout << Ruptures[i] << " ";
//     }
//     std::cout << std::endl;

//     std::cout << "RupturesCost: ";
//     for (int i = 0; i < Kmax; ++i) {
//         std::cout << RupturesCost[i] << " ";
//     }
//     std::cout << std::endl;


//     // 关闭动态库
//     dlclose(handle);

//     return 0;
// }