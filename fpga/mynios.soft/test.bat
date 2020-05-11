call setpath.bat
"nios2-elf-gcc" -mhw-div -S test.cpp -o test.s
compile test.s out.hex test.temp.s dos test.bin
pause
