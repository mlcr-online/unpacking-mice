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

#ifndef __MDDATWORDV830_H
#define __MDDATWORDV830_H

#include "MDdataWord.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>


using namespace std;

typedef enum DWV830DataType {
  DWV830_Measurement     = 0x00,
  DWV830_Header          = 0x01,
  DWV830_InvalidData     = 0x1F
} DWV830DataType;

typedef enum DWV830TriggerType {
  DWV830_ExternalTrigger     = 0x00,
  DWV830_TriggerViaTimer     = 0x01,
  DWV830_TriggerViaVME       = 0x02,
  DWV830_InvalidTrigger      = 0x04
} DWV830TriggerType;

class MDdataWordV830 : public MDdataWord {

 public:
  MDdataWordV830( void *d = 0 );
  ~MDdataWordV830(){}

  long32 GetDataType();
  long32 GetGeo();
  long32 GetTriggerType();
  long32 GetWordCount();
  long32 GetTriggerCount();
  long32 GetMeasurement();
  long32 GetChannel();

  virtual void Dump(int atTheTime=1);  
};


ostream &operator<<(ostream &s,MDdataWordV830 &dw);
istream &operator>>(istream &s,MDdataWordV830 &dw);

#endif
