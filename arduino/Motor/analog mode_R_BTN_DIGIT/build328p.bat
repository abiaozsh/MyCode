rem @echo off
if exist "C:\Program Files (x86)\Arduino" set arduinopath=C:\Program Files (x86)\Arduino
if exist "C:\Program Files\Arduino" set arduinopath=C:\Program Files\Arduino
set comPort=COM9
set filename=serial
set comptype=g++
set extptype=c

del %filename%.hex
del %filename%.i
del %filename%.o
del %filename%.s
del a.out
"%arduinopath%\hardware\tools\avr\bin\avr-%comptype%" -Os -mmcu=atmega328p -save-temps -fverbose-asm -I"%arduinopath%\hardware\arduino\cores\arduino" %filename%.cpp
"%arduinopath%\hardware\tools\avr\bin\avr-%comptype%" -Os -mmcu=atmega328p                           -I"%arduinopath%\hardware\arduino\cores\arduino" %filename%.cpp -o %filename%.o
"%arduinopath%\hardware\tools\avr\bin\avr-objcopy" -O ihex %filename%.o %filename%.hex
"%arduinopath%\hardware\tools\avr\bin\avr-size" %filename%.hex
pause
"%arduinopath%\hardware\tools\avr\bin\avrdude" -C"%arduinopath%\hardware\tools\avr\etc\avrdude.conf" -patmega328p -carduino -P\\.\%comPort% -b57600 -D -Uflash:w:%filename%.hex:i 
pause
