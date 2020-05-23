call setpath.bat
"nios2-elf-gcc" -S dd.cpp -Os -o dd.s
compile dd.s out.hex dd.temp.s dos BOOT.BIN
pause
