"gcc\bin\gcc" -S -static-libgcc -msoft-float -masm=intel e.c -o e.s
compile e.s out.hex e.temp.s
pause
