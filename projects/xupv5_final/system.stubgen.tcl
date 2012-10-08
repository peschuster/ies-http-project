cd H:/studium/ies-project/projects/xupv5_final/
if { [ catch { xload xmp system.xmp } result ] } {
  exit 10
}
xset hdl vhdl
run stubgen
exit 0
