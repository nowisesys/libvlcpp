// C++ Class Library for VLC/VLM
// Copyright (C) 2015 Anders L�vgren, Computing Department at BMC, Uppsala University
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
#include <sstream>
#include <string>
#include <vector>
#ifdef HAVE_CSTRING
#include <cstring>
#elif  HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_CSTDLIB
#include <cstdlib>
#elif  HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include "options.hpp"
#include "application.hpp"

#if defined(linux)
#define _strdup strdup
#endif

// 
// Command line option (--key=val).
// 
struct Option
{
        Option(char *args) : key(args), val(0)
        {
                if((val = strchr(args, '='))) {
                        *val++ = '\0';
                }
        }

        void SetOptional()
        {
                key = &key[2];
                val = val ? val : (char *)("");
        }

        char *key;
        char *val;
};

Options::Media::Media() 
: numopts(0), enabled(true), loop(false) 
{
}

Options::Media::Media(const char *name, const char *input, const char *output) 
: name(name), input(input), output(output) 
{
}

Options::Media::Media(const std::string &name, const std::string &input, const std::string &output) 
: name(name), input(input), output(output) 
{
}

Options::Runtime::Runtime() 
: prog(0), verbose(false), interactive(false) 
{
        version = PACKAGE_VERSION;
        bugreport = PACKAGE_BUGREPORT;
}

Options::Runtime::Runtime(const char *prog) 
: prog(prog), verbose(false), interactive(false) 
{
        version = PACKAGE_VERSION;
        bugreport = PACKAGE_BUGREPORT;
}

Options::Options()
{
}

Options::Options(int argc, char **argv) : runtime(argv[0])
{
        Parse(argc, argv);
}

Options::Options(const Options &op)
{
        if(op.media.numopts != 0) {
                media.options = new const char *[op.media.numopts];
                for(int i = 0; i < op.media.numopts; ++i) {
                        media.options[i] = op.media.options[i];
                }
        }
}

Options::~Options()
{
        if(media.numopts != 0) {
                delete [] media.options;
        }
}

// 
// Parse command line options.
// 
void Options::Parse(int argc, char **argv)
{
        std::vector<const char *> addopts;

        if(!runtime.prog) {
                runtime.prog = argv[0];
        }

        for(int i = 1; i < argc; ++i) {
                Option opt(argv[i]);

                if(strcmp(opt.key, "--help") == 0 || strcmp(opt.key, "-h") == 0) {
                        Usage();
                        exit(0);
                } else if(strcmp(opt.key, "--version") == 0 || strcmp(opt.key, "-V") == 0) {
                        Version();
                        exit(0);
                } else if(strcmp(opt.key, "--verbose") == 0 || strcmp(opt.key, "-v") == 0) {
                        runtime.verbose = true;
                } else if(strcmp(opt.key, "--interactive") == 0) {
                        runtime.interactive = true;
                } else if(strcmp(opt.key, "--name") == 0) {
                        media.name = opt.val;
                } else if(strcmp(opt.key, "--input") == 0) {
                        media.input = opt.val;
                } else if(strcmp(opt.key, "--output") == 0) {
                        media.output = opt.val;
                } else if(strcmp(opt.key, "--desktop") == 0) {
                        media.name = "desktop";
                        media.input = "screen://";
                        media.output = "#transcode{vcodec=WMV2,vb=256,acodec=none}:http{dst=:3752/desktop.asf}";
                } else {
                        opt.SetOptional();

                        std::string key(opt.key);
                        std::string val(opt.val);

                        strcpy(argv[i], ":");
                        strcat(argv[i], key.c_str());
                        strcat(argv[i], "=");
                        strcat(argv[i], val.c_str());

                        addopts.push_back(argv[i]);
                }
        }

        if(!runtime.interactive) {
                if(media.input.empty()) {
                        std::cerr << runtime.prog << ": input is missing, see '--help'.\n";
                        exit(1);
                }
                if(media.output.empty()) {
                        std::cerr << runtime.prog << ": output is missing, see '--help'.\n";
                        exit(1);
                }
        }

        if(media.name.length()) {
                queue[media.name] = Waiting;
        }

        if(addopts.size() != 0) {
                media.numopts = static_cast<int>(addopts.size());
                media.options = new const char *[media.numopts];

                for(int i = 0; i < media.numopts; ++i) {
                        media.options[i] = addopts[i];
                }
        }
}

void Options::Usage()
{
        std::cout 
                << runtime.prog << " - VLC (VLM) media player using libvlcpp\n"
                << "\n"
                << "Usage: " << runtime.prog << " [options...]\n"
                << "Options:\n"
                << "  --name=str:     The media name.\n"
                << "  --input=str:    The input media (file, screen, device, ...).\n"
                << "  --output=str:   The media output (sout).\n"
                << "  --desktop:      Alias for streaming the desktop.\n"
                << "  --interactive:  Run in interactive mode.\n"
                << "  --help,-h:      This casual help.\n"
                << "  --version,-V:   Show version.\n"
                << "  --verbose,-v:   Be more verbose.\n"
                << "\n"
                << "Notice:\n"
                << "1. Unrecognized options (like --screen-fps=0.5) get passed as optionals.\n"
                << "\n"
                << "Examples:\n"
                << "shell> " << runtime.prog << " --interactive --screen-fps=1.0\n"
                << "shell> " << runtime.prog << " --desktop\n"
                << "shell> " << runtime.prog << " --name=desktop --live-caching=0 --screen-fps=0.5 \\\n"
                << "            --input=screen:// \\\n"
                << "            --output=#transcode{vcodec=WMV2,vb=256,acodec=none}:http{dst=:3752/desktop.asf}\n"
                << "\n"
                << "Report bug to " << runtime.bugreport;
}

void Options::Version()
{
        std::cout << runtime.prog << " v" << runtime.version << std::endl;
}

void Options::Dump(std::ostream &out)
{
        out    
                << "Standard options:\n"
                << "-----------------\n"
                << "  Name:   " << media.name << "\n"
                << "  Input:  " << media.input << "\n"
                << "  Output: " << media.output << "\n";
        if(media.numopts != 0) {
                out 
                        << "Optionals:\n"
                        << "-----------------\n";
                for(int i = 0; i < media.numopts; ++i) {
                        out << "  " << i + 1 << ": " << media.options[i] << "\n";
                }
        }

}

// 
// Interactive mode. Return semantic option deduced from options string.
// 
Options::Mode Options::Read(const std::string &option)
{
        if((option.length() == 1 && (option[0] == 'h' || option[0] == '?')) || option == "help") {
                return Descr;
        } else if((option.length() == 1 && option[0] == 'a') || option == "add") {
                return Add;
        } else if((option.length() == 1 && option[0] == 'p') || option == "play") {
                return Play;
        } else if((option.length() == 1 && option[0] == 's') || option == "stop") {
                return Stop;
        } else if((option.length() == 1 && option[0] == 'q') || option == "queue") {
                return Queue;
        } else if((option.length() == 1 && option[0] == 'r') || option == "remove") {
                return Remove;
        } else if((option.length() == 1 && option[0] == 'x') || option == "quit") {
                return Quit;
        } else {
                return Unknown;
        }
}

Options::Mode Options::Read()
{
        if(!runtime.interactive) {
                return Batch;
        }

        while(true) {
                std::cout << "\n";
                std::cout << "vlcpp> ";

                std::string input, option;
                std::getline(std::cin, input);

                if(input.empty()) {
                        continue;
                }

                std::istringstream iss(input);
                iss >> option;

                Options::Mode mode = Read(option);

                if(mode == Descr) {
                        if(iss >> input) {
                                Help(input);
                                continue;
                        } else {
                                Help();
                                continue;
                        }
                } else if(mode == Add) {
                        while(iss >> input) {
                                Option opt(_strdup(input.c_str()));

                                if(strcmp(opt.key, "name") == 0) {
                                        media.name = opt.val;
                                } else if(strcmp(opt.key, "input") == 0) {
                                        media.input = opt.val;
                                } else if(strcmp(opt.key, "output") == 0) {
                                        media.output = opt.val;
                                } else {
                                        std::cerr << "Unknown option '" << opt.key << "'\n";
                                        return Unknown;
                                }
                        }
                        if(media.name.length()) {
                                return mode;
                        }
                } else if(mode == Play) {
                        if(iss >> input) {
                                media.name = input;
                                return mode;
                        }
                } else if(mode == Stop) {
                        if(iss >> input) {
                                media.name = input;
                                return mode;
                        }
                } else if(mode == Remove) {
                        if(iss >> input) {
                                media.name = input;
                                return mode;
                        }
                } else if(mode == Queue) {
                        if(iss >> input) {
                                media.name = input;
                        } else {
                                media.name = "";
                        }
                        return mode;
                } else if(mode == Quit) {
                        return mode;
                } else {
                        std::cerr << "Unknown option '" << option << "', see 'help'\n";
                        continue;
                }
        }
}

void Options::Help()
{
        std::cout 
                << "Select option:\n"
                << "--------------\n"
                << "(a) - Add media    : add <options>\n"
                << "(p) - Play media   : play <name>\n"
                << "(s) - Stop media   : stop <name>\n"
                << "(r) - Remove media : remove <name>\n"
                << "(q) - Show Queue   : queue [<name>]\n"
                << "(x) - Quit         : quit\n"
                << "(h) - Help         : help [<option>]\n";
}

void Options::Help(const std::string &option)
{
        if(option == "add") {
                std::cout 
                        << "Define media for playback queue. Use 'start <name>' and 'stop <name>' to playback control.\n"
                        << "\n"
                        << "Synopsis: add name=<name> input=<source> output=<string>\n"
                        << "Where:\n"
                        << "  - <name> is an unique identifier, like 'desktop'\n"
                        << "  - <source> is e.g. 'screen://', 'dvd://' or 'file://path'\n"
                        << "  - <string> is an sout definition, e.g. '#transcode{vcodec=h264,vb=800,scale=auto,acodec=none}:http{mux=ts,dst=:3752/desktop.mp4}'\n"
                        << "Example:\n"
                        << "  - add name=media1 input=screen:// output=#transcode{vcodec=h264,venc=x264,vb=800,scale=0}:http{mux=ffmpeg{mux=flv},dst=:3752/desktop.flv}\n";
        } else if(option == "play") {
                std::cout 
                        << "Start playback of named media. See 'help add'.\n"
                        << "\n"
                        << "Synopsis: play <name>\n"
                        << "Example:\n"
                        << "  - play media1\n";
        } else if(option == "stop") {
                std::cout 
                        << "Stop playback of named media. See 'help add' and 'help play'.\n"
                        << "\n"
                        << "Synopsis: stop <name>\n"
                        << "Example:\n"
                        << "  - stop media1\n";
        } else if(option == "remove") {
                std::cout 
                        << "Remove named media from playback queue.\n"
                        << "\n"
                        << "Synopsis: remove <name>\n"
                        << "Example:\n"
                        << "  - remove media1\n";
        } else if(option == "queue") {
                std::cout 
                        << "Display information about queued media. If optional <name> is passed, then display details about that particular media.\n"
                        << "\n"
                        << "Synopsis: queue [<name>]\n"
                        << "Example:\n"
                        << "  - queue\n"
                        << "  - queue media1\n";
        } else if(option == "help") {
                std::cout 
                        << "Show option specific help.\n"
                        << "\n"
                        << "Synopsis: help add|play|stop|remove|queue\n"
                        << "Example:\n"
                        << "  - help add\n";
        } else {
                std::cerr << "Unknown option '" << option << "', see 'help'\n";
        }
}
