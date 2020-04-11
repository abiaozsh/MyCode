call setpath.bat
"nios2-elf-gcc" -S c.cpp -o c.s
compile c.s out.hex c.temp.s dos
pause
