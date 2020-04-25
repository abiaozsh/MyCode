# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "STATUS_FINISH" -parent ${Page_0}
  ipgui::add_param $IPINST -name "STATUS_INIT" -parent ${Page_0}
  ipgui::add_param $IPINST -name "STATUS_WAIT" -parent ${Page_0}


}

proc update_PARAM_VALUE.STATUS_FINISH { PARAM_VALUE.STATUS_FINISH } {
	# Procedure called to update STATUS_FINISH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.STATUS_FINISH { PARAM_VALUE.STATUS_FINISH } {
	# Procedure called to validate STATUS_FINISH
	return true
}

proc update_PARAM_VALUE.STATUS_INIT { PARAM_VALUE.STATUS_INIT } {
	# Procedure called to update STATUS_INIT when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.STATUS_INIT { PARAM_VALUE.STATUS_INIT } {
	# Procedure called to validate STATUS_INIT
	return true
}

proc update_PARAM_VALUE.STATUS_WAIT { PARAM_VALUE.STATUS_WAIT } {
	# Procedure called to update STATUS_WAIT when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.STATUS_WAIT { PARAM_VALUE.STATUS_WAIT } {
	# Procedure called to validate STATUS_WAIT
	return true
}


proc update_MODELPARAM_VALUE.STATUS_INIT { MODELPARAM_VALUE.STATUS_INIT PARAM_VALUE.STATUS_INIT } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.STATUS_INIT}] ${MODELPARAM_VALUE.STATUS_INIT}
}

proc update_MODELPARAM_VALUE.STATUS_WAIT { MODELPARAM_VALUE.STATUS_WAIT PARAM_VALUE.STATUS_WAIT } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.STATUS_WAIT}] ${MODELPARAM_VALUE.STATUS_WAIT}
}

proc update_MODELPARAM_VALUE.STATUS_FINISH { MODELPARAM_VALUE.STATUS_FINISH PARAM_VALUE.STATUS_FINISH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.STATUS_FINISH}] ${MODELPARAM_VALUE.STATUS_FINISH}
}

