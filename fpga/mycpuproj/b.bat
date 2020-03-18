"gcc\bin\gcc" -S -static-libgcc -msoft-float -masm=intel b.c -o b.s
compile b.s out.hex b.temp.s
pause
