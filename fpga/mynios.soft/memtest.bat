call setpath.bat
"nios2-elf-gcc" -Os -S inc/divsi3.c -o divsi3.s
"nios2-elf-gcc" -S -Os memtest.cpp -o memtest.s
compile divsi3.s,memtest.s,inc\irq.asm out.hex memtest.temp.s bios memtest.bin
pause
