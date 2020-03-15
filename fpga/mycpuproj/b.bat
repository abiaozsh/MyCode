"gcc\bin\gcc" -S -static-libgcc -msoft-float -masm=intel b.c -o b.s
compile b.s b.hex b.temp.s
pause
