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

#include "MDfragmentV1290.h"

using namespace std;

MDfragmentV1290::MDfragmentV1290(void *d)
: MDfragment(d) {
  // A fragment should be created only once inside the map
  _partEventPtr = new MDpartEventV1290(0);
  Init();
}

void MDfragmentV1290::SetDataPtr( void *d, uint32_t aSize ) {
  MDfragment::SetDataPtr(d, aSize);
}

void MDfragmentV1290::Init(){
  uint32_t * ptr = Get32bWordPtr(0);
  MDdataWordV1290 dw(ptr);
  UnValidate();
  _madeOfParticles = true;

  //  cout << "Execute MDfragmentV1290::Init() " << endl;
  if ( dw.IsValid() ) {
    if (dw.GetDataType() != DWV1290_GlobalHeader ){ // The data doesn't start with a header
      cout << "WARNING in MDfragmentV1290::Init: 1st word is not a header" << endl;
      throw MDexception("ERROR in MDframentV1290::Init() : 1st word is not a header ***", MDexception::WARNING) ;
    } else {
      _boardID = dw.GetGeo();  // this should be the same as the one from equipment header
    }
    Validate();
  }
}

////////////////////////////////////////////////////////////////////////


