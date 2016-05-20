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

#ifndef __MDFRAGMENTDBB_H
#define __MDFRAGMENTDBB_H

#include "MDdataContainer.h"
#include "MDfragment.h"
#include "MDdataWordDBB.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#define DBB_NCHANNELS     64
#define DBB_HEADER_WORDS   2
#define DBB_TRAILER_WORDS  2
#define DBB_MAX_DATA_SIZE  16380

class MDfragmentDBB : public MDfragment {

 public:

  MDfragmentDBB(void *d = 0) : MDfragment(d) { Init(); }
  virtual ~MDfragmentDBB(){}

  void SetDataPtr( void *d, uint32_t aSize );
  void Dump( int atTheTime = 1 );
  void Init();

  unsigned int GetSpillWidth()  { return _spillWidth; }
  unsigned int GetSpillNumber() { return _spillNum; }
  unsigned int GetTriggerCount() {return _triggerCount;}
  unsigned int GetHitCount() {return _hitCount;}
  unsigned int GetNLeadingEdgeHits(unsigned int ich) {return _leadingEdgeHit[ich].size();}
  unsigned int GetNTrailingEdgeHits(unsigned int ich) {return _trailingEdgeHit[ich].size();}
  unsigned int GetHitMeasurement(unsigned int ih, unsigned int ich, char t);
  unsigned int GetLeadingTime(unsigned int ih, unsigned int ich){ return GetHitMeasurement(ih, ich, 'l');}
  unsigned int GetTrailingTime(unsigned int ih, unsigned int ich){ return GetHitMeasurement(ih, ich, 't');}

  std::vector<unsigned int> GetLeadingTimes(unsigned int ih)   {return _leadingEdgeHit[ih];}
  std::vector<unsigned int> GetTrailingTimes(unsigned int ih)  {return _trailingEdgeHit[ih];}

  unsigned int getNumDataWords() {return _nDataWords;}

 private:

  unsigned int _spillNum;     /// Number of Spills received by the DBB since last reset
  unsigned int _spillWidth;   /// Number of clock ticks during spill (2.5 ns each)
  unsigned int _triggerCount; /// Number of triggers received
  unsigned int _hitCount;     /// Leading edges only, triggers included

  unsigned int          _nLeadingEdgeHits[DBB_NCHANNELS+1];  /** Number leading edge hits per channel, last channel is total
                                                                 should be equal to _hitCount, channel DBB_NCHANNELS-1 is the
                                                                 trigger it should contain _triggerCount */
  unsigned int          _nTrailingEdgeHits[DBB_NCHANNELS+1]; /** Number trailing edge hits per channel, last channel is total.
                                                                 It is not guaranteed that there is a trailing edge for each
                                                                 leading edge */
  std::vector<unsigned int>  _leadingEdgeHit[DBB_NCHANNELS];      /// A vector of leading edge hits per channel, last channel is total
  std::vector<unsigned int>  _trailingEdgeHit[DBB_NCHANNELS];     /// A vector of trailing edge hits per channel, last channel is total

  unsigned int _nDataWords;

 public:
  void dumpEventVRB(uint32_t *data, int nbr);
};

ostream &operator<<(std::ostream &s,MDfragmentDBB &df);

#endif


