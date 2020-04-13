call setpath.bat
"nios2-elf-gcc" -S e.cpp -Os -o e.s
compile e.s out.hex e.temp.s bios e.bin
pause
