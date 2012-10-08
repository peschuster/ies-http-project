
-- VHDL Instantiation Created from source file system.vhd -- 17:15:17 08/16/2012
--
-- Notes: 
-- 1) This instantiation template has been automatically generated using types
-- std_logic and std_logic_vector for the ports of the instantiated module
-- 2) To use this template to instantiate this entity, cut-and-paste and then edit

	COMPONENT system
	PORT(
		fpga_0_RS232_Uart_1_RX_pin : IN std_logic;
		fpga_0_Hard_Ethernet_MAC_MII_TX_CLK_0_pin : IN std_logic;
		fpga_0_Hard_Ethernet_MAC_GMII_RXD_0_pin : IN std_logic_vector(7 downto 0);
		fpga_0_Hard_Ethernet_MAC_GMII_RX_DV_0_pin : IN std_logic;
		fpga_0_Hard_Ethernet_MAC_GMII_RX_ER_0_pin : IN std_logic;
		fpga_0_Hard_Ethernet_MAC_GMII_RX_CLK_0_pin : IN std_logic;
		fpga_0_Hard_Ethernet_MAC_PHY_MII_INT_pin : IN std_logic;
		fpga_0_clk_1_sys_clk_pin : IN std_logic;
		fpga_0_rst_1_sys_rst_pin : IN std_logic;    
		fpga_0_LEDs_8Bit_GPIO_IO_pin : INOUT std_logic_vector(0 to 7);
		fpga_0_LEDs_Positions_GPIO_IO_pin : INOUT std_logic_vector(0 to 4);
		fpga_0_Push_Buttons_5Bit_GPIO_IO_pin : INOUT std_logic_vector(0 to 4);
		fpga_0_DIP_Switches_8Bit_GPIO_IO_pin : INOUT std_logic_vector(0 to 7);
		fpga_0_IIC_EEPROM_Sda_pin : INOUT std_logic;
		fpga_0_IIC_EEPROM_Scl_pin : INOUT std_logic;
		fpga_0_DDR2_SDRAM_DDR2_DQ_pin : INOUT std_logic_vector(63 downto 0);
		fpga_0_DDR2_SDRAM_DDR2_DQS_pin : INOUT std_logic_vector(7 downto 0);
		fpga_0_DDR2_SDRAM_DDR2_DQS_n_pin : INOUT std_logic_vector(7 downto 0);
		fpga_0_Hard_Ethernet_MAC_MDIO_0_pin : INOUT std_logic;      
		fpga_0_RS232_Uart_1_TX_pin : OUT std_logic;
		fpga_0_DDR2_SDRAM_DDR2_Clk_pin : OUT std_logic_vector(1 downto 0);
		fpga_0_DDR2_SDRAM_DDR2_Clk_n_pin : OUT std_logic_vector(1 downto 0);
		fpga_0_DDR2_SDRAM_DDR2_CE_pin : OUT std_logic_vector(1 downto 0);
		fpga_0_DDR2_SDRAM_DDR2_CS_n_pin : OUT std_logic_vector(1 downto 0);
		fpga_0_DDR2_SDRAM_DDR2_ODT_pin : OUT std_logic_vector(1 downto 0);
		fpga_0_DDR2_SDRAM_DDR2_RAS_n_pin : OUT std_logic;
		fpga_0_DDR2_SDRAM_DDR2_CAS_n_pin : OUT std_logic;
		fpga_0_DDR2_SDRAM_DDR2_WE_n_pin : OUT std_logic;
		fpga_0_DDR2_SDRAM_DDR2_BankAddr_pin : OUT std_logic_vector(1 downto 0);
		fpga_0_DDR2_SDRAM_DDR2_Addr_pin : OUT std_logic_vector(12 downto 0);
		fpga_0_DDR2_SDRAM_DDR2_DM_pin : OUT std_logic_vector(7 downto 0);
		fpga_0_Hard_Ethernet_MAC_TemacPhy_RST_n_pin : OUT std_logic;
		fpga_0_Hard_Ethernet_MAC_GMII_TXD_0_pin : OUT std_logic_vector(7 downto 0);
		fpga_0_Hard_Ethernet_MAC_GMII_TX_EN_0_pin : OUT std_logic;
		fpga_0_Hard_Ethernet_MAC_GMII_TX_ER_0_pin : OUT std_logic;
		fpga_0_Hard_Ethernet_MAC_GMII_TX_CLK_0_pin : OUT std_logic;
		fpga_0_Hard_Ethernet_MAC_MDC_0_pin : OUT std_logic
		);
	END COMPONENT;

attribute box_type : string;
attribute box_type of system : component is "user_black_box";


	Inst_system: system PORT MAP(
		fpga_0_RS232_Uart_1_RX_pin => ,
		fpga_0_RS232_Uart_1_TX_pin => ,
		fpga_0_LEDs_8Bit_GPIO_IO_pin => ,
		fpga_0_LEDs_Positions_GPIO_IO_pin => ,
		fpga_0_Push_Buttons_5Bit_GPIO_IO_pin => ,
		fpga_0_DIP_Switches_8Bit_GPIO_IO_pin => ,
		fpga_0_IIC_EEPROM_Sda_pin => ,
		fpga_0_IIC_EEPROM_Scl_pin => ,
		fpga_0_DDR2_SDRAM_DDR2_Clk_pin => ,
		fpga_0_DDR2_SDRAM_DDR2_Clk_n_pin => ,
		fpga_0_DDR2_SDRAM_DDR2_CE_pin => ,
		fpga_0_DDR2_SDRAM_DDR2_CS_n_pin => ,
		fpga_0_DDR2_SDRAM_DDR2_ODT_pin => ,
		fpga_0_DDR2_SDRAM_DDR2_RAS_n_pin => ,
		fpga_0_DDR2_SDRAM_DDR2_CAS_n_pin => ,
		fpga_0_DDR2_SDRAM_DDR2_WE_n_pin => ,
		fpga_0_DDR2_SDRAM_DDR2_BankAddr_pin => ,
		fpga_0_DDR2_SDRAM_DDR2_Addr_pin => ,
		fpga_0_DDR2_SDRAM_DDR2_DQ_pin => ,
		fpga_0_DDR2_SDRAM_DDR2_DM_pin => ,
		fpga_0_DDR2_SDRAM_DDR2_DQS_pin => ,
		fpga_0_DDR2_SDRAM_DDR2_DQS_n_pin => ,
		fpga_0_Hard_Ethernet_MAC_TemacPhy_RST_n_pin => ,
		fpga_0_Hard_Ethernet_MAC_MII_TX_CLK_0_pin => ,
		fpga_0_Hard_Ethernet_MAC_GMII_TXD_0_pin => ,
		fpga_0_Hard_Ethernet_MAC_GMII_TX_EN_0_pin => ,
		fpga_0_Hard_Ethernet_MAC_GMII_TX_ER_0_pin => ,
		fpga_0_Hard_Ethernet_MAC_GMII_TX_CLK_0_pin => ,
		fpga_0_Hard_Ethernet_MAC_GMII_RXD_0_pin => ,
		fpga_0_Hard_Ethernet_MAC_GMII_RX_DV_0_pin => ,
		fpga_0_Hard_Ethernet_MAC_GMII_RX_ER_0_pin => ,
		fpga_0_Hard_Ethernet_MAC_GMII_RX_CLK_0_pin => ,
		fpga_0_Hard_Ethernet_MAC_MDC_0_pin => ,
		fpga_0_Hard_Ethernet_MAC_MDIO_0_pin => ,
		fpga_0_Hard_Ethernet_MAC_PHY_MII_INT_pin => ,
		fpga_0_clk_1_sys_clk_pin => ,
		fpga_0_rst_1_sys_rst_pin => 
	);



