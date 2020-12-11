//---------------------------------------------------------------------------------------
// This file is part of the "Lomse Command Line Tool" (lclt) program
// Copyright (c) 2015-2020 LenMus project
//
// This program is free software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this
// program. If not, see <http://www.gnu.org/licenses/>.
//
// For any comment, suggestion or feature request, please contact the manager of
// the project at s.cecilio@gmail.com
//
//---------------------------------------------------------------------------------------

//lclt
#include "lclt_injectors.h"
#include "lclt_command_processor.h"
#include "lclt_command_parser.h"
using namespace lclt;

//std
#include <iostream>
#include <string>
using namespace std;
#define system   std::system        //bypass wxWidgets complain

//wxWidgets
#include <wx/defs.h>
#include <wx/log.h>
#include <wx/image.h>


const size_t SUCCESS = 0;
const size_t ERROR_IN_COMMAND_LINE = 1;
const size_t ERROR_UNHANDLED_EXCEPTION = 2;


//---------------------------------------------------------------------------------------
void redirect_wxwidgets_messages()
{
    #if wxUSE_STD_IOSTREAM
        //redirect all wxWidgets logs to cout
        wxLog::SetActiveTarget( new wxLogStream(&cout) );
    #endif // wxUSE_STD_IOSTREAM

    //redirect wxMessageOutput to print to stderr
    wxMessageOutput::Set(new wxMessageOutputStderr);
}

//---------------------------------------------------------------------------------------
void initialize_all_image_handlers()
{
    //initialize all image handlers
    ::wxInitAllImageHandlers();
}


//=======================================================================================
int main(int argc, char **argv)
{
    ApplicationScope app(cout);

    redirect_wxwidgets_messages();
    initialize_all_image_handlers();

    //all initializations finished. Analyse command line and process commands
    CommandParser parser;
    CommandProcessor cp(app);

    //if no commands enter interactive mode
    if (argc == 1)
    {
        cp.enter_interactive_mode();
        return SUCCESS;
    }

    //otherwise, get commands and execute them in order (scripting mode)
    int numCmds = parser.parse_command_line(argc, argv);
    for (int i=0; i < numCmds; ++i)
    {
        if ( cp.process_cmd(parser.get_command(i), parser.get_argument(i)) )
            return SUCCESS;
    }

    return SUCCESS;

}   //main
