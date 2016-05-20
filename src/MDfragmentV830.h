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

#ifndef __MDFRAGMENTV830_H
#define __MDFRAGMENTV830_H

#include "MDdataContainer.h"
#include "MDfragment.h"
#include "MDdataWordV830.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

class MDfragmentV830 : public MDfragment {

 public:
  MDfragmentV830( void *d = 0 );
  virtual ~MDfragmentV830(){}
  virtual void SetDataPtr( void *d, uint32_t aSize  );
  virtual void Dump( int atTheTime = 1 );
  virtual void Init();
  //  virtual uint32_t InitPartEventVector();
  uint32_t GetChannel(unsigned int iw);
  uint32_t GetMeasurement(unsigned int iw);

 private:
  unsigned int   _ts;
  unsigned int   _triggerCount;  


};

#endif
