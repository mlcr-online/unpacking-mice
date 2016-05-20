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

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <typeinfo>
#include <exception>

// #include "TFile.h"
// #include "TH1D.h"

#include "MDRebuildManager.h"
#include "MDevent.h"
#include "MDdateFile.h"
#include "MDfileManager.h"
#include "MDargumentHandler.h"

using namespace std;

int main( int argc, char **argv ) {
  string stringBuf;
//   int parArg;
  //char filepath[128];
  char runpath[128];
  char filename[128];

  // The following shows how to use the MDargumentHandler class
  // to deal with the main arguments
  // Define the arguments
  MDargumentHandler argh("Example of unpacking application.");
  argh.AddArgument("help","print this message","h");
  argh.AddArgument("directory","path for the data file","d","<string>","/home/daq/Downloads" );
  argh.AddArgument("file","List of data file names or run number (space separated, within double quote)","f","<string>","mandatory");

  // Check the user arguments consistancy
  // All mandatory arguments should be provided and
  // There should be no extra arguments
  if ( argh.ProcessArguments(argc, argv) ) {argh.Usage(); return -1;}

  // Treat arguments, obtain values to be used later
  if ( argh.GetValue("help") ) {argh.Usage(); return 0;}
  if ( argh.GetValue("directory", stringBuf) != MDARGUMENT_STATUS_OK ) return -1;
  strcpy(runpath,stringBuf.c_str());
  cout << "Directory : " << runpath << endl;
  if ( argh.GetValue("file", stringBuf) != MDARGUMENT_STATUS_OK ) return -1;
  strcpy(filename, stringBuf.c_str());

  // Declare the rebuild Manager.
  MDRebuildManager rebuildManager(2);

  // Disable equipments that should not be decoded (in case of internal format error).
  // If an equipment is disabled, it is skipped at the equipment header level.
  rebuildManager.Disable("V1731");

  MDfileManager fileManager(filename,runpath);
  if ( fileManager.OpenFile() != DATE_FILE_OK ) return 0;	
  unsigned int nfiles = fileManager.GetNFiles();

  if (!nfiles) return 0;

  // Open the data files and loop over all the events
  rebuildManager.SetInFiles( fileManager.GetFileVector() );

  if ( !rebuildManager.Scan() ) {
    cout << "No Errors found.\n";
    return 0;
  }

  if( rebuildManager.Reorder())
   rebuildManager.Rebuild();

  return 0;
} /* End of main */


