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

#ifndef __MDFRAGMENTV1495_H
#define __MDFRAGMENTV1495_H

#include "MDdataContainer.h"
#include "MDfragment.h"
#include "MDdataWordV1495.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>


class MDfragmentV1495 : public MDfragment {

 public:

  MDfragmentV1495(void *d = 0);
  virtual ~MDfragmentV1495(){}

  void SetDataPtr( void *d, uint32_t aSize );
  void Init();
  void Dump();

  unsigned int GetSpillNumber()  {return _spillNum;}
  unsigned int GetNumTriggers()  {return _nTriggers;}
  unsigned int GetGeo()          {return _geo;}

  unsigned int GetTriggerTime(unsigned int tr);
  unsigned long32 GetPatternTOF0(unsigned int tr, char p='n');
  unsigned long32 GetPatternTOF1(unsigned int tr, char p='n');
  unsigned long32 GetPatternTOF2(unsigned int tr, char p='n');

 private:

  unsigned int _geo;
  unsigned int _spillNum;     /// Number of Spills received by the DBB since last reset
  unsigned int _nTriggers;    /// Number of triggers received
};

ostream &operator<<(std::ostream &s,MDfragmentV1495 &df);

#endif


