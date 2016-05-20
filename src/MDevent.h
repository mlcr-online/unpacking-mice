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

#ifndef __MDEVENT_H
#define __MDEVENT_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>

#ifdef _STEPIV_DATA
  #include "event.h"
#else
  #include "event_old.h"
#endif

#include "MDdataContainer.h"
#include "MDeventFragment.h"

using namespace std;

class MDevent : public MDdataContainer  {
  ///////  data   ////////
 private:
  unsigned int            nFragments;
  vector<void *> fragment;

  unsigned int            nSubEvents;
  vector<void *> subEvent;

 public:
  ///////  Member functions   ////////
  MDevent(void *d=0);

  virtual ~MDevent(){}
  virtual void SetDataPtr( void *d );

  void Init();
  bool InitFragments();
  bool InitSubEvents();

  eventHeaderStruct* HeaderPtr(){ return (eventHeaderStruct *) _data; }

  uint32_t EventSize(){ return  HeaderPtr()->eventSize ; }
  uint32_t* EventSizePtr(){ return &(HeaderPtr()->eventSize); }

  uint32_t Magic(){ return HeaderPtr()->eventMagic ; }
  uint32_t* MagicPtr(){ return &(HeaderPtr()->eventMagic); }

  uint32_t  HeadSize(){ return HeaderPtr()->eventHeadSize ; }
  uint32_t* HeadSizePtr(){ return &(HeaderPtr()->eventHeadSize) ; }

  uint32_t  Version(){ return HeaderPtr()->eventVersion ; }
  uint32_t* VersionPtr(){ return &(HeaderPtr()->eventVersion) ; }

  uint32_t  EventType(){ return HeaderPtr()->eventType ; }
  uint32_t* EventTypePtr(){ return &(HeaderPtr()->eventType) ; }

  uint32_t  RunNb(){ return HeaderPtr()->eventRunNb ; }
  uint32_t* RunNbPtr(){ return &(HeaderPtr()->eventRunNb) ; }

  uint32_t* EventIdPtr(){ return HeaderPtr()->eventId; }

  uint32_t* TriggerPatternPtr(){ return HeaderPtr()->eventTriggerPattern; }

  uint32_t* DetectorPatternPtr(){ return HeaderPtr()->eventDetectorPattern; }

  uint32_t* EventTypeAttributePtr(){ return HeaderPtr()->eventTypeAttribute; }

  uint32_t  LdcId(){ return HeaderPtr()->eventLdcId; }
  uint32_t* LdcIdPtr(){ return &(HeaderPtr()->eventLdcId); }

  uint32_t  GdcId(){ return HeaderPtr()->eventGdcId; }
  uint32_t* GdcIdPtr(){ return &(HeaderPtr()->eventGdcId); }

  uint32_t* TimeStampPtr(){ return &(HeaderPtr()->eventTimestampSec); }
  time_t    TimeStamp(){return *(this->TimeStampPtr());}

  unsigned char* PayLoadPtr(){ return (_data + *HeadSizePtr()); }

  virtual void Dump(int atTheTime=1);

  uint32_t Nequipment();
  uint32_t NsubEvent();

  uint32_t  PayLoadSize();

  void* GetFragmentPtr(int ifr){ return fragment[ifr]; }
  uint32_t GetNFragments(){ return nFragments; }

  void* GetSubEventPtr(int ifr){ return subEvent[ifr]; }
  uint32_t GetNSubEvents(){ return nSubEvents; }

  bool IsSuperEvent(){
    return TEST_SYSTEM_ATTRIBUTE( HeaderPtr()->eventTypeAttribute,
                                  ATTR_SUPER_EVENT );
  }
};

/////////////////////////////////////////////////////////////////////////////

class MDeventType : public MDdataContainer {
 public:
  MDeventType(void *d=0):MDdataContainer(d){
    if ( _data ) {
      _valid = true;
      _size = sizeof(eventTypeType);
    }
  }
  MDeventType(MDeventType& et) {
    _data  = et.GetDataPtr();
    _size  = et.GetSize();
    _valid = et.IsValid();
  }
  virtual ~MDeventType(){}
};
ostream &operator<<(ostream &s,MDeventType &e);

/////////////////////////////////////////////////////////////////////////////

class MDeventId : public MDdataContainer {
 public:
  MDeventId(void *d=0):MDdataContainer(d){
    if ( _data ) {
      _valid = true;
      _size = EVENT_ID_BYTES;
    }
  }
  virtual ~MDeventId(){}
};
ostream &operator<<(ostream &s,MDeventId &e);

/////////////////////////////////////////////////////////////////////////////

class MDeventTypeAttribute : public MDdataContainer {
 public:
  MDeventTypeAttribute(void *d=0):MDdataContainer(d){
    if ( _data ) {
      _valid = true;
      _size = ALL_ATTRIBUTE_BYTES;
    }
  }
  virtual ~MDeventTypeAttribute(){}
};
ostream &operator<<(ostream &s,MDeventTypeAttribute &e);

/////////////////////////////////////////////////////////////////////////////

class MDtriggerPattern : public MDdataContainer {
 public:
  MDtriggerPattern(void *d=0):MDdataContainer(d){
    if ( _data ) {
      _valid = true;
      _size = EVENT_TRIGGER_PATTERN_BYTES;
    }
  }
  virtual ~MDtriggerPattern(){}
};
ostream &operator<<(ostream &s,MDtriggerPattern &e);

/////////////////////////////////////////////////////////////////////////////

class MDdetectorPattern : public MDdataContainer {
 public:
  MDdetectorPattern(void *d=0):MDdataContainer(d){
    if ( _data ) {
      _valid = true;
      _size = EVENT_DETECTOR_PATTERN_BYTES;
    }
  }
  virtual ~MDdetectorPattern(){}
};
ostream &operator<<(ostream &s,MDdetectorPattern &e);


#endif
