call setpath.bat
"nios2-elf-gcc" -S a.c -o a.s
compile a.s out.hex a.temp.s
pause
