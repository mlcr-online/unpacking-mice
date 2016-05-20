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


#ifndef __MDDATWORDV775_H
#define __MDDATWORDV775_H

#include "MDdataWord.h"
#include "MDdataContainer.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>


using namespace std;

typedef enum DWV775DataType {
  DWV775_Measurement   = 0x0,
  DWV775_Header        = 0x2,
  DWV775_EOB           = 0x4
} DWV775DataType;



class MDdataWordV775 : public MDdataWord {

 public:
  MDdataWordV775( void *d = 0 );
  ~MDdataWordV775(){}

  long32 DWGetDataType();
  long32 DWGetGeo();
  long32 DWGetChannelCount();
  long32 DWGetMeasurement();
  long32 DWGetChannel();
  long32 DWGetEventCount();
};

ostream &operator<<(ostream &s,MDdataWordV775 &dw);
istream &operator>>(istream &s,MDdataWordV775 &dw);

#endif
