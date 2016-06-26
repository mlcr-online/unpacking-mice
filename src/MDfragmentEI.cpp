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

#include <sstream>

#include "MDfragmentEI.h"

using namespace std;

void MDfragmentEI::SetDataPtr( void *d, uint32_t aSize )  {
  MDfragment::SetDataPtr(d, aSize);
  this->Init();
}

void MDfragmentEI::Init() {
  _madeOfParticles = false;
  UnValidate();

  if (this->GetSize() == 0)
    return;

  _pv_id.resize(0);
  _pv_value.resize(0);
//   cout << "Calling MDfragmentEI::Init()\n";
  uint32_t* ptr = Get32bWordPtr(0);
//   cout << ptr << endl;
  MDdataWordEI dw(ptr);
  if ( dw.IsValid() ) {
    if (dw.getHpHeaderType() != MDdataWordEI::HP_HeaderType) {
      throw MDexception("ERROR in MDframentEI::Init() : 1st word is not a HP header");
    } else {
      int nPVs = dw.getHpHeaderSize();
      cout << " n PVs : " << nPVs << endl;
      _pv_id.resize(nPVs);
      _pv_value.resize(nPVs);
      for (int i=0; i<nPVs; ++i) {
        dw.SetDataPtr(++ptr);
        if (dw.getPvType() != MDdataWordEI::PV_doubleType) {
          throw MDexception("ERROR in MDframentEI::Init() : HP data type is not consistent");
        }

        _pv_id[i] = dw.getPvId();
          dw.SetDataPtr(++ptr);
        _pv_value[i] = dw.getHP();
        ptr++; // the value is of a type double (64b).
      }
    }
  }
  Validate();
}

unsigned int MDfragmentEI::GetPvId(unsigned int i) {
  if  (i >= _pv_id.size() ) {
    stringstream ss;
    ss << "ERROR in MDframentEI::GetPvId() : PV index out of range ("
       << i << ">" << _pv_id.size() -1 << ")\n";
    throw MDexception(ss.str());
  }

  return _pv_id[i];
}

double MDfragmentEI::GetPvValue(unsigned int i) {
  if  (i >= _pv_id.size() ) {
    stringstream ss;
    ss << "ERROR in MDframentEI::GetPvValue() : PV index out of range ("
       << i << ">" << _pv_id.size() -1 << ")\n";
    throw MDexception(ss.str());
  }

  return _pv_value[i];
}

ostream &operator<<(std::ostream &s,MDfragmentEI &df) {
  s << "**** Epics Interface  ****" << endl;
  int size = df.GetNPVs();
  s << "HP size: " << size << endl;
  for (int i=0;i<size;++i)
    s << i << " PV Id: " << df.GetPvId(i) << "  value: " << df.GetPvValue(i) << endl;

  s << "**************************" << endl;
  return s;
}



