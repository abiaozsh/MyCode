call setpath.bat
"nios2-elf-gcc" -Os -mhw-div -S a.cpp -o a.s
compile a.s out.hex a.temp.s dos a.bin
pause
