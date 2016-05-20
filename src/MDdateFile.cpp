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

#include "MDdateFile.h"

MDdateFile::MDdateFile(string fn, string fp):_eventBuffer(NULL),_filePath(fp),
                       _fileName(fn),_eventSize(0),_curPos(0),_fileSize(0),_nBytesRead(0){
  _status =  DATE_FILE_NOT_OPEN  ;
}

MDdateFile::~MDdateFile() {
  if (_eventBuffer)
    free(_eventBuffer);
}

dateFileStatus_t MDdateFile::OpenFile() {

  string fullName = _filePath + "/" + _fileName;

  _ifs.open( fullName.c_str() );
  if ( _ifs.fail() ) {
    cerr << "Can not open file " << fullName.c_str() << endl;
    return DATE_FILE_ERROR_IO;
  }
  long end;
  _curPos = _ifs.tellg();
  _ifs.seekg (0, ios::end);
  end = _ifs.tellg();
  _fileSize = end - _curPos;
  //  cout << " File size " << _fileSize << endl;
  if ( _fileSize%4 != 0 ) {
    cerr << " File size is not a multiple of 4. The file " << fullName.c_str() << " is rejected!" << endl;
    return DATE_FILE_ERROR_FORMAT;
  }
  _ifs.seekg (0, ios::beg); // go back to the begining ( = _curPos )
  _status =  DATE_FILE_OK ;
  return _status;
}

void MDdateFile::CloseFile() {
  if (_eventBuffer)
    free(_eventBuffer);

  _eventBuffer = 0;
  _ifs.close();
}


unsigned char* MDdateFile::GetNextEvent() {
  _curPos = _ifs.tellg();
  if ( IsOk() && _nBytesRead < _fileSize ) {
    free(_eventBuffer);
    _eventBuffer = 0;

    if  ( !_ifs.read( ( char* ) &_eventSize, sizeof( _eventSize ) ) ) { // read event size
      cerr << "Unexpected End of File while trying to read event Size" << endl;
      _status = DATE_FILE_ERROR_EOF;
      return NULL;
    } else { // read OK, go back to previous position
      _ifs.seekg (_curPos , ios::beg);
      _nBytesRead += sizeof( _eventSize );
      // cout << " Event size " << _eventSize << endl;
      if ( _eventSize ) { // allocate memory for the event
        _eventBuffer = (unsigned char*)malloc(_eventSize);
        if ( _eventBuffer == NULL )  {
          cerr << "Memory allocation failed in MDdateFile::GetNextEvent()" << endl;
          return NULL;
        }
        if  ( !_ifs.read( (char*)_eventBuffer, _eventSize ) ) { // read full DAQ event
          cerr << "Unexpected End of File while trying to read event" << endl;
          _status = DATE_FILE_ERROR_EOF;
          return NULL;
        } else { // read OK
          _nBytesRead += _eventSize;
          return _eventBuffer;
        }
      } else {
        return NULL;
      }
    }
  } else {
    return NULL;
  }
}

void MDdateFile::GoTo(long pos) {
  _ifs.seekg (pos , ios::beg);
}

unsigned char* MDdateFile::GetEvent(unsigned char *buffer, long pos, int size) {
  _ifs.seekg (pos , ios::beg);

  if ( !_ifs.read( (char*)buffer, size ) ) { // read full DAQ event
    cerr << "Unexpected End of File while trying to read event" << endl;
    _status = DATE_FILE_ERROR_EOF;
    return NULL;
  }

  return buffer;
}

