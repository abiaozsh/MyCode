rem "nios2-elf-gcc" -E inc/soft-fp/muldf3.c -o muldf3.temp.c
call setpath.bat
rem "nios2-elf-gcc" -mhw-div -Os -S inc/soft-fp/clzsi2.c -o clzsi2.s
"nios2-elf-gcc" -Os -mhw-div -S inc/soft-fp/fixdfsi.c -o fixdfsi.s
rem "nios2-elf-gcc" -mhw-div -Os -S inc/soft-fp/muldf3.c -o muldf3.s
rem "nios2-elf-gcc" -mhw-div -Os -S inc/soft-fp/adddf3.c -o adddf3.s
"nios2-elf-gcc" -Os -mhw-div -S inc/soft-fp/float.c -o float.s
"nios2-elf-gcc" -Os -mhw-div -S c.cpp -o c.s
compile float.s,c.s out.hex c.temp.s dos
pause
