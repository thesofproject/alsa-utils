/*
  Copyright(c) 2021 Intel Corporation
  All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution
  in the file called LICENSE.GPL.
*/

#ifndef __SSP_NHLT_H
#define __SSP_NHLT_H

#include "pre-processor.h"
#include "../nhlt.h"

int nhlt_ssp_init_params(struct tplg_pre_processor *tplg_pp);
int nhlt_ssp_set_params(struct tplg_pre_processor *tplg_pp, snd_config_t *cfg,
			snd_config_t *parent);
int nhlt_ssp_get_ep(struct tplg_pre_processor *tplg_pp, struct endpoint_descriptor **eps,
		    int dai_index);
int nhlt_ssp_get_ep_count(struct tplg_pre_processor *tplg_pp);

#endif
