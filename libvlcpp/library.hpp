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

#ifndef __LIBRARY_HPP__
#define __LIBRARY_HPP__

#include <vlc/vlc.h>
#include "vlcpp.hpp"

namespace VLCPP {

        // 
        // Internal libvlc binding class.
        // 
        class LIBVLCPP_API_HIDDEN Library
        {
        public:
#ifdef WIN32
                typedef HMODULE Handle;
#else
                typedef void * Handle;
#endif

                typedef const char * (*proc_libvlc_errmsg)(void);
                proc_libvlc_errmsg m_libvlc_errmsg;

                typedef void (*proc_libvlc_clearerr)(void);
                proc_libvlc_clearerr m_libvlc_clearerr;

                typedef libvlc_instance_t * (*proc_libvlc_new)(int, const char *const *);
                proc_libvlc_new m_libvlc_new;

                typedef void (*proc_libvlc_vlm_release)(libvlc_instance_t *);
                proc_libvlc_vlm_release m_libvlc_vlm_release;

                typedef int (*proc_libvlc_vlm_add_broadcast)(libvlc_instance_t *, 
                        const char *, const char *, const char *, int, const char * const*, int, int);
                proc_libvlc_vlm_add_broadcast m_libvlc_vlm_add_broadcast;

                typedef int (*proc_libvlc_vlm_add_vod)(libvlc_instance_t *,
                        const char *, const char *, int, const char * const*, int, const char *);
                proc_libvlc_vlm_add_vod m_libvlc_vlm_add_vod;

                typedef int (*proc_libvlc_vlm_change_media)(libvlc_instance_t *,
                        const char *, const char *, const char *, int, const char * const *, int, int);
                proc_libvlc_vlm_change_media m_libvlc_vlm_change_media;

                typedef int (*proc_libvlc_vlm_play_media)(libvlc_instance_t *, const char *);
                proc_libvlc_vlm_play_media m_libvlc_vlm_play_media;

                typedef int (*proc_libvlc_vlm_pause_media)(libvlc_instance_t *, const char *);
                proc_libvlc_vlm_pause_media m_libvlc_vlm_pause_media;

                typedef int (*proc_libvlc_vlm_seek_media)(libvlc_instance_t *, const char *, float);
                proc_libvlc_vlm_seek_media m_libvlc_vlm_seek_media;

                typedef int (*proc_libvlc_vlm_stop_media)(libvlc_instance_t *, const char *);
                proc_libvlc_vlm_stop_media m_libvlc_vlm_stop_media;

                typedef int (*proc_libvlc_vlm_del_media)(libvlc_instance_t *, const char *);
                proc_libvlc_vlm_del_media m_libvlc_vlm_del_media;

                typedef const char * (*proc_libvlc_vlm_show_media)(libvlc_instance_t *, const char *psz_name);
                proc_libvlc_vlm_show_media m_libvlc_vlm_show_media;

                typedef const char * (*proc_libvlc_get_version)(void);
                proc_libvlc_get_version m_libvlc_get_version;

                typedef int (*proc_libvlc_vlm_set_enabled)(libvlc_instance_t *, const char *, int);
                proc_libvlc_vlm_set_enabled m_libvlc_vlm_set_enabled;

                typedef int (*proc_libvlc_vlm_set_loop)(libvlc_instance_t *, const char *, int);
                proc_libvlc_vlm_set_loop m_libvlc_vlm_set_loop;

                typedef int (*proc_libvlc_vlm_set_mux)(libvlc_instance_t *, const char *, const char *);
                proc_libvlc_vlm_set_mux m_libvlc_vlm_set_mux;

                typedef int (*proc_libvlc_vlm_set_output)(libvlc_instance_t *, const char *, const char *);
                proc_libvlc_vlm_set_output m_libvlc_vlm_set_output;

                typedef int (*proc_libvlc_vlm_set_input)(libvlc_instance_t *, const char *, const char *);
                proc_libvlc_vlm_set_input m_libvlc_vlm_set_input;

                typedef int (*proc_libvlc_vlm_add_input)(libvlc_instance_t *, const char *, const char *);
                proc_libvlc_vlm_add_input m_libvlc_vlm_add_input;

                static Library * Instance()
                {
                        if(!hLibrary) {
                                hLibrary = new Library();
                        }
                        return hLibrary;
                }

                Handle GetHandle()
                {
                        return hModule;
                }

        private:
                Library();
                ~Library();

                static Library *hLibrary;
                Handle hModule;
        };

}       // namespace VLCPP

#endif  // __LIBRARY_HPP__
