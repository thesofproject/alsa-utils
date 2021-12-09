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

#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <alsa/input.h>
#include <alsa/output.h>
#include <alsa/conf.h>
#include <alsa/error.h>
#include "../nhlt.h"
#include "intel-nhlt.h"
#include "ssp-nhlt.h"
#include "ssp/ssp-process.h"

static int set_ssp_data(struct tplg_pre_processor *tplg_pp, snd_config_t *dai_cfg)
{
	const char *tdm_padding_per_slot;
	snd_config_iterator_t i, next;
	const char *direction = NULL;
	const char *quirks = NULL;
	int frame_pulse_width = 0;
	snd_config_t *class_cfg;
	int clks_control = 0;
	int sample_bits = 0;
	int bclk_delay = 0;
	int dai_index = 0;
	long int_val = 0;
	int mclk_id = 0;
	snd_config_t *n;
	const char *id;
	int io_clk;
	int ret;

	/* get default values from class definition */
	ret = snd_config_search(tplg_pp->input_cfg, "Class.Dai.SSP", &class_cfg);
	if (ret < 0)
		return ret;

	snd_config_for_each(i, next, class_cfg) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_id(n, &id) < 0)  {
			tplg_pp_debug("set_ssp_data no id found");
			continue;
		}

		if (!strcmp(id, "io_clk")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			io_clk = int_val;
		}

		if (!strcmp(id, "direction")) {
			if (snd_config_get_string(n, &direction))
				return -EINVAL;
		}

		if (!strcmp(id, "quirks")) {
			if (snd_config_get_string(n, &quirks))
				return -EINVAL;
		}

		if (!strcmp(id, "dai_index")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			dai_index = int_val;
		}

		if (!strcmp(id, "sample_bits")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			sample_bits = int_val;
		}

		if (!strcmp(id, "bclk_delay")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			bclk_delay = int_val;
		}

		if (!strcmp(id, "mclk_id")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			mclk_id = int_val;
		}

		if (!strcmp(id, "clks_control")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			clks_control = int_val;
		}

		if (!strcmp(id, "frame_pulse_width")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			frame_pulse_width = int_val;
		}

		if (!strcmp(id, "tdm_padding_per_slot")) {
			if (snd_config_get_string(n, &tdm_padding_per_slot))
				return -EINVAL;
		}
	}

	/* set instance specific values */
	snd_config_for_each(i, next, dai_cfg) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_id(n, &id) < 0)  {
			tplg_pp_debug("set_ssp_data no id found");
			continue;
		}

		if (!strcmp(id, "io_clk")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			io_clk = int_val;
		}

		if (!strcmp(id, "direction")) {
			if (snd_config_get_string(n, &direction))
				return -EINVAL;
		}

		if (!strcmp(id, "quirks")) {
			if (snd_config_get_string(n, &quirks))
				return -EINVAL;
		}

		if (!strcmp(id, "sample_bits")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			sample_bits = int_val;
		}

		if (!strcmp(id, "dai_index")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			dai_index = int_val;
		}

		if (!strcmp(id, "bclk_delay")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			bclk_delay = int_val;
		}

		if (!strcmp(id, "mclk_id")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			mclk_id = int_val;
		}

		if (!strcmp(id, "clks_control")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			clks_control = int_val;
		}

		if (!strcmp(id, "frame_pulse_width")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			frame_pulse_width = int_val;
		}

		if (!strcmp(id, "tdm_padding_per_slot")) {
			if (snd_config_get_string(n, &tdm_padding_per_slot))
				return -EINVAL;
		}
	}

	return ssp_set_params(tplg_pp, dai_index, io_clk, bclk_delay, sample_bits, mclk_id,
			      clks_control, frame_pulse_width, tdm_padding_per_slot, quirks);
}

static int set_hw_config(struct tplg_pre_processor *tplg_pp,
			 snd_config_t *cfg)
{
	snd_config_iterator_t i, next;
	snd_config_t *class_cfg;
	snd_config_t *n;
	const char *id;
	long int_val = 0;
	int ret;
	const char *format = NULL;
	const char *mclk = NULL;
	const char *bclk = NULL;
	const char *bclk_invert = NULL;
	const char *fsync = NULL;
	const char *fsync_invert = NULL;
	int mclk_freq;
	int bclk_freq;
	int fsync_freq;
	int tdm_slots;
	int tdm_slot_width;
	int tx_slots;
	int rx_slots;

	/* get default values */
	ret = snd_config_search(tplg_pp->input_cfg, "Class.Base.hw_config", &class_cfg);
	if (ret < 0)
		return -EINVAL;

	/* set default values */
	snd_config_for_each(i, next, class_cfg) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_id(n, &id) < 0)
			continue;

		if (!strcmp(id, "format")) {
			if (snd_config_get_string(n, &format))
				return -EINVAL;
		}

		if (!strcmp(id, "mclk")) {
			if (snd_config_get_string(n, &mclk))
				return -EINVAL;
		}

		if (!strcmp(id, "bclk")) {
			if (snd_config_get_string(n, &bclk))
				return -EINVAL;
		}

		if (!strcmp(id, "fsync")) {
			if (snd_config_get_string(n, &fsync))
				return -EINVAL;
		}

		if (!strcmp(id, "bclk_invert")) {
			if (snd_config_get_string(n, &bclk_invert))
				return -EINVAL;
		}

		if (!strcmp(id, "fsync_invert")) {
			if (snd_config_get_string(n, &fsync_invert))
				return -EINVAL;
		}

		if (!strcmp(id, "fsync_freq")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			fsync_freq = int_val;
		}

		if (!strcmp(id, "bclk_freq")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			bclk_freq = int_val;
		}

		if (!strcmp(id, "mclk_freq")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			mclk_freq = int_val;
		}

		if (!strcmp(id, "tdm_slots")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			tdm_slots = int_val;
		}

		if (!strcmp(id, "tdm_slot_width")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			tdm_slot_width = int_val;
		}

		if (!strcmp(id, "tx_slots")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			tx_slots = int_val;
		}

		if (!strcmp(id, "rx_slots")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			rx_slots = int_val;
		}
	}

	/* set object values */
	snd_config_for_each(i, next, cfg) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_id(n, &id) < 0)
			continue;

		if (!strcmp(id, "format")) {
			if (snd_config_get_string(n, &format))
				return -EINVAL;
		}

		if (!strcmp(id, "mclk")) {
			if (snd_config_get_string(n, &mclk))
				return -EINVAL;
		}

		if (!strcmp(id, "bclk")) {
			if (snd_config_get_string(n, &bclk))
				return -EINVAL;
		}

		if (!strcmp(id, "fsync")) {
			if (snd_config_get_string(n, &fsync))
				return -EINVAL;
		}

		if (!strcmp(id, "bclk_invert")) {
			if (snd_config_get_string(n, &bclk_invert))
				return -EINVAL;
		}

		if (!strcmp(id, "fsync_invert")) {
			if (snd_config_get_string(n, &fsync_invert))
				return -EINVAL;
		}

		if (!strcmp(id, "fsync_freq")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			fsync_freq = int_val;
		}

		if (!strcmp(id, "bclk_freq")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			bclk_freq = int_val;
		}

		if (!strcmp(id, "mclk_freq")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			mclk_freq = int_val;
		}

		if (!strcmp(id, "tdm_slots")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			tdm_slots = int_val;
		}

		if (!strcmp(id, "tdm_slot_width")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			tdm_slot_width = int_val;
		}

		if (!strcmp(id, "tx_slots")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			tx_slots = int_val;
		}

		if (!strcmp(id, "rx_slots")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			rx_slots = int_val;
		}
	}

	return ssp_hw_set_params(tplg_pp, format, mclk, bclk, bclk_invert, fsync, fsync_invert,
				 mclk_freq, bclk_freq, fsync_freq, tdm_slots, tdm_slot_width,
				 tx_slots, rx_slots);
}

/* init ssp parameters, should be called before parsing dais */
int nhlt_ssp_init_params(struct tplg_pre_processor *tplg_pp)
{
	return ssp_init_params(tplg_pp);
}

int nhlt_ssp_get_ep_count(struct tplg_pre_processor *tplg_pp)
{
	return ssp_get_vendor_blob_count(tplg_pp);
}

int nhlt_ssp_get_ep(struct tplg_pre_processor *tplg_pp, struct endpoint_descriptor **eps,
		    int dai_index)
{
	struct endpoint_descriptor ep;
	struct ssp_device_specific_config ssp_conf;
	struct formats_config f_conf;
	struct format_config f_conf1[8];
	uint32_t sample_rate;
	uint16_t channel_count;
	uint32_t bits_per_sample;
	uint32_t virtualbus_id;
	uint32_t formats_count;
	uint8_t *ep_target;
	size_t blob_size;
	int ret;
	int i;

	/*
	 * nhlt ssp structure:
	 *
	 * endpoint_descriptor, sizeof(struct endpoint_descriptor)
	 * device_specific_config (headset), sizeof(struct ssp_device_specific_config)
	 * formats_config (formats_count), sizeof(struct formats_config)
	 * format_config (waveex), sizeof(struct format_config)
	 * vendor_blob sizeof(vendor_blob)
	 */

	ret = ssp_get_params(tplg_pp, dai_index, &virtualbus_id, &formats_count);
	if (ret < 0) {
		fprintf(stderr, "nhlt_ssp_get_ep: ssp_get_params failed\n");
		return ret;
	}

	ep.link_type = NHLT_LINK_TYPE_SSP;
	ep.instance_id = 0;
	ep.vendor_id = NHLT_VENDOR_ID_INTEL;
	ep.device_id = NHLT_DEVICE_ID_INTEL_I2S_TDM;
	ep.revision_id = 0;
	ep.subsystem_id = 0;
	ep.device_type = 0;
	ep.direction = NHLT_ENDPOINT_DIRECTION_RENDER;
	/* ssp device index */
	ep.virtualbus_id = virtualbus_id;
	/* ssp config */
	ssp_conf.config.capabilities_size = 2;
	ssp_conf.device_config.virtual_slot = 0;
	ssp_conf.device_config.config_type = 0;

	/* formats_config */
	f_conf.formats_count = formats_count;

	for (i = 0; i < f_conf.formats_count; i++) {
		/* fill in wave format extensible types */
		/* 0xFFFE wave format extensible, do we need this? */
		f_conf1[i].format.wFormatTag = 0;

		ret = ssp_get_hw_params(tplg_pp, i, &sample_rate, &channel_count, &bits_per_sample);

		if (ret < 0) {
			fprintf(stderr, "nhlt_ssp_get_ep: ssp_get_hw_params failed\n");
			return ret;
		}

		f_conf1[i].format.nChannels = channel_count;
		f_conf1[i].format.nSamplesPerSec = sample_rate;
		f_conf1[i].format.wBitsPerSample = bits_per_sample;
		f_conf1[i].format.nBlockAlign = channel_count * bits_per_sample / 8;
		f_conf1[i].format.nAvgBytesPerSec = sample_rate * f_conf1[i].format.nBlockAlign;

		/* bytes after this value in this struct */
		f_conf1[i].format.cbSize = 22;
		/* actual bits in container */
		f_conf1[i].format.wValidBitsPerSample = 0;
		/* channel map not used at this time */
		f_conf1[i].format.dwChannelMask = 0;
		/* WAVE_FORMAT_PCM guid (0x0001) ? */
		f_conf1[i].format.SubFormat[0] = 0;
		f_conf1[i].format.SubFormat[1] = 0;
		f_conf1[i].format.SubFormat[2] = 0;
		f_conf1[i].format.SubFormat[3] = 0;

		ret = ssp_get_vendor_blob_size(tplg_pp, &blob_size);
		if (ret < 0) {
			fprintf(stderr, "nhlt_ssp_get_ep: dmic_get_vendor_blob_size failed\n");
			return ret;
		}
		f_conf1[i].vendor_blob.capabilities_size = blob_size;
	}

	ep.length = sizeof(struct endpoint_descriptor) +
		sizeof(struct ssp_device_specific_config) +
		sizeof(struct formats_config) +
		sizeof(struct format_config) * f_conf.formats_count +
		blob_size * f_conf.formats_count;

	/* allocate the final variable length ep struct */
	ep_target = calloc(ep.length, sizeof(uint8_t));
	if (!ep_target)
		return -ENOMEM;

	*eps = (struct endpoint_descriptor *)ep_target;

	/* copy all parsed sub arrays into the top level array */
	memcpy(ep_target, &ep, sizeof(struct endpoint_descriptor));

	ep_target += sizeof(struct endpoint_descriptor);

	memcpy(ep_target, &ssp_conf, sizeof(struct ssp_device_specific_config));
	ep_target += sizeof(struct ssp_device_specific_config);

	memcpy(ep_target, &f_conf, sizeof(struct formats_config));
	ep_target += sizeof(struct formats_config);

	/* copy all hw configs */
	for (i = 0; i < f_conf.formats_count; i++) {
		memcpy(ep_target, &f_conf1[i], sizeof(struct format_config));
		ep_target += sizeof(struct format_config);
		ret = ssp_get_vendor_blob(tplg_pp, ep_target, dai_index, i);
		if (ret < 0) {
			fprintf(stderr, "nhlt_sso_get_ep: ssp_get_vendor_blob failed\n");
			return ret;
		}
		ep_target += blob_size;
	}

	return 0;
}

/* Set ssp parameters from topology for ssp coefficient calculation.
 *
 * You can see an example of topology v2 config of ssp below. In this example the default
 * object parameters are spelled out for clarity. General parameters like sample_bits are parsed
 * with set_ssp_data and hw_config object data with set_hw_data. Ssp can have multiple hw_configs.
 * Values are saved into intermediate structs and the vendor specific blob is calculated at the end
 * of parsing with ssp_calculate.
 *
 * 	SSP."0" {
 *		id 			0
 *		direction		"duplex"
 *		name			NoCodec-0
 *		default_hw_conf_id	0
 *		sample_bits		16
 *		quirks			"lbm_mode"
 *		bclk_delay		0
 *		mclk_id 		0
 *		default_hw_config_id	0
 *		clks_control 		0
 *		frame_pulse_width	0
 *		tdm_padding_per_slot	false
 *
 *		Object.Base.hw_config."SSP0" {
 *			id	0
 *			mclk_freq	38400000
 *			bclk_freq	4800000
 *			tdm_slot_width	32
 *			format		"I2S"
 *			mclk		"codec_mclk_in"
 *			bclk		"codec_consumer"
 *			fsync		"codec_consumer"
 *			fsync_freq	48000
 *			tdm_slots	2
 *			tx_slots	3
 *			rx_slots	3
 *		}
 *	}
 */
int nhlt_ssp_set_params(struct tplg_pre_processor *tplg_pp,
			snd_config_t *cfg, snd_config_t *parent)
{
	snd_config_iterator_t i, next;
	snd_config_t *items;
	snd_config_t *n;
	const char *id;
	int ret;

	ret = set_ssp_data(tplg_pp, cfg);
	if (ret < 0)
		return ret;

	ret = snd_config_search(cfg, "Object.Base.hw_config", &items);
	if (ret < 0)
		return ret;

	snd_config_for_each(i, next, items) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_id(n, &id) < 0)
			continue;

		ret = set_hw_config(tplg_pp, n);
		if (ret < 0)
			return ret;
	}

	ret = ssp_calculate(tplg_pp);

	return ret;
}
