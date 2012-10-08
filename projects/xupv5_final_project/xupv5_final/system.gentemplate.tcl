proc pngeninsttemplate {} {
  cd H:/studium/ies-project/projects/xupv5_final
  if { [ catch { xload xmp system.xmp } result ] } {
    exit 10
  }
  if { [catch {run mhs2hdl} result] } {
    return -1
  }
  return 0
}
if { [catch {pngeninsttemplate} result] } {
  exit -1
}
exit $result
