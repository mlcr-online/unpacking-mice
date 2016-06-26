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

#ifndef __MDFRAGMENT_EI_H
#define __MDFRAGMENT_EI_H

#include <iostream>

#include "MDdataContainer.h"
#include "MDfragment.h"
#include "MDdataWordEI.h"

class MDfragmentEI : public MDfragment {
 public:
  MDfragmentEI(void *d = 0) : MDfragment(d) { Init(); }
  virtual ~MDfragmentEI() {}

  void SetDataPtr( void *d, uint32_t aSize );
  void Init();
  void Dump(int atTheTime=1) {cout << *this;}

  unsigned int GetPvId(unsigned int i);
  double       GetPvValue(unsigned int i);
  unsigned int GetNPVs() const   {return _pv_id.size();}
  unsigned int GetGeo()          {return _geo;}

  friend ostream &operator<<(std::ostream &s,MDfragmentEI &df);

 private:
  unsigned int _geo;
  std::vector<int>     _pv_id;
  std::vector<double>  _pv_value;

};



#endif

