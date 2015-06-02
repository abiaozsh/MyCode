del calcasm.o
del calc.map.txt
del calc.nes
..\binFile\ca65 -t nes calcasm.s
..\binFile\ld65 calcasm.o ..\lib\neschr.lib --config ..\lib\neschr.cfg -o calc.nes -m calc.map.txt
..\sim\FAMTASIA.EXE calc.nes
rem ..\sim\jnes calc.nes
rem ..\sim\VirtuaNES.exe calc.nes



exit
pause

exit
FAMTASIA.EXE calc.nes

del calc.o
rem del calc.s
del calc.map.txt
del calc.nes
rem bin\cc65 -I include -t nes calc.c
bin\ca65 -t nes calc.s
bin\ld65 calc.o lib\nes.lib --config nes.cfg -o calc.nes -m calc.map.txt
jnes calc.nes