C:\altera\13.0sp1\nios2eds\bin\gnu\H-i686-mingw32\bin\nios2-elf-gcc --help
pause
rem C:\altera\13.0sp1\nios2eds\bin\gnu\H-i686-mingw32\bin\nios2-elf-gcc -xc -MP -MMD -c -DSYSTEM_BUS_WIDTH=32 -pipe -D__hal__ -DALT_NO_INSTRUCTION_EMULATION -DALT_SINGLE_THREADED  -O0 -g -Wall   -EL -mno-hw-div -mno-hw-mul -mno-hw-mulx  -o a.o a.c
rem pause

rem 1) 预处理
C:\altera\13.0sp1\nios2eds\bin\gnu\H-i686-mingw32\bin\nios2-elf-gcc -E a.c -o a.i

rem 2) 编译        为汇编代码(Compilation)
C:\altera\13.0sp1\nios2eds\bin\gnu\H-i686-mingw32\bin\nios2-elf-gcc -S a.i -o a.s

rem 3) 汇编              (Assembly) 二进制文件
C:\altera\13.0sp1\nios2eds\bin\gnu\H-i686-mingw32\bin\nios2-elf-gcc -c a.s -o a.o

rem 4) 连接      (Linking)
C:\altera\13.0sp1\nios2eds\bin\gnu\H-i686-mingw32\bin\nios2-elf-gcc -v a.o -o a
rem 多文件链接
rem gcc test1 test2 test3  -o test
pause