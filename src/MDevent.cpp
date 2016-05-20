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

#include "MDevent.h"

MDevent::MDevent(void *d):MDdataContainer(d),nFragments(0) {
  MDevent::SetDataPtr( d );
};

void MDevent::Init( ) {
  fragment.clear();
  InitFragments();

  subEvent.clear();
  InitSubEvents();
}

bool MDevent::InitFragments()
{
  if ( ( EventType() != PHYSICS_EVENT &&
         EventType() != CALIBRATION_EVENT) ||
         IsSuperEvent() )
    return false;

  unsigned long32 neq = 0;
  MDeventFragment evtFrg;

  unsigned char* ptr = PayLoadPtr();
  while (ptr < _data + EventSize() ) {
    neq++;
    evtFrg.SetDataPtr(ptr);
    fragment.push_back(ptr);
    ptr += evtFrg.EquipmentSize();
  }
  nFragments = neq;
  return true;
}

bool MDevent::InitSubEvents()
{
  if ( !IsSuperEvent() )
    return false;

  unsigned long32 nsub = 0;
  MDevent subEvt;
  unsigned char* ptr = _data + HeadSize() ;
  while (ptr < _data + EventSize() ) {
    nsub++;
    subEvt.SetDataPtr(ptr);
    subEvent.push_back(ptr);
    ptr += subEvt.EventSize();
  }
  nSubEvents = nsub;
  return true;
}

void MDevent::SetDataPtr( void *d ) {
  MDdataContainer::SetDataPtr(d);
  if ( _data ) {
    if ( *MagicPtr() == EVENT_MAGIC_NUMBER ) {
      _valid = true;
      _size = *EventSizePtr();
    }
    Init();
  }
}

void MDevent::Dump(int atTheTime){
  MDeventType           eventType(EventTypePtr());
  MDeventId             eventId(EventIdPtr());
  MDeventTypeAttribute  attr(EventTypeAttributePtr());
  MDtriggerPattern      triggerPattern(TriggerPatternPtr());
  MDdetectorPattern     detectorPattern(DetectorPatternPtr());
  MDevent subEvt;
  MDeventFragment evtFrg;
  MDdataContainer dc;

  cout << dec;
  if (IsSuperEvent()) {
    cout << "========================= MDevent Dump ========================================" << endl;
  } else {
    cout << "......................... MDevent Dump ........................................" << endl;
  }
  cout << "Size:" << *EventSizePtr() << " (header:" << *HeadSizePtr()
       <<") Version:0x"<< setfill('0') << setw(8) << hex << *VersionPtr()
       << " " << eventType ;
  if (IsSuperEvent()) {
    cout << " Subevents:" << NsubEvent();
  }
  cout << dec << endl;
  cout << "runNb:" << *RunNbPtr() << " "
       << eventId << " ldcId:";
  if ( *LdcIdPtr() == VOID_ID  ) cout << "VOID";
  else cout << *LdcIdPtr();
  cout << " gdcId:" ;
  if (*GdcIdPtr() == VOID_ID  ) cout << "VOID";
  else cout << *GdcIdPtr();
  uint32_t *xTime = (uint32_t*) TimeStampPtr();
  time_t xMyTime = *xTime;
  cout << " time:" << ctime( &xMyTime ); // Note: ctime will add endl
  cout << attr << endl << triggerPattern << " " << detectorPattern << endl;

  if ( *HeadSizePtr() > EVENT_HEAD_BASE_SIZE ){
    cout << "Header extension (" << dec
	 <<  *HeadSizePtr() - EVENT_HEAD_BASE_SIZE << " bytes(s)):" << endl;
    dc.SetDataPtr( _data + EVENT_HEAD_BASE_SIZE );
    dc.SetSize( *HeadSizePtr() - EVENT_HEAD_BASE_SIZE );
    dc.Dump(4);
    cout << "Payload:" << endl;
  }

  if ( PayLoadSize() ) {
    unsigned char* ptr = PayLoadPtr();
    if(  IsSuperEvent() ) {
      for (unsigned int ise=0; ise<GetNSubEvents(); ise++) {
	subEvt.SetDataPtr( GetSubEventPtr(ise) );
	subEvt.Dump();
      }
    } else {
      if ( *EventTypePtr() == PHYSICS_EVENT ||
	   *EventTypePtr() == CALIBRATION_EVENT ) {
	for (unsigned int ifr=0; ifr<GetNFragments(); ifr++) {
	  evtFrg.SetDataPtr( GetFragmentPtr(ifr) );
	  evtFrg.Dump();
	}
      } else {
	dc.SetDataPtr(ptr);
	dc.SetSize( PayLoadSize() );
	dc.Dump(4);
      }
    }
  }
};

unsigned long32 MDevent::NsubEvent(){
  if (!IsSuperEvent()) return 0;
  unsigned long32 nsub = 0;
  MDevent subEvt;
  unsigned char* ptr = _data + *HeadSizePtr() ;
  while (ptr < _data + *EventSizePtr() ) {
    nsub++;
    subEvt.SetDataPtr(ptr);
    ptr += *(subEvt.EventSizePtr());
  }
  return nsub;
};

unsigned long32 MDevent::Nequipment(){
  if ( ( *EventTypePtr() != PHYSICS_EVENT &&  *EventTypePtr() != CALIBRATION_EVENT) ||
       IsSuperEvent() ) return 0;
  unsigned long32 neq = 0;
  MDeventFragment evtFrg;

  unsigned char* ptr = PayLoadPtr();
  while (ptr < _data + *EventSizePtr() ) {
    neq++;
    evtFrg.SetDataPtr(ptr);
    ptr += *(evtFrg.EquipmentSizePtr());
  }
  return neq;
};

unsigned long32 MDevent::PayLoadSize(){
  return *EventSizePtr()-*HeadSizePtr();
}

////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &s, MDeventType &et){
  s << "Type:";
  if ( et.IsValid() ) {
  switch ( *et.Get32bWordPtr(0) ) {
  case START_OF_RUN :       { s << "StartOfRun" ; break;}
  case END_OF_RUN :         { s << "EndOfRun" ; break;}
  case START_OF_RUN_FILES : { s << "StartOfRunFiles" ; break;}
  case END_OF_RUN_FILES :   { s << "EndOfRunFiles" ; break;}
  case START_OF_BURST :     { s << "StartOfBurst" ; break;}
  case END_OF_BURST :       { s << "EndOfBurst" ; break;}
  case PHYSICS_EVENT :      { s << "PhysicsEvent" ; break;}
  case CALIBRATION_EVENT :  { s << "CalibrationEvent" ; break;}
  case EVENT_FORMAT_ERROR : { s << "EventFormatError" ; break;}
  case START_OF_DATA :      { s << "StartOfData" ; break;}
  case END_OF_DATA :        { s << "EndOfData" ; break;}
  case SYSTEM_SOFTWARE_TRIGGER_EVENT :
    { s << "SystemSoftwareTriggerEvent" ; break;}
  case DETECTOR_SOFTWARE_TRIGGER_EVENT :
    { s << "DetectorSoftwareTriggerEvent" ; break;}
  default :                 { s << "UNKNOWN TYPE"; break;}
  }

  } else {
    s << "*** Not valid ***" << endl;
  }
  return s;
}

////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &s, MDeventId &e){
  if ( e.IsValid() ) {
    s <<  "nbInRun:"   << EVENT_ID_GET_NB_IN_RUN( e.Get32bWordPtr() )
      <<  " burstNb:"   << EVENT_ID_GET_BURST_NB( e.Get32bWordPtr() )
      <<  " nbInBurst:" << EVENT_ID_GET_NB_IN_BURST( e.Get32bWordPtr() );
  } else {
    s << " Event Type Data not valid " << endl;
  }
  return s;
}
////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &s, MDeventTypeAttribute &e){
  s << "Attributes:" << dec;
  if ( e.IsValid() ) {
    int word;
    bool all;
    for ( all = true, word = 0; all && word != ALL_ATTRIBUTE_WORDS; word++ )
      all = ( *e.Get32bWordPtr(word) == 0xffffffff );
    if ( all ) {
      s << "-=- (all set)";
      return s;
    }
    int  bit, i ;
    bool  first = true;
    for ( bit = 0; bit != ALL_ATTRIBUTE_BITS; bit++ ) {
      if ( TEST_ANY_ATTRIBUTE( e.Get32bWordPtr() , bit ) ) {
	if ( !first ) s << "+";
	first = false;
	if ( bit < FIRST_SYSTEM_ATTRIBUTE ) {
	  // User attribute: print the number
	  s << bit;
	} else {
          // System attribute: try to decode it
	  switch ( bit ) {
	  case ATTR_P_START:
	    s << "PhaseStart" ;
	    break;
	  case ATTR_P_END :
	    s << "PhaseEnd" ;
	    break;
	  case ATTR_EVENT_SWAPPED :
	    s << "Swapped" ;
	    break;
	  case ATTR_EVENT_PAGED :
	    s << "Paged" ;
	    break;
	  case ATTR_SUPER_EVENT :
	    s << "Super" ;
	    break;
	  case ATTR_ORBIT_BC :
	    s << "Orbit/BC" ;
	    break;
	  case ATTR_KEEP_PAGES :
	    s << "Keep" ;
	    break;
	  case ATTR_HLT_DECISION :
	    s << "HLT" ;
	    break;
	  case ATTR_BY_DETECTOR_EVENT :
	    s << "ByDetector" ;
	    break;
	  case ATTR_EVENT_DATA_TRUNCATED :
	    s << "Truncated" ;
	    break;
	  case ATTR_EVENT_ERROR :
	    s << "Error" ;
	    break;
	  default :
	    s << "??" << bit << "??";
	  }
	}
      }
    }
    if ( first ) s << "noAttr" ;
    s << " (" ;
    for ( i = 0; i != ALL_ATTRIBUTE_WORDS; i++ ) {
      s << noshowbase << setfill('0') << setw(8) << hex << *e.Get32bWordPtr(i);
      if ( i != ALL_ATTRIBUTE_WORDS-1 ) s << ".";
    }
    s << ")" ;
    if ( !SYSTEM_ATTRIBUTES_OK( e.Get32bWordPtr() ) ) s << "{NOT OK}" ;
  } else {
    s << " *** Not valid ***" << endl;
  }
  return s;
}

////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &s, MDtriggerPattern &e){
  s << "triggerPattern:" << noshowbase << hex;
  if ( e.IsValid() ) {
    s <<  setfill('0') << setw(8) << *e.Get32bWordPtr(1)
      << "-" << setfill('0') << setw(8) << *e.Get32bWordPtr(0);
    if ( !TRIGGER_PATTERN_VALID( e.Get32bWordPtr() ) ) s << "[invalid]";
    if ( !TRIGGER_PATTERN_OK( e.Get32bWordPtr() ) ) s << "{NOT OK}";
  } else {
    s << " *** Not valid ***" << endl;
  }
  s << dec ;
  return s;
}

////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &s, MDdetectorPattern &e){
  s << "detectorPattern:" << noshowbase << hex;
  if ( e.IsValid() ) {
    s << setfill('0') << setw(8) << *e.Get32bWordPtr();
    if ( !DETECTOR_PATTERN_VALID( e.Get32bWordPtr() ) ) s << "[invalid]";
    if ( !DETECTOR_PATTERN_OK( e.Get32bWordPtr() ) ) s << "{NOT OK}";

  } else {
    s << " *** Not valid ***" << endl;
  }
  s << dec ;
  return s;
}
