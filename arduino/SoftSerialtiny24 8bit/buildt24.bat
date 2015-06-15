del serial.hex
del serial.ii
del serial.o
del serial.s
del a.out
set arduinopath=C:\Program Files (x86)\Arduino
rem set arduinopath=C:\Program Files\Arduino
set comPort=COM21
"%arduinopath%\hardware\tools\avr\bin\avr-g++" -help
"%arduinopath%\hardware\tools\avr\bin\avr-g++" -Os -mmcu=attiny24 -save-temps -fverbose-asm -I"%arduinopath%\hardware\arduino\cores\arduino" serial.cpp
"%arduinopath%\hardware\tools\avr\bin\avr-g++" -Os -mmcu=attiny24                           -I"%arduinopath%\hardware\arduino\cores\arduino" serial.cpp -o serial.o
"%arduinopath%\hardware\tools\avr\bin\avr-objcopy" -O ihex serial.o serial.hex
"%arduinopath%\hardware\tools\avr\bin\avrdude" -C"%arduinopath%\hardware\tools\avr\etc\avrdude.conf" -pattiny24 -cstk500v1 -P\\.\%comPort% -b19200 -Uflash:w:serial.hex:i 
pause
