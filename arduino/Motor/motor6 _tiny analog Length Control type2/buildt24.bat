del motor6.hex
del motor6.ii
del motor6.o
del motor6.s
del a.out
set arduinopath=C:\Program Files (x86)\Arduino
rem set arduinopath=C:\Program Files\Arduino
set comPort=COM5
"%arduinopath%\hardware\tools\avr\bin\avr-g++" -Os -mmcu=attiny24 -save-temps -fverbose-asm -I"%arduinopath%\hardware\arduino\cores\arduino" motor6.cpp
"%arduinopath%\hardware\tools\avr\bin\avr-g++" -Os -mmcu=attiny24                           -I"%arduinopath%\hardware\arduino\cores\arduino" motor6.cpp -o motor6.o
"%arduinopath%\hardware\tools\avr\bin\avr-objcopy" -O ihex motor6.o motor6.hex
"%arduinopath%\hardware\tools\avr\bin\avrdude" -C"%arduinopath%\hardware\tools\avr\etc\avrdude.conf" -pattiny24 -cstk500v1 -P\\.\%comPort% -b19200 -Uflash:w:motor6.hex:i 
pause
