C:\altera\13.0sp1\nios2eds\bin\gnu\H-i686-mingw32\bin\nios2-elf-gcc --help
pause
rem C:\altera\13.0sp1\nios2eds\bin\gnu\H-i686-mingw32\bin\nios2-elf-gcc -xc -MP -MMD -c -DSYSTEM_BUS_WIDTH=32 -pipe -D__hal__ -DALT_NO_INSTRUCTION_EMULATION -DALT_SINGLE_THREADED  -O0 -g -Wall   -EL -mno-hw-div -mno-hw-mul -mno-hw-mulx  -o a.o a.c
rem pause

rem 1) Ԥ����
C:\altera\13.0sp1\nios2eds\bin\gnu\H-i686-mingw32\bin\nios2-elf-gcc -E a.c -o a.i

rem 2) ����        Ϊ������(Compilation)
C:\altera\13.0sp1\nios2eds\bin\gnu\H-i686-mingw32\bin\nios2-elf-gcc -S a.i -o a.s

rem 3) ���              (Assembly) �������ļ�
C:\altera\13.0sp1\nios2eds\bin\gnu\H-i686-mingw32\bin\nios2-elf-gcc -c a.s -o a.o

rem 4) ����      (Linking)
C:\altera\13.0sp1\nios2eds\bin\gnu\H-i686-mingw32\bin\nios2-elf-gcc -v a.o -o a
rem ���ļ�����
rem gcc test1 test2 test3  -o test
pause