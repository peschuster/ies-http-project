
 PARAMETER VERSION = 2.2.0


BEGIN OS
 PARAMETER OS_NAME = uboot
 PARAMETER OS_VER = 4.00.c
 PARAMETER PROC_INSTANCE = microblaze_0
 PARAMETER ETHERNET = Hard_Ethernet_MAC
 PARAMETER TIMER = xps_timer_0
 PARAMETER STDOUT = RS232_Uart_1
 PARAMETER STDIN = RS232_Uart_1
 PARAMETER BOARD_NAME = ml505
 PARAMETER FLASH_MEMORY = SRAM_FLASH
 PARAMETER FLASH_MEMORY_BANK = 1
 PARAMETER FLASH_MEMORY_START = 0x8c000000
 PARAMETER GPIO = LEDs_8Bit
 PARAMETER IIC = IIC_EEPROM
 PARAMETER SYSACE = SysACE_CompactFlash
 PARAMETER MAIN_MEMORY = DDR2_SDRAM
 PARAMETER MAIN_MEMORY_SIZE = 262144
 PARAMETER FLASH_MEMORY_SIZE = 32768
END


BEGIN PROCESSOR
 PARAMETER DRIVER_NAME = cpu
 PARAMETER DRIVER_VER = 1.13.a
 PARAMETER HW_INSTANCE = microblaze_0
END


BEGIN DRIVER
 PARAMETER DRIVER_NAME = mpmc
 PARAMETER DRIVER_VER = 4.01.a
 PARAMETER HW_INSTANCE = DDR2_SDRAM
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = gpio
 PARAMETER DRIVER_VER = 3.00.a
 PARAMETER HW_INSTANCE = DIP_Switches_8Bit
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = lltemac
 PARAMETER DRIVER_VER = 3.00.a
 PARAMETER HW_INSTANCE = Hard_Ethernet_MAC
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = iic
 PARAMETER DRIVER_VER = 2.04.a
 PARAMETER HW_INSTANCE = IIC_EEPROM
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = gpio
 PARAMETER DRIVER_VER = 3.00.a
 PARAMETER HW_INSTANCE = LCD
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = gpio
 PARAMETER DRIVER_VER = 3.00.a
 PARAMETER HW_INSTANCE = LEDs_8Bit
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = gpio
 PARAMETER DRIVER_VER = 3.00.a
 PARAMETER HW_INSTANCE = LEDs_Positions
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = gpio
 PARAMETER DRIVER_VER = 3.00.a
 PARAMETER HW_INSTANCE = Push_Buttons_5Bit
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = uartns550
 PARAMETER DRIVER_VER = 2.01.a
 PARAMETER HW_INSTANCE = RS232_Uart_1
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = uartns550
 PARAMETER DRIVER_VER = 2.01.a
 PARAMETER HW_INSTANCE = RS232_Uart_2
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = emc
 PARAMETER DRIVER_VER = 3.01.a
 PARAMETER HW_INSTANCE = SRAM_FLASH
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = sysace
 PARAMETER DRIVER_VER = 2.00.a
 PARAMETER HW_INSTANCE = SysACE_CompactFlash
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = bram
 PARAMETER DRIVER_VER = 3.01.a
 PARAMETER HW_INSTANCE = dlmb_cntlr
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = bram
 PARAMETER DRIVER_VER = 3.01.a
 PARAMETER HW_INSTANCE = ilmb_cntlr
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = uartlite
 PARAMETER DRIVER_VER = 2.00.a
 PARAMETER HW_INSTANCE = mdm_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = intc
 PARAMETER DRIVER_VER = 2.04.a
 PARAMETER HW_INSTANCE = xps_intc_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = tmrctr
 PARAMETER DRIVER_VER = 2.04.a
 PARAMETER HW_INSTANCE = xps_timer_0
END


