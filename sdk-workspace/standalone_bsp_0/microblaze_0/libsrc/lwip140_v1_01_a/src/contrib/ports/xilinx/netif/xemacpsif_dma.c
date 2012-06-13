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

#include "lwipopts.h"
#include "lwip/stats.h"
#include "lwip/sys.h"
#include "lwip/inet_chksum.h"

#include "netif/xadapter.h"
#include "netif/xemacpsif.h"
#include "xstatus.h"

#include "xlwipconfig.h"
#include "xparameters.h"
#include "xparameters_ps.h"
#include "xil_exception.h"
#ifdef CONFIG_XTRACE
#include "xtrace.h"
#endif

/*** IMPORTANT: Define PEEP in xemacpsif.h and sys_arch_raw.c
 *** to run it on a PEEP board
 ***/

#define INTC_BASE_ADDR		XPAR_SCUGIC_CPU_BASEADDR
#define INTC_DIST_BASE_ADDR	XPAR_SCUGIC_DIST_BASEADDR

#define RX_BD_START_ADDRESS	0x0FF00000
#define TX_BD_START_ADDRESS	0x0FF10000

/* Byte alignment of BDs */
#define BD_ALIGNMENT (XEMACPS_DMABD_MINIMUM_ALIGNMENT*2)

static int tx_pbufs_storage[XLWIP_CONFIG_N_TX_DESC];
static int rx_pbufs_storage[XLWIP_CONFIG_N_RX_DESC];

static int EmacIntrNum;

#define XEMACPS_BD_TO_INDEX(ringptr, bdptr)				\
	(((u32)bdptr - (u32)(ringptr)->BaseBdAddr) / (ringptr)->Separation)


int
is_tx_space_available(xemacpsif_s *emac)
{
	XEmacPs_BdRing *txring;
	int freecnt = 0;

	txring = &(XEmacPs_GetTxRing(&emac->emacps));

	/* tx space is available as long as there are valid BD's */
	freecnt = XEmacPs_BdRingGetFreeCnt(txring);
	return freecnt;
}

int
process_sent_bds(XEmacPs_BdRing *txring)
{
	XEmacPs_Bd *txbdset;
	XEmacPs_Bd *CurBdPntr;
	int n_bds;
	XStatus Status;
	int n_pbufs_freed = 0;
	unsigned int BdIndex;
	struct pbuf *p;
	unsigned int *Temp;

	/* obtain processed BD's */
	n_bds = XEmacPs_BdRingFromHwTx(txring, XLWIP_CONFIG_N_TX_DESC, &txbdset);
	if (n_bds == 0)  {
		return;
	}

	/* free the processed BD's */
	n_pbufs_freed = n_bds;
	CurBdPntr = txbdset;
	while (n_pbufs_freed > 0) {
		BdIndex = XEMACPS_BD_TO_INDEX(txring, CurBdPntr);
		Temp = (unsigned int *)CurBdPntr;
		Temp++;
		*Temp = 0x80000000;
		if (BdIndex == (XLWIP_CONFIG_N_TX_DESC - 1)) {
			*Temp = 0xC0000000;
		}

		p = (struct pbuf *)tx_pbufs_storage[BdIndex];
		if(p != NULL) {
			pbuf_free(p);
		}
		tx_pbufs_storage[BdIndex] = 0;
		CurBdPntr = XEmacPs_BdRingNext(txring, CurBdPntr);
		n_pbufs_freed--;
		dmb();
		dsb();
	}

	Status = XEmacPs_BdRingFree(txring, n_bds, txbdset);
	if (Status != XST_SUCCESS) {
		xil_printf("Failure while freeing in Tx Done ISR\r\n");
	}

	return 0;
}

void
emacps_send_handler(void *arg)
{
	struct xemac_s *xemac;
	xemacpsif_s   *xemacpsif;
	XEmacPs_BdRing *TxRingPtr;

	xemac = (struct xemac_s *)(arg);
	xemacpsif = (xemacpsif_s *)(xemac->state);
	TxRingPtr = &(XEmacPs_GetTxRing(&xemacpsif->emacps));

	/* If Transmit done interrupt is asserted, process completed BD's */
	process_sent_bds(TxRingPtr);
}

XStatus
emacps_sgsend(xemacpsif_s *xemacpsif, struct pbuf *p)
{
	struct pbuf *q;
	int n_pbufs;
	XEmacPs_Bd *txbdset, *txbd, *last_txbd = NULL;
	XStatus Status;
	XEmacPs_BdRing *txring;
	unsigned int BdIndex;
	unsigned int Index;
	u32 RegVal;


#ifdef PEEP
    	while((XEmacPs_ReadReg((xemacpsif->emacps).Config.BaseAddress, XEMACPS_TXSR_OFFSET)) & 0x08);
#endif

	txring = &(XEmacPs_GetTxRing(&xemacpsif->emacps));

	/* first count the number of pbufs */
	for (q = p, n_pbufs = 0; q != NULL; q = q->next)
		n_pbufs++;

	/* obtain as many BD's */
	Status = XEmacPs_BdRingAlloc(txring, n_pbufs, &txbdset);
	if (Status != XST_SUCCESS) {
		LWIP_DEBUGF(NETIF_DEBUG, ("sgsend: Error allocating TxBD\r\n"));
		return ERR_IF;
	}

	Xil_ExceptionDisable();
	for(q = p, txbd = txbdset; q != NULL; q = q->next) {
		BdIndex = XEMACPS_BD_TO_INDEX(txring, txbd);
		if (tx_pbufs_storage[BdIndex] != 0) {
			Xil_ExceptionEnable();
			xil_printf("PBUFS not available\r\n");
			return ERR_IF;
		}

		/* Send the data from the pbuf to the interface, one pbuf at a
		   time. The size of the data in each pbuf is kept in the ->len
		   variable. */
		Xil_DCacheFlushRange((unsigned int)q->payload, (unsigned)q->len);
		XEmacPs_BdSetAddressTx(txbd, (u32)q->payload);
		if (q->len > (XEMACPS_MAX_FRAME_SIZE - 18))
			XEmacPs_BdSetLength(txbd, (XEMACPS_MAX_FRAME_SIZE - 18) & 0x3FFF);
		else
			XEmacPs_BdSetLength(txbd, q->len & 0x3FFF);

		tx_pbufs_storage[BdIndex] = (int)q;

		pbuf_ref(q);
		last_txbd = txbd;
		XEmacPs_BdClearTxUsed(txbd);
		dmb();
		dsb();
 		txbd = XEmacPs_BdRingNext(txring, txbd);
	}

	XEmacPs_BdSetLast(last_txbd);
	/* enq to h/w */
	dmb();
	dsb();

	Status = XEmacPs_BdRingToHw(txring, n_pbufs, txbdset);
	if (Status != XST_SUCCESS) {
		Xil_ExceptionEnable();
		LWIP_DEBUGF(NETIF_DEBUG, ("sgsend: Error submitting TxBD\r\n"));
		return ERR_IF;
	}
	dmb();
	dsb();

	/* Start transmit */

	XEmacPs_WriteReg((xemacpsif->emacps).Config.BaseAddress,
	XEMACPS_NWCTRL_OFFSET,
	(XEmacPs_ReadReg((xemacpsif->emacps).Config.BaseAddress,
	XEMACPS_NWCTRL_OFFSET) | XEMACPS_NWCTRL_STARTTX_MASK));
	dmb();
	dsb();

	Xil_ExceptionEnable();
	return Status;
}

void
_setup_rx_bds(XEmacPs_BdRing *rxring)
{
	XEmacPs_Bd *rxbd, *CurBdPtr;
	int n_bds, i;
	XStatus Status;
	struct pbuf *p;
	u32 BdSts;
	unsigned int FreeBds, k;
	unsigned int BdIndex;
	unsigned int *Temp;

	FreeBds = XEmacPs_BdRingGetFreeCnt (rxring);
	Status = XEmacPs_BdRingAlloc(rxring, FreeBds, &rxbd);
	if (Status != XST_SUCCESS) {
		LWIP_DEBUGF(NETIF_DEBUG, ("setup_rx_bds: Error allocating RxBD\r\n"));
		return;
	}
	for (k = 0, CurBdPtr = rxbd; k < FreeBds; k++) {
		p = pbuf_alloc(PBUF_RAW, XEMACPS_MAX_FRAME_SIZE, PBUF_POOL);

		if (!p) {
#if LINK_STATS
			lwip_stats.link.memerr++;
			lwip_stats.link.drop++;
#endif
			LWIP_DEBUGF(NETIF_DEBUG, ("unable to alloc pbuf in recv_handler\r\n"));
			return;
		}
		BdIndex = XEMACPS_BD_TO_INDEX(rxring, CurBdPtr);
		Temp = (unsigned int *)CurBdPtr;
		*Temp = 0;
		if (BdIndex == (XLWIP_CONFIG_N_RX_DESC - 1)) {
			*Temp = 0x00000002;
		}
		Temp++;
		*Temp = 0;
		XEmacPs_BdSetAddressRx(CurBdPtr, (u32)p->payload);

		rx_pbufs_storage[BdIndex] = (int)p;
		CurBdPtr = XEmacPs_BdRingNext (rxring, CurBdPtr);

		/* Enqueue to HW */
	}
	Status = XEmacPs_BdRingToHw(rxring, FreeBds, rxbd);
	if (Status != XST_SUCCESS) {
		LWIP_DEBUGF(NETIF_DEBUG, ("Error committing RxBD to hardware: "));
		if (Status == XST_DMA_SG_LIST_ERROR)
			LWIP_DEBUGF(NETIF_DEBUG, ("XST_DMA_SG_LIST_ERROR: this function was called out of sequence with XEmacPs_BdRingAlloc()\r\n"));
		else
			LWIP_DEBUGF(NETIF_DEBUG, ("set of BDs was rejected because the first BD did not have its start-of-packet bit set, or the last BD did not have its end-of-packet bit set, or any one of the BD set has 0 as length value\r\n"));
		return;
	}
}

void
emacps_recv_handler(void *arg)
{
	struct pbuf *p;
	unsigned irq_status, i;
	XEmacPs_Bd *rxbdset, *CurBdPtr;
	struct xemac_s *xemac;
	xemacpsif_s *xemacpsif;
	XEmacPs_BdRing *rxring;
	volatile int bd_processed;
	int rx_bytes, k;
	unsigned int BdIndex;

	xemac = (struct xemac_s *)(arg);
	xemacpsif = (xemacpsif_s *)(xemac->state);
	rxring = &XEmacPs_GetRxRing(&xemacpsif->emacps);

	/*
	 * If Reception done interrupt is asserted, call RX call back function
	 * to handle the processed BDs and then raise the according flag.
	 */

	bd_processed = XEmacPs_BdRingFromHwRx(rxring, XLWIP_CONFIG_N_RX_DESC, &rxbdset);

	if (bd_processed <= 0) {
		return;
	}
	for (k = 0, CurBdPtr=rxbdset; k < bd_processed; k++) {
		BdIndex = XEMACPS_BD_TO_INDEX(rxring, CurBdPtr);
		p = (struct pbuf *)rx_pbufs_storage[BdIndex];
		/*
		 * Adjust the buffer size to the actual number of bytes received.
		 */
		rx_bytes = XEmacPs_BdGetLength(CurBdPtr);
		pbuf_realloc(p, rx_bytes);
		Xil_DCacheInvalidateRange((unsigned int)p->payload, (unsigned)XEMACPS_MAX_FRAME_SIZE);
		/* store it in the receive queue,
		 * where it'll be processed by a different handler
		 */
		if (pq_enqueue(xemacpsif->recv_q, (void*)p) < 0) {
#if LINK_STATS
			lwip_stats.link.memerr++;
			lwip_stats.link.drop++;
#endif
			pbuf_free(p);
		}
		CurBdPtr = XEmacPs_BdRingNext( rxring, CurBdPtr);
	}
	/* free up the BD's */
	XEmacPs_BdRingFree(rxring, bd_processed, rxbdset);
	_setup_rx_bds(rxring);
	return;
}

void *
alloc_bdspace(int n_desc, int type)
{
	void *aligned_mem;
	if (type == 0)
		aligned_mem = (void *)RX_BD_START_ADDRESS;
	else
		aligned_mem = (void *)TX_BD_START_ADDRESS;
	return aligned_mem;
}

XStatus
init_dma(struct xemac_s *xemac)
{
	XEmacPs_Config *DMAConfig;
	XEmacPs_Bd BdTemplate;
	XEmacPs_BdRing *RxRingPtr, *TxRingPtr;
	XEmacPs_Bd *rxbd;
	struct pbuf *p;
	XStatus Status;
	int i;
	unsigned int BdIndex;
	xemacpsif_s *xemacpsif = (xemacpsif_s *)(xemac->state);
#if NO_SYS
	struct xtopology_t *xtopologyp = &xtopology[xemac->topology_index];
#endif
	/*
	 * The BDs need to be allocated in uncached memory. Hence the 1 MB
	 * address range that starts at address 0xFF00000 is made uncached
	 * by setting appropriate attributes in the translation table.
	 */
	Xil_SetTlbAttributes(0x0FF00000, 0xc02); // addr, attr

	RxRingPtr = &XEmacPs_GetRxRing(&xemacpsif->emacps);
	TxRingPtr = &XEmacPs_GetTxRing(&xemacpsif->emacps);
	LWIP_DEBUGF(NETIF_DEBUG, ("RxRingPtr: 0x%08x\r\n", RxRingPtr));
	LWIP_DEBUGF(NETIF_DEBUG, ("TxRingPtr: 0x%08x\r\n", TxRingPtr));

	xemacpsif->rx_bdspace = alloc_bdspace(XLWIP_CONFIG_N_RX_DESC, 0);
	xemacpsif->tx_bdspace = alloc_bdspace(XLWIP_CONFIG_N_TX_DESC, 1);
	LWIP_DEBUGF(NETIF_DEBUG, ("rx_bdspace: 0x%08x\r\n", xemacpsif->rx_bdspace));
	LWIP_DEBUGF(NETIF_DEBUG, ("tx_bdspace: 0x%08x\r\n", xemacpsif->tx_bdspace));

	if (!xemacpsif->rx_bdspace || !xemacpsif->tx_bdspace) {
		xil_printf("%s@%d: Error: Unable to allocate memory for TX/RX buffer descriptors",
				__FILE__, __LINE__);
		return XST_FAILURE;
	}

	/*
	 * Setup RxBD space.
	 *
	 * Setup a BD template for the Rx channel. This template will be copied to
	 * every RxBD. We will not have to explicitly set these again.
	 */
	XEmacPs_BdClear(&BdTemplate);

	/*
	 * Create the RxBD ring
	 */

	Status = XEmacPs_BdRingCreate(RxRingPtr, (u32) xemacpsif->rx_bdspace,
				(u32) xemacpsif->rx_bdspace, BD_ALIGNMENT,
				     XLWIP_CONFIG_N_RX_DESC);

	if (Status != XST_SUCCESS) {
		LWIP_DEBUGF(NETIF_DEBUG, ("Error setting up RxBD space\r\n"));
		return XST_FAILURE;
	}

	Status = XEmacPs_BdRingClone(RxRingPtr, &BdTemplate, XEMACPS_RECV);
	if (Status != XST_SUCCESS) {
		LWIP_DEBUGF(NETIF_DEBUG, ("Error initializing RxBD space\r\n"));
		return XST_FAILURE;
	}

	XEmacPs_BdClear(&BdTemplate);
	XEmacPs_BdSetStatus(&BdTemplate, XEMACPS_TXBUF_USED_MASK);
	/*
	 * Create the TxBD ring
	 */
	Status = XEmacPs_BdRingCreate(TxRingPtr, (u32) xemacpsif->tx_bdspace,
				(u32) xemacpsif->tx_bdspace, BD_ALIGNMENT,
				     XLWIP_CONFIG_N_TX_DESC);

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* We reuse the bd template, as the same one will work for both rx and tx. */
	Status = XEmacPs_BdRingClone(TxRingPtr, &BdTemplate, XEMACPS_SEND);
	if (Status != XST_SUCCESS) {
		return ERR_IF;
	}

	/*
	 * Allocate RX descriptors, 1 RxBD at a time.
	 */
	for (i = 0; i < XLWIP_CONFIG_N_RX_DESC; i++) {
		Status = XEmacPs_BdRingAlloc(RxRingPtr, 1, &rxbd);
		if (Status != XST_SUCCESS) {
			LWIP_DEBUGF(NETIF_DEBUG, ("init_dma: Error allocating RxBD\r\n"));
			return ERR_IF;
		}

		p = pbuf_alloc(PBUF_RAW, XEMACPS_MAX_FRAME_SIZE, PBUF_POOL);
		if (!p) {
#if LINK_STATS
			lwip_stats.link.memerr++;
			lwip_stats.link.drop++;
#endif
			LWIP_DEBUGF(NETIF_DEBUG, ("unable to alloc pbuf in recv_handler\r\n"));
			return -1;
		}


		XEmacPs_BdSetAddressRx(rxbd, (u32)p->payload);

		BdIndex = XEMACPS_BD_TO_INDEX(RxRingPtr, rxbd);
		rx_pbufs_storage[BdIndex] = (int)p;

		/* Enqueue to HW */
		Status = XEmacPs_BdRingToHw(RxRingPtr, 1, rxbd);
		if (Status != XST_SUCCESS) {
			LWIP_DEBUGF(NETIF_DEBUG, ("Error: committing RxBD to HW\r\n"));
			return XST_FAILURE;
		}
	}

	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	XScuGic_RegisterHandler(INTC_BASE_ADDR, xtopologyp->scugic_emac_intr,
				(Xil_ExceptionHandler)XEmacPs_IntrHandler,
						(void *)&xemacpsif->emacps);
	/*
	 * Enable the interrupt for emacps.
	 */
	XScuGic_EnableIntr(INTC_DIST_BASE_ADDR, (u32) xtopologyp->scugic_emac_intr);
	EmacIntrNum = (u32) xtopologyp->scugic_emac_intr;
	return 0;
}

void FreeTxPBufs(void)
{
	int Index;
	struct pbuf *p;

	for (Index = 0; Index < XLWIP_CONFIG_N_TX_DESC; Index++) {
		if (tx_pbufs_storage[Index] != 0) {
			p = (struct pbuf *)tx_pbufs_storage[Index];
			pbuf_free(p);
			tx_pbufs_storage[Index] = 0;
		}
	}

	for (Index = 0; Index < XLWIP_CONFIG_N_RX_DESC; Index++) {
		p = (struct pbuf *)rx_pbufs_storage[Index];
		pbuf_free(p);

	}
}

void EmacDisableIntr(void)
{
	XScuGic_DisableIntr(INTC_DIST_BASE_ADDR, EmacIntrNum);
}

void EmacEnableIntr(void)
{
	XScuGic_EnableIntr(INTC_DIST_BASE_ADDR, EmacIntrNum);
}
