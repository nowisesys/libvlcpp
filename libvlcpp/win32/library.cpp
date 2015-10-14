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

#include "library.hpp"
#include "vlcpp.hpp"

VLCPP::Library * VLCPP::Library::hLibrary = 0;

VLCPP::Library::Library() : hModule(0)
{
        HKEY hKey;
        TCHAR lpInstallDir[MAX_PATH];
        DWORD bLen = sizeof(lpInstallDir);
        REGSAM regSamDesired = KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_READ;

        // 
        // Get install path. Notice that 64-bit installation overwrite values
        // for 32-bit installation (and opposite).
        // 
        if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\VideoLan\\VLC"), 0, regSamDesired, &hKey) != ERROR_SUCCESS) {
                std::cerr << "Warning: Failed open VLC registry key. Is VLC not installed?\n";
        } else if(RegQueryValueEx(hKey, TEXT("InstallDir"), 0, 0, (LPBYTE)lpInstallDir, &bLen) != ERROR_SUCCESS) {
                throw Exception("Failed Windows API call (RegQueryValue)", GetLastError());
        }

        // 
        // Set DLL search pathes. Favor application local, then default for compiled
        // target, followed by install path.
        // 
        if(!SetDllDirectory(TEXT("VLC"))) {
                throw Exception("Failed Windows API call (SetDllDirectory)", GetLastError());
        }
#if defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) || defined(_Wp64)
        if(!SetDllDirectory(L"C:\\Program Files\\VideoLAN\\VLC")) {
                throw Exception("Failed Windows API call (SetDllDirectory)", GetLastError());
        }
#else
        if(!SetDllDirectory(TEXT("C:\\Program Files (x86)\\VideoLAN\\VLC"))) {
                throw Exception("Failed Windows API call (SetDllDirectory)", GetLastError());
        }
#endif

        if(bLen != 0 && !SetDllDirectory(lpInstallDir)) {
                throw Exception("Failed Windows API call (SetDllDirectory)", GetLastError());
        }

        // 
        // Load library:
        // 
        if(!(hModule = LoadLibrary(TEXT("libvlc.dll")))) {
                throw Exception("Failed load VLC (LoadLibrary)", GetLastError());
        }

        // 
        // Assign function pointers:
        // 
        if(!(m_libvlc_errmsg = (proc_libvlc_errmsg)GetProcAddress(hModule, "libvlc_errmsg"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_errmsg)", GetLastError());
        }
        if(!(m_libvlc_clearerr = (proc_libvlc_clearerr)GetProcAddress(hModule, "libvlc_clearerr"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_clearerr)", GetLastError());
        }

        if(!(m_libvlc_get_version = (proc_libvlc_get_version)GetProcAddress(hModule, "libvlc_get_version"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_get_version)", GetLastError());
        }

        if(!(m_libvlc_new = (proc_libvlc_new)GetProcAddress(hModule, "libvlc_new"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_new)", GetLastError());
        }
        if(!(m_libvlc_vlm_release = (proc_libvlc_vlm_release)GetProcAddress(hModule, "libvlc_vlm_release"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_vlm_release)", GetLastError());
        }

        if(!(m_libvlc_vlm_add_broadcast = (proc_libvlc_vlm_add_broadcast)GetProcAddress(hModule, "libvlc_vlm_add_broadcast"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_vlm_add_broadcast)", GetLastError());
        }
        if(!(m_libvlc_vlm_add_vod = (proc_libvlc_vlm_add_vod)GetProcAddress(hModule, "libvlc_vlm_add_vod"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_vlm_add_vod)", GetLastError());
        }
        if(!(m_libvlc_vlm_change_media = (proc_libvlc_vlm_change_media)GetProcAddress(hModule, "libvlc_vlm_change_media"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_vlm_change_media)", GetLastError());
        }
        if(!(m_libvlc_vlm_play_media = (proc_libvlc_vlm_play_media)GetProcAddress(hModule, "libvlc_vlm_play_media"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_vlm_play_media)", GetLastError());
        }
        if(!(m_libvlc_vlm_pause_media = (proc_libvlc_vlm_pause_media)GetProcAddress(hModule, "libvlc_vlm_pause_media"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_vlm_pause_media)", GetLastError());
        }
        if(!(m_libvlc_vlm_seek_media = (proc_libvlc_vlm_seek_media)GetProcAddress(hModule, "libvlc_vlm_seek_media"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_vlm_seek_media)", GetLastError());
        }
        if(!(m_libvlc_vlm_stop_media = (proc_libvlc_vlm_stop_media)GetProcAddress(hModule, "libvlc_vlm_stop_media"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_vlm_stop_media)", GetLastError());
        }
        if(!(m_libvlc_vlm_del_media = (proc_libvlc_vlm_del_media)GetProcAddress(hModule, "libvlc_vlm_del_media"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_vlm_del_media)", GetLastError());
        }
        if(!(m_libvlc_vlm_show_media = (proc_libvlc_vlm_show_media)GetProcAddress(hModule, "libvlc_vlm_show_media"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_vlm_show_media)", GetLastError());
        }

        if(!(m_libvlc_vlm_set_enabled = (proc_libvlc_vlm_set_enabled)GetProcAddress(hModule, "libvlc_vlm_set_enabled"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_vlm_set_enabled)", GetLastError());
        }
        if(!(m_libvlc_vlm_set_loop = (proc_libvlc_vlm_set_loop)GetProcAddress(hModule, "libvlc_vlm_set_loop"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_vlm_set_loop)", GetLastError());
        }
        if(!(m_libvlc_vlm_set_mux = (proc_libvlc_vlm_set_mux)GetProcAddress(hModule, "libvlc_vlm_set_mux"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_vlm_set_mux)", GetLastError());
        }
        if(!(m_libvlc_vlm_set_output = (proc_libvlc_vlm_set_output)GetProcAddress(hModule, "libvlc_vlm_set_output"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_vlm_set_output)", GetLastError());
        }
        if(!(m_libvlc_vlm_set_input = (proc_libvlc_vlm_set_input)GetProcAddress(hModule, "libvlc_vlm_set_input"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_vlm_set_input)", GetLastError());
        }
        if(!(m_libvlc_vlm_add_input = (proc_libvlc_vlm_add_input)GetProcAddress(hModule, "libvlc_vlm_add_input"))) {
                throw Exception("Failed get process address (GetProcAddress: -> libvlc_vlm_add_input)", GetLastError());
        }

        // 
        // Get loaded DLL-filename:
        // 
        if(GetModuleFileName(hModule, lpInstallDir, sizeof(lpInstallDir)) == 0) {
                throw Exception("Failed Windows API call (GetModuleFileName)", GetLastError());
        }
}

VLCPP::Library::~Library() 
{
        if(hModule) {
                if(!FreeLibrary(hModule)) {
                        throw Exception("Failed close VLC (FreeLibrary)", GetLastError());
                }
        }
}
