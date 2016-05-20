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


#include "MDdataWordV1731.h"

MDdataWordV1731::MDdataWordV1731(void *d):MDdataWord(d) {
}

void MDdataWordV1731::Dump(int atTheTime){
  cout << *this;
  return;
} 

////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &s,MDdataWordV1731 &dw){
//long32 dt = dw.GetDataType();
  if (  !dw.IsValid() ) {
    s << " INVALID ( " << showbase << hex << dw.Get32bWordPtr()[0] << " ) " ;
  }
  s << " CAEN V1731 Data: " << dec 
    << dw.GetSample(0) << " ; " <<  dw.GetSample(1) << " ; " 
    << dw.GetSample(2) << " ; " <<  dw.GetSample(3) << endl; 
  s << dec << noshowbase;
  return s;
}

