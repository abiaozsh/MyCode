@echo off
if exist "C:\Program Files (x86)\Arduino" set arduinopath=C:\Program Files (x86)\Arduino
if exist "C:\Program Files\Arduino" set arduinopath=C:\Program Files\Arduino
set comPort=COM11
set filename=motor6
set comptype=g++
set extptype=cpp

del %filename%.hex
del %filename%.i
del %filename%.o
del %filename%.s
del a.out
"%arduinopath%\hardware\tools\avr\bin\avr-%comptype%" -Os -fno-inline -mmcu=attiny24 -save-temps -fverbose-asm -I"%arduinopath%\hardware\arduino\cores\arduino" %filename%.%extptype%
"%arduinopath%\hardware\tools\avr\bin\avr-%comptype%" -Os -fno-inline -mmcu=attiny24                           -I"%arduinopath%\hardware\arduino\cores\arduino" %filename%.%extptype% -o %filename%.o
"%arduinopath%\hardware\tools\avr\bin\avr-objcopy" -O ihex %filename%.o %filename%.hex
rem "%arduinopath%\hardware\tools\avr\bin\avrdude" -C"%arduinopath%\hardware\tools\avr\etc\avrdude.conf" -pattiny24 -cstk500v1 -P\\.\%comPort% -b19200 -Uflash:w:%filename%.hex:i 
"Uploader.exe" %comPort% %filename%.hex true
"%arduinopath%\hardware\tools\avr\bin\avr-size" %filename%.hex
pause
