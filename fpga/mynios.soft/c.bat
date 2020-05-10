rem "nios2-elf-gcc" -E inc/soft-fp/muldf3.c -o muldf3.temp.c
call setpath.bat
"nios2-elf-gcc" -Os -mhw-div -S inc/soft-fp/float.c -o float.s
"nios2-elf-gcc" -Os -mhw-div -S c.cpp -o c.s
compile float.s,c.s out.hex c.temp.s dos
pause
