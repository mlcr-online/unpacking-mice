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

#include "MDfragmentVLSB_C.h"

MDfragmentVLSB_C::MDfragmentVLSB_C(void *d):MDfragment(d){ 
  //  cout << " Creating a MDfragmentVLSB_C ..." << endl; 
  Init();
}

void MDfragmentVLSB_C::SetDataPtr( void *d, uint32_t aSize ) {
  MDfragment::SetDataPtr(d,aSize);
 // The dynamically correct Init() is called by MDfragment::SetDataPtr(d,aSize)
}

void MDfragmentVLSB_C::Init(){
  uint32_t * ptr = Get32bWordPtr(0);
  MDdataWordVLSB dw(ptr);
  //  cout << " Executing MDfragmentVLSB_C::Init() ..." << endl; 
  UnValidate();
  _madeOfParticles = true;
  if ( dw.IsValid() ) {
    _header = ptr;  
    _headerSize = 4*WORD_SIZE;
    _bankLength[0]=dw.GetBankLength(0);
    _bankLength[1]=dw.GetBankLength(1);
    _bankLength[2]=dw.GetBankLength(2);
    _bankLength[3]=dw.GetBankLength(3);

    _payLoadSize = (_bankLength[0] + _bankLength[1] + _bankLength[2] + _bankLength[3]) * WORD_SIZE; 
    if (_payLoadSize)
      _payLoad = Get32bWordPtr(4);

    if ( _size != _headerSize + _payLoadSize ) {
      throw MDexception("ERROR in MDframentVLSB_C::Init() : Size mismatch ") ; 
    }
    Validate();
  } else { // This is the normal case when the fragment is created the first time for the map. 
           // However, when the data pointer is not null, the first word should be a header 
           // Unfortunately, there is no particular structure for the header word so there is now
    // to check that the data is valid => Do nothing here. 
    // cout << "WARNING in MDfragmentVLSB_C::Init: 1st word is not a header" << endl;   
    // throw MDexception("ERROR in MDframentVLSB_C::Init() : 1st word is not a header ***", MDexception::WARNING) ; 
  }
}


void MDfragmentVLSB_C::Dump(int atTheTime)
{
  if (IsValid()) {
    cout << "**** VLSB Cosmic data  ****" << endl;
    unsigned int wc(0);
    unsigned int * ptr = Get32bWordPtr( 0 );
    MDdataWordVLSB dw(ptr) ;

    cout << " Bank Lengths : " ;
    cout << dw.GetBankLength(0) << " ; " ;
    cout << dw.GetBankLength(1) << " ; " ;
    cout << dw.GetBankLength(2) << " ; " ;
    cout << dw.GetBankLength(3) << endl;

    ptr = UserPayLoadPtr();
    dw.SetDataPtr(ptr);
    while (wc < GetPayLoadWordCount() ) {
      cout << "Event: " << dw.GetEventNum() << " ; Channel: " << dw.GetChannel() << " ; Adc: " << dw.GetAdc() << endl;
      dw.SetDataPtr( ++ptr );
      wc++;
    }  
  }
  return;
} 

unsigned int MDfragmentVLSB_C::GetBankLength(int iBank)
{
  if ((unsigned int)iBank < 4) {return _bankLength[(unsigned int)iBank];}
  else {
    throw MDexception("ERROR in MDfragmentVLSB::GetBankLength(int) : Index exceeding range") ;
  }
  return 0;
}

uint32_t* MDfragmentVLSB_C::GetDataWordPtr(int aI)
{
  uint32_t index((uint32_t)aI);
  if (index > _payLoadSize) {
    throw MDexception("ERROR in MDfragmentVLSB::GetDataWordPtr(int) : Index exceeding range") ;
  }
  return &_payLoad[index];
}

////////////////////////////////////////////////////////////////////////


