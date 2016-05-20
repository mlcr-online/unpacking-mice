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

#include "MDequipMap.h"
#include "MDfragment.h"
#include "MDfragmentV1290.h"
#include "MDfragmentV1724.h"
#include "MDfragmentV830.h"
#include "MDfragmentDBB.h"
#include "MDfragmentDBBChain.h"
#include "MDfragmentVRB.h"
#include "MDfragmentV1731.h"
#include "MDfragmentVLSB_C.h"
#include "MDfragmentVLSB.h"
#include "MDfragmentVLSBMaster.h"

int MDequipMap::_count = 0;
equipMap_t MDequipMap::_equipMap;

MDequipMap::MDequipMap()
{
  if (MDequipMap::_count++ == 0) {
    //cout << " Creating the equipment map ..." << endl;
    ADD_EQUIP_IN_MAP(111, V830);
    ADD_EQUIP_IN_MAP(102, V1290);
    ADD_EQUIP_IN_MAP(121, V1731);
    ADD_EQUIP_IN_MAP(120, V1724);
    ADD_EQUIP_IN_MAP(140 ,VRB);
    ADD_EQUIP_IN_MAP(141, DBB);
    ADD_EQUIP_IN_MAP(143, DBBChain);
    ADD_EQUIP_IN_MAP(80,  VLSB_C);
    ADD_EQUIP_IN_MAP(132, VLSB);
    ADD_EQUIP_IN_MAP(130, VLSBMaster);

    SetEquipmentTypes();
//     Dump();
  } else {
    cerr << "WARNING in MDequipMap::MDequipMap() : Trying to create a multiple instances of a static singleton class" << endl;
    Dump();
  }
}

MDequipMap::~MDequipMap(){
    // delete fragments and descriptors
  if (MDequipMap::_count != 0) {
    equipMap_t::iterator it=_equipMap.begin();
    for (unsigned int i=0; i < _equipMap.size(); i++,it++) {
      delete (it->second)->GetFragmentPtr();
      delete (it->second);
    }
    _count = 0;
  }
}


void  MDequipMap::SetEquipmentTypes(){
  equipMap_t::iterator it=_equipMap.begin();
  //  cout << " Setting Equipment types..." << endl;
  for (unsigned int i=0; i < _equipMap.size(); i++,it++) {
    (it->second)->GetFragmentPtr()->SetEquipmentType(it->first);
  }
  //    cout << " ... Done with Equipment types" << endl;

}

unsigned int MDequipMap::GetType( string name )
{
  if (_count) {
    equipMap_t::iterator it=_equipMap.begin();
    for (unsigned int i=0; i < _equipMap.size(); i++,it++) {
      if ( (it->second)->GetName() == name ) return it->first;
    }
  }
  return 0;
}

void MDequipMap::Dump()
{
  cout << " --- Equipment Map Dump --- " << endl;
  equipMap_t::iterator it=_equipMap.begin();
  for (unsigned int i=0; i < _equipMap.size(); i++) {
    cout << dec << it->first << " " << hex << showbase << it->second << " "
         << (it->second)->GetName() << " " << (it->second)->GetDataContainerPtr() << dec << noshowbase << endl;
    it++;
  }
  cout << "Equipment Map count : " << MDequipMap::_count << endl;
}

MDdataContainer* MDequipDesc::GetDataContainerPtr(){return (MDdataContainer*) dcPtr;}
