#include "BinSeg_MultiDim.h"
#include "fp_list.h"
// #include <R_ext/Arith.h>

// this function is visible by R
extern "C" {

void rob_fpop_RtoC(const double *profil, const int *nbi, const double *lambda_, const double *lthreshold_,
                   const double *rthreshold_, const double *lslope_, const double *rslope_, const double *mini,
                   const double *maxi, int *origine, double *cout_n, double *mean_n);
void Call_BinSeg(double *dataVec_, int *Kmax_, int *n_, int *P_, int *Ruptures_, double *RupturesCost_);
}