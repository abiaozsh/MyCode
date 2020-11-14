rem "nios2-elf-gcc" -E inc/soft-fp/muldf3.c -o muldf3.temp.c
call setpath.bat
rem -mhw-div 
"nios2-elf-gcc" -Os -S inc/divsi3.c -o divsi3.s
"nios2-elf-gcc" -Os -S inc/soft-fp/float.c -o float.s
"nios2-elf-gcc" -Os -S mathtest.cpp -o mathtest.s
compile float.s,divsi3.s,mathtest.s,inc\irq.asm out.hex mathtest.temp.s dos memtest.bin
pause
