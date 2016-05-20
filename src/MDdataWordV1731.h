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

#ifndef __MDDATWORDV1731_H
#define __MDDATWORDV1731_H

#include "MDdataWord.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

typedef enum DWV1731Header {
  DWV1731_Header_Sync                = 0xA0000000,
  DWV1731_Header_Sync_Word           = 0,
  DWV1731_Header_Sync_Mask           = 0xF0000000,
  DWV1731_Header_WordCount_Mask      = 0x0FFFFFFF,
  DWV1731_Header_WordCount_Word      = 0,
  DWV1731_Header_WordCount_Shift     = 0,
  DWV1731_Header_Geo_Mask            = 0xF8000000,
  DWV1731_Header_Geo_Word            = 1,
  DWV1731_Header_Geo_Shift           = 27,
  DWV1731_Header_ZS_Mask             = 0x07000000,
  DWV1731_Header_ZS_Word             = 1,
  DWV1731_Header_ZS_Shift            = 24,
  DWV1731_Header_ZS_Channel_Mask     = 0xfff000,
  DWV1731_Header_ZS_Channel_Shift    = 12,
  DWV1731_Header_ZS_Length_Mask      = 0xfff,
  DWV1731_Header_ZS_Length_Shift     = 0,
  DWV1731_Header_Pattern_Mask        = 0x00FFFF00,
  DWV1731_Header_Pattern_Word        = 1,
  DWV1731_Header_Pattern_Shift       = 8,
  DWV1731_Header_ChannelMask_Mask    = 0x000000FF,
  DWV1731_Header_ChannelMask_Word    = 1,
  DWV1731_Header_ChannelMask_Shift   = 0,
  DWV1731_Header_EventCounter_Mask   = 0x00FFFFFF,
  DWV1731_Header_EventCounter_Word   = 2,
  DWV1731_Header_EventCounter_Shift  = 0,
  DWV1731_Header_TriggerTimeTag_Mask = 0xFFFFFFFF,
  DWV1731_Header_TriggerTimeTag_Word = 3,
  DWV1731_Header_TriggerTimeTag_Shift = 0

} DWV1731Header;

typedef enum DWV1731Sample {
  DWV1731_Sample_Shift      = 8,
  DWV1731_Sample_DataMask   =  0x000000FF,
  DWV1731_Sample_CheckMask  =  0x00000000

} DWV1731Sample;


class MDdataWordV1731 : public MDdataWord {

 public:
  
  MDdataWordV1731( void *d = 0 );
  ~MDdataWordV1731(){}

  int16_t GetSample(unsigned int is) {
    return (Get32bWordPtr()[0] >> is*DWV1731_Sample_Shift) & DWV1731_Sample_DataMask;
  }

  bool   IsValid(){
    if (_data)
      return 1; // TODO make this a real test valid for both data and header
    else
      return 0;
 }

  virtual void Dump( int atTheTime = 1 );

 private:
};

ostream &operator<<(ostream &s,MDdataWordV1731 &dw);
istream &operator>>(istream &s,MDdataWordV1731 &dw);


#endif
