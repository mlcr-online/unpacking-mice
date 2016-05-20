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


#ifndef __MDDATWORDV1290_H
#define __MDDATWORDV1290_H

#include "MDdataWord.h"
// #include "MDTypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

typedef enum DWV1290MeasType {
  DWV1290_LeadingMeas  = 0,
  DWV1290_TrailingMeas = 1,
  DWV1290_InvalidMeas  = 2
} DWV1290MeasType;

typedef enum DWV1290TrailerStatus {
  DWV1290_StatusTriggerLost  = 0x1,
  DWV1290_StatusTdcError     = 0x2,
  DWV1290_StatusBufferOflw   = 0x4
} DWV1290TrailerStatus;

typedef enum DWV1290DataType {
  DWV1290_Measurement     = 0x00,
  DWV1290_TdcHeader       = 0x01,
  DWV1290_TdcTrailer      = 0x03,
  DWV1290_TdcError        = 0x04,
  DWV1290_GlobalHeader    = 0x08,
  DWV1290_GlobalTrailer   = 0x10,
  DWV1290_TriggerTimeTag  = 0x11,
  DWV1290_Filler          = 0x18,
  DWV1290_InvalidData     = 0x1F

} DWV1290DataType;

typedef enum DWV1290ErrorType {
  DWV1290_HitLostGr0_FifoOflw    = 0x0001,
  DWV1290_HitLostGr0_L1Oflw      = 0x0002,
  DWV1290_ErrorGr0               = 0x0004,
  DWV1290_HitLostGr1_FifoOflw    = 0x0008,
  DWV1290_HitLostGr1_L1Oflw      = 0x0010,
  DWV1290_ErrorGr1               = 0x0020,
  DWV1290_HitLostGr2_FifoOflw    = 0x0040,
  DWV1290_HitLostGr2_L1Oflw      = 0x0080,
  DWV1290_ErrorGr2               = 0x0100,
  DWV1290_HitLostGr3_FifoOflw    = 0x0200,
  DWV1290_HitLostGr3_L1Oflw      = 0x0400,
  DWV1290_ErrorGr3               = 0x0800,
  DWV1290_EventSizeLimit         = 0x1000,
  DWV1290_EventLost_TrgFifoOflw  = 0x2000,
  DWV1290_ChipFatalError         = 0x4000
} DWV1290ErrorType;


class MDdataWordV1290 : public MDdataWord {

 public:
  
  MDdataWordV1290(void *d=0);
  ~MDdataWordV1290(){}

  unsigned long32 GetDataType();

  unsigned long32 GetMeasurement();
  unsigned long32 GetChannel();
  unsigned long32 GetMeasurementType();

  unsigned long32 GetEventCount();
  unsigned long32 GetGeo();

  unsigned long32 GetBunchID();
  unsigned long32 GetEventID();
  unsigned long32 GetTdc();
  unsigned long32 GetError();
  unsigned long32 GetTriggerTimeTag();
  unsigned long32 GetWordCount();
  unsigned long32 GetStatus();
  virtual void Dump(int atTheTime=1);
 private:
};

ostream &operator<<(ostream &s,MDdataWordV1290 &dw);
istream &operator>>(istream &s,MDdataWordV1290 &dw);


#endif
