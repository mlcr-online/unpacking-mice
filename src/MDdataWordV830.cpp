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


#include "MDdataWordV830.h"

//default constructor
MDdataWordV830::MDdataWordV830(void *d):MDdataWord(d){}

long32 MDdataWordV830::GetDataType(){
  return ( ( *(unsigned long32*)( _data ) >> 26 ) & 0x1 );
}

long32 MDdataWordV830::GetGeo(){
  long32 dt = GetDataType();
  if ( dt != (uint32_t)DWV830_Header ) _valid = false;
  else _valid = true;
  return ( ( *(unsigned long32*)(_data) >> 27 ) & 0x1F ); 
}

long32 MDdataWordV830::GetTriggerType(){
  long32 dt = GetDataType();
  if ( dt != (uint32_t)DWV830_Header ) _valid = false;
  else _valid = true;
  return ( ( *(unsigned long32*)(_data) >> 16 ) & 0x03 ); 
}


long32 MDdataWordV830::GetWordCount(){
  long32 dt = GetDataType();
  if ( dt != (uint32_t)DWV830_Header ) _valid = false;
  else _valid = true;
  return ( ( *(unsigned long32*)(_data) >> 18 ) & 0x3F ); 
}

long32 MDdataWordV830::GetTriggerCount(){
  long32 dt = GetDataType();
  if ( dt != (uint32_t)DWV830_Header ) _valid = false;
  else _valid = true;
  return ( ( *(unsigned long32*)(_data) ) & 0xFFFF ); 
}

long32 MDdataWordV830::GetChannel(){
  long32 dt = GetDataType();
  if ( dt != (uint32_t)DWV830_Measurement ) _valid = false;
  else _valid = true;
  return ( ( *(unsigned long32*)(_data) >> 27 ) & 0x1F ); 
}

long32 MDdataWordV830::GetMeasurement(){
  long32 dt = GetDataType();
  if ( dt != (uint32_t)DWV830_Measurement ) _valid = false;
  else _valid = true;
  return ( ( *(unsigned long32*)(_data) ) & 0x3FFFFFF ); 
}

void MDdataWordV830::Dump(int atTheTime){
  cout << *this;
  return;
} 


ostream &operator<<( ostream &s,MDdataWordV830 &dw){
  long32 dt= dw.GetDataType();
  s << showbase << hex;
  s << " CAEN V830 ";
  switch ( dt ) {
  case DWV830_Measurement:
    {
      s  << " Ch " << setw(2) << dec << dw.GetChannel() ;
      s << " ; data: " << dec << dw.GetMeasurement() << endl ;
      break;
    }
  case DWV830_Header:
    {
      s << "GEO: " << dw.GetGeo();
      s << ", Word Count: " << dw.GetWordCount();
      s << ", Trigger Type: ";
      switch( dw.GetTriggerType() ){
      case DWV830_ExternalTrigger:
        {
	  s << "External trigger";
	  break;
	}
      case DWV830_TriggerViaTimer:
  	{
	  s << "Trigger via timer";
	  break;
	}
      case DWV830_TriggerViaVME:
        {
	  s << "Trigger via VME";
	  break;
	}
      default:
        {
	  s << "Invalid Trigger";
	  break;
	}
      }
      s << ", Trigger count: " << dw.GetTriggerCount() << endl;
      break;
    } 
  default:
    {
      s << "Invalid Data!" << endl;
      s << dec << noshowbase;
     return s;
    }
  }
  s << dec << noshowbase;
  return s;
}
