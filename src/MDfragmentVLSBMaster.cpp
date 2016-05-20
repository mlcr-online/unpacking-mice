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

#include "MDfragmentVLSBMaster.h"
#include "MDdataWordVLSBMaster.h"

MDfragmentVLSBMaster::MDfragmentVLSBMaster(void *d):MDfragment(d){ 
  //  cout << " Creating a MDfragmentVLSBMaster ..." << endl; 
  _partEventPtr = new MDpartEventVLSB(0);
  Init();
}

void MDfragmentVLSBMaster::SetDataPtr( void *d, uint32_t aSize ) {
  MDfragment::SetDataPtr(d,aSize);
 // The dynamically correct Init() is called by MDfragment::SetDataPtr(d,aSize)
}

void MDfragmentVLSBMaster::Init(){
  //  cout << "MDfragmentVLSBMaster::Init()" << endl;
  uint32_t * ptr = Get32bWordPtr(0);
  MDdataWordVLSBMaster dw(ptr);
  UnValidate();
  _madeOfParticles = false;

  // Checking the first word to start with
  if ( dw.IsValid() ) {
    Validate();
    // There is nothing else we can do here.
  }
}

uint32_t MDfragmentVLSBMaster::InitPartEventVector() {
  uint32_t * ptr = Get32bWordPtr(0);
  MDdataWordVLSBMaster dw(ptr);
  if(!dw.IsValid())
    return 0;

  return dw.GetNTriggers();
}
////////////////////////////////////////////////////////////////////////


