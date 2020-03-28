"gcc\bin\gcc" -S -static-libgcc -msoft-float -masm=intel d.c -o d.s
compile d.s out.hex d.temp.s
pause
