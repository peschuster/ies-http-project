proc pnsynth {} {
  cd C:/developuni/ies_project/projects/xupv5_final
  if { [ catch { xload xmp system.xmp } result ] } {
    exit 10
  }
  if { [catch {run netlist} result] } {
    return -1
  }
  return $result
}
if { [catch {pnsynth} result] } {
  exit -1
}
exit $result
