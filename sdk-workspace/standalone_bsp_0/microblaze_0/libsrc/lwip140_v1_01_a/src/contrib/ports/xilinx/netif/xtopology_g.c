#include "netif/xtopology.h"

struct xtopology_t xtopology[] = {
	{
		0x82000000,
		xemac_type_xps_ll_temac,
		0x81800000,
		-1,
		0x0,
		0x0,
	},
};

int xtopology_n_emacs = 1;
