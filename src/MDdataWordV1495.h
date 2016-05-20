/* This file is part of MAUS: http://micewww.pp.rl.ac.uk:8080/projects/maus
 *
 * MAUS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MAUS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MAUS.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef __MDDATWORDV1495_H
#define __MDDATWORDV1495_H

#include "MDdataWord.h"
// #include "MDTypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;


class MDdataWordV1495 : public MDdataWord {

 public:

  MDdataWordV1495(void *d=0);
  ~MDdataWordV1495(){}

 private:

  enum DWV1495Mask{
   DataTypeMask     = 0xF0000000,
   TriggerCountMask = 0xFFC00000,
   TriggerTimeMask  = 0x3FFFFF,
   NTriggersMask    = 0x7FE0000,
   SpillNumberMask  = 0xFFFF,
   BoardIdMask      = 0x1FE0000,
   PatterTOF0Mask   = 0xFFFFF,
   PatterTOF1Mask1  = 0xFFF00000,
   PatterTOF1Mask0  = 0xFF,
   PatterTOF2Mask   = 0xFFFFF00,
   Busy0Mask        = 0x0,
   Busy1Mask        = 0xFFFF0000,
   Busy2Mask        = 0x0,
   Busy3Mask        = 0xFFFF0000,
   Busy4Mask        = 0x0,
   Busy5Mask        = 0x0FFFF000
  };

  enum DWV1495Shift{
   DataTypeShift       = 28,
   TriggerCountShift   = 22,
   TriggerTimeShift    = 0,
   NTriggersShift      = 17,
   SpillNumberShift    = 0,
   BoardIdShift        = 17,
   PatterTOF0Shift     = 0,
   PatterTOF1Shift0    = 0,
   PatterTOF1Shift1    = 20,
   PatterTOF1Mask1Size = 12,
   PatterTOF2Shift     = 8,
   Busy0Shift          = 0,
   Busy1Shift          = 16,
   Busy2Shift          = 0,
   Busy3Shift          = 16,
   Busy4Shift          = 0,
   Busy5Shift          = 16
  };

 public:

  enum DWV1495DataType{
   Header       = 0x5,
   Trailer      = 0xF,
   Data         = 0xA
  };

  unsigned long32 GetDataType();
  unsigned long32 GetTriggerCount();
  unsigned long32 GetTriggerTime();
  unsigned long32 GetGeo();
  unsigned long32 GetSpillNumber();
  unsigned long32 GetNTriggers();

  unsigned long32 GetPatternTOF0();
  unsigned long32 GetPatternTOF1();
  unsigned long32 GetPatternTOF2();
  unsigned long32 GetBusy(int i);
};


#endif

