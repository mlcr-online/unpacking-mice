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

#include "MDfragmentDBBChain.h"

using namespace std;
MDfragmentDBBChain::MDfragmentDBBChain(void *d)
: MDfragment(d) {
  Init();
  for (int i=0; i<DBBS_IN_CHAIN; i++) {
    MDfragmentDBB *dbb = new MDfragmentDBB();
    dbb->SetEquipmentType(141);
    _dbb.push_back(dbb);
  }
}

MDfragmentDBBChain::~MDfragmentDBBChain() {
  for (int i=0; i<DBBS_IN_CHAIN; i++) {
    _dbb.resize(0);
  }
}

void MDfragmentDBBChain::SetDataPtr(void *d, uint32_t aSize=0) {
  MDfragment::SetDataPtr(d, aSize);
}

void MDfragmentDBBChain::Init() {
  UnValidate();
  int size = this->GetSize();
  if (size == 0)
    return;

  if (size >8) {
    uint32_t* ptr = this->Get32bWordPtr(0);
    int ndw_dbb(0);
    for (int i=0; i<DBBS_IN_CHAIN; i++) {
      int this_dbb_size = this->getDBBSize(ptr, size/4-ndw_dbb);
     _dbb[i]->SetDataPtr(ptr, this_dbb_size*4);
//      _dbb[i]->dumpEventVRB(ptr, this_dbb_size*4);
      ptr += this_dbb_size;
      ndw_dbb += this_dbb_size;
    }
  }
}

int MDfragmentDBBChain::getDBBSize(uint32_t *ptr, int max_size) {
  int size(0);
  uint32_t *header = ptr;
  MDdataWordDBB dw(header);
  if (dw.GetDataType() != MDdataWordDBB::Header ){ // The data doesn't start with a header
    throw MDexception(" ERROR in MDframentDBBChain::getDBBSize(uint32_t *ptr) : 1st word is not a header") ;
  } else {
    dw.SetDataPtr(++ptr);
    size++;
    while (1) {
      if (size > max_size) 
        throw MDexception(" ERROR in MDframentDBBChain::getDBBSize(uint32_t *ptr) : the trailer is missing") ;

      dw.SetDataPtr(ptr);
      if (dw.GetDataType() == MDdataWordDBB::Trailer) {
        size += DBB_TRAILER_WORDS;
        break;
      } else {
        ptr++;
        size++;
      }
    }
  }
  return size;
}

void MDfragmentDBBChain::Dump(int atTheTime) {}










