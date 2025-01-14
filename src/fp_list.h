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
#ifndef LISTEH
#define LISTEH
#include <list>
#include <memory>
#include "fp_CFunctPart.h"

class MyList
{
public:
    std::list<std::unique_ptr<CFunctPart>> Listes;
    MyList() 
    {
        Listes.clear();
    }
    ~MyList() = default;

    void getMin(double *min_, int *origine_, double *mean_);
    
    void add(double X_, double lthrs_, double rthrs_, double lslope, double rslope);
    
    void compare(double cost_, int origine_);

    void mergeSimilarElements();
    
};
#endif  // INLINED
