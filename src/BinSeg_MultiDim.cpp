/*
 *  Binseg.cpp
 *
 *
 *  Created by Guillem Rigaill on 16/05/11. Modified from code written with Michel Koskas
 *  Copyright 2013 INRA, UEVE. All rights reserved.
 *
 */

#include "BinSeg_MultiDim.h"

unique_ptr<Node> BinSeg_MultiDim::Best(int LowIndex, int HighIndex)
{
    if (HighIndex - LowIndex <= 1) {
    }
    double SquaresSum = 0.;
    auto LeftSum = std::make_unique<double[]>(P);
    std::fill(LeftSum.get(), LeftSum.get() + P, 0.0);
    auto RightSum = std::make_unique<double[]>(P);
    std::fill(RightSum.get(), RightSum.get() + P, 0.0);

    // Get Sum of Squares for every columns
    for (int i = LowIndex; i < HighIndex; i++)
        for (int i_p = 0; i_p < P; i_p++) SquaresSum += data[i][i_p] * data[i][i_p];
    // std::cout << "SqSum" << SquaresSum << std::endl;

    // Get Sum for each columns
    for (int i = LowIndex; i < HighIndex; i++)
        for (int i_p = 0; i_p < P; i_p++) RightSum[i_p] = RightSum[i_p] + data[i][i_p];
    // for(int i_p = 0; i_p < P; i_p++)
    //	std::cout << i_p << ": " << RightSum[i_p] << std::endl;

    // Get Whole Segment Cost
    double WholeSegmentCost = 0.0;  // SquaresSum;

    for (int i_p = 0; i_p < P; i_p++)
        WholeSegmentCost = WholeSegmentCost - RightSum[i_p] * RightSum[i_p];  /// (HighIndex - LowIndex);
    // std::cout << "WholeCost'" << WholeSegmentCost << "SqSum" << SquaresSum << std::endl;

    WholeSegmentCost = WholeSegmentCost / (HighIndex - LowIndex) + SquaresSum;

    // std::cout << "WholeCost" << WholeSegmentCost << "SqSum" << SquaresSum << std::endl;

    // Initial segment cost for the split higher the without split cost ...
    // double InitialSegmentCost = SquaresSum - WholeSegmentCost;
    // for(int i_p = 0; i_p < P; i_p++)
    //	InitialSegmentCost = InitialSegmentCost - RightSum[i_p] * RightSum[i_p] / (HighIndex - LowIndex -1);
    double SegmentCost = WholeSegmentCost + 1.0;
    // First possible node = doing nothing Cost > Whole Segment
    auto index = LowIndex;
    double newSegmentCost = 0;
    double tmpL, tmpR;

    for (int i = LowIndex + 1; i < HighIndex; i++)  // consider all possible split in two
    {
        for (int i_p = 0; i_p < P; i_p++) LeftSum[i_p] = LeftSum[i_p] + data[i - 1][i_p];
        for (int i_p = 0; i_p < P; i_p++) RightSum[i_p] = RightSum[i_p] - data[i - 1][i_p];
        newSegmentCost = SquaresSum - WholeSegmentCost;
        tmpL = 0;
        tmpR = 0;
        for (int i_p = 0; i_p < P; i_p++) {
            tmpL = tmpL - LeftSum[i_p] * LeftSum[i_p];    //  /(i - LowIndex)
            tmpR = tmpR - RightSum[i_p] * RightSum[i_p];  // /(HighIndex - i);
        }
        newSegmentCost = newSegmentCost + tmpL / (i - LowIndex) + tmpR / (HighIndex - i);
        // std::cout << "Split " << i << "New : " << newSegmentCost << " Old : "<< SegmentCost << std::endl;
        if (newSegmentCost < SegmentCost) {
            SegmentCost = newSegmentCost;
            index = i;
        }
    }
    return std::make_unique<Node>(index, SegmentCost, LowIndex, HighIndex);;
}

void BinSeg_MultiDim::Initialize(int NbRuptures)
{
    int LowIndex = 0, HighIndex = n;
    auto FirstNode = Best(LowIndex, HighIndex);
    myHeap.AddNode(std::move(FirstNode));
    //	std::cout << "FirstNode" << FirstNode.Index <<  ", " <<
    //		FirstNode.LowIndex << ", " <<
    //		FirstNode.HighIndex << std::endl;

    // MyHeap(FirstNode, 1, 2*NbRuptures+10);
    for (int i = 0; i < NbRuptures; i++) {
        //	std::cout << "Round " << i << std::endl;
        auto N = myHeap.MyHeap[0].get();
        //	std::cout << "Best Node : " << N.LowIndex << ", " << N.Index << ", " << N.HighIndex << std::endl;
        //	std::cout << "Heap Lgth : " << MyHeap.HeapSize << std::endl;

        Ruptures.push_back(N->Index);
        RupturesCost.push_back(N->Value);

        myHeap.RemoveHead();
        //	std::cout << "Heap Lgth' : " << MyHeap.HeapSize << std::endl;
        //left part
        if (N->Index - N->LowIndex > 1) {
            myHeap.AddNode(Best(N->LowIndex, N->Index));
            //		std::cout << "Left->" << Left.Index << std::endl;
        }
        //right part
        if (N->HighIndex - N->Index > 1) {
            myHeap.AddNode(Best(N->Index, N->HighIndex));
            //		std::cout << "Right->" << Right.Index << std::endl;
        }
    }
}
