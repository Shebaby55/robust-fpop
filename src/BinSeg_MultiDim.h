/*
 *  Heap.h
 *
 *
 *  Created by Guillem Rigaill on 16/05/11. Modified from Michel Koskas
 *  Copyright 2013 INRA, UEVE. All rights reserved.
 *
 */

#ifndef _BinSeg_MultiDim_H_
#define _BinSeg_MultiDim_H_

#include <list>

#include "Heap.h"

#define PlusInfinity INFINITY

class BinSeg_MultiDim
{
public:
    Heap myHeap;
    std::unique_ptr<std::unique_ptr<double[]>[]> data;
    int n;
    int P;
    std::list<int> Ruptures;
    std::list<double> RupturesCost;
    std::unique_ptr<Node> Best(int LowIndex, int HighIndex);
    void Initialize(int NbRuptures);
    BinSeg_MultiDim(std::unique_ptr<std::unique_ptr<double[]>[]> data_, int n_, int P_, int nbAllocSize_) : n(n_), P(P_)
    {
        data = std::move(data_);
        // myHeap = Heap(nbAllocSize_);
    };
    // double Cost();
};

#endif
