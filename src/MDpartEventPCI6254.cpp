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

#include "MDpartEventPCI6254.h"

MDpartEventPCI6254::MDpartEventPCI6254(void *d):MDdataContainer(d),_headerSize(0),_clockRate(0),_nSamplesPerEvent(0),_nChannels(0),_tv_sec(0),_tv_usec(0){ 
  Init();
}

void MDpartEventPCI6254::SetDataPtr( void *d ) {
  MDdataContainer::SetDataPtr(d);
  Init();
}

void MDpartEventPCI6254::Init(){
  unsigned int * ptr = Get32bWordPtr(0);
  MDdataWordPCI6254 dw(ptr);
  UnValidate();
  if ( dw.IsValid() ) {
    _headerSize = ptr[0];
    if ( ptr[GetHeaderLength()-1] != PCI6254_HEADER_SYNC ) {
      cout << "ERROR in MDpartEventPCI6254::Init: Header is not consistent" << endl;
      cerr << "ERROR in MDpartEventPCI6254::Init: Header is not consistent" << endl;
      return;
    }
    _headerVersion = ptr[1];
    switch (_headerVersion) { // header version 1
    case 1: 
	_clockRate = ptr[2];
	_nSamplesPerEvent = ptr[3];
	_nChannels = ptr[4];
	_tv_sec = ptr[5];
	_tv_usec = ptr[6];
	SetSize(_nChannels*_nSamplesPerEvent*4+_headerSize);
	_vSeq.resize(_nChannels);
	for (unsigned int ich=0 ; ich<_nChannels ; ich++) {
	  _vSeq[ich] = (float*) Get32bWordPtr(ich*_nSamplesPerEvent + GetHeaderLength());
	}
	Validate();
	break;
    default:
      cout << "ERROR in MDpartEventPCI6254::Init: Header version not recognised" << endl;
      cerr << "ERROR in MDpartEventPCI6254::Init: Header version not recognised" << endl;
      return;
      break;
    }
  }
}

float MDpartEventPCI6254::GetMeasurement(unsigned int ich, unsigned int iSample)
{
  if (ich < _nChannels && iSample< _nSamplesPerEvent) {
    return _vSeq[ich][iSample];
  } else {
    return 0;
  }
}

float* MDpartEventPCI6254::GetSequence(unsigned int ich)
{
  if (ich < _nChannels ) {
    return _vSeq[ich];
  } else {
    return NULL;
  }
}

void MDpartEventPCI6254::Dump(int atTheTime)
{
  if (IsValid()) {
    /*
    cout << "****  PCI6254  ****" << endl;
    unsigned int wc(0);
    unsigned int * ptr = Get32bWordPtr( 0 );
    MDdataWordPCI6254 dw(ptr) ;
    while (wc < _nChannels*_nSamplesPerEvent + GetHeaderLength()) {
      cout << dw ;
      dw.SetDataPtr( ++ptr );
      wc++;
    }
    */
    cout << "****  PCI6254 Header ****" << endl;
    cout << " Header Size: " << dec << _headerSize << endl;
    cout << " Header Version: " << dec << _headerVersion << endl;
    switch (_headerVersion) { // header version 1
    case 1:
      cout << " Clock Rate : " << dec << _clockRate << endl;
      cout << " Number of Sample per event : " << dec << _nSamplesPerEvent << endl;
      cout << " Number of Channels : " << dec << _nChannels << endl;
      cout << " Time tag : " << dec << ctime((time_t*)&_tv_sec);
      cout << " Micro sec  : " << dec << _tv_usec << endl;
      break;
    default:
      for (unsigned int iw=2 ; iw < GetHeaderLength()-1 ; iw++) {
	cout << dec << *Get32bWordPtr(iw) << endl; 
      }
      break;
    }
    cout << " Sync Word: " << hex << showbase << *Get32bWordPtr(GetHeaderLength()-1) << endl;
    cout << "****  End of PCI6254 Header ****" << endl;
    for (unsigned int iSam=0 ; iSam< _nSamplesPerEvent ; iSam++ ) {
      for (unsigned int ich=0 ; ich < _nChannels ; ich++ ) {
	cout << GetSequence(ich)[iSam] << " " ;
      }
      cout << endl;
    }
  }
  return;
} 

////////////////////////////////////////////////////////////////////////


