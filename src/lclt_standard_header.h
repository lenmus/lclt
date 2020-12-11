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

#ifndef __LCLT_STANDARD_HEADER_H__
#define __LCLT_STANDARD_HEADER_H__



//---------------------------------------------------------------------------------------
// for detecting and isolating memory leaks with Visual C++ in Debug builds

#if ( LCLT_COMPILER_MSVC == 1 && LCLT_DEBUG_BUILD == 1 )
    #ifndef _DEBUG
        #define _DEBUG
    #endif
    #define _CRTDBG_MAP_ALLOC
    #define _CRTDBG_MAP_ALLOC_NEW
    #include <stdlib.h>
    #include <crtdbg.h>
    //#include <wx/memory.h>
    #ifndef LCLT_NEW
        #define LCLT_NEW new ( (_NORMAL_BLOCK) , (__FILE__) , (__LINE__) )
        //#define new DEBUG_NEW
    #endif
#else
    #define LCLT_NEW new
#endif



#endif   // __LCLT_STANDARD_HEADER_H__
