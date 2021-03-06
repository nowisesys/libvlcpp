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

#include "library.hpp"
#include "vlcpp.hpp"

VLCPP::Exception::Exception(const char *msg, int code) 
: message(msg), code(code) 
{
}

VLCPP::Exception::Exception(const char *msg, const char *err, int code) 
: message(msg), code(code)
{
        if(err) {
                message += ": ";
                message += err;
        }
}

VLCPP::VLM::VLM() : lib(Library::Instance())
{
        if(!(vlc = lib->m_libvlc_new(0, 0))) {
                throw Exception("VLC: Failed create library handle", lib->m_libvlc_errmsg());
        }
}

VLCPP::VLM::VLM(int argc, char const *const *argv) : lib(Library::Instance())
{
        if(!(vlc = lib->m_libvlc_new(argc, argv))) {
                throw Exception("VLC: Failed create library handle", lib->m_libvlc_errmsg());
        }
}

VLCPP::VLM::~VLM() 
{
        lib->m_libvlc_vlm_release(vlc);
}

VLCPP::VLM::Instance * VLCPP::VLM::operator*() 
{ 
        return vlc; 
}

VLCPP::VLM::Handle VLCPP::VLM::GetHandle() const
{
        return lib->GetHandle();
}

bool VLCPP::VLM::HasError() const 
{
        return lib->m_libvlc_errmsg() != 0;
}

const char * VLCPP::VLM::GetError() const 
{
        return lib->m_libvlc_errmsg();
}

void VLCPP::VLM::ClearError() const
{
        lib->m_libvlc_clearerr();
}

const char * VLCPP::VLM::GetVersion() const
{
        return lib->m_libvlc_get_version();
}

void VLCPP::VLM::AddBroadcast(const char *name, 
                       const char *input,
                       const char *output, 
                       int numopts,
                       const char * const* options,
                       bool enabled, 
                       bool loop) const
{
        if(lib->m_libvlc_vlm_add_broadcast(vlc, name, input, output, numopts, options, enabled, loop) < 0) {
                throw Exception("VLC: Failed add broadcast", lib->m_libvlc_errmsg());
        }
}

void VLCPP::VLM::AddVideoOnDemand(const char *name, 
                           const char *input,
                           int numopts, 
                           const char * const* options,
                           bool enabled, 
                           const char *mux
                           ) const
{
        if(lib->m_libvlc_vlm_add_vod(vlc, name, input, numopts, options, enabled, mux) < 0) {
                throw Exception("VLC: Failed add video on demand", lib->m_libvlc_errmsg());
        }
}

void VLCPP::VLM::ChangeMedia(const char *name, 
                      const char *input,
                      const char *output, 
                      int numopts,
                      const char * const *options,
                      bool enabled, bool loop) const
{
        if(lib->m_libvlc_vlm_change_media(vlc, name, input, output, numopts, options, enabled, loop) < 0) {
                throw Exception("VLC: Failed change media", lib->m_libvlc_errmsg());
        }
}

void VLCPP::VLM::PlayMedia(const char *name) const
{
        if(lib->m_libvlc_vlm_play_media(vlc, name) < 0) {
                throw Exception("VLC: Failed play media", lib->m_libvlc_errmsg());
        }
}

void VLCPP::VLM::PauseMedia(const char *name) const
{
        if(lib->m_libvlc_vlm_pause_media(vlc, name) < 0) {
                throw Exception("VLC: Failed pause media", lib->m_libvlc_errmsg());
        }
}

void VLCPP::VLM::SeekMedia(const char *name, float percent) const
{
        if(lib->m_libvlc_vlm_seek_media(vlc, name, percent) < 0) {
                throw Exception("VLC: Failed seek media", lib->m_libvlc_errmsg());
        }
}

void VLCPP::VLM::StopMedia(const char *name) const
{
        if(lib->m_libvlc_vlm_stop_media(vlc, name) < 0) {
                throw Exception("VLC: Failed stop media", lib->m_libvlc_errmsg());
        }
}

void VLCPP::VLM::RemoveMedia(const char *name) const
{
        if(lib->m_libvlc_vlm_del_media(vlc, name) < 0) {
                throw Exception("VLC: Failed remove media", lib->m_libvlc_errmsg());
        }
}

void VLCPP::VLM::ShowMedia(const char *name, std::ostream &out) const
{
        out << lib->m_libvlc_vlm_show_media(vlc, name);
}

const char * VLCPP::VLM::ShowMedia(const char *name) const
{
        return lib->m_libvlc_vlm_show_media(vlc, name);
}

void VLCPP::VLM::SetEnabled(const char *name, bool enabled) const
{
        if(lib->m_libvlc_vlm_set_enabled(vlc, name, enabled) < 0) {
                throw Exception("VLC: Failed enable/disable media", lib->m_libvlc_errmsg());
        }
}

void VLCPP::VLM::SetLoop(const char *name, bool enabled) const
{
        if(lib->m_libvlc_vlm_set_loop(vlc, name, enabled) < 0) {
                throw Exception("VLC: Failed set loop status", lib->m_libvlc_errmsg());
        }
}

void VLCPP::VLM::SetMux(const char *name, const char *mux) const
{
        if(lib->m_libvlc_vlm_set_mux(vlc, name, mux) < 0) {
                throw Exception("VLC: Failed set mux", lib->m_libvlc_errmsg());
        }
}

void VLCPP::VLM::SetOutput(const char *name, const char *output) const
{
        if(lib->m_libvlc_vlm_set_output(vlc, name, output) < 0) {
                throw Exception("VLC: Failed set output", lib->m_libvlc_errmsg());
        }
}

void VLCPP::VLM::SetInput(const char *name, const char *input) const
{
        if(lib->m_libvlc_vlm_set_input(vlc, name, input) < 0) {
                throw Exception("VLC: Failed set input", lib->m_libvlc_errmsg());
        }
}

void VLCPP::VLM::AddInput(const char *name, const char *input) const
{
        if(lib->m_libvlc_vlm_add_input(vlc, name, input) < 0) {
                throw Exception("VLC: Failed add input", lib->m_libvlc_errmsg());
        }
}
