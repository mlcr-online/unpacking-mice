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

#ifndef __MDFRAGMENTVLSB_H
#define __MDFRAGMENTVLSB_H

#include "MDdataContainer.h"
#include "MDfragment.h"
#include "MDpartEventVLSB.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

class MDfragmentVLSB : public MDfragment {

 public:
  
  MDfragmentVLSB( void *d = 0 );
  virtual ~MDfragmentVLSB(){}
  virtual void SetDataPtr( void *d, uint32_t aSize  );
  virtual void Init();

  unsigned int GetEventNum(unsigned int ih);
  unsigned int GetChannel(unsigned int ih);
  unsigned int GetAdc(unsigned int ih);
  unsigned int GetTdc(unsigned int ih);
  bool GetDiscriBit(unsigned int ih);

  virtual void SetTest(DataTestCallback funk) {
    if (_partEventPtr) _partEventPtr->SetTest( funk );
  }

 private:

};

#endif
