# TCL File Generated by Component Editor 13.0sp1
# Fri Feb 28 21:39:44 CST 2020
# DO NOT MODIFY


# 
# mysdram "mysdram" v1.0
#  2020.02.28.21:39:44
# 
# 

# 
# request TCL package from ACDS 13.1
# 
package require -exact qsys 13.1


# 
# module mysdram
# 
set_module_property DESCRIPTION ""
set_module_property NAME mysdram
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property GROUP "Memories and Memory Controllers/External Memory Interfaces"
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME mysdram
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property ANALYZE_HDL AUTO
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL mysdram
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
add_fileset_file mysdram.v VERILOG PATH mysdram.v TOP_LEVEL_FILE
add_fileset_file sramtemp.qip OTHER PATH sramtemp.qip
add_fileset_file uart_hs.v VERILOG PATH ../commoncode/uart_hs.v
add_fileset_file uart_recv_hs.v VERILOG PATH ../commoncode/uart_recv_hs.v
add_fileset_file uart_send_hs.v VERILOG PATH ../commoncode/uart_send_hs.v


# 
# parameters
# 


# 
# display items
# 


# 
# connection point s0
# 
add_interface s0 avalon end
set_interface_property s0 addressUnits WORDS
set_interface_property s0 associatedClock clock
set_interface_property s0 associatedReset reset
set_interface_property s0 bitsPerSymbol 8
set_interface_property s0 burstOnBurstBoundariesOnly false
set_interface_property s0 burstcountUnits WORDS
set_interface_property s0 explicitAddressSpan 0
set_interface_property s0 holdTime 0
set_interface_property s0 isMemoryDevice true
set_interface_property s0 linewrapBursts false
set_interface_property s0 maximumPendingReadTransactions 0
set_interface_property s0 readLatency 1
set_interface_property s0 readWaitTime 1
set_interface_property s0 setupTime 0
set_interface_property s0 timingUnits Cycles
set_interface_property s0 writeWaitTime 0
set_interface_property s0 ENABLED true
set_interface_property s0 EXPORT_OF ""
set_interface_property s0 PORT_NAME_MAP ""
set_interface_property s0 SVD_ADDRESS_GROUP ""

add_interface_port s0 avs_s0_read read Input 1
add_interface_port s0 avs_s0_readdata readdata Output 16
add_interface_port s0 avs_s0_write write Input 1
add_interface_port s0 avs_s0_writedata writedata Input 16
add_interface_port s0 avs_s0_waitrequest waitrequest Output 1
add_interface_port s0 avs_s0_address address Input 24
set_interface_assignment s0 embeddedsw.configuration.isFlash 0
set_interface_assignment s0 embeddedsw.configuration.isMemoryDevice 1
set_interface_assignment s0 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment s0 embeddedsw.configuration.isPrintableDevice 0


# 
# connection point clock
# 
add_interface clock clock end
set_interface_property clock clockRate 0
set_interface_property clock ENABLED true
set_interface_property clock EXPORT_OF ""
set_interface_property clock PORT_NAME_MAP ""
set_interface_property clock SVD_ADDRESS_GROUP ""

add_interface_port clock clk clk Input 1


# 
# connection point reset
# 
add_interface reset reset end
set_interface_property reset associatedClock clock
set_interface_property reset synchronousEdges DEASSERT
set_interface_property reset ENABLED true
set_interface_property reset EXPORT_OF ""
set_interface_property reset PORT_NAME_MAP ""
set_interface_property reset SVD_ADDRESS_GROUP ""

add_interface_port reset reset_n reset_n Input 1


# 
# connection point reset_source
# 
add_interface reset_source reset start
set_interface_property reset_source associatedClock clock
set_interface_property reset_source associatedDirectReset ""
set_interface_property reset_source associatedResetSinks ""
set_interface_property reset_source synchronousEdges DEASSERT
set_interface_property reset_source ENABLED true
set_interface_property reset_source EXPORT_OF ""
set_interface_property reset_source PORT_NAME_MAP ""
set_interface_property reset_source SVD_ADDRESS_GROUP ""

add_interface_port reset_source reset_n_out reset_n Output 1


# 
# connection point conduit_end
# 
add_interface conduit_end conduit end
set_interface_property conduit_end associatedClock clock
set_interface_property conduit_end associatedReset ""
set_interface_property conduit_end ENABLED true
set_interface_property conduit_end EXPORT_OF ""
set_interface_property conduit_end PORT_NAME_MAP ""
set_interface_property conduit_end SVD_ADDRESS_GROUP ""

add_interface_port conduit_end uart_rxd export Input 1
add_interface_port conduit_end uart_txd export Output 1
add_interface_port conduit_end debug export Output 8
add_interface_port conduit_end debug2 export Output 16
