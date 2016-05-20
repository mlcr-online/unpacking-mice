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


#ifndef __MDDATWORDV792_H
#define __MDDATWORDV792_H

#include "MDdataWord.h"
#include "MDdataContainer.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>


using namespace std;

typedef enum DWV792DataType {
  DWV792_Measurement   = 0x0,
  DWV792_Header        = 0x2,
  DWV792_EOB           = 0x4
} DWV792DataType;



class MDdataWordV792 : public MDdataWord {

 public:
  MDdataWordV792( void *d = 0 );
  ~MDdataWordV792(){}

  long32 DWGetDataType();
  long32 DWGetGeo();
  long32 DWGetChannelCount();
  long32 DWGetMeasurement();
  long32 DWGetChannel();
  long32 DWGetEventCount();
};

ostream &operator<<(ostream &s,MDdataWordV792 &dw);
istream &operator>>(istream &s,MDdataWordV792 &dw);

#endif
