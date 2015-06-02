bin\ca65 -t nes libsrc\crt0.s
bin\ar65 a lib\nes.lib libsrc\crt0.o

bin\ca65 -t nes libsrc\crt0chr.s
bin\ar65 a lib\neschr.lib libsrc\crt0chr.o


pause
