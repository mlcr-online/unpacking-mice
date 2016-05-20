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

#include "MDpartEventV1290.h"
#include "MDexception.h"

MDpartEventV1290::MDpartEventV1290(void *d):MDdataContainer(d),_wordCount(0),_geo(0),_triggerTimeTag(0){
  Init();
}

void MDpartEventV1290::Init(){
  unsigned int * ptr = Get32bWordPtr(0);
  unsigned int unit(0);
  MDdataWordV1290 dw(ptr);
  UnValidate();
  _wordCount = 0 ;
  for (int ich=0 ; ich < V1290_NCHANNELS ; ich++) {
    _nLeadingEdgeHits[ich]=0;
    _nTrailingEdgeHits[ich]=0;
    _leadingEdgeHit[ich].clear();
    _trailingEdgeHit[ich].clear();
  }
  _nLeadingEdgeHits[V1290_NCHANNELS]=0;
  _nTrailingEdgeHits[V1290_NCHANNELS]=0;
  if ( dw.IsValid() ) {
    if ( dw.GetDataType() == DWV1290_GlobalHeader ) {
      _geo = dw.GetGeo();
      _wordCount++;
      dw.SetDataPtr(++ptr);
      while ( dw.GetDataType() != DWV1290_GlobalTrailer) {
        switch ( dw.GetDataType() ) {
          case DWV1290_Measurement:
          { // Normal case
            // Fill the hit vectors, depending on the measurement type
            int theCh = dw.GetChannel();
            _unitWordCount++;
            switch( dw.GetMeasurementType() ){
              case DWV1290_TrailingMeas:
              {
                _nTrailingEdgeHits[V1290_NCHANNELS]++;
                _nTrailingEdgeHits[theCh]++;
                _trailingEdgeHit[theCh].push_back(dw.GetMeasurement());
                break;
              }
              case DWV1290_LeadingMeas:
              {
                _nLeadingEdgeHits[V1290_NCHANNELS]++;
                _nLeadingEdgeHits[theCh]++;
                _leadingEdgeHit[theCh].push_back(dw.GetMeasurement());
                if( _testCallBack )
                _testCallBack( dw.GetMeasurement() );
                break;
              }
              default:
              {
                stringstream ss;
                ss << "ERROR in MDpartEventV1290::Init(): ";
                ss << "Unknown V1290 Measurement Type. ";
                throw MDexception( ss.str() );
              }
            }
            break;
          }
          case DWV1290_TdcHeader:
          {
            unit =  dw.GetTdc();
            _bunchID[unit]= dw.GetBunchID();
            _eventID[unit]= dw.GetEventID();
            _unitWordCount = 1;
            break;
          }
          case DWV1290_TdcTrailer:
          {
            _unitWordCount++;
            //if ( _unitWordCount != dw.GetWordCount() ||
            // It happens sometimes that the TDC error comes after the trailer
            // but is still counted as a word
            if ( _eventID[unit] != dw.GetEventID()
                 || unit !=  dw.GetTdc() ) {
              stringstream ss;
              ss << "ERROR in MDpartEventV1290::Init(): ";
              ss << "TDC trailer is not consistent.";
              throw MDexception( ss.str() );
              /*
              cout << "ERROR in MDpartEventV1290::Init: "
                   << "TDC trailer is not consistent" << endl;
              cout << _unitWordCount <<  " != " << dw.GetWordCount()
                   << " || " << _eventID[unit] << " != " << dw.GetEventID()
                   << " || " << unit << " != " <<  dw.GetTdc() << endl;
              */
            }
            break;
          }
          case DWV1290_TdcError:
          {
            _unitWordCount++;
            stringstream ss;
            ss << "ERROR in MDpartEventV1290::Init(): ";
            ss << "Unexpected TDC Error";
            throw MDexception( ss.str() );
          }
          case DWV1290_TriggerTimeTag:
          {
            _unitWordCount++;
            _triggerTimeTag = dw.GetTriggerTimeTag();
            break;
          }
          case DWV1290_Filler:
          {
            stringstream ss;
            ss << "ERROR in MDpartEventV1290::Init(): ";
            ss << "Unexpected Data Filler";
            throw MDexception( ss.str() );
          }
          default:
          {
            stringstream ss;
            ss << "ERROR in MDpartEventV1290::Init(): ";
            ss << "Unknown data Type !";
            throw MDexception( ss.str() );
            /*
            cout << " ERROR in MDpartEventV1290::Init: "
                 << "Unknown data Type !" << endl
                 << " Data value : " << hex << showbase << *ptr  << dec << noshowbase << endl
                 << " It looks like there is a global trailer missing " << endl
                 << " That is serious error. Call the expert" << endl;
            */
          }
        }
        _wordCount++;
        dw.SetDataPtr(++ptr);
      }
      if ( dw.GetDataType() == DWV1290_GlobalTrailer ) {
        _wordCount++;
        if ( _wordCount == dw.GetWordCount() && _geo == dw.GetGeo()) {
          Validate();
          SetSize( _wordCount*4 );
          _tdcStatus = dw.GetStatus();
        } else throw MDexception("ERROR in MDpartEventV1290::Init: Word count not consistent");
      }
    } else throw MDexception("ERROR in MDpartEventV1290::Init: 1st word is not global header");
  }
}
unsigned int MDpartEventV1290::GetBunchID(unsigned int unit)
{
  if (unit<4) {
    return _bunchID[unit];
  } else {
    return (_bunchID[0]+_bunchID[1]+_bunchID[2]+_bunchID[3])/4;
  }

}

void MDpartEventV1290::SetDataPtr( void *d )
{
  MDdataContainer::SetDataPtr(d);
  Init();
}

unsigned int MDpartEventV1290::GetEventCount()
{
  if (IsValid()) {
    MDdataWordV1290 dw( Get32bWordPtr(0) );
    return dw.GetEventCount();
  } else {
    return 0;
  }
}

unsigned int MDpartEventV1290::GetWordCount(){
  if (IsValid()) {
    return _size/4;
  } else {
    return 0;
  }
}

unsigned int MDpartEventV1290::GetStatus(){
  if (IsValid()) {
    return _tdcStatus;
  } else {
    return 0;
  }
}

unsigned int MDpartEventV1290::GetNHits(unsigned int ich,char t){
  unsigned int nh(0);
  if ( ich>V1290_NCHANNELS ) {
    stringstream ss;
    ss << "ERROR in MDpartEventV1290::GetNHits(): ";
    ss << "Wrong argument: ch = " << ich;
    throw MDexception( ss.str() );
  }
  switch(t){
  case 'a':
    {
      nh = _nLeadingEdgeHits[ich] + _nTrailingEdgeHits[ich] ;
      break;
    }
  case 'l':
    {
      nh = _nLeadingEdgeHits[ich];
      break;
    }
  case 't':
    {
      nh = _nTrailingEdgeHits[ich];
      break;
    }
  default:
    {
      stringstream ss;
      ss << "ERROR in MDpartEventV1290::GetNHits(): ";
      ss << "Wrong argument: t = " << t;
      throw MDexception( ss.str() );
    }
  }

  return nh;
}


unsigned int  MDpartEventV1290::GetHitMeasurement(unsigned int ih, unsigned int ich, char t){
  int rv = 0xFFFFFFFF ;
  if ( ich>V1290_NCHANNELS-1 ) {
    stringstream ss;
    ss << "ERROR in MDpartEventV1290::GetHitMeasurement() : ";
    ss << "Wrong argument: ch = " << ich;
    throw MDexception( ss.str() );
  }
  switch(t){
    case 'l':
    {
      if (ih<_nLeadingEdgeHits[ich]) { rv = _leadingEdgeHit[ich][ih]; }
      else {
        stringstream ss;
        ss << "ERROR in MDpartEventV1290::GetHitMeasurement() case l : ";
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
        ss << "ERROR in MDpartEventV1290::GetHitMeasurement() case t : ";
        ss << "Wrong argument: ih = " << ih;
        throw MDexception( ss.str() );
      }
      break;
    }
    default:
    {
      stringstream ss;
      ss << "ERROR in MDpartEventV1290::GetHitMeasurement() : ";
      ss << "Wrong argument: t = " << t;
      throw MDexception( ss.str() );
    }
  }

  return rv;
}

void MDpartEventV1290::Dump(int atTheTime){
  if (IsValid()) {
    unsigned int wc(0);
    unsigned int * ptr = Get32bWordPtr( 0 );
    MDdataWordV1290 dw(ptr) ;
    while (wc < GetWordCount() ) {
      cout << dw;
      dw.SetDataPtr( ++ptr );
      wc++;
    }
    // Alternative method follows (more appropriate example for unpacking applications)
    // (TDC errors are not printed out)

    /*
    cout << " TDC V1290 Particle Event: Event Count: " << dec << GetEventCount()
	 << " Geo Address: " << GetGeo() << " Word Count: " << GetWordCount() <<endl;
    cout << " Total number of hits: " << GetNHits() << endl;
    for ( unsigned int ich=0; ich <V1290_NCHANNELS ; ich++) {
      cout << " Total number of hits in channel: " << ich << " : "
           << GetNHits(ich) << endl;
      cout << " Leading edge: " << GetNHits(ich,'l')
           << " , Trailing edge: " << GetNHits(ich,'t') << endl;
      for ( unsigned int ih=0; ih < GetNHits(ich,'l') ; ih++) {
	cout << " Channel: " << dec << ich << " ;  Hit: " << ih
	     << " : " << GetHitMeasurement(ih,ich,'l') ;
	cout << " (Leading Edge)" ;
	cout << endl;
      }
      for ( unsigned int ih=0; ih < GetNHits(ich,'t') ; ih++) {
	cout << " Channel: " << dec << ich << " ;  Hit: " << ih
	     << " : " << GetHitMeasurement(ih,ich,'t') ;
	cout << " (Trailing Edge)" ;
	cout << endl;
      }
    }
    */
  } else {
    cout << "**** ERROR: INVALID CAEN TDC V1290 Particle Event ****" << endl;
  }
  return;
}

////////////////////////////////////////////////////////////////////////


