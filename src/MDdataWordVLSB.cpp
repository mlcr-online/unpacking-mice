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

#include "MDdataWordVLSB.h"

MDdataWordVLSB::MDdataWordVLSB(void *d):MDdataWord(d){}

bool   MDdataWordVLSB::IsValid(){
  if (!_data) return false;
  if (Get32bWordPtr()[0]==MDdataWordVLSB::Error_Word) return false;
  //  if ((Get32bWordPtr()[0] & MDdataWordVLSB::Adc_Mask) == 0) return false; // the bank length word can also be a valid word.
  return true;
}

void MDdataWordVLSB::Dump(int atTheTime){
  cout << *this;
  return;
} 

////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &s,MDdataWordVLSB &dw){
  s << " VLSB Word : ";
  if (dw.IsValid()) {
    s << "Event: " << dw.GetEventNum() << " ; Channel: " << dw.GetChannel() 
      << " ; Adc: " << dw.GetAdc() ;
    if (dw.GetDiscriBit()) {
      s << " (over thr) ";
    } else {
      s << " (under thr) ";
    }
    s << " ; Tdc: " << dw.GetTdc() << endl ;
  } else {
    s << "INVALID : " << showbase << hex << dw.Get32bWordPtr()[0] << ")" << dec << noshowbase << endl;  
  }
  return s;
}

