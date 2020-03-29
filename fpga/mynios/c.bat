call setpath.bat
"nios2-elf-gcc" -S c.c -o c.s
compile c.s out.hex c.temp.s
pause
