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

#ifndef __MDEVENTFRAGMENT_H
#define __MDEVENTFRAGMENT_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <map>

#ifdef _STEPIV_DATA
  #include "event.h"
#else
  #include "event_old.h"
#endif

#include "MDequipMap.h"
#include "MDdataWord.h"
#include "MDfragment.h"
#include "MDdataContainer.h"

using namespace std;

class MDeventFragment  : public MDdataContainer {
 private:
  void Init();

 public:
  MDeventFragment(void *d=0);

  virtual ~MDeventFragment(){}
  virtual void SetDataPtr( void *d );

  equipmentHeaderStruct* HeaderPtr(){ return (equipmentHeaderStruct *) _data; }

  unsigned long32  EquipmentSize(){ return  HeaderPtr()->equipmentSize ; }
  long32* EquipmentSizePtr(){ return &(HeaderPtr()->equipmentSize) ; }
  unsigned long32  EquipmentType(){ return  HeaderPtr()->equipmentType ; }
  long32* EquipmentTypePtr(){ return &(HeaderPtr()->equipmentType); }
  unsigned long32  EquipmentId(){ return  HeaderPtr()->equipmentId ; }
  long32* EquipmentIdPtr(){ return &(HeaderPtr()->equipmentId); }
  unsigned long32* EquipmentTypeAttributePtr(){ return HeaderPtr()->equipmentTypeAttribute; }
  unsigned long32  BasicElementSize(){ return  HeaderPtr()->equipmentBasicElementSize ; }
  long32* BasicElementSizePtr(){ return &(HeaderPtr()->equipmentBasicElementSize); }

  unsigned long32  EquipmentDataSize() {
    return *EquipmentSizePtr() - sizeof(equipmentHeaderStruct);
  }

  unsigned char* EquipmentDataPtr() {
    return _data + sizeof(equipmentHeaderStruct);
  }

  virtual void Dump(int atTheTime=1);


};

#endif
