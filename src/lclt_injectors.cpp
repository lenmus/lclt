//---------------------------------------------------------------------------------------
// This file is part of the "Lomse Command Line Tool" (lclt) program
// Copyright (c) 2015-2021 LenMus project
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

//lenmus
#include "lclt_injectors.h"

//std
#include <sstream>
using namespace std;
#define system   std::system        //bypass wxWidgets complain

//wxWidgets
#include <wx/filename.h>

namespace lclt
{

#define LCLT_VERSION_MAJOR  0
#define LCLT_VERSION_MINOR  1
#define LCLT_VERSION_PATCH  0
#define LCLT_VERSION_TYPE   " "


//=======================================================================================
// ApplicationScope implementation
//=======================================================================================
ApplicationScope::ApplicationScope(ostream& reporter)
    : m_reporter(reporter)
    , m_fVerbose(false)
    , m_imgFormat("jpg")
    , m_imgFile("score")
    , m_imgWidth(797)       //img size for DIN A4 paper size at 72 ppi
    , m_imgHeight(1124)
    , m_sAppName("lclt")
    , m_sVendorName("LenMus project")
    , m_runMode(k_scripting_mode)
{
    set_version_string();
    initialize_lomse();

    //get current working directory
    wxFileName fn;
    fn.AssignCwd();
    m_curDir = string( fn.GetPathWithSep() );
}

//---------------------------------------------------------------------------------------
ApplicationScope::~ApplicationScope()
{
}


//---------------------------------------------------------------------------------------
void ApplicationScope::initialize_lomse()
{
//    //redirect cout to my own stream, to intercept and display error msgs.
//    m_cout_buffer = cout.rdbuf();
//    cout.rdbuf (m_lomseReporter.rdbuf());

    //Now, initialize the library:
    // pixmap RGB 24 bits, default resolution 96 ppi
    m_lomse.init_library(k_pix_format_rgb24, 96);
}

//---------------------------------------------------------------------------------------
void ApplicationScope::set_version_string()
{
    //examples: "5.0.a0", "5.0.b2", "5.1", "5.1.2", "5.3.1.rc1"

    m_sVersionString = "0.1";

//    int major = LCLT_VERSION_MAJOR;
//    int minor = LCLT_VERSION_MINOR;
//    int patch = LCLT_VERSION_PATCH;
//    string type = LCLT_VERSION_TYPE;
//
//    if (type.empty() || type == " ")
//    {
//        if (patch == 0)
//            m_sVersionString = string::Format("%d.%d", major, minor);
//        else
//            m_sVersionString = string::Format("%d.%d.%d", major, minor, patch);
//    }
//    else
//    {
//        if (patch == 0)
//            m_sVersionString = string::Format("%d.%d.%s", major, minor, type.wx_str());
//        else
//            m_sVersionString = string::Format("%d.%d.%d.%s", major, minor, patch,
//                                                type.wx_str());
//    }
}

//---------------------------------------------------------------------------------------
string ApplicationScope::get_app_full_name()
{
    //i.e. "lclt v5.0 alpha 0"
    //     "lclt v5.2.1 beta 2"
    //     "lclt v5.2.1 rc1"

    string name = get_app_name();
    name += " v";

    name += get_version_string();

//    int major = LCLT_VERSION_MAJOR;
//    int minor = LCLT_VERSION_MINOR;
//    int patch = LCLT_VERSION_PATCH;
//    string type = LCLT_VERSION_TYPE;
//
//    if (type.empty() || type == " ")
//    {
//        name += get_version_string();
//    }
//    else
//    {
//        string typeNum = "";
//        if ( type.StartsWith("a") )
//        {
//            typeNum = type.Right(1);
//            if (patch == 0)
//                name += string::Format("%d.%d alpha %s", major, minor, typeNum.wx_str());
//            else
//                name += string::Format("%d.%d.%d alpha %s", major, minor, patch,
//                                         typeNum.wx_str());
//        }
//        else if ( type.StartsWith("b") )
//        {
//            typeNum = type.Right(1);
//            if (patch == 0)
//                name += string::Format("%d.%d beta %s", major, minor, typeNum.wx_str());
//            else
//                name += string::Format("%d.%d.%d beta %s", major, minor, patch,
//                                         typeNum.wx_str());
//        }
//        else
//        {
//            if (patch == 0)
//                name += string::Format("%d.%d %s", major, minor, type.wx_str());
//            else
//                name += string::Format("%d.%d.%d %s", major, minor, patch,
//                                         type.wx_str());
//        }
//    }
    return name;
}


}  //namespace lclt
