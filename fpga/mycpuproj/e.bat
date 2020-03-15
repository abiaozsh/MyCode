"gcc\bin\gcc" -S -static-libgcc -msoft-float -masm=intel c.c -o c.s
compile c.s out.hex c.temp.s
pause
