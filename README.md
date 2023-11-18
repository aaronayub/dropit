# gdrag 
A lightweight, GTK-based drag-and-drop panel meant to be used with command-line file managers.

### Build Instructions
To build this project, run `meson setup build && ninja -C build`. This will create the executable file 'gdrag'.

### Usage
After building the program, the program can be run with `./gdrag {...FILES}`, where the argument is a list of files for which the drag-and-drop panel is created.

### Build Requirements
- GTK 4
- meson

### License
Copyright (C) 2023 Aaron Ayub

This project and its source code is made available under the GNU GPL version 3.0 only.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
