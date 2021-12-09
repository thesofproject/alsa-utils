// SPDX-License-Identifier: BSD-3-Clause
//
// Copyright(c) 2021 Intel Corporation. All rights reserved.
//
// Author: Seppo Ingalsuo <seppo.ingalsuo@linux.intel.com>
//         Jaska Uimonen <jaska.uimonen@linux.intel.com>

#include <stdint.h>
#include <stdio.h>
#include <alsa/input.h>
#include <alsa/output.h>
#include <alsa/conf.h>
#include <alsa/error.h>
#include "pre-processor.h"
#include "dmic-debug.h"

#ifdef NHLT_DEBUG

/* print blob as bytes hex string like: 0x11,0xff,0xff,0xff etc. */
void dmic_print_bytes_as_hex(uint8_t *src, size_t size)
{
	int i;

	tplg_pp_debug("printing dmic vendor blob as bytes:");

	for (i = 0; i < size; i++) {
		printf("0x%02x,", *src);
		src++;
	}

	tplg_pp_debug("");
}

/* print blob as 32 bit integer hex string like: 0xffffffff,0x00000010 etc. */
void dmic_print_integers_as_hex(uint32_t *src, size_t size)
{
	int i;

	tplg_pp_debug("printing dmic vendor blob as integers:");

	for (i = 0; i < size; i++) {
		printf("0x%08x,", *src);
		src++;
	}

	tplg_pp_debug("");
}

void dmic_print_internal(struct intel_dmic_params *dmic)
{
	int i, j, line, lines, remain;

	tplg_pp_debug("printing dmic nhlt internal data");

	/* top level struct */
	tplg_pp_debug("gateway attributes: 0x%08x", dmic->dmic_blob.gateway_attributes);

	tplg_pp_debug("ts_group: 0x%08x 0x%08x 0x%08x 0x%08x", dmic->dmic_blob.ts_group[0],
		      dmic->dmic_blob.ts_group[1], dmic->dmic_blob.ts_group[2],
		      dmic->dmic_blob.ts_group[3]);

	tplg_pp_debug("clock_on_delay: 0x%08x", dmic->dmic_blob.clock_on_delay);

	tplg_pp_debug("channel_ctrl_mask: 0x%08x", dmic->dmic_blob.channel_ctrl_mask);

	tplg_pp_debug("chan_ctrl_cfg: 0x%08x 0x%08x", dmic->dmic_blob.chan_ctrl_cfg[0],
		      dmic->dmic_blob.chan_ctrl_cfg[1]);

	tplg_pp_debug("channel_pdm_mask: 0x%08x", dmic->dmic_blob.channel_pdm_mask);

	/* first pdm struct */
	tplg_pp_debug("pdm_ctrl_cfg 0");
	tplg_pp_debug("cic_control: 0x%08x", dmic->dmic_blob_pdm[0].cic_control);
	tplg_pp_debug("cic_config: 0x%08x", dmic->dmic_blob_pdm[0].cic_config);
	tplg_pp_debug("mic_control: 0x%08x", dmic->dmic_blob_pdm[0].mic_control);
	tplg_pp_debug("pdmsm: 0x%08x", dmic->dmic_blob_pdm[0].pdmsm);
	tplg_pp_debug("reuse_fir_from_pdm: 0x%08x", dmic->dmic_blob_pdm[0].reuse_fir_from_pdm);

	/* first pdm struct, first fir */
	tplg_pp_debug("fir_config 0");
	tplg_pp_debug("fir_control: 0x%08x", dmic->dmic_blob_fir[0][0].fir_control);
	tplg_pp_debug("fir_config: 0x%08x", dmic->dmic_blob_fir[0][0].fir_config);
	tplg_pp_debug("dc_offset_left: 0x%08x", dmic->dmic_blob_fir[0][0].dc_offset_left);
	tplg_pp_debug("dc_offset_right: 0x%08x", dmic->dmic_blob_fir[0][0].dc_offset_right);
	tplg_pp_debug("out_gain_left: 0x%08x", dmic->dmic_blob_fir[0][0].out_gain_left);
	tplg_pp_debug("out_gain_right: 0x%08x", dmic->dmic_blob_fir[0][0].out_gain_right);

	/* first pdm struct, second fir */
	tplg_pp_debug("fir_config 1");
	tplg_pp_debug("fir_control: %0x%08x", dmic->dmic_blob_fir[0][1].fir_control);
	tplg_pp_debug("fir_config: %0x%08x", dmic->dmic_blob_fir[0][1].fir_config);
	tplg_pp_debug("dc_offset_left: %0x%08x", dmic->dmic_blob_fir[0][1].dc_offset_left);
	tplg_pp_debug("dc_offset_right: %0x%08x", dmic->dmic_blob_fir[0][1].dc_offset_right);
	tplg_pp_debug("out_gain_left: %0x%08x", dmic->dmic_blob_fir[0][1].out_gain_left);
	tplg_pp_debug("out_gain_right: %0x%08x", dmic->dmic_blob_fir[0][1].out_gain_right);

	/* first pdm struct, fir coeffs */
	for (j = 0; j < DMIC_HW_CONTROLLERS; j++) {
		tplg_pp_debug("fir_coeffs a length %u:", dmic->dmic_fir_array.fir_len[0]);
		lines = dmic->dmic_fir_array.fir_len[0] / 8;
		remain = dmic->dmic_fir_array.fir_len[0] % 8;
		for (i = 0; i < lines; i++) {
			line = i * 8;
			tplg_pp_debug("%d %d %d %d %d %d %d %d %d", i,
				      dmic->dmic_fir_array.fir_coeffs[j][0][line],
				      dmic->dmic_fir_array.fir_coeffs[j][0][line + 1],
				      dmic->dmic_fir_array.fir_coeffs[j][0][line + 2],
				      dmic->dmic_fir_array.fir_coeffs[j][0][line + 3],
				      dmic->dmic_fir_array.fir_coeffs[j][0][line + 4],
				      dmic->dmic_fir_array.fir_coeffs[j][0][line + 5],
				      dmic->dmic_fir_array.fir_coeffs[j][0][line + 6],
				      dmic->dmic_fir_array.fir_coeffs[j][0][line + 7]);
		}
		line += 1;
		for (i = 0; i < remain; i++)
			tplg_pp_debug("%d ", dmic->dmic_fir_array.fir_coeffs[j][0][line + i]);
	}

	/* second pdm struct */
	tplg_pp_debug("pdm_ctrl_cfg 1");
	tplg_pp_debug("cic_control: 0x%08x", dmic->dmic_blob_pdm[1].cic_control);
	tplg_pp_debug("cic_config: 0x%08x", dmic->dmic_blob_pdm[1].cic_config);
	tplg_pp_debug("mic_control: 0x%08x", dmic->dmic_blob_pdm[1].mic_control);
	tplg_pp_debug("pdmsm: 0x%08x", dmic->dmic_blob_pdm[1].pdmsm);
	tplg_pp_debug("reuse_fir_from_pdm: 0x%08x", dmic->dmic_blob_pdm[1].reuse_fir_from_pdm);

	/* second pdm struct, first fir */
	tplg_pp_debug("fir_config 0");
	tplg_pp_debug("fir_control: 0x%08x", dmic->dmic_blob_fir[1][0].fir_control);
	tplg_pp_debug("fir_config: 0x%08x", dmic->dmic_blob_fir[1][0].fir_config);
	tplg_pp_debug("dc_offset_left: 0x%08x", dmic->dmic_blob_fir[1][0].dc_offset_left);
	tplg_pp_debug("dc_offset_right: 0x%08x", dmic->dmic_blob_fir[1][0].dc_offset_right);
	tplg_pp_debug("out_gain_left: 0x%08x", dmic->dmic_blob_fir[1][0].out_gain_left);
	tplg_pp_debug("out_gain_right: 0x%08x", dmic->dmic_blob_fir[1][0].out_gain_right);

	/* second pdm struct, second fir */
	tplg_pp_debug("fir_config 1");
	tplg_pp_debug("fir_control: 0x%08x", dmic->dmic_blob_fir[1][1].fir_control);
	tplg_pp_debug("fir_config: 0x%08x", dmic->dmic_blob_fir[1][1].fir_config);
	tplg_pp_debug("dc_offset_left: 0x%08x", dmic->dmic_blob_fir[1][1].dc_offset_left);
	tplg_pp_debug("dc_offset_right: 0x%08x", dmic->dmic_blob_fir[1][1].dc_offset_right);
	tplg_pp_debug("out_gain_left: 0x%08x", dmic->dmic_blob_fir[1][1].out_gain_left);
	tplg_pp_debug("out_gain_right: 0x%08x", dmic->dmic_blob_fir[1][1].out_gain_right);

	for (j = 0; j < DMIC_HW_CONTROLLERS; j++) {
		tplg_pp_debug("fir_coeffs b length %u:", dmic->dmic_fir_array.fir_len[1]);
		lines = dmic->dmic_fir_array.fir_len[1] / 8;
		remain = dmic->dmic_fir_array.fir_len[1] % 8;
		for (i = 0; i < lines; i++) {
			line = i * 8;
			tplg_pp_debug("%d %d %d %d %d %d %d %d %d", i,
				      dmic->dmic_fir_array.fir_coeffs[j][1][line],
				      dmic->dmic_fir_array.fir_coeffs[j][1][line + 1],
				      dmic->dmic_fir_array.fir_coeffs[j][1][line + 2],
				      dmic->dmic_fir_array.fir_coeffs[j][1][line + 3],
				      dmic->dmic_fir_array.fir_coeffs[j][1][line + 4],
				      dmic->dmic_fir_array.fir_coeffs[j][1][line + 5],
				      dmic->dmic_fir_array.fir_coeffs[j][1][line + 6],
				      dmic->dmic_fir_array.fir_coeffs[j][1][line + 7]);
		}
		line += 1;
		for (i = 0; i < remain; i++)
			tplg_pp_debug("%d ", dmic->dmic_fir_array.fir_coeffs[j][1][line + i]);
	}
}

#else /* NHLT_DEBUG */
void dmic_print_bytes_as_hex(uint8_t *src, size_t size) {}
void dmic_print_integers_as_hex(uint32_t *src, size_t size) {}
void dmic_print_internal(struct intel_dmic_params *dmic) {}
#endif
