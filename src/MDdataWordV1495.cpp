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


#include "MDdataWordV1495.h"

MDdataWordV1495::MDdataWordV1495(void *d)
: MDdataWord(d) {}

unsigned long32 MDdataWordV1495::GetDataType() {
  if (IsValid()) return ( (*(unsigned long32*)(_data) & DataTypeMask ) >> DataTypeShift );
  return 0;
}

unsigned long32 MDdataWordV1495::GetTriggerCount() {
  if (IsValid())  return ( (*(unsigned long32*)(_data) & TriggerCountMask ) >> TriggerCountShift );
  return 0;
}

unsigned long32 MDdataWordV1495::GetTriggerTime() {
  if (IsValid())  return ( (*(unsigned long32*)(_data) & TriggerTimeMask ) >> TriggerTimeShift );
  return 0;
}

unsigned long32 MDdataWordV1495::GetNTriggers() {
  if (IsValid())  return ( (*(unsigned long32*)(_data) & NTriggersMask ) >> NTriggersShift );
  return 0;
}

unsigned long32 MDdataWordV1495::GetGeo() {
  if (IsValid())  return ( (*(unsigned long32*)(_data) & BoardIdMask ) >> BoardIdShift );
  return 0;
}

unsigned long32 MDdataWordV1495::GetSpillNumber() {
  if (IsValid())  return ( (*(unsigned long32*)(_data) & SpillNumberMask ) >> SpillNumberShift );
  return 0;
}

unsigned long32 MDdataWordV1495::GetPatternTOF0() {
  unsigned int *dw = Get32bWordPtr(1);
  if (IsValid())  return ( (*dw) & PatterTOF0Mask ) >> PatterTOF0Shift;
  return 0;
}

unsigned long32 MDdataWordV1495::GetPatternTOF1() {
  if( !IsValid() )  return 0;

  unsigned int *dw0 = Get32bWordPtr(0);
  unsigned int *dw1 = Get32bWordPtr(1);
  unsigned int pattern_msb, pattern_lsb;

  pattern_msb = ( (*dw0) & PatterTOF1Mask0 ) >> PatterTOF1Shift0;
  pattern_lsb = ( (*dw1) & PatterTOF1Mask1 ) >> PatterTOF1Shift1;

//   cout << hex <<  pattern_msb << " " << pattern_lsb << dec << endl;
  return ( pattern_msb << PatterTOF1Mask1Size ) + pattern_lsb;
}

unsigned long32 MDdataWordV1495::GetPatternTOF2() {
  if (IsValid())  return ( (*(unsigned long32*)(_data) & PatterTOF2Mask ) >> PatterTOF2Shift );
  return 0;
}

unsigned long32 MDdataWordV1495::GetBusy(int i) {
  int mask(0), shift(0);
  switch (i) {
    case 0:
      mask  = Busy0Mask;
      shift = Busy0Shift;

    case 1:
      mask  = Busy1Mask;
      shift = Busy1Shift;

    case 2:
      mask  = Busy2Mask;
      shift = Busy2Shift;

    case 3:
      mask  = Busy3Mask;
      shift = Busy3Shift;

    case 4:
      mask  = Busy4Mask;
      shift = Busy4Shift;

    case 5:
      mask  = Busy5Mask;
      shift = Busy5Shift;
  }

  if (IsValid())  return ( (*(unsigned long32*)(_data) & mask ) >> shift );
  return 0;
}


