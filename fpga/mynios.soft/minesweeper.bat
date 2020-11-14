call setpath.bat
rem 
"nios2-elf-gcc" -Os -S inc/divsi3.c -o divsi3.s
"nios2-elf-gcc" -S -Os minesweeper.cpp -o minesweeper.s
compile divsi3.s,minesweeper.s,inc\irq.asm out.hex minesweeper.temp.s dos minesweeper.bin
pause
