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

#ifndef __MDDATWORDVLSBMASTER_H
#define __MDDATWORDVLSBMASTER_H

#include "MDdataWord.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

class MDdataWordVLSBMaster : public MDdataWord {

 public:
  
  MDdataWordVLSBMaster( void *d = 0 );
  ~MDdataWordVLSBMaster(){}

  bool IsValid() {
    if (!_data) return false;
    int16_t type = (Get32bWordPtr(0)[0] & DataType_Mask) >> DataType_Shift;
    if (type != VLSBMDataType)
      return false;

    return true;
  }

  int16_t GetNTriggers() {
    if (!IsValid()) return 0;
    return (Get32bWordPtr(0)[0] & NTriggers_Mask) >> NTriggers_Shift;
  }

  int16_t getTriggerDataTDC() {
    if (!IsValid()) return 0;
    return (Get32bWordPtr(0)[0] & TriggerDataTDC_Mask) >> TriggerDataTDC_Shift;
  }

  virtual void Dump( int atTheTime = 1 );

 private:

  enum MasterWord {
    TriggerDataTDC_Mask    = 0x0000FFFF,
    TriggerDataTDC_Shift   = 0,
    NTriggers_Mask         = 0x0FFF0000,
    NTriggers_Shift        = 16,
    VLSBMDataType          = 0xA,
    DataType_Mask          = 0xF0000000,
    DataType_Shift         = 28
  };


};

ostream &operator<<(ostream &s,MDdataWordVLSBMaster &dw);
istream &operator>>(istream &s,MDdataWordVLSBMaster &dw);


#endif
