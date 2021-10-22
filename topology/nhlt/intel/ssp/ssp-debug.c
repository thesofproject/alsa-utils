// SPDX-License-Identifier: BSD-3-Clause
//
// Copyright(c) 2021 Intel Corporation. All rights reserved.
//
// Author: Jaska Uimonen <jaska.uimonen@linux.intel.com>

#include <stdint.h>
#include <stdio.h>
#include <alsa/input.h>
#include <alsa/output.h>
#include <alsa/conf.h>
#include <alsa/error.h>
#include "pre-processor.h"
#include "ssp-debug.h"

#ifdef NHLT_DEBUG

void ssp_print_calculated(struct intel_ssp_params *ssp)
{
	int i, j;

	tplg_pp_debug("printing ssp nhlt calculated data");

	/* top level struct */
	tplg_pp_debug("ssp count %d", ssp->ssp_count);

	for (i = 0; i < ssp->ssp_count; i++)
		tplg_pp_debug("ssp %d dai_index: %u", i, ssp->ssp_dai_index[i]);

	for (i = 0; i < ssp->ssp_count; i++)
		tplg_pp_debug("ssp %d hw_config_count: %u", i, ssp->ssp_hw_config_count[i]);

	tplg_pp_debug("");

	for (i = 0; i < ssp->ssp_count; i++) {
		for (j = 0; j < ssp->ssp_hw_config_count[i]; j++) {
			tplg_pp_debug("ssp blob %d hw_config %d", i, j);
			tplg_pp_debug("gateway_attributes %u",
				      ssp->ssp_blob[i][j].gateway_attributes);
			tplg_pp_debug("ts_group[0] %u", ssp->ssp_blob[i][j].ts_group[0]);
			tplg_pp_debug("ts_group[1] %u", ssp->ssp_blob[i][j].ts_group[1]);
			tplg_pp_debug("ts_group[2] %u", ssp->ssp_blob[i][j].ts_group[2]);
			tplg_pp_debug("ts_group[3] %u", ssp->ssp_blob[i][j].ts_group[3]);
			tplg_pp_debug("ts_group[4] %u", ssp->ssp_blob[i][j].ts_group[4]);
			tplg_pp_debug("ts_group[5] %u", ssp->ssp_blob[i][j].ts_group[5]);
			tplg_pp_debug("ts_group[6] %u", ssp->ssp_blob[i][j].ts_group[6]);
			tplg_pp_debug("ts_group[7] %u", ssp->ssp_blob[i][j].ts_group[7]);
			tplg_pp_debug("ssc0% u", ssp->ssp_blob[i][j].ssc0);
			tplg_pp_debug("ssc1 %u", ssp->ssp_blob[i][j].ssc1);
			tplg_pp_debug("sscto %u", ssp->ssp_blob[i][j].sscto);
			tplg_pp_debug("sspsp %u", ssp->ssp_blob[i][j].sspsp);
			tplg_pp_debug("sstsa %u", ssp->ssp_blob[i][j].sstsa);
			tplg_pp_debug("ssrsa %u", ssp->ssp_blob[i][j].ssrsa);
			tplg_pp_debug("ssc2 %u", ssp->ssp_blob[i][j].ssc2);
			tplg_pp_debug("sspsp2 %u", ssp->ssp_blob[i][j].sspsp2);
			tplg_pp_debug("ssc3% u", ssp->ssp_blob[i][j].ssc3);
			tplg_pp_debug("ssioc %u", ssp->ssp_blob[i][j].ssioc);
			tplg_pp_debug("mdivc %u", ssp->ssp_blob[i][j].mdivc);
			tplg_pp_debug("mdivr %u", ssp->ssp_blob[i][j].mdivr);
		}
	}

	tplg_pp_debug("");
}

void ssp_print_internal(struct intel_ssp_params *ssp)
{
	int i;

	tplg_pp_debug("printing ssp nhlt internal data");

	tplg_pp_debug("io_clk %u", ssp->ssp_prm.io_clk);
	tplg_pp_debug("dai_index %u", ssp->ssp_prm.dai_index);
	tplg_pp_debug("mclk_id %u", ssp->ssp_prm.mclk_id);
	tplg_pp_debug("sample_valid_bits %u", ssp->ssp_prm.sample_valid_bits);
	tplg_pp_debug("mclk_direction %u", ssp->ssp_prm.mclk_direction);
	tplg_pp_debug("frame_pulse_width %u", ssp->ssp_prm.frame_pulse_width);
	tplg_pp_debug("tdm_per_slot_padding_flag %u", ssp->ssp_prm.tdm_per_slot_padding_flag);
	tplg_pp_debug("clks_control %u", ssp->ssp_prm.clks_control);
	tplg_pp_debug("quirks %u", ssp->ssp_prm.quirks);
	tplg_pp_debug("bclk_delay %u", ssp->ssp_prm.bclk_delay);

	tplg_pp_debug("");

	tplg_pp_debug("hw_config_count %u", ssp->ssp_hw_config_count[ssp->ssp_count]);

	for (i = 0; i < ssp->ssp_hw_config_count[ssp->ssp_count]; i++) {
		tplg_pp_debug("mclk_rate %u", ssp->ssp_prm.hw_cfg[i].mclk_rate);
		tplg_pp_debug("bclk_rate %u", ssp->ssp_prm.hw_cfg[i].bclk_rate);
		tplg_pp_debug("fsync_rate %u", ssp->ssp_prm.hw_cfg[i].fsync_rate);
		tplg_pp_debug("tdm_slots %u", ssp->ssp_prm.hw_cfg[i].tdm_slots);
		tplg_pp_debug("tdm_slot_width %u", ssp->ssp_prm.hw_cfg[i].tdm_slot_width);
		tplg_pp_debug("tx_slots %u", ssp->ssp_prm.hw_cfg[i].tx_slots);
		tplg_pp_debug("rx_slots %u", ssp->ssp_prm.hw_cfg[i].rx_slots);
		tplg_pp_debug("format %u", ssp->ssp_prm.hw_cfg[i].format);
	}

	tplg_pp_debug("");
}

#else /* NHLT_DEBUG */
void ssp_print_internal(struct intel_ssp_params *ssp) {}
void ssp_print_calculated(struct intel_ssp_params *ssp) {}
#endif
