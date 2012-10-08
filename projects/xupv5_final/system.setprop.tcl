cd H:/studium/ies-project/projects/xupv5_final
if { [ catch { xload xmp system.xmp } result ] } {
  exit 10
}
if { [xset hier sub] != 0 } {
  exit -1
}
set bMisMatch false
set xpsArch [xget arch]
if { ! [ string equal -nocase $xpsArch "virtex5" ] } {
   set bMisMatch true
}
set xpsDev [xget dev]
if { ! [ string equal -nocase $xpsDev "xc5vlx110t" ] } {
   set bMisMatch true
}
set xpsPkg [xget package]
if { ! [ string equal -nocase $xpsPkg "ff1136" ] } {
   set bMisMatch true
}
set xpsSpd [xget speedgrade]
if { ! [ string equal -nocase $xpsSpd "-1" ] } {
   set bMisMatch true
}
if { $bMisMatch == true } {
   puts "Settings Mismatch:"
   puts "Current Project:"
   puts "	Family: virtex5"
   puts "	Device: xc5vlx110t"
   puts "	Package: ff1136"
   puts "	Speed: -1"
   puts "XPS File: "
   puts "	Family: $xpsArch"
   puts "	Device: $xpsDev"
   puts "	Package: $xpsPkg"
   puts "	Speed: $xpsSpd"
   exit 11
}
#default language
xset hdl vhdl
xset intstyle ise
save proj
exit
