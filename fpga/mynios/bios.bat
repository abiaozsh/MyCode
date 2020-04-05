call setpath.bat
"nios2-elf-gcc" -S bios.cpp -Os -o bios.s
compile bios.s out.hex bios.temp.s
pause
