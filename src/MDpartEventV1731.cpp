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

#include "MDpartEventV1731.h"

MDpartEventV1731::MDpartEventV1731(void *d):MDdataContainer(d){
  UnValidate();
  for (unsigned int ch=0; ch < V1731_NCHANNELS ; ch++) {
    _sequence[ch]=NULL;
  }
  Init();
}

void MDpartEventV1731::Init(){
  // cout << "Init particle event V1731" << endl;
  UnValidate();
  unsigned int * header = Get32bWordPtr(0);
  //cout << *header << endl;
  int index;
  unsigned int length;
  if (header) {
    if ( (header[DWV1731_Header_Sync_Word] & DWV1731_Header_Sync_Mask) == DWV1731_Header_Sync ) {
      Validate();
      SetSize( GetWordCount() * V1731_WORD_SIZE );
      //cout << "Size: " << dec << 4*GetWordCount() << " ( " << hex << showbase << 4*GetWordCount() << " ) " <<dec<< endl;
      //cout << "ZS: " << GetZS() << endl;
      // find the number of non null bit in the channel mask:
      _nChannels = 0;
      for (unsigned int ch=0; ch < V1731_NCHANNELS ; ch++) {
        _nChannels += (GetChannelMask()>>ch) & 0x00000001;
      }
      switch( GetZS() ) {
        case 0:
        { // No zero length encoding. Life is easy.
          length = (GetWordCount() - V1731_HEADER_WORDS)/_nChannels;
          for (unsigned int ch=0; ch < V1731_NCHANNELS ; ch++) {
            _length[ch] = ((GetChannelMask()>>ch) & 0x00000001)*length;
            if ( _length[ch] ){
              _sequence[ch] = Get32bWordPtr(V1731_HEADER_WORDS + ch*length);
              if( _testCallBack ) _testCallBack( _sequence[ch][10] );
            }
            else _sequence[ch] = NULL;
          }
          break;
        }
        case 1:
        { // Zero length encoding. That's a pain
          index = V1731_HEADER_WORDS;
          for (unsigned int ch=0; ch < V1731_NCHANNELS ; ch++) {
            // Not tested !
            // what if some channels are disabled ?
            _sequence[ch] = Get32bWordPtr(index);
            _length[ch] += _sequence[ch][0];
            index +=_length[ch];
          }
          break;
        }
        case 7 :
        {
          index = V1731_HEADER_WORDS;
          for (unsigned int ch=0; ch < V1731_NCHANNELS ; ch++) {
            unsigned int * ch_ptr = Get32bWordPtr(index);
            unsigned int ch_code = (*ch_ptr & DWV1731_Header_ZS_Channel_Mask) >> DWV1731_Header_ZS_Channel_Shift;
            int l = *ch_ptr & DWV1731_Header_ZS_Length_Mask;
            if(ch!=ch_code)
              throw MDexception("ERROR in MDpartEventV1724::Init: Channel number mismatch in ZS mode.");

            if(l)_sequence[ch] = Get32bWordPtr(index+1);
            _length[ch] = l;
            index += l+1;
          }
          break;
        }
        default:
        { // Invalid data structure
          throw MDexception("ERROR in MDpartEventV1731::Init: Unexpected ZS code.");
        }
      }
    } else {
      throw MDexception("ERROR in MDpartEventV1731::Init: INVALID particle Event");
      //cout << "Data Word : " << hex << showbase << Get32bWordPtr(0) << dec << noshowbase << endl;
    }
  }
}

void MDpartEventV1731::SetDataPtr( void *d ) {
  MDdataContainer::SetDataPtr(d);
  Init();
}

unsigned int MDpartEventV1731::GetWordCount(){
  if (IsValid()) {
  unsigned int * header = Get32bWordPtr(0);
  return ( header[DWV1731_Header_WordCount_Word] & DWV1731_Header_WordCount_Mask ) >> DWV1731_Header_WordCount_Shift;
  } else {
    return 0;
  }
}

unsigned int MDpartEventV1731::GetGeo(){
  if (IsValid()) {
  unsigned int * header = Get32bWordPtr(0);
  return ( header[DWV1731_Header_Geo_Word] & DWV1731_Header_Geo_Mask ) >> DWV1731_Header_Geo_Shift;
  } else {
    return 0;
  }
}

int MDpartEventV1731::GetZS(){
  if (IsValid()) {
  unsigned int * header = Get32bWordPtr(0);
  return ( header[DWV1731_Header_ZS_Word] & DWV1731_Header_ZS_Mask ) >> DWV1731_Header_ZS_Shift;
  } else {
    return -1;
  }
}

unsigned int MDpartEventV1731::GetPattern(){
  if (IsValid()) {
  unsigned int * header = Get32bWordPtr(0);
  return ( header[DWV1731_Header_Pattern_Word] & DWV1731_Header_Pattern_Mask ) >> DWV1731_Header_Pattern_Shift;
  } else {
    return 0;
  }
}

unsigned int MDpartEventV1731::GetChannelMask(){
  if (IsValid()) {
  unsigned int * header = Get32bWordPtr(0);
  return ( header[DWV1731_Header_ChannelMask_Word] & DWV1731_Header_ChannelMask_Mask ) >> DWV1731_Header_ChannelMask_Shift;
  } else {
    return 0;
  }
}

unsigned int MDpartEventV1731::GetEventCounter(){
  if (IsValid()) {
  unsigned int * header = Get32bWordPtr(0);
  return ( header[DWV1731_Header_EventCounter_Word] & DWV1731_Header_EventCounter_Mask ) >> DWV1731_Header_EventCounter_Shift;
  } else {
    return 0;
  }
}

unsigned int MDpartEventV1731::GetTriggerTimeTag(){
  if (IsValid()) {
  unsigned int * header = Get32bWordPtr(0);
  return ( header[DWV1731_Header_TriggerTimeTag_Word] & DWV1731_Header_TriggerTimeTag_Mask ) >> DWV1731_Header_TriggerTimeTag_Shift;
  } else {
    return 0;
  }
}

int16_t MDpartEventV1731::GetSampleData( unsigned short aChannel , unsigned long aSample ) {
  if ( aChannel >= V1731_NCHANNELS ) return 0;
  if ( aSample >= GetLength(aChannel)*4 ) return 0;
  if ( _length[aChannel] == 0 ) return 0;
  MDdataWordV1731 dw1731;
  if ( _sequence[aChannel] ) {
    dw1731.SetDataPtr( &_sequence[aChannel][aSample/4] );
    if ( dw1731.IsValid() ) return dw1731.GetSample(aSample%4);
  }
  return 0;
}

void MDpartEventV1731::Dump(int atTheTime){
  cout << *this;

  return;
}

////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &s,MDpartEventV1731 &dw){
  MDdataWordV1731 dw1731;

  s << showbase << hex;
  s << " ------------   CAEN V1731 Header    ------------ " << endl ;
  s << " Word Count        : " << dec << dw.GetWordCount() << endl;
  s << " Geo               : " << dw.GetGeo() ;
  int zs = dw.GetZS();
  if ( zs ) s << " ; ZLE enabled  (" << zs << ")";
  else s << " ; ZLE disabled" ;
  s << " ; Channel Mask : "    << showbase << hex << dw.GetChannelMask() << endl;
  s << " Event Counter     : " << dec << dw.GetEventCounter() << endl;
  s << " Trigger Time Tag  : " << dw.GetTriggerTimeTag() << endl;
  s << " ------------ End of CAEN V1731 Header ----------- " << endl ;

  //  unsigned int nSamplePerChannel = 512;
  //  unsigned int nWordPerChannel   = nSamplePerChannel/2 ; // two samples per word

  if ( dw.GetZS() == 0 ) {
    unsigned int expectWordCount( dw.GetNChannels()* dw.GetLength(0) + V1731_HEADER_WORDS );
    if ( expectWordCount != dw.GetWordCount() ) {
      s << " *** ERROR in CAEN V1731 data format: Word count is not consistent *** " << endl ;
    }

    for (unsigned int ch=0; ch < V1731_NCHANNELS ; ch++) {
      s << " ----------   Channel " << dec << ch << " ( Length = " << dw.GetLength(ch) << " ) ---------- " << endl ;
      for ( unsigned int iw=0; iw <dw.GetLength(ch) ; iw++ ) {
        unsigned int iww = ch * dw.GetLength(ch) + iw;
        unsigned int* ptr = dw.Get32bWordPtr(V1731_HEADER_WORDS + iww );
        dw1731.SetDataPtr(ptr);
        dw1731.Dump() ;
      }
    }
    /*
    //  Exemple for using MDpartEventV1731::GetSampleData();
    cout << " ****** Alternative dump ****** " << endl;
    for (unsigned int ch=0; ch < V1731_NCHANNELS ; ch++) {
      cout << " AD ------- Channel " << dec << ch << " ( Length = " << dw.GetLength(ch) << " ) -------- " << endl ;
      for (unsigned int s=0; s < dw.GetLength(ch)*2 ; s++) {
	cout << dw.GetSampleData(ch,s) << " ; " ;
	if ((s+1)%8 == 0) cout << endl;
      }
    }
    */

  } else if (dw.GetZS() == 7) {
    unsigned int iw_count = V1731_HEADER_WORDS;
    for (unsigned int ch=0; ch < V1731_NCHANNELS ; ch++) {
      s << " ----------   Channel " << dec << ch << " ( Length = " << dw.GetLength(ch) << " ) ---------- " << endl ;
      iw_count++;
      if ( dw.GetLength(ch) ) {
        for ( unsigned int iw=0; iw <dw.GetLength(ch) ; iw++ ) {
          unsigned int* ptr = dw.Get32bWordPtr(iw_count + iw );
          dw1731.SetDataPtr(ptr);
          dw1731.Dump() ;
        }
        iw_count += dw.GetLength(ch);
      }
    }
  }
  s << dec << noshowbase ;
  return s;
}

