call setpath.bat
"nios2-elf-gcc" -S tetris.cpp -o tetris.s
compile tetris.s out.hex tetris.temp.s dos
pause
