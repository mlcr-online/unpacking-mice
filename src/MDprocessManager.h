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

#ifndef __MDPROCESSMANAGER_H
#define __MDPROCESSMANAGER_H

#include "MDprocessor.h"
#include "MDequipMap.h"
#include "MDevent.h"
#include "MDeventFragment.h"
#include "MDfragment.h"

typedef std::map<uint32_t,MDprocessor*> procMap_t;
typedef std::map<uint32_t,bool> enableMap_t;

class MDprocessManager {
 protected:
  MDequipMap _eMap; // The equipment map is a static singleton
                    // where all the equipments to be unpacked are defined
                    // together with their equipment Type ID, matching the
                    // DATE equipment data base.
  int      _status;
  uint32_t _runNumber;
  uint32_t _timeStamp;
  uint32_t _eventId;
  uint32_t _spillNumber;
  std::vector<uint32_t> _eventSizes;
  uint32_t _gdcId;
  uint32_t _ldcId;
  uint32_t _nLdcs;
  uint32_t _eventType;
  uint32_t _physEventNumber;
  uint32_t _partEventNumber;
  uint32_t _equipmentType;
  uint32_t _boardId;

  MDprocessor* _firstEventProc;
  MDprocessor* _startOfRunProc;
  MDprocessor* _endOfRunProc;
  MDprocessor* _startOfSpillProc;
  MDprocessor* _endOfSpillProc;

  MDprocessor* _superHeaderProc;
  MDprocessor* _eventHeaderProc;
  MDprocessor* _equipmentHeaderProc;

  procMap_t _fragmentProc;
  procMap_t _partEventProc;

  enableMap_t _enableMap;

  int ProcessSubEvent(unsigned char* aDataPtr);

 public:
  enum StatusCode {
    OK           = 0,
    FormatError  = 1,
    GenericError = 2
  };

  MDprocessManager();
  ~MDprocessManager(){}

  int Process(unsigned char *aDataPtr);

  void SetTest(DataTestCallback funk);

  void SetFirstEventProc(MDprocessor* aProc){_firstEventProc=aProc; aProc->SetProcessManager(this);}
  void SetStartOfRunProc(MDprocessor* aProc){_startOfRunProc=aProc; aProc->SetProcessManager(this);}
  void SetEndOfRunProc(MDprocessor* aProc){_endOfRunProc=aProc; aProc->SetProcessManager(this);}
  void SetStartOfSpillProc(MDprocessor* aProc){_startOfSpillProc=aProc; aProc->SetProcessManager(this);}
  void SetEndOfSpillProc(MDprocessor* aProc){_endOfSpillProc=aProc; aProc->SetProcessManager(this);}

  void SetSuperHeaderProc(MDprocessor* aProc){_superHeaderProc=aProc; aProc->SetProcessManager(this);}
  void SetEventHeaderProc(MDprocessor* aProc){_eventHeaderProc=aProc; aProc->SetProcessManager(this);}
  void SetEquipmentHeaderProc(MDprocessor* aProc){_equipmentHeaderProc=aProc; aProc->SetProcessManager(this);}
  void SetFragmentProc(unsigned int aType,MDprocessor* aProc);
  void SetFragmentProc(string aName,MDprocessor* aProc);
  void SetPartEventProc(unsigned int aType,MDprocessor* aProc);
  void SetPartEventProc(string aName,MDprocessor* aProc);

  void Disable(unsigned int aType){_enableMap[aType] = false;}
  void Enable(unsigned int aType){_enableMap[aType] = true;}
  void Disable(string aName){Disable(MDequipMap::GetType(aName));}
  void Enable(string aName){Enable(MDequipMap::GetType(aName));}
  bool IsEnabled(unsigned int aType){return _enableMap[aType];}
  bool IsEnabled(string aName){return IsEnabled(MDequipMap::GetType(aName));}

  void SetRunNumber(uint32_t aRunNumber){_runNumber = aRunNumber;}
  void SetTimeStamp(uint32_t aTimeStampPtr){_timeStamp = aTimeStampPtr;}
  void SetTimeStamp();
  void SetEventId(uint32_t aEventId){_eventId = aEventId;}
  void SetGdcId(uint32_t aGdcId){_gdcId = aGdcId;}
  void SetLdcId(uint32_t aLdcId){_ldcId = aLdcId;}
  void SetEventType(uint32_t aEventType){_eventType = aEventType;}
  void SetSpillNumber(uint32_t aNum){_spillNumber = aNum;}
  void SetPhysEventNumber(uint32_t aNum){_physEventNumber = aNum;}
  void SetEquipmentType(uint32_t aEquipmentType){_equipmentType = aEquipmentType;}
  void SetBoardId(uint32_t aBoardId){_boardId = aBoardId;}

  uint32_t GetRunNumber(){return _runNumber;}
  uint32_t GetEventId(){return _eventId;}
  uint32_t GetTimeStamp(){return _timeStamp;}
  uint32_t GetGdcId(){return _gdcId;}
  uint32_t GetLdcId(){return _ldcId;}
  uint32_t GetEventType(){return _eventType;}
  uint32_t GetSuperEventSize(){return _eventSizes[0];}
  uint32_t GetSubEventSize(int LdcId){return _eventSizes[LdcId+1];}
  int GetSpillNumber(){return (int)_spillNumber;}
  uint32_t GetPhysEventNumber(){return _physEventNumber;}
  uint32_t GetPartEventNumber(){return _partEventNumber;}
  uint32_t GetEquipmentType(){return _equipmentType;}
  uint32_t GetBoardId(){return _boardId;}

  void     DumpContextData();
  void     DumpProcessors();
  std::string   GetTimeString();

};

#endif
