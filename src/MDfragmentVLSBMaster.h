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

#ifndef __MDFRAGMENTVLSBMASTER_H
#define __MDFRAGMENTVLSBMASTER_H

#include "MDdataContainer.h"
#include "MDfragment.h"
#include "MDpartEventVLSB.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

class MDfragmentVLSBMaster : public MDfragment {

 public:
  
  MDfragmentVLSBMaster( void *d = 0 );
  virtual ~MDfragmentVLSBMaster(){}
  virtual void SetDataPtr( void *d, uint32_t aSize  );
  virtual void Init();

  uint32_t InitPartEventVector();
  
  virtual void SetTest(DataTestCallback funk) {
    if (_partEventPtr) _partEventPtr->SetTest( funk );
  }

 private:

};

#endif
