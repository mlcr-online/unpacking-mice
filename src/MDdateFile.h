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

#ifndef __MDDATEFILE_H
#define __MDDATEFILE_H

#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

typedef enum dateFileStatus_t {
  DATE_FILE_OK             = 0,
  DATE_FILE_ERROR_IO       = 1,
  DATE_FILE_ERROR_FORMAT   = 2,
  DATE_FILE_ERROR_EOF      = 3,
  DATE_FILE_NOT_OPEN       = 4
} dateFileStatus_t ;


using namespace std;

class MDdateFile {
 protected:
  ifstream               _ifs;               // Input File Stream
  unsigned char*         _eventBuffer;

 public:
  string                 _filePath;
  string                 _fileName;
  uint32_t               _eventSize;
  long                   _curPos;            // current stream position in file
  long                   _fileSize;
  long                   _nBytesRead;
  dateFileStatus_t       _status;

  MDdateFile(string fn, string fp=".");
  ~MDdateFile();

  void SetFileName(string fn) { _fileName = fn ;}
  string GetFileName()        { return _fileName;}

  void SetFilePath(string fp) { _filePath = fp ;}
  string GetFilePath()        { return _filePath;}

  bool IsOk() { return !_status; }

  dateFileStatus_t  OpenFile();
  void              CloseFile();
  unsigned char*    GetNextEvent();
  long GetStreamPos() {
    _curPos = _ifs.tellg();
    return _curPos;
  }
  void GoTo(long pos);
  unsigned char*    GetEvent(unsigned char *buffer, long pos, int size);

  void Reset() {
    /* go back to the begining */
    _nBytesRead = 0;
    _ifs.clear();
    _ifs.seekg (0, ios::beg);
  }

 private:

};

#endif
