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

#ifndef __MDPARTEVENTPCI6254_H
#define __MDPARTEVENTPCI6254_H

#include "MDdataContainer.h"
#include "MDdataWordPCI6254.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define  PCI6254_HEADER_SYNC 0xE0F00E0F

class MDpartEventPCI6254 : public MDdataContainer {

 public:
  
  MDpartEventPCI6254( void *d = 0 );
  virtual ~MDpartEventPCI6254(){}
  virtual void SetDataPtr( void *d );
  float  GetMeasurement(unsigned int ich, unsigned int iSample);
  float* GetSequence(unsigned int ich);

  void Init();
  unsigned int GetHeaderLength(){return _headerSize/4; }

  virtual void Dump( int atTheTime = 1 );
 private:
  unsigned int   _headerSize;
  unsigned int   _headerVersion;
  unsigned int   _clockRate;
  unsigned int   _nSamplesPerEvent;
  unsigned int   _nChannels;  
  unsigned int   _tv_sec;  
  unsigned int   _tv_usec;
  vector<float*> _vSeq;
};

#endif
