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

#include <iostream>
#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif

#include "library.hpp"
#include "vlcpp.hpp"

#if ! defined(HAVE_DLOPEN) || ! defined(HAVE_DLSYM)
#error "dlopen() and dlsym() are required functions!"
#endif
#if ! defined(HAVE_DLERROR)
#define dlerror() ("<unknown>")
#endif
#if ! defined(HAVE_DLCLOSE)
#define dlclose((h) (1)
#endif

VLCPP::Library * VLCPP::Library::hLibrary = 0;

VLCPP::Library::Library() : hModule(0)
{
        const char *libvlc[] = {
                "libvlc/libvlc.so",             // Sub directory
                "vlc/libvlc.so",                // Sub directory
                "/usr/lib/libvlc.so",           // Standard installation
                "/usr/local/lib/libvlc.so",     // Local installation
                0
        };
        
        // 
        // Load library:
        // 
        for(const char **p = libvlc; *p; ++p) {
                if((hModule = dlopen(*p, RTLD_LAZY))) {
                        break;
                }
        }
        
        // 
        // Give up if handle is unset:
        // 
        if(!hModule) {
                throw Exception("Failed load VLC (dlopen)", dlerror());
        }
        
        // 
        // Assign function pointers:
        // 
        if(!(m_libvlc_errmsg = (proc_libvlc_errmsg)dlsym(hModule, "libvlc_errmsg"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_errmsg)", dlerror());
        }
        if(!(m_libvlc_clearerr = (proc_libvlc_clearerr)dlsym(hModule, "libvlc_clearerr"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_clearerr)", dlerror());
        }

        if(!(m_libvlc_get_version = (proc_libvlc_get_version)dlsym(hModule, "libvlc_get_version"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_get_version)", dlerror());
        }

        if(!(m_libvlc_new = (proc_libvlc_new)dlsym(hModule, "libvlc_new"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_new)", dlerror());
        }
        if(!(m_libvlc_vlm_release = (proc_libvlc_vlm_release)dlsym(hModule, "libvlc_vlm_release"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_vlm_release)", dlerror());
        }

        if(!(m_libvlc_vlm_add_broadcast = (proc_libvlc_vlm_add_broadcast)dlsym(hModule, "libvlc_vlm_add_broadcast"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_vlm_add_broadcast)", dlerror());
        }
        if(!(m_libvlc_vlm_add_vod = (proc_libvlc_vlm_add_vod)dlsym(hModule, "libvlc_vlm_add_vod"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_vlm_add_vod)", dlerror());
        }
        if(!(m_libvlc_vlm_change_media = (proc_libvlc_vlm_change_media)dlsym(hModule, "libvlc_vlm_change_media"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_vlm_change_media)", dlerror());
        }
        if(!(m_libvlc_vlm_play_media = (proc_libvlc_vlm_play_media)dlsym(hModule, "libvlc_vlm_play_media"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_vlm_play_media)", dlerror());
        }
        if(!(m_libvlc_vlm_pause_media = (proc_libvlc_vlm_pause_media)dlsym(hModule, "libvlc_vlm_pause_media"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_vlm_pause_media)", dlerror());
        }
        if(!(m_libvlc_vlm_seek_media = (proc_libvlc_vlm_seek_media)dlsym(hModule, "libvlc_vlm_seek_media"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_vlm_seek_media)", dlerror());
        }
        if(!(m_libvlc_vlm_stop_media = (proc_libvlc_vlm_stop_media)dlsym(hModule, "libvlc_vlm_stop_media"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_vlm_stop_media)", dlerror());
        }
        if(!(m_libvlc_vlm_del_media = (proc_libvlc_vlm_del_media)dlsym(hModule, "libvlc_vlm_del_media"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_vlm_del_media)", dlerror());
        }
        if(!(m_libvlc_vlm_show_media = (proc_libvlc_vlm_show_media)dlsym(hModule, "libvlc_vlm_show_media"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_vlm_show_media)", dlerror());
        }

        if(!(m_libvlc_vlm_set_enabled = (proc_libvlc_vlm_set_enabled)dlsym(hModule, "libvlc_vlm_set_enabled"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_vlm_set_enabled)", dlerror());
        }
        if(!(m_libvlc_vlm_set_loop = (proc_libvlc_vlm_set_loop)dlsym(hModule, "libvlc_vlm_set_loop"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_vlm_set_loop)", dlerror());
        }
        if(!(m_libvlc_vlm_set_mux = (proc_libvlc_vlm_set_mux)dlsym(hModule, "libvlc_vlm_set_mux"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_vlm_set_mux)", dlerror());
        }
        if(!(m_libvlc_vlm_set_output = (proc_libvlc_vlm_set_output)dlsym(hModule, "libvlc_vlm_set_output"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_vlm_set_output)", dlerror());
        }
        if(!(m_libvlc_vlm_set_input = (proc_libvlc_vlm_set_input)dlsym(hModule, "libvlc_vlm_set_input"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_vlm_set_input)", dlerror());
        }
        if(!(m_libvlc_vlm_add_input = (proc_libvlc_vlm_add_input)dlsym(hModule, "libvlc_vlm_add_input"))) {
                throw Exception("Failed get process address (dlsym: -> libvlc_vlm_add_input)", dlerror());
        }
}

VLCPP::Library::~Library() 
{
        if(hModule) {
                if(!dlclose(hModule)) {
                        throw Exception("Failed close VLC (dlclose)", dlerror());
                }
                
        }
}
