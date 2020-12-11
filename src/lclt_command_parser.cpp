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
#include "lclt_command_parser.h"

namespace lclt
{

//---------------------------------------------------------------------------------------
CommandParser::CommandParser()
{

}

//---------------------------------------------------------------------------------------
CommandParser::~CommandParser()
{

}

//---------------------------------------------------------------------------------------
int CommandParser::parse_command_line(int argc, char **argv)
{
    m_commands.clear();
    m_args.clear();

    for (int i=1; i < argc; ++i)
    {
        string arg(argv[i]);
        string value = (i+1 < argc ? string(argv[i+1]) : "");
        if (value.substr(0,1) == "-")
            value = "";
        if (arg.substr(0,1) == "-")
        {
            m_commands.push_back( extract_command_name(arg) );
            m_args.push_back(value);
        }
        else
        {
            cout << "ERROR: Invalid command '" << arg << "'." << endl;
            //return ERROR_IN_COMMAND_LINE;
        }
        if (!value.empty())
            i++;
    }

    return int( m_commands.size() );
}

//---------------------------------------------------------------------------------------
int CommandParser::parse_line(const string& line)
{
    // returns number of commands in the line

    m_commands.clear();
    m_args.clear();

    //TODO: more than one command in the line
    m_commands.push_back( extract_command_name(line) );
    m_args.push_back( extract_argument(line) );

    return 1;
}

//---------------------------------------------------------------------------------------
const string& CommandParser::get_command(int i)
{
    return m_commands[i];
}

//---------------------------------------------------------------------------------------
const string& CommandParser::get_argument(int i)
{
    return m_args[i];
}

//---------------------------------------------------------------------------------------
string CommandParser::extract_command_name(const string& var)
{
    int i = 0;
    if (var.substr(0,2) == "--")
        i = 2;
    else if (var.substr(0,1) == "-")
        i = 1;

    size_t j = var.find("=");
    if (j == string::npos)
        j = var.find(" ");

    if (j != string::npos)
        return var.substr(i,j-i);
    else
        return var.substr(i);
}

//---------------------------------------------------------------------------------------
string CommandParser::extract_argument(const string& var)
{
    size_t i = var.find("=");
    if (i == string::npos)
        i = var.find(" ");

    string arg = "";
    if (i != string::npos)
        arg = var.substr(i+1);

    return trim(arg);
}


} // namespace lclt
