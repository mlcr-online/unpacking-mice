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

#ifndef __MDREBUILDMANAGER_H
#define __MDREBUILDMANAGER_H

//C++
#include <utility>
#include <queue>
#include <deque>

#include "MDdateFile.h"
#include "MDfileManager.h"
#include "MDprocessManager.h"

// typedef std::pair<long, int>  EvntLabel;

struct EvntLabel {
  EvntLabel()
  :_pos(0), _nPartEv(0), _size(0) {}

  EvntLabel(long p, int n, int s)
  :_pos(p), _nPartEv(n), _size(s) {}

  long _pos;
  int  _nPartEv;
  int _size;
};
// typedef std::queue<EvntLabel>        EvntFIFO;
typedef std::deque<EvntLabel>        EvntFIFO;

class MDRebuildManager : public MDprocessManager {

 public:
  MDRebuildManager(int nLDCs)
  : MDprocessManager(), _nLDCs(nLDCs) {_addresMap.resize(nLDCs);}

  virtual ~MDRebuildManager() {}

  void SetInFiles(vector<MDdateFile*> f) {_infiles = f;}
  void SetOutFile(ofstream* f) {_outfile = f;}
  bool Scan();
  int  Scan(MDdateFile *infile);
  bool Reorder();
  void Rebuild();
  void Rebuild(MDdateFile *infile, ofstream *outfile);
  bool Synchronize();
  void DumpAddresMap(int LdcId);

 private:
  int ProcessSubEvent(unsigned char* aDataPtr);

  std::vector<EvntFIFO>  _addresMap;
  int _nLDCs;
//   MDdateFile *_infile;
  vector< MDdateFile* > _infiles;
  ofstream   *_outfile;
};

#endif
