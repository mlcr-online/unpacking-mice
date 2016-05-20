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

#ifndef __MDPROCESSOR_H
#define __MDPROCESSOR_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>
#include <string>

#include "MDdataContainer.h"

class MDprocessManager;

class MDprocessor {
 protected:
  //  void* _objPtr; // the object the process will fill
  //  void* _parPtr; // The parameter structure used for the process
  MDprocessManager* _proMan; // The process manager this process belongs to
  //  bool  _valid;

 public:
  enum ReturnCode {
    OK           = 0,
    FormatError  = 1,
    CastError    = 2,
    GenericError = 3
  };

  // MDprocessor(void *aObjPtr=NULL, void *aParPtr=NULL):_objPtr(aObjPtr),_parPtr(aParPtr),_proMan(0){}
 MDprocessor():_proMan(NULL){}  virtual ~MDprocessor(){}

  virtual int Process(MDdataContainer*) {return OK;}
  void SetProcessManager(MDprocessManager* aPM){_proMan=aPM;}

  MDprocessManager* GetProcessManager(){return _proMan;}

  uint32_t GetRunNumber();
  uint32_t GetTimeStamp();
  std::string   GetTimeString();
  uint32_t GetLdcId();
  uint32_t GetEventType();
  uint32_t GetSpillNumber();
  uint32_t GetPhysEventNumber();
  uint32_t GetPartEventNumber();
  uint32_t GetEquipmentType();
  uint32_t GetBoardId();

};

#endif
