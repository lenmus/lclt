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

#ifndef __LCLT_COMMAND_PARSER_H__
#define __LCLT_COMMAND_PARSER_H__

//lclt
#include "lclt_standard_header.h"

//std
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
using namespace std;


namespace lclt
{

//---------------------------------------------------------------------------------------
class CommandParser
{
protected:
    vector<string> m_commands;
    vector<string> m_args;

public:
    CommandParser();
    ~CommandParser();

    int parse_command_line(int argc, char **argv);
    int parse_line(const string& line);
    const string& get_command(int i);
    const string& get_argument(int i);

protected:

    string extract_command_name(const string& var);
    string extract_argument(const string& var);

    // Helpers for trimming strings

    /// trim from start
    static inline string& ltrim(string &s) {
            s.erase(s.begin(), find_if(s.begin(), s.end(),
                                       not1(ptr_fun<int, int>(::isspace))));
            return s;
    }

    /// trim from end
    static inline string& rtrim(string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(),
                                 not1(ptr_fun<int, int>(std::isspace))).base(), s.end());
            return s;
    }

    /// trim from both ends
    static inline string& trim(string &s) {
            return ltrim(rtrim(s));
    }



};


} // namespace lclt

#endif  //__LCLT_COMMAND_PARSER_H__

