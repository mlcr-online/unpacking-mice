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

#ifndef __MDEQUIPMAP_H
#define __MDEQUIPMAP_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>

class MDfragment;
#include "MDdataContainer.h"

#define ADD_EQUIP_IN_MAP(t,s)  _equipMap[(t)] = new MDequipDesc((#s),new MDfragment##s)

using namespace std;

class MDequipDesc {
 private:
  string              name;
  MDfragment*         dcPtr;

  public:
  MDequipDesc(string s, MDfragment* ptr):name(s),dcPtr(ptr){
    //        cout << " Create a new MDequipDesc" << endl;
  }
  ~MDequipDesc(){}
  string GetName(){return name;}
  MDdataContainer* GetDataContainerPtr();
  MDfragment* GetFragmentPtr(){return dcPtr;}

};

typedef map<uint32_t, MDequipDesc*> equipMap_t ;

class MDequipMap {
 private:
  void SetEquipmentTypes();
  equipMap_t::iterator find( unsigned int type ) {
    return _equipMap.find(type);
  }

 public:
  //  static equipMap_t _equipMap;
  static equipMap_t _equipMap;
  static int _count;

  MDequipMap();
  ~MDequipMap();
  static void Dump();

  static equipMap_t::iterator begin() {
    return _equipMap.begin();
  }

  static equipMap_t::iterator end() {
    return _equipMap.end();
  }

  static unsigned int size() {
    return _equipMap.size();
  }


  static bool IsDefined(unsigned int aType ){
    if (_count) {
      equipMap_t::iterator it=_equipMap.find(aType);
      if ( it->first == aType ) return true;
    }
    return false;
  }

  static  unsigned int GetType( string name );

  static bool IsDefined(string name){
    return !!GetType(name);
  }

  static string GetName( unsigned int type ) {
    if (IsDefined(type)) {return _equipMap[type]->GetName();}
    return string("Unknown");
  }

  static MDdataContainer* GetDataContainerPtr(int type) {
    if (IsDefined(type)) {return _equipMap[type]->GetDataContainerPtr();}
    return NULL;
  }

  static MDfragment* GetFragmentPtr(int type) {
    if (IsDefined(type)) {return _equipMap[type]->GetFragmentPtr();}
    return NULL;
  }

  static MDfragment* GetFragmentPtr(string name) {
    unsigned int type = GetType(name);
    if (IsDefined(type)) {return _equipMap[type]->GetFragmentPtr();}
    return NULL;
  }

};


#endif
