call setpath.bat
"nios2-elf-gcc" -S e.cpp -o e.s
compile e.s out.hex e.temp.s dos e.bin
pause
