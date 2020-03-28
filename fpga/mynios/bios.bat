"gcc\bin\gcc" -S -static-libgcc -msoft-float -masm=intel bios.c -o bios.s
compile bios.s out.hex bios.temp.s
pause
