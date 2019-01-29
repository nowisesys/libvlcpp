// C++ Class Library for VLC/VLM
// 
// Copyright (C) 2015-2018 Anders Lövgren, BMC-IT, Uppsala University
// Copyright (C) 2018-2019 Anders Lövgren, Nowise Systems
// 
// This program is free software: you can redistribute it and/or modify 
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// Lesser GNU General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_STDAFX_H
# include "stdafx.h"
#endif

#if WIN32
#pragma comment(lib, "libvlcpp")
#endif

#include <iostream>
#include <string>

#include "options.hpp"
#include "application.hpp"

int main(int argc, char **argv)
{
#ifdef HAVE_SETLOCALE
        setlocale(LC_CTYPE, "swedish");
#endif
        Options options(argc, argv);

        if(options.runtime.verbose) {
                options.Dump(std::cout);
        }

        Application application(options);
        application.Run();

        return 0;
}
