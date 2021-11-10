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

//lclt
#include "lclt_command_processor.h"
#include "lclt_command_parser.h"

//wxWidgets
#include <wx/defs.h>
#include <wx/filename.h>
#include <wx/image.h>


namespace lclt
{

//---------------------------------------------------------------------------------------
CommandProcessor::CommandProcessor(ApplicationScope& appScope)
    : m_app(appScope)
    , m_pPresenter(NULL)
    , m_pInteractor(NULL)
    , m_lomse( appScope.get_lomse() )
{

}

//---------------------------------------------------------------------------------------
CommandProcessor::~CommandProcessor()
{

}

//---------------------------------------------------------------------------------------
bool CommandProcessor::process_cmd(const string& cmd, const string& arg)
{
    //returns true if lclt must be terminated
    if (m_app.verbose_mode())
        cout << "INFO: Processing cmd: " << cmd << "(" << arg << ")" << endl;

    if (cmd == "cd")
        change_dir(arg);

    else if (cmd == "export")
        export_img(arg);

    else if (cmd == "e")            //<= For tests
        export_img("prueba2");

    else if (cmd == "import")
        load_file(arg);

    else if (cmd == "i")            //<= For tests
        load_file("test3.xml");

    else if (cmd == "interactive")
    {
        if (m_app.run_mode() == k_scripting_mode)
        {
            enter_interactive_mode();
            return true;    //exit lclt
        }
    }

    else if (cmd == "help" || cmd == "h" || cmd == "?")
        print_help();

    else if (cmd == "ls")
        system("ls");

    else if (cmd == "pwd")
        cout << m_app.current_dir() << endl;

    else if (cmd == "v" || cmd == "verbose")
        set_verbose();

    else if (cmd == "vers" || cmd == "version")
        print_version();

    else
    {
        cout << "ERROR: Invalid command '" << cmd << "'." << endl;
        print_help();
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------------------
void CommandProcessor::change_dir(const string& filename)
{
    if (!filename.empty())
    {
        wxFileName fn;
        wxString pathname;
        wxString sep = wxFileName::GetPathSeparator();
        string separator = string(sep);

        if (filename.substr(0,1) == separator)        //C++11  .front()
            pathname = wxString(filename);
        else
            pathname = wxString(m_app.current_dir() + filename);

        if (*filename.rbegin() != separator.substr(0,1))      //C++11 .back()
            pathname += separator;

        fn.Assign( pathname );
        fn.Normalize();
        wxString path = fn.GetPathWithSep();

        if (wxFileName::DirExists(path))
        {
            m_app.current_dir( string(path) );
            wxFileName::SetCwd(path);
        }
        else
            cout << "Invalid path " << path << endl;
    }
    cout << m_app.current_dir() << endl;
}

//---------------------------------------------------------------------------------------
void CommandProcessor::load_file(const string& filename)
{
    try
    {
        delete m_pPresenter;
        int viewType = k_view_vertical_book;
        m_pPresenter = m_lomse.open_document(viewType, filename, cout);

        if (SpInteractor spInteractor = m_pPresenter->get_interactor(0).lock())
            spInteractor->set_operating_mode(Interactor::k_mode_edition);
    }
    catch(std::exception& e)
    {
        cout << e.what() << endl;
    }
}

//---------------------------------------------------------------------------------------
void CommandProcessor::export_img(const string& filename)
{
    //determine MIME type
    wxString mimetype;
    if (m_app.image_format() == "jpg")
        mimetype = "image/jpeg";
    else
        mimetype = "image/png";

    try
    {
        if (SpInteractor spInteractor = m_pPresenter->get_interactor(0).lock())
        {
            int maxPage = spInteractor->get_num_pages();
            if (m_app.verbose_mode())
                cout << "generating images for " << maxPage << " page(s)." << endl;

            RenderingBuffer rbuf_print;
            wxImage* buffer = NULL;         //the wxImage to serve as buffer
            #define BYTES_PP 3              //bytes per pixel
            VPoint viewport(0,0);
            long prevBufSize = 0L;

            int width = m_app.image_width();
            int height = m_app.image_height();
            long curBufSize = width * height;
            spInteractor->set_print_page_size(width, height);

            //loop to create images for pages
            for (int page=1; page <= maxPage; ++page)
            {

//                //get this page size (pixels) for Lomse initialization resolution (72ppi)
//                VSize size = spInteractor->get_page_size_in_pixels(page);
//                double vPageWidth( size.width );
//                double vPageHeigh( size.height );
//
//                //get target size and determine buffer size (pixels)
//                int width = m_app.image_width();
//                int height = m_app.image_height();
//                long curBufSize = width * height;
//
//                //determine view scaling for the required export resolution
//                double scaleX = double(width) / vPageWidth;
//                double scaleY = double(height) / vPageHeigh;
//                double scale = max(scaleX, scaleY);

                //allocate page buffer
                if (curBufSize != prevBufSize)
                {
                    delete buffer;
                    buffer = new wxImage(width, height);    // allocate the rendering buffer
                    unsigned char* pdata = buffer->GetData();       //ptr to the real bytes buffer
                    spInteractor->set_print_buffer(pdata, width, height);
                    prevBufSize = curBufSize;
                }

                //get this page image
                spInteractor->print_page(page-1, viewport);
                if (!buffer || !buffer->IsOk())
                {
                    cout << "No buffer or it is not OK." << endl;
                    return;
                }

                //export the page image
                stringstream name;
                name << filename << "-" << page << "." << m_app.image_format();
                buffer->SaveFile(wxString(name.str()), mimetype);

//                //export as bitmap
//                //wxBitmap bitmap = *buffer;
//                wxBitmap bmp(*buffer, 32);
//                bmp.SaveFile(wxString(name.str()), wxBITMAP_TYPE_BMP);

                if (m_app.verbose_mode())
                    cout << "Created " << name.str() << endl;
            }
            delete buffer;
        }
        else
            cout << "Error: no interactor!" << endl;
    }
    catch(std::exception& e)
    {
        cout << e.what() << endl;
    }
}

//---------------------------------------------------------------------------------------
void CommandProcessor::print_help()
{
    cout << "Usage:" << endl
         << "\tlclt [initial-options] [subcommand]*" << endl
         << endl
         << "Current working directory:" << endl
         << "\t pwd     \t\t\t Print working directory" << endl
         << "\t cd <spec> \t\t Change working directory" << endl
         << endl
         << "Show info commands:" << endl
         << "\t pwd     \t\t\t Print working directory" << endl
         << "\t s lmd   \t\t\t Display source code in LMD format" << endl
         << "\t s ldp   \t\t\t Display source code in LDP format" << endl
         << "\t s chk   \t\t\t Display checkpoint data" << endl
         << endl
         << "Cursor commands:" << endl
         << "\t c+      \t\t\t Cursor: move next" << endl
         << "\t c-      \t\t\t Cursor: move back" << endl
         << "\t cin     \t\t\t Cursor: enter into element" << endl
         << "\t cout    \t\t\t Cursor: move out of element" << endl
         << "\t c?      \t\t\t Cursor: dump cursor" << endl
         << endl
         << "Insert commands:" << endl
         << "\t ih <text> \t\t Insert section title (header)" << endl
         << "\t ip <text> \t\t Insert paragraph" << endl
         << "\t is      \t\t\t Insert empty score" << endl
         << "\t i so <ldp> \t\t Insert staffobj. i.e. 'i so (n c4 q)'" << endl
         << "\t i mso <ldp>\t Insert many staffobjs. i.e. 'i mso (n c4 e g+)(n d4 e g-)'" << endl
         << "\t i blo <ldp>\t Insert top level object (block)" << endl
         << endl
         << "Delete commands:" << endl
         << "\t d       \t\t\t Delete block level object" << endl
         << endl
         << "Miscellaneous commands:" << endl
         << "\t undo    \t\t\t Undo one step" << endl
         << "\t redo    \t\t\t Redo one step" << endl
         << "\t quit | exit \t\t\t Close console" << endl
         << endl
         << "Debug commands:" << endl
         << "\t cmp    \t\t\t Generate checkpoint data and compare it with" << endl
         << "        \t\t\t\t data from last 's chk' issued command."
         << endl
         << endl;
}

//---------------------------------------------------------------------------------------
void CommandProcessor::set_verbose()
{
    m_app.verbose_mode(true);
    cout << "INFO: verbose mode ON" << endl;
}

//---------------------------------------------------------------------------------------
void CommandProcessor::print_version()
{
    cout << "Lomse command-line Tool v0.1 - Lomse library v"
         << m_lomse.get_version_long_string() << endl;
}

//---------------------------------------------------------------------------------------
void CommandProcessor::enter_interactive_mode()
{
    print_version();
    m_app.run_mode(k_interactive_mode);

    cout << endl << "Interactive mode. Enter 'q' or 'quit' to finish." << endl;
    char cmd[500];
    CommandParser parser;
    while(true)
    {
        cout << endl << "lclt -> ";
        cin.getline(cmd, 500, '\n');
        if (strcmp(cmd, "q") == 0 || strcmp(cmd, "quit") == 0)
            break;
        else
        {
            int numCmds = parser.parse_line(cmd);
            for (int i=0; i < numCmds; ++i)
                process_cmd(parser.get_command(i), parser.get_argument(i));
        }
    }
}



} // namespace lclt
