#!/bin/sh
#
# This file was automatically generated.
#
# It can be overwritten by nios2-flash-programmer-generate or nios2-flash-programmer-gui.
#

#
# Converting SOF File: E:\MyCode.github\fpga\qsys_sdram\output_files\aaa.sof to: "..\flash/aaa_epcs.flash"
#
sof2flash --input="E:/MyCode.github/fpga/qsys_sdram/output_files/aaa.sof" --output="../flash/aaa_epcs.flash" --epcs --verbose 

#
# Programming File: "..\flash/aaa_epcs.flash" To Device: epcs
#
nios2-flash-programmer "../flash/aaa_epcs.flash" --base=0x4001000 --epcs --sidp=0x4002060 --id=0x0 --accept-bad-sysid --device=1 --instance=0 '--cable=USB-Blaster on localhost [USB-0]' --program --verbose 

#
# Converting ELF File: E:\MyCode.github\fpga\workspace\epcs\epcs.elf to: "..\flash/epcs_epcs.flash"
#
elf2flash --input="E:/MyCode.github/fpga/workspace/epcs/epcs.elf" --output="../flash/epcs_epcs.flash" --epcs --after="../flash/aaa_epcs.flash" --verbose 

#
# Programming File: "..\flash/epcs_epcs.flash" To Device: epcs
#
nios2-flash-programmer "../flash/epcs_epcs.flash" --base=0x4001000 --epcs --sidp=0x4002060 --id=0x0 --accept-bad-sysid --device=1 --instance=0 '--cable=USB-Blaster on localhost [USB-0]' --program --verbose 

