del timer.hex
del timer.ii
del timer.o
del timer.s
del a.out
rem set arduinopath=C:\Program Files (x86)\Arduino
set arduinopath=C:\Program Files\Arduino
set comPort=COM5
"%arduinopath%\hardware\tools\avr\bin\avr-g++" -Os -mmcu=attiny24 -save-temps -fverbose-asm -I"%arduinopath%\hardware\arduino\cores\arduino" timer.cpp
"%arduinopath%\hardware\tools\avr\bin\avr-g++" -Os -mmcu=attiny24                           -I"%arduinopath%\hardware\arduino\cores\arduino" timer.cpp -o timer.o
"%arduinopath%\hardware\tools\avr\bin\avr-objcopy" -O ihex timer.o timer.hex
"%arduinopath%\hardware\tools\avr\bin\avrdude" -C"%arduinopath%\hardware\tools\avr\etc\avrdude.conf" -pattiny24 -cstk500v1 -P\\.\COM11 -b19200 -Uflash:w:timer.hex:i 
pause
