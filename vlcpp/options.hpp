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

#ifndef __OPTIONS_HPP__
#define __OPTIONS_HPP__

#include <map>

// 
// Command line and interactive options.
// 
struct Options
{
        enum Mode { 
                Add,            // Add media
                Play,           // Play media
                Stop,           // Stop media
                Remove,         // Remove media
                Queue,          // Show media queue
                Quit,           // Quit application
                Batch,          // Run in batch mode
                Descr,          // Show command description
                Unknown         // Unknown command option
        };

        enum State {
                Waiting,        // Media defined.
                Running,        // Media playback started
                Stopped         // Media playback stopped
        };

        explicit Options();
        Options(const Options &options);
        Options(int argc, char **argv);
        ~Options();

        void Parse(int argc, char **argv);      // Command line
        void Usage();                           // Command line
        void Version();                         // Command line

        Mode Read();                            // Interactive
        Mode Read(const std::string &option);   // Interactive
        void Help();                            // Interactive
        void Help(const std::string &option);   // Interactive

        void Dump(std::ostream &out);           // Command line and interactive

        // 
        // Media options:
        // 
        struct Media
        {
                std::string name;
                std::string input;
                std::string output; 
                int numopts;
                const char **options;
                bool enabled;
                bool loop;
                
                Media(); 
                Media(const char *name, const char *input, const char *output);
                Media(const std::string &name, const std::string &input, const std::string &output);

        } media;

        // 
        // Runtime options:
        // 
        struct Runtime
        {
                const char *prog;       // Program name
                const char *version;    // Program version
                const char *bugreport;  // Bug report address
                bool verbose;
                bool interactive;
                
                Runtime();
                Runtime(const char *prog);

        } runtime;

        // 
        // Queued media names.
        // 
        std::map<std::string, State> queue;
};

#endif  // __OPTIONS_HPP__
