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

#ifndef __MDDATWORDDBB_H
#define __MDDATWORDDBB_H

#include "MDdataWord.h"
// #include "MDTypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;


class MDdataWordDBB : public MDdataWord {

 public:

  MDdataWordDBB(void *d=0);
  ~MDdataWordDBB(){}

 private:

  enum DWDBBMask{
   TriggerCountMask = 0x3FF0000,
   HitCountMask     = 0x1FFF,
   DataTypeMask     = 0xF0000000,
   BoardIdMask      = 0xFC00000,
   SpillNumberMask  = 0xFFFF,
   ChannelIdMask    = 0xFC00000,
   HitTimeMask      = 0x3FFFFF,
   StatusMask       = 0x3F0000,
   SpillWidthMask   = 0x3FFFFF
  };

  enum DWDBBShift{
   TriggerCountShift = 16,
   HitCountShift     = 0,
   DataTypeShift     = 28,
   BoardIdShift      = 22,
   SpillNumberShift  = 0,
   ChannelIdShift    = 22,
   HitTimeShift      = 0,
   StatusShift       = 16,
   SpillWidthShift   = 0
  };

 public:

  enum DWDBBDataType{
   TrailingMeas = 0x2,
   LeadingMeas  = 0x3,
   Header       = 0xD,
   Trailer      = 0xE,
   TriggerCount = 0x8
  };

  unsigned long32 GetDataType();
  unsigned long32 GetTriggerCount();
  unsigned long32 GetHitCount();
  unsigned long32 GetGeo();
  unsigned long32 GetSpillNumber();
  unsigned long32 GetChannelId();
  unsigned long32 GetHitTime();
  unsigned long32 GetStatus();
  unsigned long32 GetSpillWidth();
};


#endif

