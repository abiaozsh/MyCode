del LED10.hex
del LED10.ii
del LED10.o
del LED10.s
del a.out
set arduinopath=C:\Program Files (x86)\Arduino
rem "%arduinopath%\hardware\tools\avr\bin\avr-g++" -Os                                                                 -mmcu=atmega32u4 -save-temps -fverbose-asm                                              -I"%arduinopath%\hardware\arduino\cores\arduino" LED10.cpp
    "%arduinopath%\hardware\tools\avr\bin\avr-g++" -c -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega32u4 -DF_CPU=16000000L -MMD -DUSB_VID=0x2341 -DUSB_PID=0x8036 -DARDUINO=105 -I"%arduinopath%\hardware\arduino\cores\arduino" LED10.cpp
rem "%arduinopath%\hardware\tools\avr\bin\avr-g++" -Os -mmcu=atmega32u4                           -I"%arduinopath%\hardware\arduino\cores\arduino" motor6.cpp -o motor6.o

rem "%arduinopath%\hardware\tools\avr\bin\avr-objcopy" -O ihex LED10.o LED10.hex
    "%arduinopath%\hardware\tools\avr\bin\avr-gcc" -Os -Wl,--gc-sections -mmcu=atmega32u4 -o LED10.elf LED10.o -lm 
    "%arduinopath%\hardware\tools\avr\bin\avr-objcopy" -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 LED10.elf LED10.eep 
    "%arduinopath%\hardware\tools\avr\bin\avr-objcopy" -O ihex -R .eeprom LED10.elf LED10.hex 

rem "%arduinopath%\hardware\tools\avr\bin\avrdude" -C"%arduinopath%\hardware\tools\avr\etc\avrdude.conf" -pattiny24 -cstk500v1 -P\\.\COM11 -b19200 -Uflash:w:LED10.hex:i 
    "%arduinopath%\hardware\tools\avr\bin\avrdude" -C"%arduinopath%\hardware\tools\avr\etc\avrdude.conf" -patmega32u4 -cavr109 -P\\.\COM4 -b57600 -D -Uflash:w:LED10.hex:i 
pause
