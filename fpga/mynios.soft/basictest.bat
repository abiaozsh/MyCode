call setpath.bat
"nios2-elf-gcc" -S -Os basictest.cpp -o basictest.s
compile basictest.s,inc\irq.asm out.hex basictest.temp.s dos basictest.bin
pause
