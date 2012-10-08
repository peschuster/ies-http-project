proc exportToSDK {} {
  cd C:/developuni/ies_project/projects/xupv5_final
  if { [ catch { xload xmp system.xmp } result ] } {
    exit 10
  }
  if { [run exporttosdk] != 0 } {
    return -1
  }
  return 0
}

if { [catch {exportToSDK} result] } {
  exit -1
}

set sExportDir [ xget sdk_export_dir ]
set sExportDir [ file join "C:/developuni/ies_project/projects/xupv5_final" "$sExportDir" "hw" ] 
if { [ file exists C:/developuni/ies_project/projects/xupv5_final_project/xupv5_final/edkBmmFile_bd.bmm ] } {
   puts "Copying placed bmm file C:/developuni/ies_project/projects/xupv5_final_project/xupv5_final/edkBmmFile_bd.bmm to $sExportDir" 
   file copy -force "C:/developuni/ies_project/projects/xupv5_final_project/xupv5_final/edkBmmFile_bd.bmm" $sExportDir
}
if { [ file exists C:/developuni/ies_project/projects/xupv5_final_project/xupv5_final/system_top.bit ] } {
   puts "Copying bit file C:/developuni/ies_project/projects/xupv5_final_project/xupv5_final/system_top.bit to $sExportDir" 
   file copy -force "C:/developuni/ies_project/projects/xupv5_final_project/xupv5_final/system_top.bit" $sExportDir
}
exit $result
