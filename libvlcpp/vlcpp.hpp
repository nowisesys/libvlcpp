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

#ifndef __LIBVLCPP_HPP__
#define __LIBVLCPP_HPP__

#ifndef __cplusplus
extern "C++" {
#endif

#include <exception>
#include <string>

#if defined(WIN32) || defined(_WINDOWS) || defined(__CYGWIN__)
        /* Define LIBVLCPP_EXPORTS when building library on windows. */
# if defined(LIBVLCPP_EXPORTS)
#  if defined(__GNUC__)
#   define LIBVLCPP_API_PUBLIC __attribute__((dllexport))
#  else
        /* Note: GCC seems to support this syntax too. */
#   define LIBVLCPP_API_PUBLIC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__)
#   define LIBVLCPP_API_PUBLIC __attribute__((dllimport))
#  else
        /* Note: actually gcc seems to also supports this syntax. */
#   define LIBVLCPP_API_PUBLIC __declspec(dllimport) 
#  endif
# endif
# define LIBVLCPP_API_HIDDEN
#else
# if __GNUC__ >= 4
#  define LIBVLCPP_API_PUBLIC __attribute__ ((visibility("default")))
#  define LIBVLCPP_API_HIDDEN __attribute__ ((visibility("hidden")))
# else
#  define LIBVLCPP_API_PUBLIC
#  define LIBVLCPP_API_HIDDEN
# endif
#endif

#define LIBVLCPP_MAJOR 2
#define LIBVLCPP_MINOR 1
#define LIBVLCPP_REVIS 1

        namespace VLCPP {

                // 
                // Common exception class.
                // 
                struct LIBVLCPP_API_PUBLIC Exception
                {
                        Exception(const char *msg, int code = 0);
                        Exception(const char *msg, const char *err, int code = 0); 

                        std::string message;
                        int code;
                };

                // 
                // Wrapper C++ class for the VLC manager (libvlc).
                // 
                class LIBVLCPP_API_PUBLIC VLM
                {
                public:
#if defined(LIBVLCPP_EXPORTS)
                        typedef libvlc_instance_t Instance;
                        typedef void * Handle;
#else
                        typedef void Instance;          // libvlc_instance_t
                        typedef void * Handle;
#endif

                        VLM();
                        VLM(int argc, char const *const *argv);
                        ~VLM();

                        Instance * operator*();         // VLC instance
                        Handle GetHandle() const;       // VLC library handle

                        bool HasError() const;
                        const char * GetError() const;
                        void ClearError() const;

                        const char * GetVersion() const;

                        void AddBroadcast(
                                const char *name, 
                                const char *input,
                                const char *output, 
                                int numopts,
                                const char * const* options,
                                bool enabled = true, 
                                bool loop = false
                                ) const;
                        void AddVideoOnDemand(
                                const char *name, 
                                const char *input,
                                int numopts, 
                                const char * const* options,
                                bool enabled, 
                                const char *mux
                                ) const;

                        void ChangeMedia(
                                const char *name, 
                                const char *input,
                                const char *output, 
                                int numopts,
                                const char * const *options,
                                bool enabled, bool loop) const;

                        void PlayMedia(const char *name) const;
                        void PauseMedia(const char *name) const;
                        void SeekMedia(const char *name, float percent) const;
                        void StopMedia(const char *name) const;
                        void RemoveMedia(const char *name) const;

                        void ShowMedia(const char *name, std::ostream &out) const;
                        const char * ShowMedia(const char *name) const;

                        void SetEnabled(const char *name, bool enabled) const;
                        void SetLoop(const char *name, bool enabled) const;

                        void SetMux(const char *name, const char *output) const;
                        void SetOutput(const char *name, const char *output) const;
                        void SetInput(const char *name, const char *input) const;
                        void AddInput(const char *name, const char *input) const;

                private:
                        class Library *lib;
                        Instance *vlc;
                };
        }       // namespace VLCPP
#ifndef __cplusplus
}       // extern "C++"
#endif

#endif  // __LIBVLCPP_HPP__
