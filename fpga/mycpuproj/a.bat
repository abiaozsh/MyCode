"gcc\bin\gcc" -S -static-libgcc -msoft-float -masm=intel a.c -o a.s
compile a.s out.hex a.temp.s
pause
