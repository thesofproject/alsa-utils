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

#ifndef __NHLT_PROCESSOR_H
#define __NHLT_PROCESSOR_H

#include "pre-processor.h"

int nhlt_init(struct tplg_pre_processor *tplg_pp);
int nhlt_set_dai(struct tplg_pre_processor *tplg_pp, snd_config_t *cfg, snd_config_t *parent);
int nhlt_create(struct tplg_pre_processor *tplg_pp);
int nhlt_delete(struct tplg_pre_processor *tplg_pp);

#endif
