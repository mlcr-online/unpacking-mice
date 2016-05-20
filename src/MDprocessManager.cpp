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

#include "MDprocessManager.h"
#include <time.h>

using namespace std;

MDprocessManager::MDprocessManager()
:_eMap(), _status(OK), _runNumber((uint32_t)-1), _timeStamp(0), _eventId(0),
_spillNumber(-1), _eventSizes(1,0), _gdcId(-1), _ldcId(-1), _eventType(0), _physEventNumber(-1),
_equipmentType(0), _boardId(-1), _firstEventProc(0), _startOfRunProc(0),
_endOfRunProc(0), _startOfSpillProc(0), _endOfSpillProc(0), _superHeaderProc(0),
_eventHeaderProc(0), _equipmentHeaderProc(0)
{
  // Enable all equipments by default
  equipMap_t::iterator it=MDequipMap::begin();
  while ( it != MDequipMap::end() ){
    //      cout << " Enable equipment ID " << it->first << endl;
    _enableMap[it->first] = true;
    ++it;
  }
}

void MDprocessManager::SetTest(DataTestCallback funk)
{
  // set the test function for all fragments
  equipMap_t::iterator it=MDequipMap::begin();
  while ( it != MDequipMap::end() ){
    //      cout << " Enable equipment ID " << it->first << endl;
    (it->second)->GetFragmentPtr()->SetTest( funk );
    ++it;
  }
}

void MDprocessManager::SetFragmentProc(unsigned int aType,MDprocessor* aProc){
  if (MDequipMap::IsDefined(aType)){
    _fragmentProc[aType] = aProc;
    aProc->SetProcessManager(this);
  } else {
    stringstream ss;
    ss << "WARNING in MDprocessManager::SetFragmentProc : Type " ;
    ss << dec << aType << " not found in the map, ignored." << endl;
    throw MDexception( ss.str() );
  }
}

void MDprocessManager::SetFragmentProc(string aName,MDprocessor* aProc){
  SetFragmentProc(MDequipMap::GetType(aName), aProc);
}

void MDprocessManager::SetPartEventProc(unsigned int aType,MDprocessor* aProc){
  if (MDequipMap::IsDefined(aType)){
    _partEventProc[aType] = aProc;
    aProc->SetProcessManager(this);
  } else {
    stringstream ss;
    ss << "WARNING in MDprocessManager::SetPartEventProc : Type " ;
    ss << dec << aType << " not found in the map, ignored." << endl;
    throw MDexception( ss.str() );
  }
}

void MDprocessManager::SetPartEventProc(string aName,MDprocessor* aProc){
  SetPartEventProc(MDequipMap::GetType(aName), aProc);
}

void MDprocessManager::DumpContextData()
{
    cout << "  *** Process Manager Context Data ***" << dec << endl;
    cout << " Run Number : " << GetRunNumber();
    cout << " ; GDC Id : " << GetGdcId();
    cout << " ; LDC Id : " << GetLdcId();
    cout << " ; Event Type : " << GetEventType();
    cout << " ; Physics Event Number : " << GetPhysEventNumber();
    cout << " ; Equipment Type : " << GetEquipmentType();
    cout << " ; Board Id : " << GetBoardId() ;
    cout << " ; Spill Number : " << GetSpillNumber() ;
    cout << " ; Time Stamp : " << GetTimeString();
    cout << " ; EventId : " << GetEventId() << endl;
}

string MDprocessManager::GetTimeString()
{
  time_t xMyTime = _timeStamp;
  return ctime( &xMyTime );
}

void MDprocessManager::SetTimeStamp() {
  time_t rawtime;
  // get the time:
  time(&rawtime);
  uint32_t t = rawtime;
  this->SetTimeStamp(t);
}

int MDprocessManager::Process(unsigned char* aDataPtr) {
  _status = OK;
  MDevent theEvent(aDataPtr);
  uint32_t oldType = GetEventType();
  SetEventType(theEvent.EventType());
  SetTimeStamp( *(theEvent.TimeStampPtr()) );
  SetEventId( *(theEvent.EventIdPtr()) );

//   cout << "\nRun: " << GetRunNumber() << "   Spill: " << GetSpillNumber() << "  Evt Type: " << GetEventType() << endl;

  if (_runNumber != theEvent.RunNb()) { // first event
    if ( GetEventType() != START_OF_RUN ) {
      cerr << "WARNING : The first event is not a START_OF_RUN. Spill count and Event count not accurate. " << endl;
      _spillNumber = -1;
    }

    if (_firstEventProc) {
      try { _status = _firstEventProc->Process(&theEvent);}
      catch(...) {throw MDexception("ERROR processing First Event");}
    }

    if (theEvent.IsSuperEvent()) {
      _nLdcs = theEvent.NsubEvent();
      _eventSizes.resize(_nLdcs+1);
    } else {
      _nLdcs = 1;
    }
  }

  SetRunNumber(theEvent.RunNb());

  switch ( GetEventType() ) {
    case START_OF_RUN:
      if ( oldType != START_OF_RUN ) {        // There are two consecutive START_OF_RUN events per LDC.
                                              // Only the first one generates a call to the
                                              // startOfRun Procedure
        _spillNumber = -1;
        if (_startOfRunProc) {
          try { _status = _startOfRunProc->Process(&theEvent);}
          catch(...) {throw MDexception("ERROR processing Start of Run");}
        }
      }
      break;

    case END_OF_RUN:
      if ( oldType != END_OF_RUN ) {
        if (_endOfRunProc) {
          try { _status = _endOfRunProc->Process(&theEvent);}
          catch(...) {throw MDexception("ERROR processing End of Run");}
        }
      }
      break;

    case START_OF_BURST:  // MICE start of Spill corresponds to DATE START_OF_BURST (clash of name space)
      if ( oldType != START_OF_BURST ) {      // There might be several consecutive START_OF_BURST
                                              // events (one per LDC if no building)
                                              // Only the first one generates a call to the
                                              // startOfSpill Procedure
        ++_spillNumber;
        if (_startOfSpillProc) {
          try { _status = _startOfSpillProc->Process(&theEvent);}
          catch(...) {throw MDexception("ERROR processing Start of Spill");}
        }
      }
      break;

    case END_OF_BURST:
      if ( oldType != END_OF_BURST ) {
        if (_endOfSpillProc) {
          try {_status = _endOfSpillProc->Process(&theEvent);}
          catch(...) {throw MDexception("ERROR processing End of Spill");}
        }
      }
      break;
/*
    case PHYSICS_EVENT:
      if ( oldType !=  PHYSICS_EVENT ) {
        ++_physEventNumber;
      } else {
        //cerr << "WARNING : Two consecutive DAQ events found in data file (no event building?)" << endl;
        //cout<<"WARNING : Two consecutive DAQ events found in data file (no event building?)" << endl;
      }
      break;
*/
    case PHYSICS_EVENT:
      ++_physEventNumber;
      break;

    default:
      break;
  }

  int nPart(0), xPart(0);
  if (theEvent.IsSuperEvent()) {
    SetGdcId(theEvent.GdcId());
    _eventSizes[0] = theEvent.GetSize();
    if (_superHeaderProc) {
      try {
        _status = _superHeaderProc->Process(&theEvent);
      }
      catch(...) {throw MDexception("ERROR processing Superevent header.");}
    }

    if (theEvent.NsubEvent() != _nLdcs) {
      stringstream ss;
      ss << "ERROR in MDprocessManager::ProcessEvent :"<< endl;
      ss << "Sub Event Mismatch (nLdcs: " << _nLdcs << "!=" << theEvent.NsubEvent() << ").";
      throw MDexception( ss.str() );
    }

    nPart = ProcessSubEvent((unsigned char*)theEvent.GetSubEventPtr(0));
//     cout << "nPart: " << nPart << endl;

    for (unsigned int iEvt=1; iEvt<_nLdcs; iEvt++){
      if (_status != OK) return _status;
      xPart = ProcessSubEvent((unsigned char*)theEvent.GetSubEventPtr(iEvt));
      if (nPart != xPart) {
        stringstream ss;
        ss << "ERROR in MDprocessManager::ProcessEvent :"<< endl;
        ss << "Trigger Mismatch (nEvts " << nPart << "!=" << xPart << ").";
        throw MDexception( ss.str() );
      }
//       cout << "nPart: " << nPart << " " << xPart << endl;
    }
  } else {
    return ProcessSubEvent(theEvent.GetDataPtr());
  }
  return nPart;
}

int MDprocessManager::ProcessSubEvent(unsigned char* aDataPtr){
  MDevent theEvent(aDataPtr);
  MDeventFragment theEquipmentHeader;
  MDfragment* myFragPtr;
  int32_t nPart(-1), nPart_this(0), xLdc(0);
  xLdc = theEvent.LdcId();
  this->SetLdcId(xLdc);
  _eventSizes[xLdc+1] = theEvent.GetSize();

  if (_eventHeaderProc) {
    try {
      _status = _eventHeaderProc->Process(&theEvent);
    }
    catch(MDexception & lExc) {
      string message = lExc.GetDescription();
      throw MDexception(message + "\nExeption while processing event header.");
      }
    if (_status != OK) throw MDexception("ERROR in MDprocessManager::ProcessSubEvent from _eventHeaderProc->Process");
  }
  // loop over equipments
  if ( theEvent.PayLoadSize() ) { // There is some data loop over eventFragments
    for (unsigned int ifr=0; ifr<theEvent.GetNFragments(); ifr++) {
      theEquipmentHeader.SetDataPtr(theEvent.GetFragmentPtr(ifr));
      SetEquipmentType(theEquipmentHeader.EquipmentType());
//       cout << ifr << " eqType: " << GetEquipmentType() << endl;
      SetBoardId(theEquipmentHeader.EquipmentId());
      if (_equipmentHeaderProc) {
        try {
          _status = _equipmentHeaderProc->Process(&theEquipmentHeader);
        }
        catch(...) {throw MDexception("Exeption while processing equipment header.");}
        if(_status != OK) throw MDexception("ERROR in MDprocessManager::ProcessSubEvent from _equipmentHeaderProc->Process ");
      }
      // Check if the user wants this equipment to be decoded
      if (IsEnabled(GetEquipmentType())) {
        // Get a pointer to the dynamically assigned pointer from the map
        myFragPtr = MDequipMap::GetFragmentPtr(GetEquipmentType());
        // Get the data pointer
        unsigned char* myFragDataPtr = theEquipmentHeader.EquipmentDataPtr();
        // Get the data size
        uint32_t myFragDataSize = theEquipmentHeader.EquipmentDataSize();
//         cout << ifr << " Equipment " << MDequipMap::GetName(GetEquipmentType())
//              << " found, size: " << myFragDataSize << endl;

        if (nPart > 0 && myFragDataSize==0)
          if (MDequipMap::GetName(GetEquipmentType())!="VRB") {
            stringstream ss;
            ss << "ERROR in MDprocessManager::ProcessSubEvent :"<< endl;
            ss << "In fragment " << MDequipMap::GetName(GetEquipmentType()) << " (board Id: " << GetBoardId()
               << ")  Event size is 0.";
            throw MDexception( ss.str() );
         }

        if (myFragPtr && myFragDataSize) { // the equipment is defined in the map and filled with data
          try {
            myFragPtr->SetDataPtr(myFragDataPtr, myFragDataSize);
            myFragPtr->SetBoardID(_boardId);
            if ( myFragPtr->IsMadeOfParticles() || MDequipMap::GetName(GetEquipmentType())=="VLSBMaster") {
              int xPart = myFragPtr->InitPartEventVector();
              if (nPart<0) nPart = xPart;
              else {
                nPart_this = xPart;
                if (nPart != nPart_this) {
                  stringstream ss;
                  ss << "ERROR in MDprocessManager::ProcessSubEvent :"<< endl;
                  ss << "Trigger Mismatch (nEvts " << nPart << "!=" << nPart_this << ").";
                  throw MDexception( ss.str() );
                }
              }
//               cout << "@ nPart: " << nPart << " " << nPart_this << endl;
            }
          }
          catch (MDexception lExc)
          {
            stringstream ss;
            ss << lExc.GetDescription() << endl;
            ss << "*** Unpacking exception in MDprocessManager::ProcessSubEvent :"<< endl;
            ss << "while initializing the fragment";
            throw MDexception( ss.str() );
          }
          if (_fragmentProc[GetEquipmentType()]) {
            try {
//               cout << " Equipment " << MDequipMap::GetName(GetEquipmentType())
//                    << " going to call the fragment processor." << endl;
              _status = _fragmentProc[GetEquipmentType()]->Process(myFragPtr);
            }
            catch(MDexception lExc){
                stringstream ss;
                ss << lExc.GetDescription() << endl;
                ss << "*** Unpacking exception in MDprocessManager::ProcessSubEvent : "<< endl;
                ss << "while processing fragment in board " << MDequipMap::GetName( this->GetEquipmentType() );
                ss << " Geo " << this->GetBoardId(); 
                throw MDexception( ss.str() );
            }
          }
          if (_status != OK) {
              stringstream ss;
              ss << "ERROR in MDprocessManager::ProcessSubEvent : "<< endl;
              ss << "while processing fragment in board " << MDequipMap::GetName( this->GetEquipmentType() );
              ss << " Geo " << this->GetBoardId() << endl;
              ss << "The status is : " << _status;
              throw MDexception( ss.str() );
          }
          // loop over particle events if there are
          for (int32_t iPart=0 ; iPart<nPart ; iPart++) {
            _partEventNumber = iPart;
            if (_partEventProc[GetEquipmentType()]) {
              try {
//                    cout << " Equipment " << MDequipMap::GetName(GetEquipmentType())
//                         << " going to call the part. event processor." << endl;
                  _status = _partEventProc[GetEquipmentType()]->Process(myFragPtr->GetPartEventPtr(iPart));
              }
              catch(MDexception lExc){
                stringstream ss;
                ss << lExc.GetDescription() << endl;
                ss << "*** Unpacking exception in MDprocessManager::ProcessSubEvent : "<< endl;
                ss << "while processing particle event in board ";
                ss << MDequipMap::GetName( this->GetEquipmentType() ) << " Geo " << this->GetBoardId(); 
                throw MDexception( ss.str() );
              }
            }
            if (_status != OK) {
              stringstream ss;
              ss << "ERROR in MDprocessManager::ProcessSubEvent : "<< endl;
              ss << "while processing particle event in board ";
              ss << MDequipMap::GetName( this->GetEquipmentType() ) << " Geo " << this->GetBoardId() << endl;
              ss << "The status is : " << _status;
              throw MDexception( ss.str() );
            }
          }
        }
      }
    }
  }

  if (nPart < 0) return 0;

  return nPart;
}

void MDprocessManager::DumpProcessors()
{
  procMap_t::iterator it;

  cout<<"+++++++ Dump Processors +++++++"<<endl;
  cout << "First Event processor is " ;
  if  (!_firstEventProc) cout << " not " ; cout << " active";
  if  (_firstEventProc) cout << ", ptr : " << _firstEventProc; cout << endl;
  cout << "Start of run processor is " ;
  if  (!_startOfRunProc) cout << " not " ; cout << " active";
  if  (_startOfRunProc) cout << ", ptr : " << _startOfRunProc; cout << endl;
  cout << "End of run processor is " ;
  if  (!_endOfRunProc) cout << " not " ; cout << " active ";
  if  (_endOfRunProc) cout << ", ptr : " << _endOfRunProc; cout << endl;
  cout << "Start of spill processor is " ;
  if  (!_startOfSpillProc) cout << " not " ; cout << " active";
  if  (_startOfSpillProc) cout << ", ptr : " << _startOfSpillProc; cout << endl;
  cout << "End of Spill processor is " ;
  if  (!_endOfSpillProc) cout << " not " ; cout << " active";
  if  (_endOfSpillProc) cout << ", ptr : " << _endOfSpillProc; cout << endl;
  cout << "Super Event Header processor is " ;
  if  (!_superHeaderProc) cout << " not " ; cout << " active";
  if  (_superHeaderProc) cout << ", ptr : " << _superHeaderProc; cout << endl;
  cout << "Event Header processor is " ;
  if  (!_eventHeaderProc) cout << " not " ; cout << " active";
  if  (_eventHeaderProc) cout << ", ptr : " << _eventHeaderProc; cout << endl;
  cout << "Equipment Header processor is " ;
  if  (!_equipmentHeaderProc) cout << " not " ; cout << " active";
  if  (_equipmentHeaderProc) cout << ", ptr : " << _equipmentHeaderProc; cout << endl;

  for ( it=_fragmentProc.begin() ; it != _fragmentProc.end(); it++ ){
    cout << "Fragment processor for equipment " << MDequipMap::GetName((*it).first) << " is active,  ptr : " << (*it).second << endl;
  }
  cout << _fragmentProc.size() << " active fragment processors "<< endl;

  for ( it=_partEventProc.begin() ; it != _partEventProc.end(); it++ ){
    cout << "Particle event processor for equipment "<<  MDequipMap::GetName((*it).first) << " is active,  ptr : " << (*it).second <<  endl;
  }
  cout << _partEventProc.size() << " active Particle event processors "<< endl;
  cout << "+++++++++++++++++++++++++++++++" << endl;
}
