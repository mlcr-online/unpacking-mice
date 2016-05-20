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

#ifndef __MDPARTEVENTV1290_H
#define __MDPARTEVENTV1290_H

#include "MDdataContainer.h"
#include "MDdataWordV1290.h"
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

#define V1290_NCHANNELS    32


class MDpartEventV1290 : public MDdataContainer {

 public:

  MDpartEventV1290( void *d = 0 );
  virtual ~MDpartEventV1290(){}
  virtual void SetDataPtr( void *d );

  void Init();
  unsigned int GetEventCount();

  unsigned int GetGeo(){ return _geo; }

  unsigned int GetNHits(unsigned int ich=V1290_NCHANNELS,char t='a'); 

  unsigned int GetWordCount();

  unsigned int GetStatus();
  unsigned int GetTriggerTimeTag(){ return _triggerTimeTag; }
  unsigned int GetBunchID(unsigned int unit=4);

  unsigned int GetHitMeasurement(unsigned int ih, unsigned int ich, char t);
  vector<int>  GetLHitMeasurements(unsigned int ich) { return _leadingEdgeHit[ich]; }
  vector<int>  GetTHitMeasurements(unsigned int ich) { return _trailingEdgeHit[ich]; }

  virtual void Dump( int atTheTime = 1 );

 private:

  long32         _tdcStatus;
  unsigned int   _wordCount;
  unsigned int   _geo;
  unsigned int   _triggerTimeTag;
  unsigned int   _bunchID[4];
  unsigned int   _eventID[4];
  unsigned int   _unitWordCount;
  unsigned int   _nLeadingEdgeHits[V1290_NCHANNELS+1];
  unsigned int   _nTrailingEdgeHits[V1290_NCHANNELS+1];
  vector<int>    _leadingEdgeHit[V1290_NCHANNELS];
  vector<int>    _trailingEdgeHit[V1290_NCHANNELS];
};

#endif
