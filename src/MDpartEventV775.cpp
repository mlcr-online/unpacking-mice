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

#include "MDpartEventV775.h"
using namespace std;

MDpartEventV775::MDpartEventV775(void *d)
:MDdataContainer(d), _evtCount(0), _eob_record(false) {
  UnValidate();
  Init();
}

void MDpartEventV775::Init() {
  MDdataWordV775 dw;
  unsigned int *header = Get32bWordPtr(0);
  int nWords=0;
  unsigned int length;
  if (header) {
    dw.SetDataPtr(header);
    if ( dw.DWGetDataType() == DWV775_Header ) {
      Validate();
      length = dw.DWGetChannelCount();
//      int geo = dw.DWGetGeo();

      nWords = V775_HEADER_WORDS + length + V775_EOB_WORDS;
      this->SetSize(nWords*V775_WORD_SIZE);
    }
    else {
      //cout << "Data Word : " << hex << showbase << Get32bWordPtr(0) << dec << noshowbase << endl;
      throw MDexception("ERROR in MDpartEventV775::Init: INVALID particle Event! Fist word is not a header.");
    }
  }

  unsigned int *endOfBlock = header + length + 1;
  dw.SetDataPtr(endOfBlock);
  if ( dw.DWGetDataType() == DWV775_EOB ) {
    _evtCount = dw.DWGetEventCount();
//     cout << "EOB reached TDC. Setting evtCount to " << dw.DWGetEventCount() << endl;
  }
}

void MDpartEventV775::SetDataPtr( void *d ) {
  MDdataContainer::SetDataPtr(d);
  Init();
}

unsigned int MDpartEventV775::GetGeo() {
  if( !IsValid() )
    return 0;

  MDdataWordV775 dw;
  unsigned int *header = Get32bWordPtr(0);
  dw.SetDataPtr(header);
  return dw.DWGetGeo();
}

unsigned int MDpartEventV775::GetChannelCount() {
  if( !IsValid() )
    return 0;

  MDdataWordV775 dw;
  unsigned int *header = Get32bWordPtr(0);
  dw.SetDataPtr(header);
  return dw.DWGetChannelCount();
}

int MDpartEventV775::GetHitChannel(unsigned int i) {
  if( !IsValid() )
    return -1;

  if (i >= this->GetChannelCount())
    return -1;

  MDdataWordV775 dw;
  unsigned int *mesur = Get32bWordPtr(i+V775_HEADER_WORDS);
  dw.SetDataPtr(mesur);
  return dw.DWGetChannel();
}

int MDpartEventV775::GetHitMeasurement(unsigned int i) {
  if( !IsValid() )
    return -1;

  if (i >= this->GetChannelCount())
    return -1;

  MDdataWordV775 dw;
  unsigned int *measure = Get32bWordPtr(i+V775_HEADER_WORDS);
  dw.SetDataPtr(measure);
  return dw.DWGetMeasurement();
}


int MDpartEventV775::GetEvtCount(){
  int chan = this->GetChannelCount();
  unsigned int *eob = Get32bWordPtr(V775_HEADER_WORDS + chan);
  MDdataWordV775 dw;
  dw.SetDataPtr(eob);
  _eob_record = true;
  return dw.DWGetEventCount();
}

ostream& operator<<(ostream &s,MDpartEventV775 &ev) {
  s << " ------------  CAEN V775 Header        ------------ " << endl ;
  s << " Geo      : " << ev.GetGeo() ;
  s << " Ch Count : " << ev.GetChannelCount() << endl;
  s << " ------------  CAEN V775 Data          ------------ " << endl ;
  for (unsigned int i=0; i<ev.GetChannelCount(); i++)
    s << "Ch " << ev.GetHitChannel(i)
      << " --> TDC: " << ev.GetHitMeasurement(i) << endl;
  s << " ------------  CAEN V775 End of Block  ------------ " << endl ;
  s << " EvtCount: " << ev.GetEvtCount();
  if (ev.EOB()) s << endl;
  else s << "  EOB " << endl;

  return s;
}
