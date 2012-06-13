#!/bin/bash 

xbash -q -c "cd ../; /usr/bin/make -f ml505_bsb_system.make program; exit;"

xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../TestApp_Memory_microblaze_0/executable.elf -ace ml505_std_ip_testapp_mem.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/bootload_video.elf -ace bootload_video.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/button_led_test.elf -ace button_led_test.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/colorbar.elf -ace colorbar.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/flash_hello.elf -ace flash_hello.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/hello.elf -ace hello.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/hello_uart.elf -ace hello_uart.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/hello_uart_1.elf -ace hello_uart_1.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/iic_clock.elf -ace iic_clock.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/iic_ddr2.elf -ace iic_ddr2.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/iic_eeprom.elf -ace iic_eeprom.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/iic_fan.elf -ace iic_fan.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/iic_sfp.elf -ace iic_sfp.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/my_ace.elf -ace my_ace.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/my_plat_flash.elf -ace my_plat_flash.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/piezo.elf -ace piezo.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/ps2_scancodes_polled.elf -ace ps2_scancodes_polled.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/simon.elf -ace simon.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/sysace_rebooter.elf -ace sysace_rebooter.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/testfatfs.elf -ace testfatfs.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/xflash.elf -ace xflash.ace -board ml505 -target mdm
xmd -tcl ../data/genace.tcl -hw ../implementation/download.bit -elf ../microblaze_0/code/xrom.elf -ace xrom.ace -board ml505 -target mdm


rm -rf ML50X
mkdir ML50X
cd ML50X
mkdir cfg0 cfg1 cfg2 cfg3 cfg4 cfg5 cfg6 cfg7
cd ..

cp -p bootload_video.ace ML50X/cfg0

cp -p ml505_std_ip_testapp_mem.ace ML50X/cfg1
cp -p button_led_test.ace ML50X/cfg2
cp -p colorbar.ace ML50X/cfg3
cp -p flash_hello.ace ML50X/cfg4
cp -p hello.ace ML50X/cfg5
cp -p hello_uart.ace ML50X/cfg6
cp -p hello_uart_1.ace ML50X/cfg7
cp -p iic_clock.ace ML50X/cfg1
cp -p iic_ddr2.ace ML50X/cfg2
cp -p iic_eeprom.ace ML50X/cfg3
cp -p iic_fan.ace ML50X/cfg4
cp -p iic_sfp.ace ML50X/cfg5
cp -p my_ace.ace ML50X/cfg7
cp -p my_plat_flash.ace ML50X/cfg1
cp -p piezo.ace ML50X/cfg2
cp -p ps2_scancodes_polled.ace ML50X/cfg3
cp -p simon.ace ML50X/cfg4
cp -p sysace_rebooter.ace ML50X/cfg5
cp -p testfatfs.ace ML50X/cfg6
cp -p xflash.ace ML50X/cfg7
cp -p xrom.ace ML50X/cfg1

cp -p ../implementation/download.bit ../implementation/ml505_std_ip_bootloop.bit

rm *.svf 
rm ../implementation/*.svf 
rm ../TestApp_Memory_microblaze_0/*.svf 
rm ../microblaze_0/code/*.svf 
 
