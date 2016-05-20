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


#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <typeinfo>
#include <exception>

#include "MDprocessManager.h"
#include "MDevent.h"
#include "MDdateFile.h"
#include "MDfileManager.h"
#include "MDargumentHandler.h"
#include "MDfragmentVLSB_C.h"
#include "MDfragmentV830.h"
#include "MDfragmentV1290.h"
#include "MDfragmentV1724.h"
#include "MDpartEventV1290.h"
#include "MDpartEventV1724.h"
#include "MDpartEventV1731.h"
#include "MDfragmentDBB.h"

using namespace std;

// Create a MDprocessor daughter class for each data structure to be analysed.
// In this example, all the classes are declared and defined in the same file
// but for real applications, C++ good practice is to have one .h and one .cpp
// file per class.

class OnStartOfRun : public MDprocessor {
 public:
  OnStartOfRun():MDprocessor(){}
  virtual ~OnStartOfRun(){}
  virtual int Process(MDdataContainer*);
};

int OnStartOfRun::Process(MDdataContainer* aEvtPtr){
  cout << "Starting run " << GetRunNumber() << " on " <<  GetTimeString() << endl;
  return OK;
}

///////////////////////////////////////////////////////

class OnStartOfSpill : public MDprocessor {
 public:
  OnStartOfSpill():MDprocessor(){}
  virtual ~OnStartOfSpill(){}
  virtual int Process(MDdataContainer*);
};

int OnStartOfSpill::Process(MDdataContainer* aEvtPtr){
  cout << "---> Start of  Spill Number " << GetSpillNumber() << " on " <<  GetTimeString() << endl;
  return OK;
}
/////////////////////////////////////////////////////

class OnEndOfSpill : public MDprocessor {
 public:
  OnEndOfSpill():MDprocessor(){}
  virtual ~OnEndOfSpill(){}
  virtual int Process(MDdataContainer*);
};

int OnEndOfSpill::Process(MDdataContainer* aEvtPtr){
  cout << "---> End of Spill Number " << GetSpillNumber();
  cout << " on " <<  GetTimeString(); // GetTimeString add an endl
  cout << " DAQ Physics Event Count : " << GetPhysEventNumber()+1 << endl;
  return OK;
}
/////////////////////////////////////////////////////

//********* OnSuperHeader **************
// In this example, the object to fill is meant to be the standard output
// and the parameter is meant to be a single int value (given in the main argument list)

class OnSuperHeader : public MDprocessor {
 public:
  OnSuperHeader(ostream &aStream):MDprocessor(),myObj(&aStream) {}
  virtual ~OnSuperHeader(){}
  virtual int Process(MDdataContainer*);

  ostream* myObj;
};

int OnSuperHeader::Process(MDdataContainer* aEvtPtr){
  // cast the argument to structure it points to
  // This process should be called only with MDevent argument corresponding to super event
  if ( typeid(*aEvtPtr) != typeid(MDevent) ) return CastError;
  MDevent* theEvent = static_cast<MDevent*>(aEvtPtr);
  if ( *(theEvent->MagicPtr()) != EVENT_MAGIC_NUMBER ) {
    throw MDexception("ERROR in OnSuperHeader::Process() : The function is called with unvalid argument ***", MDexception::FATAL) ;
  }
  if (!theEvent->IsSuperEvent()) {
    throw MDexception("ERROR in OnSuperHeader::Process() : This function should not be called for non-super event ***", MDexception::FATAL) ;
  }

  static unsigned int superEventCount(0);
  *myObj << "*** Super header count : " << ++superEventCount << " ***" << endl;
  *myObj << " Run Number : " << theEvent->RunNb() << endl;
  *myObj << " Event Size : " << theEvent->EventSize() << endl;
  MDeventType eventType(theEvent->EventTypePtr());
  *myObj << " Event Type : " << eventType << endl;
  uint32_t *xTime = (uint32_t*) theEvent->TimeStampPtr(); // explicit cast required for 64 bit compatibility
  time_t xMyTime = *xTime;
  cout << " time:" << ctime( &xMyTime ); // Note: ctime will add endl

  return OK;
}

/////////////////////////////////////////////////////////////////////////
// This class is an example to show how to pass information from one
// processor to the other, using a user class passed either as object or param.
// This is only for illustration. Context data should be taken directly from the Process Manager

class ContextData {
 public:
  uint32_t _runNumber;
  uint32_t _ldcId;
  uint32_t _eventType;
  uint32_t _physEventCount;
  uint32_t _equipmentType;
  uint32_t _boardId;

  ContextData():_runNumber(0),_ldcId(0),_eventType(0),_physEventCount(0),_equipmentType(0),_boardId(0){}
  ~ContextData(){}

  void SetRunNumber(uint32_t aRunNumber){_runNumber = aRunNumber;}
  void SetLdcId(uint32_t aLdcId){_ldcId = aLdcId;}
  void SetEventType(uint32_t aEventType){_eventType = aEventType;}
  void SetPhysEventCount(uint32_t aCount){_physEventCount = aCount;}
  void SetEquipmentType(uint32_t aEquipmentType){_equipmentType = aEquipmentType;}
  void SetBoardId(uint32_t aBoardId){_boardId = aBoardId;}

  uint32_t GetRunNumber(){return _runNumber;}
  uint32_t GetLdcId(){return _ldcId;}
  uint32_t GetEventType(){return _eventType;}
  uint32_t GetPhysEventCount(){return _physEventCount;}
  uint32_t GetEquipmentType(){return _equipmentType;}
  uint32_t GetBoardId(){return _boardId;}

  void Print(){
    cout << "  *** Context Data ***" << dec << endl;
    cout << " Run Number : " << GetRunNumber();
    cout << " ; LDC Id : " << GetLdcId();
    cout << " ; Event Type : " << GetEventType();
    cout << " ; Physics Event Count : " << GetPhysEventCount();
    cout << " ; Equipment Type : " << GetEquipmentType();
    cout << " ; Board Id : " << GetBoardId() << endl;
  }
};

/////////////////////////////////////////////////////////////////////////

//********* OnEventHeader **************
// In this example, the object to fill is meant to be an instance of a user defined
// class describing the context,
// and the parameter is meant to be the name of an existing file containing a int value
//
class OnEventHeader : public MDprocessor {
  public:
  OnEventHeader(ContextData &aContext):MDprocessor(),myObj(&aContext) {}
  virtual ~OnEventHeader(){}
  virtual int Process(MDdataContainer*);

  ContextData* myObj;
};

int OnEventHeader::Process(MDdataContainer* aEvtPtr){
  // cast the argument to structure it points to
  // This process should be called only with MDevent argument corresponding to LDC event
  if ( typeid(*aEvtPtr) != typeid(MDevent) ) return CastError;
  MDevent* theEvent = static_cast<MDevent*>(aEvtPtr);
  if ( *(theEvent->MagicPtr()) != EVENT_MAGIC_NUMBER ) {
    throw MDexception("ERROR in OnEventHeader::Process() : The function is called with unvalid argument ***", MDexception::FATAL) ;
  }
  if (theEvent->IsSuperEvent()) {
    throw MDexception("ERROR in OnEventHeader::Process() : This function should not be called for super event ***", MDexception::FATAL) ;
  }

  static vector<uint32_t> eventCount(2);

  static int firstCall(1);
  if (firstCall) {
    eventCount[0]=0;
    eventCount[1]=0;
   firstCall = 0;
  }

  // fill the context data structure with the data available at this level
  myObj->SetRunNumber(theEvent->RunNb());
  myObj->SetLdcId(theEvent->LdcId());
  myObj->SetEventType(theEvent->EventType());
  myObj->SetPhysEventCount(eventCount[theEvent->LdcId()]);
  //  myObj->Print();

  cout << " LDC Id : " << theEvent->LdcId() << endl;
  cout << " Event Size : " << theEvent->EventSize() << endl;
  MDeventType eventType(theEvent->EventTypePtr());
  cout << " Event Type : " << eventType << endl;
  cout << " Physics Event Number : " << eventCount[theEvent->LdcId()] << endl;
  uint32_t *xTime = (uint32_t*) theEvent->TimeStampPtr(); // explicit cast required for 64 bit compatibility
  time_t xMyTime = *xTime;
  cout << " Time Stamp:" << ctime( &xMyTime ); // Note: ctime will add endl

  if (theEvent->EventType() == PHYSICS_EVENT) { eventCount[theEvent->LdcId()]++;}

  return OK;
}
//********* OnEquipmentHeader **************
// In this example, the object to fill is meant to be an instance of a user defined
// class describing the context and there is no parameter
//
class OnEquipmentHeader : public MDprocessor {
  public:
  OnEquipmentHeader(ContextData &aContext):MDprocessor(),myObj(&aContext){}
  virtual ~OnEquipmentHeader(){}
  virtual int Process(MDdataContainer*);

  ContextData* myObj;
};

int OnEquipmentHeader::Process(MDdataContainer* aEquipmentPtr){
  // cast the argument to structure it points to
  // This process should be called only with MDeventFragment argument
  if ( typeid(*aEquipmentPtr) != typeid(MDeventFragment) ) return CastError;
  MDeventFragment* theEquipment = static_cast<MDeventFragment*>(aEquipmentPtr);

  // fill the context data structure with the data available at this level
  myObj->SetEquipmentType(theEquipment->EquipmentType());
  myObj->SetBoardId(theEquipment->EquipmentId());
  //  myObj->Print();
  return OK;
}

//********* OnFragmentVLSB_C **************
// In this example, useful for Tracker cosmic data
// the object to fill is meant to be an output stream
// and the parameter is meant to be a instance of a user defined class
// describing the context
//
class OnFragmentVLSB_C : public MDprocessor {
  public:
  OnFragmentVLSB_C(ostream &aOs, ContextData &aContext):MDprocessor(),myObj(&aOs),myPar(&aContext){}
  virtual ~OnFragmentVLSB_C(){}
  virtual int Process(MDdataContainer*);

  ostream* myObj;
  ContextData* myPar;
};

int OnFragmentVLSB_C::Process(MDdataContainer* aFragPtr){
  // cast the argument to structure it points to
  // This process should be called only with MDfragmentVLSB_C argument
  if ( typeid(*aFragPtr) != typeid(MDfragmentVLSB_C) ) return CastError;
  MDfragmentVLSB_C* theFrag = static_cast<MDfragmentVLSB_C*>(aFragPtr);

  if ( myPar->GetEventType() == PHYSICS_EVENT || myPar->GetEventType() == CALIBRATION_EVENT){
    // print context
    *myObj << " In" << MDequipMap::GetName(myPar->GetEquipmentType()) ;
    *myObj << " board # " << myPar->GetBoardId() << endl;
    // print bank lengths
    *myObj << " Bank Lengths : ";
    for (int ib=0; ib<4; ib++){
      *myObj << theFrag->GetBankLength(ib) << " ; ";
    }
    *myObj << endl;

    // print unpacked channel and adc data
    MDdataWordVLSB dw;
    for (unsigned int iw=0; iw<theFrag->GetPayLoadWordCount(); iw++){
      dw.SetDataPtr(theFrag->GetDataWordPtr(iw));
      *myObj << "channel : " << dw.GetChannel() << " ; adc : " << dw.GetAdc() << endl;
    }
  }
  return OK;
}

//********* OnFragmentV830 **************
// In this example, the VME scaler data is unpacked and sent to the output stream
// the object to fill is meant to be an output stream
// and the parameter is meant to be a instance of a user defined class
// describing the context
//
class OnFragmentV830 : public MDprocessor {
  public:
  OnFragmentV830(ostream &aOs, ContextData &aContext):MDprocessor(),myObj(&aOs),myPar(&aContext){}
  virtual ~OnFragmentV830(){}
  virtual int Process(MDdataContainer*);

  ostream* myObj;
  ContextData* myPar;
};

int OnFragmentV830::Process(MDdataContainer* aFragPtr){
  // cast the argument to structure it points to
  // This process should be called only with MDfragmentV830 argument
  if ( typeid(*aFragPtr) != typeid(MDfragmentV830) ) return CastError;
  MDfragmentV830* theFrag = static_cast<MDfragmentV830*>(aFragPtr);

  unsigned int * ptr = theFrag->Get32bWordPtr(0);
  MDdataWordV830 dw(ptr) ;
  unsigned int wc(0);

  // The vme scaler has only valid data for PHYSICS_EVENTs
  if ( myPar->GetEventType() == PHYSICS_EVENT ){
    while (wc < theFrag->GetWordCount() ) {
      uint32_t dt= dw.GetDataType();
      switch ( dt ) {
      case DWV830_Measurement:
      {
          *myObj  << " Ch " << setw(2) << dec << dw.GetChannel() ;
          *myObj << " ; data: " << dec << dw.GetMeasurement() << endl ;
          break;
      }
      case DWV830_Header:
      {
          *myObj << " Equipment " << theFrag->GetName() << " ; board " << dw.GetGeo() << endl;
          if ((uint32_t)dw.GetGeo() != myPar->GetBoardId()) return FormatError;
          break;
        }
      }
      wc++;
      dw.SetDataPtr( ++ptr );
   }
  }
  return OK;
}
//********* OnFragmentV1290 **************
// Minimal example, no object to fill, no parameter
//
class OnFragmentV1290 : public MDprocessor {
  public:
  OnFragmentV1290():MDprocessor(){}
  virtual ~OnFragmentV1290(){}
  virtual int Process(MDdataContainer*);
};

int OnFragmentV1290::Process(MDdataContainer* aFragPtr){
  // cast the argument to structure it points to
  MDfragmentV1290* theFrag = static_cast<MDfragmentV1290*>(aFragPtr);

  cout << " Number of particle Events in " << theFrag->GetName();
  cout << " : " << theFrag->GetNPartEvents()  << endl;

  return OK;
}
//********* OnFragmentDBB **************
// Minimal example, no object to fill, no parameter
//
class OnFragmentDBB : public MDprocessor {
  public:
  OnFragmentDBB():MDprocessor(){}
  virtual ~OnFragmentDBB(){}
  virtual int Process(MDdataContainer*);
};

int OnFragmentDBB::Process(MDdataContainer* aFragPtr){
  // cast the argument to structure it points to
//   MDfragmentDBB* theFrag = static_cast<MDfragmentDBB*>(aFragPtr);

  return OK;
}


//********* OnPartEventV1290 **************
// In this example, the tdc data is unpacked and sent to the output stream
// the object to fill is meant to be an output stream
// The context data is taken directly from the Process Manager
//
class OnPartEventV1290 : public MDprocessor {
  public:
  OnPartEventV1290(ostream &aOs):MDprocessor(),myObj(&aOs){}
  virtual ~OnPartEventV1290(){}
  virtual int Process(MDdataContainer*);

  ostream* myObj;
};

int OnPartEventV1290::Process(MDdataContainer* aPartEventPtr){
  // cast the argument to structure it points to
  if ( typeid(*aPartEventPtr) != typeid(MDpartEventV1290) )  return CastError;
  MDpartEventV1290* thePart=static_cast<MDpartEventV1290*>(aPartEventPtr);  ;

  static int iPartEvt(0);
  // print only the first 10 particle events
  static uint32_t oldBoard(0);
  if ( GetBoardId() != oldBoard ) {
    oldBoard=GetBoardId();
    iPartEvt=0;
  }
  if (iPartEvt++ < 10) {
    *myObj << "In Equipment " << MDequipMap::GetName(GetEquipmentType()) ;
    *myObj << " ; Crate # " << GetLdcId() ;
    *myObj << " ; Board # " << GetBoardId() ;
    *myObj << " ; Particle Event Number : " << thePart->GetEventCount() << endl;
    *myObj << " Number of Leading Edge Hits : " << thePart->GetNHits(V1290_NCHANNELS,'l') << endl;
    *myObj << " Number of Trailing Edge Hits : " << thePart->GetNHits(V1290_NCHANNELS,'t') << endl;
    *myObj << " Total Number of Hits : " << thePart->GetNHits() << endl;
    *myObj << " Bunch ID : " << thePart->GetBunchID(0) ;
    *myObj << " ; Trigger Time Tag : " << thePart->GetTriggerTimeTag() << endl;
    // loop over channels
    if (!thePart->GetNHits(0,'l')) {
      cerr << " Missing hit in particle trigger channel " << endl;
      return FormatError;
    }
    for (int ich =0 ; ich<V1290_NCHANNELS ; ich++) {
      // print leading edge (default) hits data
      if (thePart->GetNHits(ich,'l')) { *myObj << " Channel : " << ich << " :";}
      for (unsigned int ih =0 ; ih<thePart->GetNHits(ich,'l') ; ih++) {
        *myObj << " " << thePart->GetHitMeasurement(ih,ich,'l');
      }
      if (thePart->GetNHits(ich,'l')) { *myObj << endl; }
    }
  }
  return OK;
}

//********* OnPartEventV1724 **************
// In this example, the flash adc data is unpacked and sent to the output stream
// the object to fill is meant to be an output stream
// The context data is taken directly from the Process Manager
//
class OnPartEventV1724 : public MDprocessor {
  public:
  OnPartEventV1724(ostream &aOs):MDprocessor(),myObj(&aOs){}
  virtual ~OnPartEventV1724(){}
  virtual int Process(MDdataContainer*);

  ostream* myObj;
};

int OnPartEventV1724::Process(MDdataContainer* aPartEventPtr){
  // cast the argument to structure it points to
  if ( typeid(*aPartEventPtr) != typeid(MDpartEventV1724) ) return CastError;
  MDpartEventV1724* thePart = static_cast<MDpartEventV1724*>(aPartEventPtr);

  static int iPartEvt(0);
  // print only the first 10 particle events per board
  static uint32_t oldBoard(0);
  if ( GetBoardId() != oldBoard ) {
    oldBoard=GetBoardId();
    iPartEvt=0;
  }

  if (iPartEvt++ < 10) {
    *myObj << "In Equipment " << MDequipMap::GetName(GetEquipmentType()) ;
    *myObj << " ; Crate # " << GetLdcId() ;
    *myObj << " ; Board # " << GetBoardId() ;
    *myObj << " ; Particle Event Number : " << thePart->GetEventCount() << endl;
    *myObj << " Trigger Time Tag : " << thePart->GetTriggerTimeTag() << endl;

    // print data for the last channel
    for (int ich=0 ; ich<V1724_NCHANNELS ; ich++) {
      *myObj << " Channel : " << ich << "\n";
      if( thePart->GetLength(ich) )
      for (unsigned int is =0 ; is<thePart->GetNSamples(ich) ; is++) {
        *myObj << " " << thePart->GetSampleData(ich,is);
      }
      *myObj << endl;
    }
  }
  return OK;
}


//********* OnPartEventV1731 **************
// In this example, the flash adc data is unpacked and sent to the output stream
// the object to fill is meant to be an output stream
// The context data is taken directly from the Process Manager
//
class OnPartEventV1731 : public MDprocessor {
  public:
  OnPartEventV1731(ostream &aOs):MDprocessor(),myObj(&aOs){}
  virtual ~OnPartEventV1731(){}
  virtual int Process(MDdataContainer*);

  ostream* myObj;
};

int OnPartEventV1731::Process(MDdataContainer* aPartEventPtr){
  // cast the argument to structure it points to
  if ( typeid(*aPartEventPtr) != typeid(MDpartEventV1731) ) return CastError;
  MDpartEventV1731* thePart = static_cast<MDpartEventV1731*>(aPartEventPtr);

  static int iPartEvt(0);
  // print only the first 10 particle events per board
  static uint32_t oldBoard(0);
  if ( GetBoardId() != oldBoard ) {
    oldBoard=GetBoardId();
    iPartEvt=0;
  }

  if (iPartEvt++ < 10) {
    *myObj << "In Equipment " << MDequipMap::GetName(GetEquipmentType()) ;
    *myObj << " ; Crate # " << GetLdcId() ;
    *myObj << " ; Board # " << GetBoardId() ;
    *myObj << " ; Particle Event Number : " << thePart->GetEventCount() << endl;
    *myObj << " Trigger Time Tag : " << thePart->GetTriggerTimeTag() << endl;

    // print data for the last channel
    for (int ich=0 ; ich<V1724_NCHANNELS ; ich++) {
      *myObj << " Channel : " << ich << "\n";
      if( thePart->GetLength(ich) )
      for (unsigned int is =0 ; is<thePart->GetNSamples(ich) ; is++) {
        *myObj << " " << thePart->GetSampleData(ich,is);
      }
      *myObj << endl;
    }
  }
  return OK;
}

/* Application main */
int main( int argc, char **argv ) {
  string stringBuf;
//   int parArg;
  //char filepath[128];
  char runpath[128];
  char filename[128];

  // The following shows how to use the MDargumentHandler class
  // to deal with the main arguments
  // Define the arguments
  MDargumentHandler argh("Example of unpacking application.");
  argh.AddArgument("help","print this message","h");
  argh.AddArgument("directory","path for the data file","d","<string>","/home/daq/Downloads" );
  argh.AddArgument("file","List of data file names or run number (space separated, within double quote)","f","<string>","mandatory");

  // Check the user arguments consistancy
  // All mandatory arguments should be provided and
  // There should be no extra arguments
  if ( argh.ProcessArguments(argc,argv) ) { argh.Usage(); return -1; }

  // Treat arguments, obtain values to be used later
  if ( argh.GetValue("help") ) { argh.Usage(); return 0; }
  if ( argh.GetValue("directory",stringBuf) != MDARGUMENT_STATUS_OK ) return -1;
  strcpy(runpath,stringBuf.c_str());
  cout << "Directory : " << runpath << endl;
  if ( argh.GetValue("file",stringBuf) != MDARGUMENT_STATUS_OK ) return -1;
  strcpy(filename,stringBuf.c_str());

  // Declare the process Manager. This is mandatory ! 
  MDprocessManager thePM;

  // Used for the example only
  ContextData theContext; // This is only for illustration. Context data should be taken directly from the Process Manager

  // Declare and define the processors with their target objects and parameters
  OnSuperHeader*     superHeaderProc = new OnSuperHeader(cout);
  OnStartOfRun*      sorProc = new OnStartOfRun();
  OnStartOfSpill*    sosProc = new OnStartOfSpill();
  OnEndOfSpill*      eosProc = new OnEndOfSpill();
  OnEventHeader*     eventHeaderProc = new OnEventHeader(theContext);
  OnEquipmentHeader* equipmentHeaderProc = new OnEquipmentHeader(theContext);
  OnFragmentVLSB_C*  vlsb_cProc = new OnFragmentVLSB_C(cout,theContext);
  OnFragmentV830*    v830Proc = new OnFragmentV830(cout,theContext);
  OnFragmentV1290*   v1290Proc = new OnFragmentV1290();
  OnFragmentDBB*     dbbProc = new OnFragmentDBB();
  OnPartEventV1290*  v1290PartEventProc = new OnPartEventV1290(cout);
  OnPartEventV1724*  v1724PartEventProc = new OnPartEventV1724(cout);
  OnPartEventV1731*  v1731PartEventProc = new OnPartEventV1731(cout);

  // Set the user processors
  thePM.SetStartOfRunProc(sorProc);  // The ProcessManager will execute
                                             // sorProc->Process(&theEvent) each time a new
                                             // start of run event is found in the file
  thePM.SetStartOfSpillProc(sosProc);
  thePM.SetEndOfSpillProc(eosProc);
  thePM.SetSuperHeaderProc(superHeaderProc); // The ProcessManager will execute
                                             // superHeaderProc->Process(&theEvent) each time a
                                             // super event header is found in the file
  //thePM.SetEventHeaderProc(eventHeaderProc); // The ProcessManager will execute
                                             // eventHeaderProc->Process(&theEvent) each time a
                                             // normal LDC event header is found in the file
  //thePM.SetEquipmentHeaderProc(equipmentHeaderProc); // The ProcessManager will execute
                                             // equipmentHeaderProc->Process(&theEquipmentHeader) each
                                             //  time an equipment header is found in the file
  //thePM.SetFragmentProc("VLSB_C",vlsb_cProc); // for fragments and particle events, the equipment name must be specified
  thePM.SetFragmentProc("V830",v830Proc);
  //thePM.SetFragmentProc("V1290",v1290Proc);
  thePM.SetFragmentProc("DBB",dbbProc);
  thePM.SetPartEventProc("V1290",v1290PartEventProc);
  thePM.SetPartEventProc("V1724",v1724PartEventProc);
  thePM.SetPartEventProc("V1731",v1731PartEventProc);
  thePM.DumpProcessors();

  // Disable equipments that should not be decoded (in case of internal format error).
  // If an equipment is disabled, it is skipped at the equipment header level.
  //thePM.Disable("V1290");


  // Declare the DATE data file handler
  //MDdateFile dFile(filename,filepath);
  MDfileManager dFileManager(filename,runpath);
  if ( dFileManager.OpenFile() != DATE_FILE_OK ) return 0;	
  unsigned int nfiles = dFileManager.GetNFiles();
  cout<<"nfiles = "<<nfiles<<endl;
  // Open the file and loop over events
  unsigned char *eventBuffer;
  if ( nfiles ) { // There are valid files to unpack
    eventBuffer =  dFileManager.GetNextEvent(); // Get the pointer to the first word of the next event,
                                         // It is NULL if we have reached the end of files.
                                         // The event received here is a DAQ event !
                                         // A DAQ event may be of Type:
                                         // START_OF_RUN, END_OF_RUN,
                                         // START_OF_BURST (corresponds to a Start of Spill in MICE)
                                         // END_OF_BURST (corresponds to a End of Spill in MICE)
                                         // PHYSICS_EVENT (contains all the data for a Spill)
                                         // CALIBRATION_EVENT (may contains data from a single
                                         // LDC taken in between spills)
                                         // only START_OF_BURST, END_OF_BURST and PHYSICS_EVENT events are
                                         // embedded in a super event structure generated by the eventbuilder
   while ( eventBuffer ) { // Loop over valid DAQ events
      try {
        thePM.Process(eventBuffer); // Loop over the data and call the Proccess() function
                                    // of the appropriate MDprocessor object when the corresponding
                                    // structure is met.
      }
      // Deal with exceptions
      catch ( MDexception & lExc) {
        std::cerr <<  lExc.GetDescription() << endl;
        std::cerr << "Unpacking exception,  DAQ Event skipped" << std::endl;
      }
      catch(std::exception & lExc) {
        std::cerr << "Standard exception" << std::endl;
        std::cerr << lExc.what() << std::endl;
      }
      catch(...) {
         std::cerr << "Unknown exception occurred..." << std::endl;
      }
      eventBuffer =  dFileManager.GetNextEvent();
    }
  } else {
    cerr << "Error in opening the file. Aborted." << endl;
  }
  // Good practice: delete objects created with new;
  delete superHeaderProc;
  delete eventHeaderProc;
  delete equipmentHeaderProc;
  delete vlsb_cProc;
  delete v830Proc;
  delete v1290Proc;
  delete v1290PartEventProc;

  return 0;
} /* End of main */


