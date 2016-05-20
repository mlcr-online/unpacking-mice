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

#include "MDRebuildManager.h"

bool MDRebuildManager::Scan() {
  bool data_ERR;
  cout << "\n### Scanning the raw data files ###\n\n";
  for(unsigned int xfile=0; xfile<_infiles.size(); xfile++) {
    cout << "--- Scanning file " << _infiles[xfile]->GetFileName() << " --> ";
    bool Err = this->Scan(_infiles[xfile]);
    if (!Err) cout << "OK\n";

    data_ERR |= Err;
  }

  return data_ERR;
}

int MDRebuildManager::Scan(MDdateFile *infile) {
  int nEv(0), nErr(0);
  unsigned char *eventBuffer;
  eventBuffer =  infile->GetNextEvent();
//  while (nEv < 30) {
  while ( eventBuffer ) {
    long pos = infile->_curPos;
//     cout << "\n\nEvent " << nEv << "   pos: " << pos << endl;
    try {
      MDevent theEvent(eventBuffer);
      if (theEvent.EventType()==PHYSICS_EVENT && theEvent.IsSuperEvent()) {
        int nSubEvts = theEvent.NsubEvent();
//         cout << "" << _infile->_curPos << " type: " << theEvent.EventType()
//                                       << " nSub: " << nSubEvts
//                                       << " size: " << theEvent.EventSize()
//                                       << " hsize: " << theEvent.HeadSize() << endl;

        uint32_t nPart(0), nPart_this(0);
        pos += theEvent.HeadSize();
        if (nSubEvts>1)
        for (int iEvt=0; iEvt<nSubEvts; iEvt++){
          unsigned char *subEvPtr = (unsigned char*)theEvent.GetSubEventPtr(iEvt);
          MDevent theSubEvent(subEvPtr);
//           cout << pos << " type: " << theSubEvent.EventType()
//                                    << " size: " << theSubEvent.EventSize() << endl;

          int xPart = MDprocessManager::ProcessSubEvent(subEvPtr);
          _addresMap[iEvt].push_back(EvntLabel(pos, xPart, theSubEvent.EventSize()));
          pos += theSubEvent.EventSize();

          if (nPart==0) nPart = xPart;
          else {
            nPart_this = xPart;
            if (nPart != nPart_this) {
              nErr++;
              stringstream ss;
              ss << "ERROR in  in MDRebuildManager::Scan :"<< endl;
              ss << "Trigger Mismatch (nEvts " << nPart << "!=" << nPart_this << ").\n";
//             throw MDexception( ss.str() );
              if (nErr<5) cout << ss.str();
            }
          }
//       cout << "nPart: " << nPart << " " << nPart_this << endl;
        }
      nEv++;
      }
    } // Deal with exceptions
    catch ( MDexception & lExc) {
      cerr <<  lExc.GetDescription() << endl;
      cerr << "Unpacking exception,  DAQ Event skipped" << endl;// return 1;
      nErr++;
    } catch(exception & lExc) {
      cerr << "Standard exception" << endl;
      cerr << lExc.what() << endl;
      nErr++;
    } catch(...) {
      cerr << "Unknown exception occurred..." << endl;
      nErr++;
    }

    eventBuffer = infile->GetNextEvent();
  }

  return nErr;
}

bool MDRebuildManager::Reorder() {

  std::vector<EvntFIFO> newmap;
  newmap.resize(_nLDCs);

  EvntLabel subEv0, subEv1;
  int xEvt=0;
  while(1) {
    if(_addresMap[0].empty() || _addresMap[1].empty())
      break;

    subEv0 = _addresMap[0].front();
    subEv1 = _addresMap[1].front();
//     cout << xEvt << " @ " << subEv0._nPartEv << " " << subEv1._nPartEv << endl;
    if (subEv0._nPartEv == subEv1._nPartEv) {
      newmap[0].push_back(subEv0);
      newmap[1].push_back(subEv1);
      _addresMap[0].pop_front();
      _addresMap[1].pop_front();
    } else if ( !Synchronize() ) {
      cout << " ERROR: Unable to restore the event synchronization.\n"
           << " Trying again\n";
        if( !Synchronize() ) {
          cout << " ERROR: Unable to restore the event synchronization.\n"
               << "Give up!";
          return false;
        }
    }
    xEvt++;
  }

  _addresMap = newmap;
  return true;
}
void printV(std::vector<int> v) {
  cout << "pt: " << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << endl;
}
bool MDRebuildManager::Synchronize() {
  // Remove 3 spills from the first LDC. In principle the messy spill must be only one, but ...
  _addresMap[0].erase(_addresMap[0].begin(), _addresMap[0].begin()+3);
  std::vector<int> pt_vector0(4), pt_vector_this(4);
  std::vector<bool> ldfc_ok(false, _nLDCs-1);
  // For the first LDC - create a vector with the number of particle triggers for 4 spills.
  for(int xSpill=0; xSpill<4; xSpill++)
    pt_vector0[xSpill] = _addresMap[0][xSpill]._nPartEv;

//   printV(pt_vector0);
  // Now try to resynchronize this LDC with the other LDCs.
  for(int xLDC=1;xLDC<_nLDCs;xLDC++) {
    // Try 10 possible shifts.
    for(int xSpill_shift=0; xSpill_shift<10; xSpill_shift++) {
      // Create a vector with the number of particle triggers for this particular case.
      for(int xSpill=0; xSpill<4; xSpill++)
        pt_vector_this[xSpill] = _addresMap[xLDC][xSpill_shift+xSpill]._nPartEv;

//       printV(pt_vector_this);
      if(pt_vector0==pt_vector_this){
        _addresMap[xLDC].erase(_addresMap[xLDC].begin(), _addresMap[xLDC].begin()+xSpill_shift);
        ldfc_ok[xLDC] = true;
        break;
      }
    }
  }
  for(int xLDC=1;xLDC<_nLDCs;xLDC++)
    if(ldfc_ok[xLDC]==false)
      return false;

  return true;
}

void MDRebuildManager::Rebuild() {
  // Get the Run Number from the name of tha first data file.
  int    runNum_int;
  stringstream ss;
  ss.str(_infiles[0]->GetFileName());
  ss >> runNum_int;

  for(unsigned int xfile=0; xfile<_infiles.size(); xfile++) {
    if(_addresMap[0].empty() || _addresMap[1].empty() )
      break;

    cout << "--- Rebuilding file " << _infiles[xfile]->GetFileName() << endl;

    ss.str("");
    // Create a new binary file. The rebuilt data will be recorded here.
    if (xfile<10) ss << runNum_int << ".90" << xfile;
    else if (xfile<100) ss << runNum_int << ".9" << xfile;
    else ss << runNum_int << "." << xfile;
    cout << "Creating file " << ss.str() << endl;
    ofstream outfile (ss.str().c_str(), ofstream::binary);

    this->Rebuild(_infiles[xfile], &outfile);
    outfile.close();
  }
}

void MDRebuildManager::Rebuild(MDdateFile *infile, ofstream *outfile) {
//   cout << "MDRebuildManager::Rebuild()  " << _infile->GetFileName() << endl;
  // Go back to the begining of the original binary file.
  infile->Reset();

  int nEvts=0;
  unsigned char *eventBuffer;
  eventBuffer =  infile->GetNextEvent();

//  while (nEvts < 30) {
  while ( eventBuffer ) {
    long pos = infile->_curPos;
    long pos_next = infile->GetStreamPos();
    MDevent theEvent(eventBuffer);
    if( theEvent.EventType()!=PHYSICS_EVENT) {
       outfile->write((char*)eventBuffer, theEvent.EventSize());
    } else {

      EvntLabel subEv0 = _addresMap[0].front();
      EvntLabel subEv1 = _addresMap[1].front();
//       cout << "rebuilding Event " << nEvts << "   pos: " << pos << "  next pos: " << pos_next << endl;
//       cout << "SubEv0 pos: " << subEv0._pos << "  SubEv1 pos: " << subEv1._pos << endl;

      if (subEv0._pos < pos || subEv1._pos < pos) {
        // This is an event in the next file. Skip few spills and continue with the next file.
        while ( (subEv0._pos > pos || subEv1._pos > pos) && 
               !_addresMap[0].empty() && ! _addresMap[1].empty() ) {
//           cout << "Removing SubEv0 pos: " << subEv0._pos << "  SubEv1 pos: " << subEv1._pos << endl;
          _addresMap[0].pop_front();
          _addresMap[1].pop_front();
          subEv0 = _addresMap[0].front();
          subEv1 = _addresMap[1].front();
        }
        break;
      }

      _addresMap[0].pop_front();
      _addresMap[1].pop_front();

      ( *theEvent.EventSizePtr() ) = theEvent.HeadSize() +  subEv0._size + subEv1._size;
      outfile->write((char*)eventBuffer, theEvent.HeadSize());

      unsigned char *buffer = new unsigned char[subEv0._size];
      infile->GetEvent(buffer, subEv0._pos, subEv0._size);
      MDevent theSubEvent0(buffer);
//       cout /*<< (int*)eventBuffer */<< "SubEv0 type: " << theSubEvent0.EventType()
//                                     << " pos: " << subEv0._pos
//                                     << " size: " << subEv0._size << endl;
      outfile->write((char*)buffer, subEv0._size);
      delete[] buffer;

      buffer = new unsigned char[subEv1._size];
      infile->GetEvent(buffer, subEv1._pos, subEv1._size);
      MDevent theSubEvent1(buffer);
//       cout /*<< (int*)eventBuffer */<< "SubEv1 type: " << theSubEvent1.EventType()
//                                     << " pos: " << subEv1._pos
//                                     << " size: " << subEv1._size << endl;
      outfile->write((char*)buffer, subEv1._size);
      delete[] buffer;

//       cout << nEvts << " " << subEv0._nPartEv << " " << subEv1._nPartEv << endl;
      nEvts++;
      infile->GoTo(pos_next);
    }

//     cout << "FIFO size: " <<_addresMap[0].size() << " " << _addresMap[1].size() << endl;
    if(_addresMap[0].empty() || _addresMap[1].empty() )
      break;

    eventBuffer = infile->GetNextEvent();
  }
}

void MDRebuildManager::DumpAddresMap(int LdcId) {
//   int nEvts = _addresMap[LdcId];
//   cout << "#### AddresMap LDC ####" << LdcId << endl;
//   for (int iEvt=0; iEvt<nEvts; iEvt++) {
//     cout << iEvt << " addres: " << _addresMap[LdcId][iEvt].first
//          << "  nTr: " << _addresMap[LdcId][iEvt].second << endl;
//   }
//   cout << "#######################\n\n";
}


