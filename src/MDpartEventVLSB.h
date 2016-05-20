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

#ifndef __MDPARTEVENTVLSB_H
#define __MDPARTEVENTVLSB_H

#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "MDdataContainer.h"
#include "MDdataWordVLSB.h"
#include "MDexception.h"

using namespace std;

#define VLSB_BANK_NCHANNELS    128

class MDpartEventVLSB : public MDdataContainer {
 public:
  MDpartEventVLSB( void *d = 0 );
  virtual ~MDpartEventVLSB(){}
  virtual void SetDataPtr( void *d );

  /// Loop of the data to set the correct _size and check data format
  void Init(); 

  unsigned int GetNHits(){return _nHits;}
  unsigned int GetWordCount(){return _nHits;}
  int GetEventNum(){return _eventNumber;}
  unsigned int GetChannel(unsigned int ih);
  unsigned int GetAdc(unsigned int ih);
  unsigned int GetTdc(unsigned int ih);
  bool GetDiscriBit(unsigned int ih);
  virtual void Dump(int atTheTime=1);

 private:
  int _eventNumber;
  unsigned int _nHits; // one word per hits, life is easy

};

#endif
