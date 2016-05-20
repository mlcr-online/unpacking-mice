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

#include "MDdataWordV775.h"

//default constructor
MDdataWordV775::MDdataWordV775(void *d):MDdataWord(d){}

long32 MDdataWordV775::DWGetDataType(){
  return ( ( *(unsigned long32*)( _data ) >> 24 ) & 0x7 );
}

long32 MDdataWordV775::DWGetGeo(){
  long32 dt = DWGetDataType();
  if ( dt != (uint32_t)DWV775_Header ) 
    _valid = false;
  else 
    _valid = true;
  return ( ( *(unsigned long32*)(_data) >> 27 ) & 0x1F );
}

long32 MDdataWordV775::DWGetChannelCount(){
  long32 dt = DWGetDataType();
  if ( dt != (uint32_t)DWV775_Header ) 
    _valid = false;
  else 
    _valid = true;
  return ( ( *(unsigned long32*)(_data) >> 8 ) & 0x3F );
}

long32 MDdataWordV775::DWGetChannel(){
  long32 dt = DWGetDataType();
  if ( dt != (uint32_t)DWV775_Measurement ) 
    _valid = false;
  else 
    _valid = true;
  return ( ( *(unsigned long32*)(_data) >> 16 ) & 0x1F );
}

long32 MDdataWordV775::DWGetMeasurement(){
  long32 dt = DWGetDataType();
  if ( dt != (uint32_t)DWV775_Measurement ) 
    _valid = false;
  else 
    _valid = true;
  return ( ( *(unsigned long32*)(_data) ) & 0xFFF );
}

long32 MDdataWordV775::DWGetEventCount(){
  long32 dt = DWGetDataType();
  if ( dt != (uint32_t)DWV775_EOB ) 
    _valid = false;
  else 
    _valid = true;
  return ( ( *(unsigned long32*)(_data) ) & 0xFFFFFF );
}


ostream &operator<<( ostream &s,MDdataWordV775 &dw){
  long32 dt= dw.DWGetDataType();
  s << " CAEN V775N ";
  switch ( dt ) {
    case DWV775_Measurement:
    {
      s  << " Ch " << setw(2) << dec << dw.DWGetChannel() ;
      s << " ; data: " << dec << dw.DWGetMeasurement() << endl ;
      break;
    }
    case DWV775_Header:
    {
      s << "Header GEO: " << dw.DWGetGeo();
      s << ",         Channel Count: " << dw.DWGetChannelCount();
      break;
    }
    case DWV775_EOB:
    {
      s << " Event counts: " << dw.DWGetEventCount();
      break;
    }
    default:
    {
      s << "Invalid Data! (" << dt << ")" << endl;
      s << dec << noshowbase;
     return s;
    }
  }
  s << dec << noshowbase;
  return s;
}
