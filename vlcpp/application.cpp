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


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_STDAFX_H
# include "stdafx.h"
#endif

#include <iostream>
#include <string>

#include "application.hpp"

Application::Application(Options &options) : options(options), done(0)
{
}

void Application::Run()
{
        if(options.runtime.interactive) {
                ShowLicense();
                Interactive();
        } else {
                BatchMode();
        }
}

void Application::Interactive()
{
        if(!options.media.name.empty()) {
                vlc.AddBroadcast(options.media.name.c_str(), options.media.input.c_str(), options.media.output.c_str(), options.media.numopts, options.media.options, options.media.enabled, options.media.loop);
                options.queue[options.media.name.c_str()] = Options::Waiting;
        }

        while(!done) {
                try {
                        switch(options.Read()) {
                                case Options::Add:
                                        vlc.AddBroadcast(options.media.name.c_str(), options.media.input.c_str(), options.media.output.c_str(), options.media.numopts, options.media.options, options.media.enabled, options.media.loop);
                                        options.queue[options.media.name.c_str()] = Options::Waiting;
                                        break;
                                case Options::Play:
                                        vlc.PlayMedia(options.media.name.c_str());
                                        options.queue[options.media.name.c_str()] = Options::Running;
                                        break;
                                case Options::Remove:
                                        vlc.RemoveMedia(options.media.name.c_str());
                                        options.queue.erase(options.media.name.c_str());
                                        break;
                                case Options::Stop:
                                        vlc.StopMedia(options.media.name.c_str());
                                        options.queue[options.media.name.c_str()] = Options::Stopped;
                                        break;
                                case Options::Queue:
                                        ShowQueue();
                                        break;
                                case Options::Quit:
                                        done = 1;
                                        break;
                        }
                } catch(VLCPP::Exception &exception) {
                        std::cerr << exception.message << " (" << exception.code << ")\n\n";
                }
        }
}

void Application::BatchMode()
{
        try {
                vlc.AddBroadcast(options.media.name.c_str(), options.media.input.c_str(), options.media.output.c_str(), options.media.numopts, options.media.options, options.media.enabled, options.media.loop);
                vlc.PlayMedia(options.media.name.c_str());
                std::cin >> done;
                vlc.StopMedia(options.media.name.c_str());
                vlc.RemoveMedia(options.media.name.c_str());
        } catch(VLCPP::Exception &exception) {
                std::cerr << exception.message << " (" << exception.code << ")\n";
        }
}

void Application::ShowQueue()
{
        if(options.queue.empty()) {
                std::cout << "No media defined. See 'help add'\n";
        } else if(options.media.name.empty()) {
                typedef std::map<std::string, Options::State>::const_iterator iterator;
                std::cout << "Name:\tState:\n";
                std::cout << "-----\t------\n";
                for(iterator it = options.queue.begin(); it != options.queue.end(); ++it) {
                        std::cout << it->first << "\t" << GetState(it->second) << std::endl;
                }
                std::cout << "\n";
        } else {
                std::cout << "Queue " << options.media.name << ":\n";
                std::cout << vlc.ShowMedia(options.media.name.c_str()) << "\n\n";
        }
}

void Application::ShowLicense()
{
        std::cout 
                << "Copyright (C) 2015 Anders Lövgren, Computing Department at BMC, Uppsala University\n\n"
                << "This program comes with ABSOLUTELY NO WARRANTY. It is free software and you are\n"
                << "welcome to redistribute it under certain conditions.\n"
                << "For details visit http://www.gnu.org/licenses/gpl-3.0.en.html.\n\n";
}

std::string Application::GetState(Options::State state)
{
        switch(state) {
                case Options::Waiting:
                        return "<waiting>";
                case Options::Running:
                        return "<started>";
                case Options::Stopped:
                        return "<stopped>";
                default:
                        return "<unknown>";
        }
}
