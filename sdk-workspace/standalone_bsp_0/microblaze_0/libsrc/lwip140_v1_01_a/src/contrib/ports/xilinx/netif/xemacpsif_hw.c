/*
 * Copyright (c) 2010 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "netif/xemacpsif.h"
#include "lwipopts.h"

/*** IMPORTANT: Define PEEP in xemacpsif.h and sys_arch_raw.c
 *** to run it on a PEEP board
 ***/

void emacps_recv_handler(void *arg);
void emacps_send_handler(void *arg);
void emacps_error_handler(void *arg, u8 Direction, u32 ErrorWord);
unsigned int link_speed = 100;

XEmacPs_Config *
lookup_config(unsigned mac_base)
{
	extern XEmacPs_Config XEmacPs_ConfigTable[];
	XEmacPs_Config *CfgPtr = NULL;
	int i;

	for (i = 0; i < XPAR_XEMACPS_NUM_INSTANCES; i++) {
		if (XEmacPs_ConfigTable[i].BaseAddress == mac_base) {
			CfgPtr = &XEmacPs_ConfigTable[i];
			break;
		}
	}

	return (CfgPtr);
}

void
init_emacps(xemacpsif_s *xemacps, struct netif *netif)
{
	int rdy;
	unsigned mac_address = (unsigned)(netif->state);
	unsigned options;
        unsigned lock_message_printed = 0;
	XEmacPs *xemacpsp;
	XEmacPs_Config *mac_config;
	int Status = XST_SUCCESS;

	/* obtain config of this emac */
	mac_config = lookup_config(mac_address);

	xemacpsp = &xemacps->emacps;

	/* set mac address */
	Status = XEmacPs_SetMacAddress(xemacpsp, (void*)(netif->hwaddr), 1);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s:Emac Mac Address set failed...\r\n",__func__);
	}
	XEmacPs_SetMdioDivisor(xemacpsp, MDC_DIV_224);
	link_speed = Phy_Setup(xemacpsp);
    	XEmacPs_SetOperatingSpeed(xemacpsp, link_speed);
	sleep(1);

	/* Setting the operating speed of the MAC needs a delay. */
	{
		volatile int wait;
		for (wait=0; wait < 20000; wait++);
	}
}

void init_emacps_on_error (xemacpsif_s *xemacps, struct netif *netif)
{
	int rdy;
	unsigned mac_address = (unsigned)(netif->state);
	unsigned options;
	unsigned lock_message_printed = 0;
	XEmacPs *xemacpsp;
	XEmacPs_Config *mac_config;
	int Status = XST_SUCCESS;

	/* obtain config of this emac */
	mac_config = lookup_config(mac_address);

	xemacpsp = &xemacps->emacps;

	/* set mac address */
	Status = XEmacPs_SetMacAddress(xemacpsp, (void*)(netif->hwaddr), 1);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s:Emac Mac Address set failed...\r\n",__func__);
	}

	XEmacPs_SetOperatingSpeed(xemacpsp, link_speed);

	/* Setting the operating speed of the MAC needs a delay. */
	{
		volatile int wait;
		for (wait=0; wait < 20000; wait++);
	}
}

void setup_isr (struct xemac_s *xemac)
{
	xemacpsif_s   *xemacpsif;

	xemacpsif = (xemacpsif_s *)(xemac->state);
	/*
	 * Setup callbacks
	 */
	XEmacPs_SetHandler(&xemacpsif->emacps, XEMACPS_HANDLER_DMASEND,
				     (void *) emacps_send_handler,
				     (void *) xemac);

	XEmacPs_SetHandler(&xemacpsif->emacps, XEMACPS_HANDLER_DMARECV,
				    (void *) emacps_recv_handler,
				    (void *) xemac);

	XEmacPs_SetHandler(&xemacpsif->emacps, XEMACPS_HANDLER_ERROR,
				    (void *) emacps_error_handler,
				    (void *) xemac);
}

void start_emacps (xemacpsif_s *xemacps)
{
	/* start the temac */
    	XEmacPs_Start(&xemacps->emacps);
}

void restart_emacps_transmitter (xemacpsif_s *xemacps) {
	u32 Reg;
	Reg = XEmacPs_ReadReg(xemacps->emacps.Config.BaseAddress,
					XEMACPS_NWCTRL_OFFSET);
	Reg = Reg & (~XEMACPS_NWCTRL_TXEN_MASK);
	XEmacPs_WriteReg(xemacps->emacps.Config.BaseAddress,XEMACPS_NWCTRL_OFFSET, Reg);

	Reg = XEmacPs_ReadReg(xemacps->emacps.Config.BaseAddress,
						XEMACPS_NWCTRL_OFFSET);
	Reg = Reg | (XEMACPS_NWCTRL_TXEN_MASK);
	XEmacPs_WriteReg(xemacps->emacps.Config.BaseAddress,XEMACPS_NWCTRL_OFFSET, Reg);
}

void
emacps_error_handler(void *arg,u8 Direction, u32 ErrorWord)
{
	struct xemac_s *xemac;
	xemacpsif_s   *xemacpsif;
	struct xtopology_t *xtopologyp;
	XEmacPs *xemacps;

	xemac = (struct xemac_s *)(arg);
	xemacpsif = (xemacpsif_s *)(xemac->state);
	xtopologyp = &xtopology[xemac->topology_index];
	xemacps = &xemacpsif->emacps;

	if (ErrorWord != 0) {
		if (Direction == XEMACPS_RECV) {
			LWIP_DEBUGF(NETIF_DEBUG, ("ERROR on Rx Path\r\n"));
		}
		else if (Direction == XEMACPS_SEND) {
			LWIP_DEBUGF(NETIF_DEBUG, ("ERROR on xmit Path\r\n"));
		}
		switch (Direction) {
			case XEMACPS_RECV:
			if (ErrorWord & XEMACPS_RXSR_HRESPNOK_MASK) {
				LWIP_DEBUGF(NETIF_DEBUG, ("Receive DMA error\r\n"));
			}
			if (ErrorWord & XEMACPS_RXSR_RXOVR_MASK) {
				LWIP_DEBUGF(NETIF_DEBUG, ("Receive over run\r\n"));
			}
			if (ErrorWord & XEMACPS_RXSR_BUFFNA_MASK) {
				LWIP_DEBUGF(NETIF_DEBUG, ("Receive buffer not available\r\n"));
			}
			break;
			case XEMACPS_SEND:
			if (ErrorWord & XEMACPS_TXSR_HRESPNOK_MASK) {
				LWIP_DEBUGF(NETIF_DEBUG, ("Transmit DMA error\r\n"));
			}
			if (ErrorWord & XEMACPS_TXSR_URUN_MASK) {
				LWIP_DEBUGF(NETIF_DEBUG, ("Transmit under run\r\n"));
			}
			if (ErrorWord & XEMACPS_TXSR_BUFEXH_MASK) {
				LWIP_DEBUGF(NETIF_DEBUG, ("Transmit buffer exhausted\r\n"));
			}
			if (ErrorWord & XEMACPS_TXSR_RXOVR_MASK) {
				LWIP_DEBUGF(NETIF_DEBUG, ("Transmit retry excessed limits\r\n"));
			}
			if (ErrorWord & XEMACPS_TXSR_FRAMERX_MASK) {
				LWIP_DEBUGF(NETIF_DEBUG, ("Transmit collision\r\n"));
			}
			if (ErrorWord & XEMACPS_TXSR_USEDREAD_MASK) {
				LWIP_DEBUGF(NETIF_DEBUG, ("Transmit buffer not available\r\n"));
			}
			break;
		}
		HandleEmacPsError(xemac);
	}
}
