$info = get-content C:\developuni\ies_project\xmd\syslog_neu12.txt
foreach ($i in $info) {
   $item = $i.split(":")
   if ($item[1])
   {
   $in = $item[1].Trim()
   $i0 = [convert]::toint16($in.Substring(0,2),16)
   $i1 = [convert]::toint16($in.Substring(2,2),16)
   $i2 = [convert]::toint16($in.Substring(4,2),16)
   $i3 = [convert]::toint16($in.Substring(6,2),16)
   
   if ($in0 = 0) { break }
   
   Write-Host -NoNewline ([CHAR][BYTE]$i0)
   Write-Host -NoNewline ([CHAR][BYTE]$i1)
   Write-Host -NoNewline ([CHAR][BYTE]$i2)
   Write-Host -NoNewline ([CHAR][BYTE]$i3)
   }
}