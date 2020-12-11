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

#ifndef __LCLT_INJECTORS_H__
#define __LCLT_INJECTORS_H__

//lclt
#include "lclt_standard_header.h"

//lomse headers
#include <lomse/lomse_doorway.h>
#include <lomse/lomse_document.h>
#include <lomse/lomse_graphic_view.h>
#include <lomse/lomse_interactor.h>
#include <lomse/lomse_presenter.h>
#include <lomse/lomse_events.h>
#include <lomse/lomse_graphical_model.h>
using namespace lomse;

namespace lclt
{

// run modes
enum { k_scripting_mode=0, k_interactive_mode, k_batch_mode, };

//---------------------------------------------------------------------------------------
class ApplicationScope
{
protected:
    ostream& m_reporter;

    //Lomse related
    LomseDoorway    m_lomse;        //the Lomse library doorway
    ostringstream   m_lomseReporter;    //to have access to error messages

    //variables set by commands
    bool    m_fVerbose;
    string  m_curDir;
    string  m_imgFormat;
    string  m_imgFile;
    int     m_imgWidth;
    int     m_imgHeight;

    string m_sAppName;
    string m_sVendorName;
    string m_sVersionString;

    int    m_runMode;

    streambuf*      m_cout_buffer;      //to restore cout

public:
    ApplicationScope(ostream& reporter=cout);
    ~ApplicationScope();


    //setters
    inline void run_mode(int mode) { m_runMode = mode; }
    inline void current_dir(const string& dir) { m_curDir = dir; }
    inline void image_format(const string& format) { m_imgFormat = format; }
    inline void verbose_mode(bool mode) { m_fVerbose = mode; }
    inline void image_height(int pixels) { m_imgHeight = pixels; }
    inline void image_width(int pixels) { m_imgWidth = pixels; }

    //getters
    inline LomseDoorway& get_lomse() { return m_lomse; }
    inline int run_mode() { return m_runMode; }
    inline string current_dir() { return m_curDir; }
    inline string image_format() { return m_imgFormat; }
    inline bool verbose_mode() { return m_fVerbose; }
    inline int image_height() { return m_imgHeight; }
    inline int image_width() { return m_imgWidth; }

    //app info
    inline string& get_app_name() { return m_sAppName; }
    inline string& get_vendor_name() { return m_sVendorName; }
    inline string& get_version_string() { return m_sVersionString; }
    string get_app_full_name();


protected:
    void set_version_string();
    void initialize_lomse();

};

//---------------------------------------------------------------------------------------
class Injector
{
public:
    Injector() {}
    ~Injector() {}
};


} // namespace lclt

#endif  //__LCLT_INJECTORS_H__

