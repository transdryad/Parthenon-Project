# Tahoot!

Made Hazel V & [@t3mp0r4ry23](https://github.com/t3mp0r4ry23)

Basic idea: Kahoot, but in the terminal (need a server which can handle multiple clients).
Built using c++ with meson. If provided executables don't fuction, please compile from source.

To build: meson setup build && meson compile -C build

## Usage:

See example.ini for an example question file.

The server takes these arguments: {ip address} {port} {question file}

The client takes thses arguments: {ip address} {port} 
