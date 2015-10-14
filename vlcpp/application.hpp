// C++ Class Library for VLC/VLM
// Copyright (C) 2015 Anders Lövgren, Computing Department at BMC, Uppsala University
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

#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include "vlc.hpp"
#include "options.hpp"

class Application
{
public:
        Application(Options &options);
        void Run();

private:
        void Interactive();
        void BatchMode();

        void ShowQueue();
        void ShowLicense();

        static std::string GetState(Options::State state);

        VLC vlc;
        Options &options;
        int done;
};

#endif  // __APPLICATION_HPP__
