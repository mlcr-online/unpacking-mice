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

#include "MDfragment.h"

void MDfragment::SetDataPtr( void *d, uint32_t aSize ) {
  _payLoad = (uint32_t*)d;
  _payLoadSize = aSize;
  _partEventVector.clear();

  try{
    MDdataContainer::SetDataPtr(d);
    SetSize(aSize);
    Init(); // This Init() will by dynamically asigned
  }
  catch ( MDexception & lExc) {
    stringstream ss;
    ss << lExc.GetDescription() << std::endl;
    ss << "*** Unpacking exception in MDfragment::SetDataPtr(void*,  uint32_t)" << endl;
    ss << "while processing " << this->GetName() << " fragment (board Id: " << this->GetGeo() << ")";
    throw  MDexception( ss.str() );
  }
}

void* MDfragment::GetPartEventDataPtr(int ipe){ 
  if ((uint32_t)ipe < GetNPartEvents() ) {
    return _partEventVector[ipe].first;
  } else {
    throw MDexception("ERROR in MDfragment::GetPartEventDataPtr(int ) : index larger than the size of the vecor"); 
    return 0;
  } 
}

MDdataContainer* MDfragment::GetPartEventPtr(int ipe) {
  if (ipe==-1) return  _partEventPtr; // by default, if no argument is given, return the pointer not initialised.
  try {
    if (_partEventPtr) _partEventPtr->SetDataPtr(GetPartEventDataPtr(ipe));
    // The  SetDataPtr() function is asigned dynamically
    return _partEventPtr;  // This pointer can be statically casted to the appropriate MDpartEventXXX*
  }
  catch ( MDexception & lExc) {
    stringstream ss;
    ss << lExc.GetDescription() << std::endl;
    ss << "*** Unpacking exception in MDfragment::GetPartEventPtr(int)" << endl;
    throw  MDexception( ss.str() );
  }
  return 0;
}

uint32_t MDfragment::GetPartEventSize(int ipe){ 
  if ((uint32_t)ipe < GetNPartEvents() ) {
    return _partEventVector[ipe].second; 
  } else {
    throw MDexception("ERROR in MDfragment::GetPartEventSize(int ) : index larger than the size of the vecor."); 
    return 0;
  } 
}

string MDfragment::GetName(){
  return  MDequipMap::GetName(_equipType);
}

uint32_t MDfragment::InitPartEventVector(){
  // If a size has been provided, we can apply the generic algorithm
  // assuming the particle events start at the beginning.
  // This function only fills the vector of pointers to the first word of each particle event.
  //  cout << "Calling generic InitPartEventVector() " << endl;
  if ( _madeOfParticles ) {
    if (_size) {
      unsigned long32 npe(0);
      try {
        _partEventVector.clear();
        unsigned char* ptr = _data;
        if (_partEventPtr) { // _partEventPtr is assumed to be set dynamically to point to the appropriate partEventXXX
          while (ptr < _data + _size ) { // while we have not reached the end of the Fragment
            _partEventPtr->SetDataPtr( ptr );
            if ( _partEventPtr->IsValid() ) {
              npe++;
//              cout << "In InitPartEventVector() for equipment " << this->GetName() << " (board Id: " << this->GetGeo() << ")"
//                   << "\n Adding particle event with ptr: " << ptr << " and size : " << dec
//                   << _partEventPtr->GetSize() << endl;
              _partEventVector.push_back(make_pair(ptr,_partEventPtr->GetSize()));
              ptr += _partEventPtr->GetSize();
            }
          }
        }
      }
      catch ( MDexception & lExc) {
        stringstream ss;
        ss << lExc.GetDescription() << std::endl;
        ss << "*** Unpacking exception in MDfragment::InitPartEventVector" << endl;
        ss << "while processing " << this->GetName() << " fragment (board Id: " << this->GetGeo() << ")";
        throw  MDexception( ss.str() );
      }
      return npe;
    }
  }
  return 0;
}

void MDfragment::Init(){
  // impossible to know the size of the fragment before we know how it is structured 
  // This function is purely virtual 
  //  cout << " Executing MDfragment::Init() ..." << endl; 
}

void MDfragment::Dump(int atTheTime){
  if (IsValid() && _size) {
    if ( _madeOfParticles) { // in this case, we can just loop over the particles events
                             // assuming that InitPartEventVector() has been called before
      uint32_t nPart(GetNPartEvents());
      cout << "**** " << nPart << " Particle Events in Equipment " ;
      cout <<  GetName() << " ****" << endl;
      // Loop over all the particle events and dump
      for ( uint32_t ipe=0; ipe < nPart; ipe++) {
        _partEventPtr->SetDataPtr(GetPartEventDataPtr(ipe));
        _partEventPtr->Dump();
      }
    } else { // only the size has been provided, we can only dump 32 bits words without decoding 
      // A virtual Dump() should be provided in the daughter class
      MDdataContainer::Dump(4);
    }
  }
};

