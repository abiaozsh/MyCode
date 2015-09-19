@echo off
set arduinopath=C:\Program Files (x86)\Arduino
rem set arduinopath=C:\Program Files\Arduino
set comPort=COM16
set filename=lamp
set comptype=gcc
set extptype=c

del %filename%.hex
del %filename%.i
del %filename%.o
del %filename%.s
del a.out
"%arduinopath%\hardware\tools\avr\bin\avr-%comptype%" -Os -fno-inline -mmcu=attiny24 -save-temps -fverbose-asm -I"%arduinopath%\hardware\arduino\cores\arduino" %filename%.%extptype%
"%arduinopath%\hardware\tools\avr\bin\avr-%comptype%" -Os -fno-inline -mmcu=attiny24                           -I"%arduinopath%\hardware\arduino\cores\arduino" %filename%.%extptype% -o %filename%.o
"%arduinopath%\hardware\tools\avr\bin\avr-objcopy" -O ihex %filename%.o %filename%.hex
rem "%arduinopath%\hardware\tools\avr\bin\avrdude" -C"%arduinopath%\hardware\tools\avr\etc\avrdude.conf" -pattiny24 -cstk500v1 -P\\.\%comPort% -b19200 -Uflash:w:%filename%.hex:i 
"%arduinopath%\Uploader.exe" %comPort% %filename%.hex
"%arduinopath%\hardware\tools\avr\bin\avr-size" %filename%.hex
pause
