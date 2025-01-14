/* July 2017 Guillem Rigaill <guillem.rigaill@inra.fr>

   This file is part of the R package Robseg

   robseg is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License (LGPL) as published by
   the Free Software Foundation; either version 2.1 of the License, or
   (at your option) any later version.

   robseg is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with opfp; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "apis.h"

void rob_fpop_RtoC(const double *profil, const int *nbi, const double *lambda_, const double *lthreshold_,
                   const double *rthreshold_, const double *lslope_, const double *rslope_, const double *mini,
                   const double *maxi, int *origine, double *cout_n, double *mean_n)
{
    int nb = *nbi;
    double lambda = *lambda_;
    double lthreshold = *lthreshold_;
    double rthreshold = *rthreshold_;
    double lslope = *lslope_;
    double rslope = *rslope_;

    double min = *mini;
    double max = *maxi;

    int minPosition = -10;
    double minCurrent = 100, meanCurrent = 0.;

    auto myList = std::make_unique<MyList>(std::make_unique<CFunctPart>(min, max));
    myList->add(profil[0], lthreshold, rthreshold, lslope, rslope);
    myList->getMin(&minCurrent, &minPosition, &meanCurrent);
    cout_n[0] = minCurrent + lambda;
    origine[0] = minPosition;
    mean_n[0] = meanCurrent;

    for (int t = 1; t < nb; t++) {
        // cout << "Tour : " << t << "\n";
        myList->compare(cout_n[t - 1], t);
        myList->mergeSimilarElements();
        myList->add(profil[t], lthreshold, rthreshold, lslope, rslope);
        myList->mergeSimilarElements();
        myList->getMin(&minCurrent, &minPosition, &meanCurrent);
        cout_n[t] = minCurrent + lambda;
        origine[t] = minPosition;
        mean_n[t] = meanCurrent;
    }
}

void Call_BinSeg(double *dataVec_, int *Kmax_, int *n_, int *P_, int *Ruptures_, double *RupturesCost_)
{
    int n = *n_;
    int P = *P_;
    int Kmax = *Kmax_;
    auto data = std::make_unique<std::unique_ptr<double[]>[]>(n);
    for (int i = 0; i < n; i++) data[i] = std::make_unique<double[]>(P);
    for (int i = 0; i < n; i++)  //{
        for (int j = 0; j < P; j++)
            //{
            data[i][j] = dataVec_[i + n * j];
    // printf("%d, %d, %d, %f, ", i, j, i+ n*j, data[i][j]);
    // }
    // printf("\n");
    // }
    BinSeg_MultiDim BinSegRun(std::move(data), n, P, 2 * Kmax + 10);
    BinSegRun.Initialize(Kmax);

    int i = 0;
    for (std::list<int>::iterator iter = BinSegRun.Ruptures.begin(); iter != BinSegRun.Ruptures.end(); iter++) {
        Ruptures_[i] = *iter;
        i++;
    }

    i = 0;
    for (std::list<double>::iterator iter = BinSegRun.RupturesCost.begin(); iter != BinSegRun.RupturesCost.end();
         iter++) {
        RupturesCost_[i] = *iter;
        i++;
    }
}