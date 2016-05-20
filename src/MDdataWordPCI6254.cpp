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

#include "MDdataWordPCI6254.h"

//default constructor
MDdataWordPCI6254::MDdataWordPCI6254(void *d):MDdataWord(d){}

void MDdataWordPCI6254::Dump(int atTheTime){
  cout << *this;
  return;
} 

ostream &operator<<( ostream &s,MDdataWordPCI6254 &dw){
  s << showbase << hex << showbase ;
  s << " PCI6254 data : ";
  s << *(dw.Get32bWordPtr()) ;
  s << " (or  " << dec << *((float*)dw.Get32bWordPtr() )<< ")" <<  endl ;

  return s;
}
