/* February 2014 Guillem Rigaill <rigaill@evry.inra.fr>

   This file is part of the R package fpop

   fpop is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License (LGPL) as published by
   the Free Software Foundation; either version 2.1 of the License, or
   (at your option) any later version.

   opfp is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with opfp; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <math.h>
#include <memory>
#include <vector>

#include "Node.h"
#ifndef _Heap_
#define _Heap_

using namespace std;

class Heap 
{
public:
    std::vector<std::unique_ptr<Node>> MyHeap;

    // Heap(Node *TheN = NULL, int NbNodes = 0, int AllocationSize = 0);
    Heap(int AllocationSize_)
    {
        AllocatedSize = std::max(AllocatedSize, AllocationSize_);
        MyHeap.resize(AllocatedSize);
    };
    Heap()
    {
        MyHeap.resize(AllocatedSize);
    };
    ~Heap() = default;
    void AddNode(std::unique_ptr<Node> N)
    {
        if (HeapSize == AllocatedSize) {
            AllocatedSize *= 2;
            if (AllocatedSize == 0) AllocatedSize = 1;
            MyHeap.resize(AllocatedSize);
        };
        int CurIndex = HeapSize;
        MyHeap[CurIndex] = std::move(N);
        while ((CurIndex > 0) && (MyHeap[CurIndex] < MyHeap[(CurIndex - 1) / 2])) {
            std::swap(MyHeap[CurIndex], MyHeap[(CurIndex - 1) / 2]);
            CurIndex = (CurIndex - 1) / 2;
        }
        HeapSize++;
    }
    void RemoveHead()
    {
        MyHeap[0] = std::move(MyHeap[HeapSize - 1]);
        HeapSize--;
        int CurIndex = 0;
        while (CurIndex < (HeapSize - 2) / 2)  // CurNode has two sons
        {
            if ((MyHeap[CurIndex] <= MyHeap[2 * CurIndex + 1]) && (MyHeap[CurIndex] <= MyHeap[2 * CurIndex + 2])) break;
            int ExchangeIndex = 2 * CurIndex + 1;
            if (MyHeap[ExchangeIndex + 1] < MyHeap[ExchangeIndex]) ExchangeIndex++;
            std::swap(MyHeap[CurIndex], MyHeap[ExchangeIndex]);
            CurIndex = ExchangeIndex;
        }
        if (CurIndex < (HeapSize - 1) / 2) {
            if (MyHeap[CurIndex] < MyHeap[2 * CurIndex + 1]) std::swap(MyHeap[CurIndex], MyHeap[2 * CurIndex + 1]);
        }
    }
private:
    int HeapSize = 0;
    int AllocatedSize = 100;
};

// ostream & operator<<(ostream &s, const Heap &H)
// {
//   for (int i = 0; i < H.HeapSize; i++)
//                 s << H.MyHeap[i];
//         s << endl;
//   return s;
// }

#endif
