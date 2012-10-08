cd H:/studium/ies-project/projects/xupv5_final
if { [ catch { xload xmp system.xmp } result ] } {
  exit 10
}

if { [catch {run init_bram} result] } {
  exit -1
}

exit 0
