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

#ifndef __MDPARTEVENTV1724_H
#define __MDPARTEVENTV1724_H

#include "MDdataContainer.h"
#include "MDdataWordV1724.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
 
#define V1724_HEADER_WORDS 4
#define V1724_WORD_SIZE    4
#define V1724_NCHANNELS    8


class MDpartEventV1724 : public MDdataContainer {
 private:
  unsigned int  _nChannels;
  unsigned int  _length[ V1724_NCHANNELS ];
  unsigned int* _sequence[ V1724_NCHANNELS ];

 public:
  MDpartEventV1724( void *d = 0 );
  virtual ~MDpartEventV1724(){}
  virtual void SetDataPtr( void *d );

  void Init();

  unsigned int GetNChannels(){ return _nChannels; }
  unsigned int GetLength(int ch){ return _length[ch]; }
  unsigned int GetNSamples(int ch){ return 2*_length[ch]; } // 2 sample data per word
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


  virtual void Dump( int atTheTime = 1 );

};

ostream &operator<<(ostream &s,MDpartEventV1724 &dw);
istream &operator>>(istream &s,MDpartEventV1724 &dw);


#endif
