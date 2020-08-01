call setpath.bat
"nios2-elf-gcc" -S -Os dd.cpp -o dd.s
compile dd.s,inc\irq.asm out.hex dd.temp.s dos BOOT.BIN
pause
