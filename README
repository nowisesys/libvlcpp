
	************************************
 	 C++ Class Library for VLC/VLM 
	************************************

This C++ library is a lightweight frontend for VLM (VideoLan Manager) found in 
the VLC library (libvlc). It's an development library meant to be used for 
application development.

The linking with the VLC library (libvlc) is done at runtime instead of loadtime. 
On Windows, this means that programs using libvlcpp can use a system installation 
of VLC.

The VLC library (libvlc and its related plugin directory) are searched for in:

  1. A sub directory relative to the application.
  2. The standard location for VLC. This is system dependent.
  3. The registered install directory (only on Windows).
  
Like VLC, this library is released under the GNU Lesser General Public License 
(LGPL-3). See COPYING.txt and LGPL-3 distributed with the source code.

Usage:
------------------

	#include <iostream>
	#include <vlcpp.hpp>

	int main(int argc, char **argv)
	{
		try {
			VLCPP::VLM vlc(argc, argv);
		
			const char *name   = "desktop";
			const char *input  = "screen://";
			const char *output = "#transcode{vcodec=WMV2,vb=256,acodec=none}:http{dst=:3752/desktop.asf}";
			
			vlc.AddBroadcast(name, input, output, 0, 0);
			vlc.PlayMedia(name);

			std::cout << "Hit <enter> to quit.\n";
			std::cin.get();
	        
			vlc.StopMedia(name);
			vlc.RemoveMedia(name);
	        
		} catch(VLC::Exception &exception) {
			std::cerr << argv[0] << ": " << exception.message << " (" << exception.code << ")\n";
		}
	}

Extending:
------------------

The VLC library handle and instance wrapped by the VLC class can be reused for 
extended functionality (exposing new function from the VLC library):

	#include <dlfcn.h>
	#include <vlc/vlc.h>
	#include <vlcpp.hpp>

	typedef libvlc_media_t * (*proc)(libvlc_instance_t *p_instance, const char * psz_mrl);
	proc func_media_new_location;

	if(func_media_new_location = (proc)dlsym(vlc->GetHandle(), "libvlc_media_new_location")) {
			if(func_media_new_location(*vlc, "file://file.mp4") != -1) {
					// ...do something
			}
	}


// Anders Lövgren, 2015-10-10
