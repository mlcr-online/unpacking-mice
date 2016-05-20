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

#ifndef __MDFRAGMENTDBB_CHAIN_H
#define __MDFRAGMENTDBB_CHAIN_H

#include "MDdataContainer.h"
#include "MDfragment.h"
#include "MDdataWordDBB.h"
#include "MDfragmentDBB.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#define DBBS_IN_CHAIN     6

class MDfragmentDBBChain : public MDfragment {

 public:
  MDfragmentDBBChain(void *d = 0);
  virtual ~MDfragmentDBBChain();

  void SetDataPtr( void *d, uint32_t aSize );
  void Dump( int atTheTime = 1 );
  void Init();

  MDfragmentDBB* getFragmentDBB(int i) {return _dbb[i];}

 private:
  int getDBBSize(uint32_t* ptr, int max_size);

  std::vector<MDfragmentDBB*> _dbb;
};

#endif

