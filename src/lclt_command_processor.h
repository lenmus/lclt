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

#ifndef __LCLT_COMMAND_PROCESSOR_H__
#define __LCLT_COMMAND_PROCESSOR_H__

//lclt
#include "lclt_injectors.h"
#include "lclt_standard_header.h"

//std
#include <iostream>
#include <string>
using namespace std;
#define system   std::system        //bypass wxWidgets complain

namespace lclt
{

//---------------------------------------------------------------------------------------
class CommandProcessor
{
protected:
    ApplicationScope& m_app;
    Presenter*      m_pPresenter;	//relates the View, the Document and the Interactor
    Interactor*     m_pInteractor;  //to interact with the View
    LomseDoorway&   m_lomse;        //the Lomse library doorway

public:
    CommandProcessor(ApplicationScope& appScope);
    ~CommandProcessor();

    bool process_cmd(const string& cmd, const string& value);
    void enter_interactive_mode();

protected:

    void change_dir(const string& filename);
    void load_file(const string& filename);
    void export_img(const string& filename);
    void print_help();
    void set_verbose();
    void print_version();



};


} // namespace lclt

#endif  //__LCLT_COMMAND_PROCESSOR_H__

