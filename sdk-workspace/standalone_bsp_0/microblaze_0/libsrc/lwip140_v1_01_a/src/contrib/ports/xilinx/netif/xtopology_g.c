#include "netif/xtopology.h"

struct xtopology_t xtopology[] = {
	{
		0x81000000,
		xemac_type_xps_emaclite,
		0x81800000,
		0,
		0x0,
		0x0,
	},
};

int xtopology_n_emacs = 1;
