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

#ifndef __MDPARTEVENTV792_H
#define __MDPARTEVENTV792_H

#include "MDdataContainer.h"
#include "MDdataWordV792.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define V792_HEADER_WORDS 1
#define V792_EOB_WORDS    1
#define V792_WORD_SIZE    4

class MDpartEventV792 : public MDdataContainer {
 public:
  MDpartEventV792( void *d = 0 );
  virtual ~MDpartEventV792(){}
  virtual void SetDataPtr( void *d );

  void Init();
  void SetEvtCount(int ev)  {_evtCount = ev;}
  
  int GetEvtCount()         {return _evtCount;}
  int GetHitChannel(unsigned int i);
  int GetHitMeasurement(unsigned int i);
  
  unsigned int GetGeo();
  unsigned int GetChannelCount();
  
  bool EOB() {return _eob_record;}

 private:
  int _evtCount;
  bool _eob_record;

};

ostream &operator<<(ostream &s,MDpartEventV792 &dw);

#endif

