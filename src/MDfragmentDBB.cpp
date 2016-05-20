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

#include "MDfragmentDBB.h"

using namespace std;

void MDfragmentDBB::SetDataPtr( void *d, uint32_t aSize=0 ) {
  MDfragment::SetDataPtr(d, aSize);
}

void MDfragmentDBB::Init() {
  UnValidate();

  if (this->GetSize() == 0)
    return;

  uint32_t* ptr = Get32bWordPtr(0);
  MDdataWordDBB dw(ptr);

  //   cout << " Calling MDfragmentDBB::Init() " << endl;
  for (int ich=0 ; ich < DBB_NCHANNELS ; ich++) {
    _nLeadingEdgeHits[ich]=0;
    _nTrailingEdgeHits[ich]=0;
    _leadingEdgeHit[ich].clear();
    _trailingEdgeHit[ich].clear();
  }

  _madeOfParticles = false;
  if ( dw.IsValid() ) {
//     this->dumpEventVRB(Get32bWordPtr(0), GetSize());
    // Check the reliability of the header and decode the header information.
    if (dw.GetDataType() != MDdataWordDBB::Header ){ // The data doesn't start with a header
      throw MDexception("ERROR in MDframentDBB::Init() : 1st word is not a header") ;
    } else {
      _headerSize = DBB_HEADER_WORDS*WORD_SIZE;
      _trailerSize = DBB_TRAILER_WORDS*WORD_SIZE;
      _payLoadSize = GetSize() - _headerSize - _trailerSize;
      _header = ptr;
      _boardID = dw.GetGeo();
      _spillNum = dw.GetSpillNumber();
      dw.SetDataPtr( ++ptr );
      _triggerCount = dw.GetTriggerCount();
      _hitCount = dw.GetHitCount() ;
/*
      if ( (_hitCount*WORD_SIZE*2 - _payLoadSize) > 1 ) { // May not be always false, we can miss some trailing ege hits...
        cout << "ERROR in MDfragmentDBB()::Init(): Number of words : ";
        cout << _payLoadSize/WORD_SIZE << "  expected : " << _hitCount*2 << endl;
      }
*/
      _payLoad = _header + _headerSize/WORD_SIZE;
      _trailer = _payLoad + _payLoadSize/WORD_SIZE;
    }
    // Check the reliability of the trailer and decode the trailer information.
    ptr = _trailer;
    dw.SetDataPtr( ptr );
    if ( dw.IsValid() ) {
      if (dw.GetDataType()    != MDdataWordDBB::Trailer ||
          dw.GetGeo()         != _boardID      || //temporary disable this for debugging
          dw.GetSpillNumber() != _spillNum     ||
          dw.GetStatus() ) {// The data doesn't end with a proper trailer

//         this->dumpEventVRB(Get32bWordPtr(0), GetSize());
        throw MDexception("ERROR in MDframentDBB::Init() : DBB trailer is not consistent") ;
      } else {
        dw.SetDataPtr( ++ptr );
        _spillWidth = dw.GetSpillWidth();
      }
    } else {
      throw MDexception("ERROR in MDframentDBB::Init() : DBB trailer is not valid") ;
    }

    // Decode the payload.
    ptr = _payLoad;
    while(ptr != _trailer) {
      dw.SetDataPtr( ptr++ ); // don't need to check that the word is valid here since we know the trailer is valid
      unsigned int theCh = dw.GetChannelId();
      unsigned int theTime = dw.GetHitTime();
      switch( dw.GetDataType() ) {
        case MDdataWordDBB::TrailingMeas:
        {
          _nTrailingEdgeHits[ DBB_NCHANNELS ]++;
          _nTrailingEdgeHits[ theCh ]++;
          _trailingEdgeHit[ theCh ].push_back( theTime );

          break;
        }
        case MDdataWordDBB::LeadingMeas:
        {
          _nLeadingEdgeHits[ DBB_NCHANNELS ]++;
          _nLeadingEdgeHits[ theCh ]++;
          _leadingEdgeHit[ theCh ].push_back( theTime );
          //if( _testCallBack )
            //_testCallBack( dw.GetMeasurement() );
          break;
        }
        default:
        {
          throw MDexception("ERROR in MDframentDBB::Init() : Unexpected identifier") ;
        }
      }
    }
    Validate();
  }
}

unsigned int  MDfragmentDBB::GetHitMeasurement(unsigned int ih, unsigned int ich, char t){
  int rv = 0xFFFFFFFF ;
  if ( ich>DBB_NCHANNELS-1 ) {
    stringstream ss;
    ss << "ERROR in MDfragmentDBB::GetHitMeasurement() : ";
    ss << "Wrong argument: ch = " << ich;
    throw MDexception( ss.str() );
  }
 switch(t){
  case 'l':
    {
      if (ih<_nLeadingEdgeHits[ich]) { rv = _leadingEdgeHit[ich][ih]; }
      else {
        stringstream ss;
        ss << "ERROR in MDfragmentDBB::GetHitMeasurement() case l : ";
        ss << "Wrong argument: ih = " << ih;
        throw MDexception( ss.str() );
      }
      break;
    }
  case 't':
    {
      if (ih<_nTrailingEdgeHits[ich]) rv = _trailingEdgeHit[ich][ih];
      else {
        stringstream ss;
        ss << "ERROR in MDfragmentDBB::GetHitMeasurement() case t : ";
        ss << "Wrong argument: ih = " << ih;
        throw MDexception( ss.str() );
      }
      break;
    }
  default:
    {
      stringstream ss;
      ss << "ERROR in MDfragmentDBB::GetHitMeasurement() : ";
      ss << "Wrong argument: t = " << t;
      throw MDexception( ss.str() );
    }
  }

  return rv;
}

void MDfragmentDBB::Dump( int atTheTime )
{
  if (IsValid()) {
    cout << "**** DBB  ****" << endl;
    cout << "Board Id      : " << _boardID << endl;
    cout << "Spill Num     : " << _spillNum << endl;
    cout << "Trigger Count : " << _triggerCount << endl;
    cout << "Hit Count     : " << _hitCount << endl;
    //cout << "Payload Size  : " << _payLoadSize << endl;
    cout << "Spill Width   : " << _spillWidth << endl;
    uint32_t* ptr = _payLoad;
    while(ptr != _trailer) {
      MDdataWordDBB dw( ptr++ );
      long32 dt= dw.GetDataType();
      switch(dt){
      case  MDdataWordDBB::LeadingMeas:
        cout << "Leading edge Measurement : ";
        break;
      case  MDdataWordDBB::TrailingMeas:
        cout << "Trailing edge Measurement : ";
        break;
      default:
        cout << "Invalid Measurement!";
        return;
        break;
      }
      cout<< dw.GetHitTime() <<" ; channel : "<< dw.GetChannelId() <<endl;
    }
  }
}




void MDfragmentDBB::dumpEventVRB(uint32_t *data, int nbr) {

  int type = (*data & 0xf0000000) >> 28;
  int id   = (*data & 0x0fc00000) >> 22;
  int hit  = *data & 0x1fff;
//   int zero = (*data & 0x3f0000) >> 16;
  int status;
  cout << "----------------------------------------------------------------" << endl;
  cout << "header type: "<< hex << type << dec << "    board id: " << id << "    spill number: " << hit << endl;
//   cout << "zeros: " << zero << endl;
  data++;

  int tr_count = (*data & 0x3ff0000) >> 16;
  int h_count  = *data & 0xffff;
  int hc = (nbr/4) - 4;
  cout << "trigger count: "<< tr_count << "    hit count:" << h_count
            << " (" << hc << ")" << endl;
  cout << "ndw: " << h_count*2 + 4 << " / " << (nbr/4) << endl;
  cout << " ----------------------------------------------------------------" << endl;
  data++;
//   for (int i=0; i<(h_count*2); i++) {
   for (int i=0; i<hc; i++) {
     type = (*data & 0xf0000000) >> 28;
     id   = (*data & 0x0fc00000) >> 22;
     hit  = *data & 0x3fffff;
     status  = (*data & 0x3f0000) >> 16;
     cout << " " << i << "    hit type: " << hex << type << dec << "    ch id: " << id << "    hit time: " << hit;
     if(type==0xe)
       cout << "    status: " << status;

     cout << endl;
     data++;
   }

  type    = (*data & 0xf0000000) >> 28;
  id      = (*data & 0x0fc00000) >> 22;
  status  = (*data & 0x3f0000) >> 16;
  hit      = *data & 0xffff;
  cout << "----------------------------------------------------------------" << endl;
  cout << "trailer type: "<< hex << type << dec << "    board id: " << id
       << "    status: " << hex << status << dec << "    spill number: " << hit << endl;
  data++;

  cout << "spill width: " << *data << endl;
  cout << "----------------------------------------------------------------" << endl << endl;
}

