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

#include "MDfragmentV1731.h"

MDfragmentV1731::MDfragmentV1731(void *d):MDfragment(d){ 
  // A fragment should be created only once inside the map
  //   cout << " Creating a MDfragmentV1731 ..." << endl; 
 _partEventPtr = new MDpartEventV1731(0);
  Init();
}

void MDfragmentV1731::SetDataPtr( void *d, uint32_t aSize ) {
  MDfragment::SetDataPtr(d, aSize);
}

void MDfragmentV1731::Init() {
  //  cout << " Executing MDfragmentV1731::Init() ..." << endl;
  UnValidate();
  _madeOfParticles = true;

  if (this->GetSize() == 0)
    return;

  uint32_t * ptr = Get32bWordPtr(0);
  MDdataWordV1731 dw(ptr);

  if ( dw.IsValid() ) {
   // TODO : Basic checks on the data consistency
    Validate();
  }
  //  cout << "  ... Done with MDfragmentV1731" << endl;
}

////////////////////////////////////////////////////////////////////////


