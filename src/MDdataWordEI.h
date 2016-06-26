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

#ifndef __MDDATAWORF_EI_H
#define __MDDATAWORF_EI_H

#include "MDdataWord.h"

class MDdataWordEI : public MDdataWord {

 public:

  MDdataWordEI(void *d=0) : MDdataWord(d) {}
  ~MDdataWordEI() {}

  enum DWEIDataType {
   HP_HeaderType = 0xA,
   PV_intType    = 0x0,
   PV_doubleType = 0x1,
   PV_stringType = 0x2
  };

  unsigned long32 getHpHeaderType();
  unsigned long32 getHpHeaderSize();
  unsigned long32 getPvType();
  unsigned long32 getPvId();
  double          getHP();

 private:

  enum DWEIMask {
   HpHeaderTypeMask = 0xF0000000,
   HpHeaderSizeMask = 0x00FFFFFF,
   PvIdMask         = 0x000000FF,
   PvTypeMask       = 0x0F000000,
  };

  enum DWEIShift {
   HpHeaderTypeShift = 28,
   HpHeaderSizeShift = 0,
   PvIdShift         = 0,
   PvTypeShift       = 24
  };

};

#endif


