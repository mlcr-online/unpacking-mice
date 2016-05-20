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

#include "MDfragmentVLSB.h"

MDfragmentVLSB::MDfragmentVLSB(void *d):MDfragment(d){ 
  //  cout << " Creating a MDfragmentVLSB ..." << endl; 
  _partEventPtr = new MDpartEventVLSB(0);
  Init();
}

void MDfragmentVLSB::SetDataPtr( void *d, uint32_t aSize ) {
  MDfragment::SetDataPtr(d,aSize);
 // The dynamically correct Init() is called by MDfragment::SetDataPtr(d,aSize)
}

void MDfragmentVLSB::Init(){
  //  cout << "MDfragmentVLSB::Init()" << endl;
  uint32_t * ptr = Get32bWordPtr(0);
  MDdataWordVLSB dw(ptr);
  UnValidate();
//   _madeOfParticles = true;
  _madeOfParticles = false;

  // Checking the first word to start with
  if ( dw.IsValid() ) {
    Validate();
    // There is nothing else we can do here.
  }
}

unsigned int MDfragmentVLSB::GetEventNum(unsigned int ih){
  if (ih<GetPayLoadWordCount()) {
    MDdataWordVLSB dw(Get32bWordPtr(ih));
    return dw.GetEventNum();
  } else {
    return 0;
  }
}

bool MDfragmentVLSB::GetDiscriBit(unsigned int ih){
  if (ih<GetPayLoadWordCount()) {
    MDdataWordVLSB dw(Get32bWordPtr(ih));
    return dw.GetDiscriBit();
  } else {
    return 0;
  }
}

unsigned int MDfragmentVLSB::GetChannel(unsigned int ih){
  if (ih<GetPayLoadWordCount()) {
    MDdataWordVLSB dw(Get32bWordPtr(ih));
    return dw.GetChannel();
  } else {
    return 0;
  }
}

unsigned int MDfragmentVLSB::GetAdc(unsigned int ih){
  if (ih<GetPayLoadWordCount()) {
    MDdataWordVLSB dw(Get32bWordPtr(ih));
    return dw.GetAdc();
  } else {
    return 0;
  }
}

unsigned int MDfragmentVLSB::GetTdc(unsigned int ih){
  if (ih<GetPayLoadWordCount()) {
    MDdataWordVLSB dw(Get32bWordPtr(ih));
    return dw.GetTdc();
  } else {
    return 0;
  }
}

////////////////////////////////////////////////////////////////////////


