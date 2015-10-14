
	**************************
 	 VLC Client Program
	**************************

Program linked against libvlcpp providing a lightweight shell on top of the C++ 
class library. This application can be used for (but not limited to) desktop
streaming or playing media files.

The program is either run in batch mode (controlled by command options) or started 
interactive. When running interactive, it can control play/stop of multiple media 
definitions in the queue:

vlcpp> queue 
Name:    State:
-----    ------
prog726  <waiting>
desktop  <started>

vlcpp> queue desktop
Queue desktop:
{
        "name": "desktop",
        "type": "broadcast",
        "enabled": "yes",
        "loop": "no",
        "inputs": [
                "screen://"

        ],
        "output": "#transcode{vcodec=WMV2,vb=256,acodec=none}:http{dst=:3752/desktop.asf}",
        "options": null,
        "instances": {
                "instance": {
                        "name": "default",
                        "state": "playing",
                        "position": "0.000000",
                        "time": "205205000",
                        "length": "0",
                        "rate": "1.000000",
                        "title": "0",
                        "chapter": "0",
                        "can-seek": "0",
                        "playlistindex": "1"

                }

        }

}


// Anders Lövgren, 2015-10-10
