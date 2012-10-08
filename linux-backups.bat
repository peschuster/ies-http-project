@ECHO off
pushd %~dp0

SET name=%DATE:~6,6%%DATE:~3,2%%DATE:~0,2%_%TIME:~0,2%%TIME:~3,2%%TIME:~6,2%
SET tdir=linux-backups\%name%

MKDIR "%tdir%"

copy sdk-linux\linux_dts\microblaze_0\libsrc\device-tree_v0_00_x\xilinx.dts "%tdir%"
copy projects\ml505_linux\system.mhs "%tdir%"
copy projects\ml505_linux\data\system.ucf "%tdir%"
copy projects\ml505_linux\implementation\system.bit "%tdir%"

echo %date% %time% > "%tdir%\changes.txt"
echo. >> "%tdir%\changes.txt"
echo. >> "%tdir%\changes.txt"
echo %* >> "%tdir%\changes.txt"

notepad "%tdir%\changes.txt"

popd