rem "nios2-elf-gcc" -E inc/soft-fp/muldf3.c -o muldf3.temp.c
call setpath.bat
"nios2-elf-gcc" -Os -S inc/soft-fp/clzsi2.c -o clzsi2.s
"nios2-elf-gcc" -Os -S inc/soft-fp/fixdfsi.c -o fixdfsi.s
"nios2-elf-gcc" -Os -S inc/soft-fp/muldf3.c -o muldf3.s
"nios2-elf-gcc" -Os -S inc/soft-fp/adddf3.c -o adddf3.s
"nios2-elf-gcc" -Os -mhw-div -S c.cpp -o c.s
compile clzsi2.s,fixdfsi.s,muldf3.s,adddf3.s,c.s out.hex c.temp.s dos
pause
