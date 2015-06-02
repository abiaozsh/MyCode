
del tetrisasm.o
del tetris.map.txt
del tetris.nes
..\bin\ca65 -t nes tetrisasm.s
..\bin\ld65 tetrisasm.o ..\lib\neschr.lib --config ..\lib\neschr.cfg -o tetris.nes -m tetris.map.txt
..\sim\famtasia.exe tetris.nes
rem ..\sim\Jnes tetris.nes

pause


exit





del tetris.o
del tetris.s
del tetris.map.txt
del tetris.nes
bin\cc65 -I include -t nes tetris.c
bin\ca65 -t nes tetris.s
bin\ld65 tetris.o lib\nes.lib --config nes.cfg -o tetris.nes -m tetris.map.txt
jnes tetris.nes
