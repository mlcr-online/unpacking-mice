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

#include "MDdataWordV1290.h"

MDdataWordV1290::MDdataWordV1290(void *d):MDdataWord(d){}

unsigned long32 MDdataWordV1290::GetDataType(){
  return ( ( *(unsigned long32*)(_data)>>27 ) & 0x1f );
}

unsigned long32 MDdataWordV1290::GetChannel(){
  if ( GetDataType() != (uint32_t)DWV1290_Measurement ) _valid = false;
  else _valid = true;

  return ( ( *(unsigned long32*)(_data)>>21 ) & 0x1F );
}

unsigned long32 MDdataWordV1290::GetMeasurement(){
  if ( GetDataType() != (uint32_t)DWV1290_Measurement ) _valid = false;
  else _valid = true;

  return ( *(unsigned long32*)(_data) & 0x1FFFFF );
}

unsigned long32 MDdataWordV1290::GetMeasurementType(){
  if ( GetDataType() != (uint32_t)DWV1290_Measurement ) _valid = false;
  else _valid = true;

  return ( ( *(unsigned long32*)(_data)>>26 ) & 0x1 );
}

unsigned long32 MDdataWordV1290::GetTdc(){
  long32 dt = GetDataType();
  if ( dt != (uint32_t)DWV1290_TdcHeader  &&  
       dt != (uint32_t)DWV1290_TdcTrailer && 
       dt != (uint32_t)DWV1290_TdcError ) _valid = false;
  else _valid = true;

  return ( ( *(unsigned long32*)(_data)>>24 ) & 0x3 );
}

unsigned long32 MDdataWordV1290::GetEventID(){
  long32 dt = GetDataType();
  if ( dt != (uint32_t)DWV1290_TdcHeader &&  
       dt != (uint32_t)DWV1290_TdcTrailer ) _valid = false;
  else _valid = true;

  return ( ( *(unsigned long32*)(_data)>>12 ) & 0x0FFF );
}

unsigned long32 MDdataWordV1290::GetBunchID(){
  if ( GetDataType() != (uint32_t)DWV1290_TdcHeader ) _valid = false;
  else _valid = true;

  return ( *(unsigned long32*)(_data) & 0x0FFF );
}

unsigned long32 MDdataWordV1290::GetTriggerTimeTag(){
  if ( GetDataType() != (uint32_t)DWV1290_TriggerTimeTag ) _valid = false;
  else _valid = true;

  return ( *(unsigned long32*)(_data) & 0x7FFFFFF );
}

unsigned long32 MDdataWordV1290::GetWordCount(){
  long32 dt = GetDataType();
  switch (dt) {
  case DWV1290_TdcTrailer: 
    return ( *(unsigned long32*)(_data) & 0x0FFF );
    break; 
  case DWV1290_GlobalTrailer:
    return ( ( *(unsigned long32*)(_data)>>5 ) & 0xFFFF );
     break;
  default:
    return DWV1290_InvalidData;
    break;
  }
}

unsigned long32 MDdataWordV1290::GetStatus(){
  if ( GetDataType() != (uint32_t)DWV1290_GlobalTrailer ) _valid = false;
  else _valid = true;

  return ( ( *(unsigned long32*)(_data)>>24 ) & 0x7 );
}

unsigned long32 MDdataWordV1290::GetGeo(){
  long32 dt = GetDataType();
  if ( dt != (uint32_t)DWV1290_GlobalHeader &&  
       dt != (uint32_t)DWV1290_GlobalTrailer ) _valid = false;
  else _valid = true;

  return ( *(unsigned long32*)(_data) & 0x1F );
}

unsigned long32 MDdataWordV1290::GetEventCount(){
  if ( GetDataType() != (uint32_t)DWV1290_GlobalHeader ) _valid = false;
  else _valid = true;

  return ( ( *(unsigned long32*)(_data)>>5 ) & 0x3FFFFF );
}

unsigned long32 MDdataWordV1290::GetError(){
  if ( GetDataType() != (uint32_t)DWV1290_TdcError ) _valid = false;
  else _valid = true;

  return ( *(unsigned long32*)(_data) & 0x7FFF );
}

void MDdataWordV1290::Dump(int atTheTime){
  cout << *this;
  return;
} 

////////////////////////////////////////////////////////////////////////

ostream &operator<<(ostream &s,MDdataWordV1290 &dw){
  long32 dt= dw.GetDataType();
  s << showbase << hex;
  s << " CAEN V1290 ";
  switch (dt) {
  case DWV1290_Measurement:
    switch(dw.GetMeasurementType()){
    case DWV1290_LeadingMeas:
      s << "Leading edge Measurement:  ";
      break;
    case DWV1290_TrailingMeas:
      s << "Trailing edge Measurement: ";
      break;
    default:
      s << "Invalid Measurement!";
      s << dec << noshowbase << endl;
      return s;
      break;
    }
      s << dec << dw.GetMeasurement() << ";  Channel: " ;
      s << dw.GetChannel() << endl;
      break;
      
    case DWV1290_TdcHeader:
      s << "TDC Header:  TDC # " ;
      s << dec << dw.GetTdc();
      s << ";  Event ID: " ;
      s << dw.GetEventID();
      s << ";  Bunch ID: ";
      s << dw.GetBunchID()<< endl;
      break;
      
    case DWV1290_TdcTrailer:
      s << "TDC Trailer: TDC # " ;
      s << dec << dw.GetTdc();
      s << ";  Event ID: " ;
      s << dw.GetEventID();
      s << ";  Word Count: " ;
      s << dw.GetWordCount()<< endl;
      break;
      
    case DWV1290_TdcError:
      s << "TDC Error: " ;
      s << showbase << hex << dw.GetError() << " TDC #" ;
      s << dec << dw.GetTdc()<< endl;
      break;
      
    case DWV1290_GlobalHeader:
      s << "Global Header:  Event Count: " ;
      s << dec << dw.GetEventCount();
      s << "; Geo Address: " ;
      s << showbase << hex << dw.GetGeo() << endl;
      break;
      
    case DWV1290_GlobalTrailer:
      s << "Global Trailer: Word Count: ";
      s << dec << dw.GetWordCount();
      s << "; Geo Address: " ;
      s <<  showbase << hex << dw.GetGeo() << endl << endl;
      break;
      
    case DWV1290_TriggerTimeTag:
      s << "Trigger Time: ";
      s << dec << dw.GetTriggerTimeTag()<< endl;
      break;
      
    case DWV1290_Filler:
      s << "No data"<< endl;
      break;

    case DWV1290_InvalidData:
      s << "Invalid data"<< endl;
      break;

    default:
      s << "Data Type not implemented" << endl;
      break;
    }
  s << dec << noshowbase;
  return s;
}

