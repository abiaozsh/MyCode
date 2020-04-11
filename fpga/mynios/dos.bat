call setpath.bat
"nios2-elf-gcc" -S dos.cpp -Os -o dos.s
compile dos.s out.hex dos.temp.s dos BOOT.BIN
pause
