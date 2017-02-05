@echo off
if exist "C:\Program Files (x86)\Arduino" set arduinopath=C:\Program Files (x86)\Arduino
if exist "C:\Program Files\Arduino" set arduinopath=C:\Program Files\Arduino
set filename=serial
del %filename%.hex
del %filename%.ii
del %filename%.o
del %filename%.s
del a.out
set comPort=COM15
"%arduinopath%\hardware\tools\avr\bin\avr-g++" -Os -mmcu=atmega328p -save-temps -fverbose-asm -I"%arduinopath%\hardware\arduino\cores\arduino" %filename%.cpp
"%arduinopath%\hardware\tools\avr\bin\avr-g++" -Os -mmcu=atmega328p                           -I"%arduinopath%\hardware\arduino\cores\arduino" %filename%.cpp -o %filename%.o
"%arduinopath%\hardware\tools\avr\bin\avr-objcopy" -O ihex %filename%.o %filename%.hex
"%arduinopath%\hardware\tools\avr\bin\avr-size" %filename%.hex
pause
"%arduinopath%\hardware\tools\avr\bin\avrdude" -C"%arduinopath%\hardware\tools\avr\etc\avrdude.conf" -patmega328p -carduino -P\\.\%comPort% -b57600 -D -Uflash:w:%filename%.hex:i 
pause
