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

#include "MDfragmentV830.h"

using namespace std;

MDfragmentV830::MDfragmentV830(void *d):MDfragment(d){
  Init();
}

void MDfragmentV830::SetDataPtr( void *d, uint32_t aSize=0 ) {
  MDfragment::SetDataPtr(d, aSize);
}

void MDfragmentV830::Init(){
  uint32_t * ptr = Get32bWordPtr(0);
  MDdataWordV830 dw(ptr);
  UnValidate();
  // cout << " Calling MDfragmentV830::Init() " << endl;
  _madeOfParticles = false;
  if ( dw.IsValid() ) {
    if (dw.GetDataType() == DWV830_Header ){
      _header = ptr;
      _headerSize = WORD_SIZE;
      // cout << " V830 header size = " << dec << _headerSize << endl;
      _boardID = dw.GetGeo(); // this should be the same as the one from equipment header
      // cout << " Geo Address " << dec << _boardID << endl;
      _payLoad = ++ptr;
      _payLoadSize = dw.GetWordCount() * WORD_SIZE; // the header is not included in the word count
      // cout << " V830 payload size = " << dec << _payLoadSize << endl;
      _ts = dw.GetTriggerType();
      // cout << " V830 Trigger Type = " << dec << _ts << endl;
      _triggerCount = dw.GetTriggerCount();
      // cout << " V830 Trigger Count = " << dec << _triggerCount << endl;
      if (GetSize() != _headerSize + _payLoadSize) {
        throw MDexception("ERROR in MDframentV830::Init() : Data Size not consistent.") ;	
      }
    } else { // The data doesn't start with a header
      throw MDexception("ERROR in MDframentV830::Init() : 1st word is not a header.") ;
    }
    if ( _triggerCount != 0 ){
      throw MDexception("ERROR in MDframentV830::Init() : More than 1 trigger count.") ;
    }
    Validate();
    if( _testCallBack ) _testCallBack( GetMeasurement(1) );
  }
  // cout << " Done MDfragmentV830::Init() " << endl;
}

// The 2 functions below assume single event structure
uint32_t MDfragmentV830::GetChannel(unsigned int iw)
{
  if (iw < GetPayLoadWordCount() ) {
    unsigned int * ptr = UserPayLoadPtr();
    MDdataWordV830 dw(&ptr[iw]);
    return dw.GetChannel();
  } else {
    throw MDexception("ERROR in MDframentV830::GetChannel() : index lager than data length.") ;
  }
}

uint32_t MDfragmentV830::GetMeasurement(unsigned int iw)
{
  if (iw < GetPayLoadWordCount() ) {
    unsigned int * ptr = UserPayLoadPtr();
    MDdataWordV830 dw(&ptr[iw]);
    return dw.GetMeasurement();
  } else {
    throw MDexception("ERROR in MDframentV830::GetMeasurment() : index lager than data length.") ;
  }
}


void MDfragmentV830::Dump(int atTheTime)
{
  if (IsValid()) {
    cout << "**** CAEN V830  ****" << endl;
    unsigned int wc(0);
    unsigned int * ptr = Get32bWordPtr( 0 );
    MDdataWordV830 dw(ptr) ;
    while (wc < GetWordCount() ) {
      cout << dw ;
      dw.SetDataPtr( ++ptr );
      wc++;
    }
  }
  return;
}

////////////////////////////////////////////////////////////////////////


