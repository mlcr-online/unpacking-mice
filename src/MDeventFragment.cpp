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

#include "MDeventFragment.h"

MDeventFragment::MDeventFragment(void *d):MDdataContainer(d){
  Init();
};

void MDeventFragment::Init() {
  _valid = false;
  if (_data) {
    _size = *EquipmentSizePtr();
    _valid = true;
  }
} 

void MDeventFragment::SetDataPtr( void *d ) { 
  MDdataContainer::SetDataPtr(d);
  Init();
}


void MDeventFragment::Dump(int atTheTime){
  MDfragment* frag;
  
  int i;
  cout << "- - - - - - - - - - - - - MDeventFragment Dump - - - - - - - - - - - - - - - - " << dec << endl;
  cout << "Size:" << *EquipmentSizePtr() 
       << " (header:" << sizeof(equipmentHeaderStruct) << ")"
       << " Type:" << *EquipmentTypePtr()
       << " EquipId:" << *EquipmentIdPtr()
       << " BasicSize:" << *BasicElementSizePtr() <<  endl;
  cout << "Attributes: (" ;
  for ( i = 0; i != ALL_ATTRIBUTE_WORDS; i++ ) {
    cout << noshowbase << setfill('0') << setw(8) << hex << EquipmentTypeAttributePtr()[i];
    if ( i != ALL_ATTRIBUTE_WORDS-1 ) cout << ".";
  }
  cout << ")" << dec << endl ; 

  frag = MDequipMap::GetFragmentPtr(*EquipmentTypePtr()); 
  // frag is assigned dynamically to the corret MDfragmentEquip 
  //    cout << "Equipment " << MDequipMap::GetName(*EquipmentTypePtr()) ;
  //    cout << " found in the equipment map with type " << frag->GetEquipmentType() << endl ;
  if(frag) { // The equipment has been found in the map
    frag->SetDataPtr(EquipmentDataPtr(), EquipmentDataSize());
    frag->InitPartEventVector();
    frag->Dump();
  } else {
    cout << "Equipment not found in the map " << endl ;
  }

};
