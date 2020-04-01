call setpath.bat
"nios2-elf-gcc" -S b.cpp -o b.s
compile b.s out.hex b.temp.s
pause
