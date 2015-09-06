del serial.hex
del serial.ii
del serial.o
del serial.s
del a.out
rem set arduinopath=C:\Program Files (x86)\Arduino
set arduinopath=C:\Program Files\Arduino
set comPort=COM5
"%arduinopath%\hardware\tools\avr\bin\avr-g++" -Os -mmcu=atmega328p -save-temps -fverbose-asm -I"%arduinopath%\hardware\arduino\cores\arduino" serial.cpp
"%arduinopath%\hardware\tools\avr\bin\avr-g++" -Os -mmcu=atmega328p                           -I"%arduinopath%\hardware\arduino\cores\arduino" serial.cpp -o serial.o
"%arduinopath%\hardware\tools\avr\bin\avr-objcopy" -O ihex serial.o serial.hex
pause
"%arduinopath%\hardware\tools\avr\bin\avrdude" -C"%arduinopath%\hardware\tools\avr\etc\avrdude.conf" -patmega328p -carduino -P\\.\%comPort% -b57600 -D -Uflash:w:serial.hex:i 
pause
