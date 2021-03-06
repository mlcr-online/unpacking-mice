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

#ifndef __MDFRAGMENTV1290_H
#define __MDFRAGMENTV1290_H

#include "MDdataContainer.h"
#include "MDfragment.h"
#include "MDdataWordV1290.h"
#include "MDpartEventV1290.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

class MDfragmentV1290 : public MDfragment {

 public:

  MDfragmentV1290( void *d = 0 );
  virtual ~MDfragmentV1290(){}
  virtual void SetDataPtr( void *d, uint32_t aSize );
  virtual void Init();

  virtual void SetTest(DataTestCallback funk) {
    if (_partEventPtr) _partEventPtr->SetTest( funk );
  }

 private:
};

#endif
