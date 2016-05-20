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

#include "MDfragmentV1495.h"

MDfragmentV1495::MDfragmentV1495(void *d) : MDfragment(d) {
 if(d)   Init();
}
void MDfragmentV1495::SetDataPtr(void *d, uint32_t aSize=0) {
  MDfragment::SetDataPtr(d, aSize);
}

void MDfragmentV1495::Init() {
  MDdataWordV1495 dw(_payLoad);
  if (dw.GetDataType() != MDdataWordV1495::Header) {
    throw MDexception("ERROR in MDfragmentV1495::Init() : 1st word is not a header") ;
  }

  _spillNum = dw.GetSpillNumber();
  _geo = dw.GetGeo();

  dw.SetDataPtr(_payLoad + this->GetSize()/4 - 1);
  if (dw.GetDataType()    != MDdataWordV1495::Trailer ||
      dw.GetSpillNumber() != _spillNum) {
    throw MDexception("ERROR in MDfragmentV1495::Init() : trailer is not consistent") ;
  }

  _nTriggers = dw.GetNTriggers();
}


unsigned int MDfragmentV1495::GetTriggerTime(unsigned int tr) {
  // The maximum number of trigger records is 300.
  // This number is determined by the size of the FIFO.
  if (tr > 299)
    return 0;

  MDdataWordV1495 dw( Get32bWordPtr(tr*3 + 1) );
  if (dw.GetDataType() != MDdataWordV1495::Data) {
    throw MDexception("ERROR in MDfragmentV1495::GetTriggerTime(int) : wrong data type.") ;
  }

  dw.SetDataPtr( this->Get32bWordPtr(tr*3 + 3) );
  if (dw.GetTriggerCount() != tr) {
    throw MDexception("ERROR in MDfragmentV1495::GetTriggerTime(int) : wrong trigger count.") ;
  }
  return dw.GetTriggerTime();
}

unsigned long32 MDfragmentV1495::GetPatternTOF0(unsigned int tr, char p) {
  // The maximum number of trigger records is 300.
  // This number is determined by the size of the FIFO.
  if (tr > 299)
    return 0;

  MDdataWordV1495 dw( this->Get32bWordPtr(tr*3 + 1) );
  dw.SetSize(8);
  if (dw.GetDataType() != MDdataWordV1495::Data) {
    throw MDexception("ERROR in MDfragmentV1495::GetPatternTOF0(int) : wrong data type.") ;
  }

  unsigned long pattern = dw.GetPatternTOF0();
  if (p == 'v')
    return pattern && 0x3FF;
  else if (p == 'h')
    return (pattern && 0xFFC00) > 10;
  else
    return pattern;
}

unsigned long32 MDfragmentV1495::GetPatternTOF1(unsigned int tr, char p) {
  // The maximum number of trigger records is 300.
  // This number is determined by the size of the FIFO.
  if (tr > 299)
    return 0;

  MDdataWordV1495 dw( this->Get32bWordPtr(tr*3 + 1) );
  dw.SetSize(8);
  if (dw.GetDataType() != MDdataWordV1495::Data) {
    throw MDexception("ERROR in MDfragmentV1495::GetPatternTOF1(int) : wrong data type.") ;
  }

  unsigned long pattern = dw.GetPatternTOF1();
  if (p == 'v')
    return pattern && 0x3FF;
  else if (p == 'h')
    return (pattern && 0xFFC00) > 10;
  else
    return pattern;
}

unsigned long32 MDfragmentV1495::GetPatternTOF2(unsigned int tr, char p) {
  // The maximum number of trigger records is 300.
  // This number is determined by the size of the FIFO.
  if (tr > 299)
    return 0;

  MDdataWordV1495 dw( this->Get32bWordPtr(tr*3 + 1) );
  if (dw.GetDataType() != MDdataWordV1495::Data) {
    throw MDexception("ERROR in MDfragmentV1495::GetPatternTOF2(int) : wrong data type.") ;
  }

  unsigned long pattern = dw.GetPatternTOF2();
  if (p == 'v')
    return pattern && 0x3FF;
  else if (p == 'h')
    return (pattern && 0xFFC00) > 10;
  else
    return pattern;
}

void MDfragmentV1495::Dump() {
  cout << *this;
  return;
}

ostream &operator<<(std::ostream &s,MDfragmentV1495 &df) {
  s << " ------------ MICE Trigger (CAEN V1495) ------------ " << endl ;
  s << " Geo        : " << df.GetGeo() << endl;
  s << " Spill Num  : " << df.GetSpillNumber() << endl;
  unsigned int nTr = df.GetNumTriggers();
  s << " N Triggers : " << nTr << endl;
  s << " ------------           Data            ------------ " << endl ;
  for (unsigned int i=0; i < nTr; i++) {
    if (i>299)
      s << "tr" << i << " no record" << endl;
    else
      s << "tr" << i << " time: " << df.GetTriggerTime(i)*10 << " ns"
        << "  Pattern TOF0(" << std::hex << df.GetPatternTOF0(i) << ")"
        << "  TOF1(" <<  df.GetPatternTOF1(i) << ")"
        << "  TOF2(" <<  df.GetPatternTOF2(i) << ")"
        << std::dec << endl;
  }
  s << " --------------------------------------------------- " << endl ;
  return s;
}

