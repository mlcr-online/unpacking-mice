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

#ifndef __MDPARTEVENTV1731_H
#define __MDPARTEVENTV1731_H

#include "MDdataContainer.h"
#include "MDdataWordV1731.h"
#include "MDexception.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define V1731_HEADER_WORDS 4
#define V1731_WORD_SIZE    4
#define V1731_NCHANNELS    8


class MDpartEventV1731 : public MDdataContainer {
 private:
  unsigned int  _nChannels;
  unsigned int  _length[ V1731_NCHANNELS ];
  unsigned int* _sequence[ V1731_NCHANNELS ];


 public:

  MDpartEventV1731( void *d = 0 );
  virtual ~MDpartEventV1731(){}
  virtual void SetDataPtr( void *d );
  virtual void Dump( int atTheTime = 1 );
  void Init();

  unsigned int GetNChannels(){ return _nChannels; }
  unsigned int GetLength(int ch){ return _length[ch]; }
  unsigned int GetNSamples(int ch){ return 4*_length[ch]; } // 4 sample data per word
  unsigned int* GetSequence(int ch){ return _sequence[ch]; }

  unsigned int GetWordCount();

  unsigned int GetGeo();
  int          GetZS();
  unsigned int GetPattern();
  unsigned int GetChannelMask();

  unsigned int GetEventCounter();
  unsigned int GetEventCount(){ return GetEventCounter()-1; }


  unsigned int GetTriggerTimeTag();

  int16_t GetSampleData( unsigned short aChannel, unsigned long aSample );

};

ostream &operator<<(ostream &s,MDpartEventV1731 &dw);
istream &operator>>(istream &s,MDpartEventV1731 &dw);


#endif
