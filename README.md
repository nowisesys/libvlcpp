# C++ Class Library for VLC/VLM 

This is a lightweight C++ frontend library for the VLC/VLM (VideoLan Manager) API. The goal was 
to create a robust and simple to use library for desktop streaming, even though its not limited 
to that specific task.

### Runtime linking:

Linking with libvlc is done at runtime instead of loadtime. On Windows, this means that 
programs using this library (libvlcpp) can use e.g. the system installation of VLC without 
requiring DLL-files and plug-ins to be bundled with the application.

### Search locations:

The VLC library (libvlc and its related plug-in directory) are searched for in:

1. A sub directory relative to the application.
2. The standard location for VLC. This is system dependent.
3. The registered install directory (only on Windows).

### License:

Like VLC, this library is released under the GNU Lesser General Public License (LGPL-3). 
See COPYING.txt and LGPL-3 distributed with the source code.

### Usage:

Example application streaming local computer desktop on http://localhost:3752/desktop.asf 
as video only stream (using WMV2 codec)

```c++
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
```

### Linux/UNIX:

Run autogen.sh to generate configure script and *.in template files:

```bash
bash$> ./autogen.sh
```

Then configure, build and install the library:

```bash
bash$> ./configure
bash$> make
bash$> make install
```

### Windows:

Use Visual Studio 2008 or later for building the libvlcpp.sln solution.

### Extending:

The VLC library handle and instance wrapped by the VLC class can be reused for extended 
functionality (exposing new function from the VLC library):

```c++
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
```
