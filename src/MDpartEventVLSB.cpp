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

#include "MDpartEventVLSB.h"

MDpartEventVLSB::MDpartEventVLSB(void *d):MDdataContainer(d),_eventNumber(0),_nHits(0){
  Init();
}
void MDpartEventVLSB::SetDataPtr( void *d )
{
  MDdataContainer::SetDataPtr(d);
  Init();
}

void MDpartEventVLSB::Init(){
  //  cout << " Entering MDpartEventVLSB::Init() " << endl;
  unsigned int * ptr = Get32bWordPtr(0);
  MDdataWordVLSB dw(ptr);
  _nHits=0;
  UnValidate();
  if (dw.IsValid()) {
    _eventNumber = dw.GetEventNum();
    while (dw.GetEventNum() == _eventNumber) {
      if (dw.IsValid()) {
        _nHits++;
        // cout << " In Init : in event " << _eventNumber << " reading " << dw.GetEventNum() ;
        // cout << " ch " << dw.GetChannel() << " adc  " << dw.GetAdc() << endl;
        if (dw.GetAdc()==0) {
          cerr << "WARNING in MDpartEventVLSB::Init(): null Adc value " << endl;
        }
      } else {
        throw MDexception("ERROR in MDpartEventVLSB::Init(): Invalid data word");
      }
      dw.SetDataPtr(++ptr);
    }
    _size = 4*_nHits;
  }
  Validate();
}

bool MDpartEventVLSB::GetDiscriBit(unsigned int ih){
  if (ih<_nHits) {
    MDdataWordVLSB dw(Get32bWordPtr(ih));
    return dw.GetDiscriBit();
  } else {
    return 0;
  }
}

unsigned int MDpartEventVLSB::GetChannel(unsigned int ih){
  if (ih<_nHits) {
    MDdataWordVLSB dw(Get32bWordPtr(ih));
    return dw.GetChannel();
  } else {
    return 0;
  }
}

unsigned int MDpartEventVLSB::GetAdc(unsigned int ih){
  if (ih<_nHits) {
    MDdataWordVLSB dw(Get32bWordPtr(ih));
    return dw.GetAdc();
  } else {
    return 0;
  }
}

unsigned int MDpartEventVLSB::GetTdc(unsigned int ih){
  if (ih<_nHits) {
    MDdataWordVLSB dw(Get32bWordPtr(ih));
    return dw.GetTdc();
  } else {
    return 0;
  }
}

void MDpartEventVLSB::Dump(int atTheTime){
  if (IsValid()) {
    unsigned int wc(0);
    unsigned int * ptr = Get32bWordPtr( 0 );
    MDdataWordVLSB dw(ptr) ;
    while (wc < GetWordCount() ) {
      cout << dw;
      dw.SetDataPtr( ++ptr );
      wc++;
    }
  } else {
    cout << "**** ERROR: INVALID VLSB Particle Event ****" << endl;
  }
  return;
}

////////////////////////////////////////////////////////////////////////


