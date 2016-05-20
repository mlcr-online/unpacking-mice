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

// #include "TFile.h"
// #include "TH1D.h"

#include "MDprocessManager.h"
#include "MDevent.h"
#include "MDdateFile.h"
#include "MDfileManager.h"
#include "MDargumentHandler.h"
#include "MDpartEventV1290.h"
#include "MDpartEventV1724.h"
#include "MDfragmentVLSB.h"
// vector< vector<TH1D*> > histos;


//********* OnPartEventV1290 **************
// In this example, the tdc data is unpacked and sent to the output stream
// the object to fill is meant to be an output stream
// The context data is taken directly from the Process Manager
//
class OnPartEventV1290 : public MDprocessor {
  public:
  OnPartEventV1290():MDprocessor(){}
  virtual ~OnPartEventV1290(){}
  virtual int Process(MDdataContainer*);
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
    cout << "In Equipment " << MDequipMap::GetName(GetEquipmentType()) ;
    cout << " ; Crate # " << GetLdcId() ;
    cout << " ; Board # " << GetBoardId() ;
    cout << " ; Particle Event Number : " << thePart->GetEventCount() << endl;
    cout << " Number of Leading Edge Hits : " << thePart->GetNHits(V1290_NCHANNELS,'l') << endl;
    cout << " Number of Trailing Edge Hits : " << thePart->GetNHits(V1290_NCHANNELS,'t') << endl;
    cout << " Total Number of Hits : " << thePart->GetNHits() << endl;
    cout << " Bunch ID : " << thePart->GetBunchID(0) ;
    cout << " ; Trigger Time Tag : " << thePart->GetTriggerTimeTag() << endl;
    // loop over channels
    if (!thePart->GetNHits(0,'l')) {
      cerr << " Missing hit in particle trigger channel " << endl;
      return FormatError;
    }
    for (int ich =0 ; ich<V1290_NCHANNELS ; ich++) {
      // print leading edge (default) hits data
      if (thePart->GetNHits(ich,'l')) { cout << " Channel : " << ich << " :";}
      for (unsigned int ih =0 ; ih<thePart->GetNHits(ich,'l') ; ih++) {
        cout << " " << thePart->GetHitMeasurement(ih,ich,'l');
      }
      if (thePart->GetNHits(ich,'l')) { cout << endl; }
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
  OnPartEventV1724(): MDprocessor() {}
  virtual ~OnPartEventV1724(){}
  virtual int Process(MDdataContainer*);
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

  if (iPartEvt<10) {
    iPartEvt++;
    std::cout << "In Equipment " << MDequipMap::GetName(GetEquipmentType()) ;
    std::cout << " ; Crate # " << GetLdcId() ;
    std::cout << " ; Board # " << GetBoardId() ;
    std::cout << " ; Particle Event Number : " << thePart->GetEventCount() << endl;
    std::cout << " Trigger Time Tag : " << thePart->GetTriggerTimeTag() << endl;

    // print data for the last channel
    for (int ich=0 ; ich<V1724_NCHANNELS ; ich++) {
      int nSamples = thePart->GetLength(ich)*2;
      std::cout << " Channel : " << ich << "   " << nSamples << endl;
      for (int is =0 ; is<nSamples ; is++) {
        std::cout << " " << thePart->GetSampleData(ich,is);
//         histos[GetBoardId()][ich]->Fill(is, thePart->GetSampleData(ich,is));
      }
      std::cout << endl;
    }
  }

  return OK;
}

//********* OnFragmentVLSB **************
// In this example, useful for Tracker cosmic data
// the object to fill is meant to be an output stream
// and the parameter is meant to be a instance of a user defined class
// describing the context
//
class OnFragmentVLSB : public MDprocessor {
  public:
  OnFragmentVLSB():MDprocessor() {}
  virtual ~OnFragmentVLSB(){}
  virtual int Process(MDdataContainer*);
};

int OnFragmentVLSB::Process(MDdataContainer* aFragPtr){
  // cast the argument to structure it points to
  // This process should be called only with MDfragmentVLSB_C argument
  if ( typeid(*aFragPtr) != typeid(MDfragmentVLSB) ) return CastError;
  MDfragmentVLSB* theFrag = static_cast<MDfragmentVLSB*>(aFragPtr);

    // print context
  int xLdc, xTimeStamp, xPhysEvNum;
  string xDetector;
 
  xLdc = this->GetLdcId();
  xTimeStamp = this->GetTimeStamp();
  xPhysEvNum = this->GetPhysEventNumber();

  cout << " In " << MDequipMap::GetName(theFrag->GetEquipmentType());
  cout << " LDC # " << xLdc; 
  cout << " board # " << theFrag->GetBoardID() << endl;
  cout << " PhysEvNum # " << xPhysEvNum; 
  cout << " time # " << xTimeStamp << endl;

  return OK;
}

class OnPartEventVLSB : public MDprocessor {
  public:
  OnPartEventVLSB(): MDprocessor() {}
  virtual ~OnPartEventVLSB(){}
  virtual int Process(MDdataContainer*);
};

int OnPartEventVLSB::Process(MDdataContainer* aPartEventPtr){
  // cast the argument to structure it points to
  if ( typeid(*aPartEventPtr) != typeid(MDpartEventVLSB) ) return CastError;
  MDpartEventVLSB* thePart = static_cast<MDpartEventVLSB*>(aPartEventPtr);
  cout << "\n\nVLSB Part. evt: " << thePart->GetEventNum() << endl;
  thePart->Dump();
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
  if ( argh.ProcessArguments(argc, argv) ) {argh.Usage(); return -1;}

  // Treat arguments, obtain values to be used later
  if ( argh.GetValue("help") ) {argh.Usage(); return 0;}
  if ( argh.GetValue("directory", stringBuf) != MDARGUMENT_STATUS_OK ) return -1;
  strcpy(runpath,stringBuf.c_str());
  cout << "Directory : " << runpath << endl;
  if ( argh.GetValue("file", stringBuf) != MDARGUMENT_STATUS_OK ) return -1;
  strcpy(filename, stringBuf.c_str());

  // Declare the process Manager. This is mandatory ! 
  MDprocessManager thePM;

  OnPartEventV1724*  v1724PartEventProc = new OnPartEventV1724();
  thePM.SetPartEventProc("V1724", v1724PartEventProc);

  OnPartEventV1290*  v1290PartEventProc = new OnPartEventV1290();
  thePM.SetPartEventProc("V1290", v1290PartEventProc);

  OnPartEventVLSB *vLSBPartEventProc = new OnPartEventVLSB();
  thePM.SetPartEventProc("VLSB", vLSBPartEventProc);

//   OnFragmentVLSB*  vlsbProc = new OnFragmentVLSB();
//   thePM.SetFragmentProc("VLSB",vlsbProc);

  thePM.DumpProcessors();

  // Disable equipments that should not be decoded (in case of internal format error).
  // If an equipment is disabled, it is skipped at the equipment header level.
//   thePM.Disable("V1731");
//  thePM.Disable("DBBChain");
//   thePM.Disable("V1290");
//  thePM.Disable("V1724");

//   TFile file("histos.root", "recreate");
//   histos.resize(9);
//   for(int xBoard=0; xBoard<9; xBoard++) {
//     for(int xCh=0; xCh<8; xCh++) {
//       stringstream ss;
//       ss << "board" << xBoard << "ch" << xCh;
//       TH1D *h = new TH1D(ss.str().c_str(), ss.str().c_str(),  64, -.5, 63.5);
// //       TH1D *h = new TH1D(ss.str().c_str(), ss.str().c_str(), 128, -.5, 127.5);
//       histos[xBoard].push_back(h);
//     }
//   }


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

  int xEv(0);
//    while (5 > xEv) {
  while ( eventBuffer ) { // Loop over valid DAQ events
//     cout << "\n\nEvent " << xEv << endl;
    MDevent theEvent(eventBuffer);
    if (theEvent.IsSuperEvent()) {
//      int nSubEvts = theEvent.NsubEvent();
//       cout << " type: " << theEvent.EventType()
//                                     << " nSub: " << nSubEvts
//                                     << " size: " << theEvent.EventSize()
//                                     << " hsize: " << theEvent.HeadSize() << endl;
     }
      xEv++;
      try {
        thePM.Process(eventBuffer); // Loop over the data and call the Proccess() function
                                    // of the appropriate MDprocessor object when the corresponding
                                    // structure is met.
      }
      // Deal with exceptions
      catch ( MDexception & lExc) {
        std::cerr <<  lExc.GetDescription() << endl;
        std::cerr << "Unpacking exception,  DAQ Event skipped" << std::endl;// return 1;
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
  delete v1724PartEventProc;
  delete v1290PartEventProc;
  delete vLSBPartEventProc;

//   for(int xBoard=0; xBoard<9; xBoard++)
//     for(int xCh=0; xCh<8; xCh++)
//       histos[xBoard][xCh]->Write();
// 
//   file.Close();

  return 0;
} /* End of main */


