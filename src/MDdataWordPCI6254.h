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


#ifndef __MDDATWORDPCI6254_H
#define __MDDATWORDPCI6254_H

#include "MDdataWord.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>


using namespace std;

class MDdataWordPCI6254 : public MDdataWord {

 public:
  MDdataWordPCI6254( void *d = 0 );
  ~MDdataWordPCI6254(){}

  virtual void Dump(int atTheTime=1);  
};


ostream &operator<<(ostream &s,MDdataWordPCI6254 &dw);
istream &operator>>(istream &s,MDdataWordPCI6254 &dw);

#endif
