set arduinopath=C:\Program Files (x86)\Arduino
rem set arduinopath=C:\Program Files\Arduino
set comPort=COM4
set filename=motor6

del %filename%.hex
del %filename%.ii
del %filename%.o
del %filename%.s
del a.out
"%arduinopath%\hardware\tools\avr\bin\avr-gcc" -Os -mmcu=attiny24 -fno-inline -save-temps -fverbose-asm -I"%arduinopath%\hardware\arduino\cores\arduino" %filename%.c
"%arduinopath%\hardware\tools\avr\bin\avr-gcc" -Os -mmcu=attiny24 -fno-inline                           -I"%arduinopath%\hardware\arduino\cores\arduino" %filename%.c -o motor6.o
"%arduinopath%\hardware\tools\avr\bin\avr-objcopy" -O ihex %filename%.o %filename%.hex
"%arduinopath%\hardware\tools\avr\bin\avrdude" -C"%arduinopath%\hardware\tools\avr\etc\avrdude.conf" -pattiny24 -cstk500v1 -P\\.\%comPort% -b19200 -Uflash:w:%filename%.hex:i 
"%arduinopath%\hardware\tools\avr\bin\avr-size" %filename%.hex
pause
