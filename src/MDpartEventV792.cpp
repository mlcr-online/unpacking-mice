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

#include "MDpartEventV792.h"
using namespace std;

MDpartEventV792::MDpartEventV792(void *d)
:MDdataContainer(d), _evtCount(0) {
  UnValidate();
  Init();
}

void MDpartEventV792::Init() {
  MDdataWordV792 dw;
  unsigned int *header = Get32bWordPtr(0);
  int nWords=0;
  unsigned int length;
  if (header) {
    dw.SetDataPtr(header);
    if ( dw.DWGetDataType() == DWV792_Header ) {
      Validate();
      length = dw.DWGetChannelCount();
//      int geo = dw.DWGetGeo();

      nWords = V792_HEADER_WORDS + length + V792_EOB_WORDS;
      this->SetSize(nWords*V792_WORD_SIZE);
    } 
    else {
      //cout << "Data Word : " << hex << showbase << Get32bWordPtr(0) << dec << noshowbase << endl;
      throw MDexception("ERROR in MDpartEventV792::Init: INVALID particle Event! Fist word is not a header.");
    }
  }

  unsigned int *endOfBlock = header + length + 1;
  dw.SetDataPtr(endOfBlock);
  if ( dw.DWGetDataType() == DWV792_EOB ) {
    _evtCount = dw.DWGetEventCount();
//     cout << "EOB reached. Setting evtCount to " << dw.DWGetEventCount() << endl ;
  }
}

void MDpartEventV792::SetDataPtr( void *d ) {
  MDdataContainer::SetDataPtr(d);
  Init();
}

unsigned int MDpartEventV792::GetGeo() {
  if( !IsValid() )
    return 0;

  MDdataWordV792 dw;
  unsigned int *header = Get32bWordPtr(0);
  dw.SetDataPtr(header);
  return dw.DWGetGeo();
}

unsigned int MDpartEventV792::GetChannelCount() {
  if( !IsValid() )
    return 0;

  MDdataWordV792 dw;
  unsigned int *header = Get32bWordPtr(0);
  dw.SetDataPtr(header);
  return dw.DWGetChannelCount();
}

int MDpartEventV792::GetHitChannel(unsigned int i) {
  if( !IsValid() )
    return -1;

  if (i >= this->GetChannelCount())
    return -1;

  MDdataWordV792 dw;
  unsigned int *mesur = Get32bWordPtr(i+V792_HEADER_WORDS);
  dw.SetDataPtr(mesur);
  return dw.DWGetChannel();
}

int MDpartEventV792::GetHitMeasurement(unsigned int i) {
  if( !IsValid() )
    return -1;

  if (i >= this->GetChannelCount())
    return -1;

  MDdataWordV792 dw;
  unsigned int *measure = Get32bWordPtr(i+V792_HEADER_WORDS);
  dw.SetDataPtr(measure);
  return dw.DWGetMeasurement();
}

ostream& operator<<(ostream &s,MDpartEventV792 &ev) {
  s << " ------------  CAEN V792 Header        ------------ " << endl ;
  s << " Geo      : " << ev.GetGeo() << endl;
  s << " Ch Count : " << ev.GetChannelCount() << endl;
  s << " ------------  CAEN V792 Data          ------------ " << endl ;
  for (unsigned int i=0; i<ev.GetChannelCount(); i++)
    s << "Ch " << ev.GetHitChannel(i)
      << " -->  ADC: " << ev.GetHitMeasurement(i) << endl;
  s << " ------------  CAEN V792 End of Block  ------------ " << endl ;
  s << " EvtCount: " << ev.GetEvtCount();
  if (ev.EOB()) s << endl;
  else s << " EOB " << endl;

  return s;
}

