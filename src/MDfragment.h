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

#ifndef __MDFRAGMENT_H
#define __MDFRAGMENT_H

#include "MDdataContainer.h"
#include "MDequipMap.h"
#include "MDexception.h"

#define WORD_SIZE 4

using namespace std;
typedef pair<void*, uint32_t> ptrAndSize_t;
class MDequipMap;

class MDfragment : public MDdataContainer {
 public:
  MDfragment(void *d=0, uint32_t aSize=0, uint32_t aID=0xFFFFFFFF, uint32_t aType=0 )
  :MDdataContainer(d),_header(NULL),_headerSize(0),_payLoad((uint32_t*)d),_payLoadSize(aSize),
  _trailer(NULL),_trailerSize(0),_madeOfParticles(false),_partEventPtr(NULL),_equipType(aType),
  _boardID(aID) {
    //    cout << " Creating a MDfragment "<< endl ;
    Init();
  }

  virtual ~MDfragment(){ if (_partEventPtr) {delete  _partEventPtr;} _partEventPtr=NULL;}

  virtual void SetDataPtr( void *d, uint32_t aSize );
  /*{
    _payLoad = (uint32_t*)d;
    _payLoadSize = aSize;
    _partEventVector.clear();
    //   cout << "Execute MDfragment::SetDataPtr() for Equipment " << GetName() << endl;
    MDdataContainer::SetDataPtr(d);
    SetSize(aSize);
    Init(); // This Init() will by dynamically asigned
  }*/

  virtual void Dump(int atTheTime=1);

  virtual void Init();
  virtual uint32_t InitPartEventVector();

  MDdataContainer*    GetPartEventPtr(int ipe=-1); // { return _partEventPtr;}
  uint32_t            GetNPartEvents() {return _partEventVector.size();}
  void*               GetPartEventDataPtr(int );
  uint32_t            GetPartEventSize(int );

  uint32_t* UserHeaderPtr() {return _header;}
  uint32_t* UserPayLoadPtr() {return _payLoad;}
  uint32_t* UserTrailerPtr() {return _trailer;}

  bool HasHeader()  {return !!_header;}
  bool HasPayLoad() {return !!_payLoad;}
  bool HasTrailer() {return !!_trailer;}
  bool IsMadeOfParticles() { return _madeOfParticles; }

  void SetEquipmentType(uint32_t aId){_equipType=aId;}
  uint32_t GetEquipmentType(){return _equipType;}


  string GetName();
  uint32_t GetGeo(){return GetBoardID();}
  uint32_t GetBoardID(){return _boardID;}
  void     SetBoardID(uint32_t aId){_boardID=aId;}

  uint32_t GetWordCount(){return _size/WORD_SIZE;}
  uint32_t GetHeaderWordCount(){return _headerSize/WORD_SIZE;}
  uint32_t GetPayLoadWordCount(){return _payLoadSize/WORD_SIZE;}
  uint32_t GetTrailerWordCount(){return _trailerSize/WORD_SIZE;}

 protected:
  uint32_t* _header;
  uint32_t  _headerSize;

  uint32_t* _payLoad;
  uint32_t  _payLoadSize;

  uint32_t* _trailer;
  uint32_t  _trailerSize;

  bool      _madeOfParticles;

  vector<ptrAndSize_t> _partEventVector; // a vector of pointers to the first data location
                                         // of the particle events, together with the size
  MDdataContainer *_partEventPtr; // a pointer to a particle event. This is set dynamically
                                  // by the map to point to the correct MDpartEventXXX class.
  uint32_t  _equipType; // This is needed to allow the user to get the name of the equipment
                        // while decoding the fragment data. It is set automatically by the map
  uint32_t  _boardID; // the board ID is often given inside the data. In general it should match
                      // the Equipment ID found in the equipment header (in MDeventFragment).
                      // In CAEN euqipments, the board ID is often identified to the GEO address

};

#endif
