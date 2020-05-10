"gcc\bin\gcc" -S -static-libgcc -msoft-float -masm=intel f.c -o f.s
compile f.s out.hex f.temp.s
pause
