set arduinopath=C:\Program Files (x86)\Arduino
rem set arduinopath=C:\Program Files\Arduino
set comPort=COM11
set filename=serial

del %filename%.hex
del %filename%.ii
del %filename%.o
del %filename%.s
del a.out
"%arduinopath%\hardware\tools\avr\bin\avr-g++" -Os -mmcu=atmega328p -save-temps -fverbose-asm -I"%arduinopath%\hardware\arduino\cores\arduino" serial.cpp
"%arduinopath%\hardware\tools\avr\bin\avr-g++" -Os -mmcu=atmega328p                           -I"%arduinopath%\hardware\arduino\cores\arduino" serial.cpp -o serial.o
"%arduinopath%\hardware\tools\avr\bin\avr-objcopy" -O ihex serial.o serial.hex
"%arduinopath%\hardware\tools\avr\bin\avr-size" serial.hex
pause
"%arduinopath%\hardware\tools\avr\bin\avrdude" -C"%arduinopath%\hardware\tools\avr\etc\avrdude.conf" -patmega328p -carduino -P\\.\%comPort% -b57600 -D -Uflash:w:serial.hex:i 
pause
