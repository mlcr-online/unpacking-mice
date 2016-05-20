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

#include "MDprocessManager.h"

using namespace std;

uint32_t MDprocessor::GetRunNumber(){
  if (_proMan) return _proMan->GetRunNumber();
  return 0;
}

uint32_t MDprocessor::GetTimeStamp(){
  if (_proMan) return _proMan->GetTimeStamp();
  return 0;
}

string MDprocessor::GetTimeString(){
  if (_proMan) return _proMan->GetTimeString();
  return "";
}

uint32_t MDprocessor::GetLdcId(){
  if (_proMan) return _proMan->GetLdcId();
  return 0;
}

uint32_t MDprocessor::GetEventType(){
  if (_proMan) return _proMan->GetEventType();
  return 0;
}

uint32_t MDprocessor::GetSpillNumber(){
  if (_proMan) return _proMan->GetSpillNumber();
  return 0;
}

uint32_t MDprocessor::GetPhysEventNumber(){
  if (_proMan) return _proMan->GetPhysEventNumber();
  return 0;
}

uint32_t MDprocessor::GetPartEventNumber(){
  if (_proMan) return _proMan->GetPartEventNumber();
  return 0;
}

uint32_t MDprocessor::GetEquipmentType(){
  if (_proMan) return _proMan->GetEquipmentType();
  return 0;
}

uint32_t MDprocessor::GetBoardId(){
  if (_proMan) return _proMan->GetBoardId();
  return 0;
}
