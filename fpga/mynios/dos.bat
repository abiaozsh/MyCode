call setpath.bat
"nios2-elf-gcc" -S dos.cpp -o dos.s
compile dos.s dos.hex dos.temp.s dos BOOT.BIN
pause
