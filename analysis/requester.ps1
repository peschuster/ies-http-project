$t = 1000000000
for($i = 0; $i -lt $t; $i++) {
    $w = New-Object System.Net.WebClient
    $w.DownloadData("http://192.168.2.125/10K.html") | Out-Null
    write-progress -id 1 -activity "Downloading" -status "$i" -percentComplete ($i/$t); 
}