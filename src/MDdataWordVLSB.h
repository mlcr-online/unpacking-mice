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


#ifndef __MDDATWORDVLSB_H
#define __MDDATWORDVLSB_H

#include "MDdataWord.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

class MDdataWordVLSB : public MDdataWord {

 public:
  
  MDdataWordVLSB( void *d = 0 );
  ~MDdataWordVLSB(){}

  int32_t GetBankLength(int ibank){
    return (Get32bWordPtr()[ibank] & Header_Bank0Length_Mask);
  }

  int16_t GetAdc() {
    if (!IsValid()) return 0;
    return (Get32bWordPtr()[0] & Adc_Mask) >> Adc_Shift ;
  }

  int16_t GetTdc() {
    if (!IsValid()) return 0;
    return (Get32bWordPtr()[0] & Tdc_Mask) >> Tdc_Shift ;
  }

  int16_t GetChannel() {
    if (!IsValid()) return 0;
    return (Get32bWordPtr()[0] & Channel_Mask) >> Channel_Shift ;
  }

  bool GetDiscriBit() {
    if (!IsValid()) return false;
    return !!((Get32bWordPtr()[0] & Discri_Mask) >> Discri_Shift) ;
  }

  int16_t GetEventNum() {
    if (!IsValid()) return 0;
    return (Get32bWordPtr()[0] & EventNum_Mask) >> EventNum_Shift ;
  }

  bool IsValid();

  virtual void Dump( int atTheTime = 1 );

 private:

  enum Header {
    Header_Bank0Length_Mask    = 0x0003FFFF,
    Header_Bank0Length_Shift   = 0,
    Header_Bank0Length_Word    = 0,
    Header_Bank1Length_Mask    = 0x0003FFFF,
    Header_Bank1Length_Shift   = 0,
    Header_Bank1Length_Word    = 1,
    Header_Bank2Length_Mask    = 0x0003FFFF,
    Header_Bank2Length_Shift   = 0,
    Header_Bank2Length_Word    = 2,
    Header_Bank3Length_Mask    = 0x0003FFFF,
    Header_Bank3Length_Shift   = 0,
    Header_Bank3Length_Word    = 3
  };

  enum BankWord {
    Error_Word        = 0xdeadbeef,
    Adc_Shift         = 0,
    Adc_Mask          = 0x000000FF,
    Tdc_Shift         = 8,
    Tdc_Mask          = 0x0000FF00,
    Channel_Shift     = 16,
    Channel_Mask      = 0x007F0000,
    Discri_Shift      = 23,
    Discri_Mask       = 0x00800000,
    EventNum_Shift    = 24,
    EventNum_Mask     = 0xFF000000
  };

};

ostream &operator<<(ostream &s,MDdataWordVLSB &dw);
// istream &operator>>(istream &s,MDdataWordVLSB &dw);


#endif
