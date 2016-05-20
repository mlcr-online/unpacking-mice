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

#include "MDdataWordDBB.h"

MDdataWordDBB::MDdataWordDBB(void *d):MDdataWord(d){}

unsigned long32 MDdataWordDBB::GetDataType()
{
  if (IsValid()) return ( (*(unsigned long32*)(_data) & DataTypeMask ) >> DataTypeShift );
  return 0;
}

unsigned long32 MDdataWordDBB::GetTriggerCount()
{
  if (IsValid())  return ( (*(unsigned long32*)(_data) & TriggerCountMask ) >> TriggerCountShift );
  return 0;
}

unsigned long32 MDdataWordDBB::GetHitCount()
{
  if (IsValid())  return ( (*(unsigned long32*)(_data) & HitCountMask ) >> HitCountShift );
  return 0;
}

unsigned long32 MDdataWordDBB::GetGeo()
{
  if (IsValid())  return ( (*(unsigned long32*)(_data) & BoardIdMask ) >> BoardIdShift );
  return 0;
}

unsigned long32 MDdataWordDBB::GetSpillNumber()
{
  if (IsValid())  return ( (*(unsigned long32*)(_data) & SpillNumberMask ) >> SpillNumberShift );
  return 0;
}

unsigned long32 MDdataWordDBB::GetChannelId()
{
  if (IsValid())  return ( (*(unsigned long32*)(_data) & ChannelIdMask ) >> ChannelIdShift );
  return 0;
}

unsigned long32 MDdataWordDBB::GetHitTime()
{
  if (IsValid())  return ( (*(unsigned long32*)(_data) & HitTimeMask ) >> HitTimeShift );
  return 0;
}

unsigned long32 MDdataWordDBB::GetStatus()
{
  if (IsValid())  return ( (*(unsigned long32*)(_data) & StatusMask ) >> StatusShift );
  return 0;
}

unsigned long32 MDdataWordDBB::GetSpillWidth()
{
  if (IsValid())  return ( (*(unsigned long32*)(_data) & SpillWidthMask ) >> SpillWidthShift );
  return 0;
}







