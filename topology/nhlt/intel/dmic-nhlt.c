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
#include "dmic-nhlt.h"
#include "dmic/dmic-process.h"

static int set_dmic_data(struct tplg_pre_processor *tplg_pp,
			 snd_config_t *dai_cfg)
{
	snd_config_iterator_t i, next;
	snd_config_t *class_cfg;
	int unmute_ramp_time_ms;
	int fifo_word_length;
	int driver_version;
	int num_pdm_active;
	int sample_rate;
	int dai_index_t;
	snd_config_t *n;
	const char *id;
	long int_val;
	int duty_min;
	int duty_max;
	int clk_min;
	int clk_max;
	int io_clk;
	int ret;

	/* get default values */
	ret = snd_config_search(tplg_pp->input_cfg, "Class.Dai.DMIC", &class_cfg);
	if (ret < 0)
		return -EINVAL;

	/* set default values */
	snd_config_for_each(i, next, class_cfg) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_id(n, &id) < 0)  {
			tplg_pp_debug("set_dmic_data no id found");
			continue;
		}

		if (!strcmp(id, "driver_version")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			driver_version = int_val;
		}

		if (!strcmp(id, "io_clk")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			io_clk = int_val;
		}

		if (!strcmp(id, "dai_index")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			dai_index_t = int_val;
		}

		if (!strcmp(id, "num_pdm_active")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			num_pdm_active = int_val;
		}

		if (!strcmp(id, "fifo_word_length")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			fifo_word_length = int_val;
		}

		if (!strcmp(id, "clk_min")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			clk_min = int_val;
		}

		if (!strcmp(id, "clk_max")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			clk_max = int_val;
		}

		if (!strcmp(id, "duty_min")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			duty_min = int_val;
		}

		if (!strcmp(id, "duty_max")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			duty_max = int_val;
		}

		if (!strcmp(id, "sample_rate")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			sample_rate = int_val;
		}

		if (!strcmp(id, "unmute_ramp_time_ms")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			unmute_ramp_time_ms = int_val;
		}
	}

	/* set object values */
	snd_config_for_each(i, next, dai_cfg) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_id(n, &id) < 0)  {
			tplg_pp_debug("set_dmic_data no id found");
			continue;
		}

		if (!strcmp(id, "driver_version")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			driver_version = int_val;
		}

		if (!strcmp(id, "io_clk")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			io_clk = int_val;
		}

		if (!strcmp(id, "dai_index")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			dai_index_t = int_val;
		}

		if (!strcmp(id, "num_pdm_active")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			num_pdm_active = int_val;
		}

		if (!strcmp(id, "fifo_word_length")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			fifo_word_length = int_val;
		}

		if (!strcmp(id, "clk_min")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			clk_min = int_val;
		}

		if (!strcmp(id, "clk_max")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			clk_max = int_val;
		}

		if (!strcmp(id, "duty_min")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			duty_min = int_val;
		}

		if (!strcmp(id, "duty_max")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			duty_max = int_val;
		}

		if (!strcmp(id, "sample_rate")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			sample_rate = int_val;
		}

		if (!strcmp(id, "unmute_ramp_time_ms")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			unmute_ramp_time_ms = int_val;
		}
	}

	return dmic_set_params(tplg_pp, dai_index_t, driver_version, io_clk, num_pdm_active,
			       fifo_word_length, clk_min, clk_max, duty_min, duty_max, sample_rate,
			       unmute_ramp_time_ms);
}

static int set_pdm_data(struct tplg_pre_processor *tplg_pp,
			snd_config_t *cfg)
{
	snd_config_iterator_t i, next;
	snd_config_t *class_cfg;
	int mic_a_enable;
	int mic_b_enable;
	snd_config_t *n;
	const char *id;
	int polarity_a;
	int polarity_b;
	int clk_edge;
	long int_val;
	int ctrl_id;
	int skew;
	int ret;

	/* get default values */
	ret = snd_config_search(tplg_pp->input_cfg, "Class.Base.pdm_config", &class_cfg);
	if (ret < 0)
		return -EINVAL;

	/* set default values */
	snd_config_for_each(i, next, class_cfg) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_id(n, &id) < 0)
			continue;

		if (!strcmp(id, "ctrl_id")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			ctrl_id = int_val;
		}

		if (!strcmp(id, "mic_a_enable")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			mic_a_enable = int_val;
		}

		if (!strcmp(id, "mic_b_enable")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			mic_b_enable = int_val;
		}

		if (!strcmp(id, "polarity_a")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			polarity_a = int_val;
		}

		if (!strcmp(id, "polarity_b")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			polarity_b = int_val;
		}

		if (!strcmp(id, "clk_edge")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			clk_edge = int_val;
		}

		if (!strcmp(id, "skew")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			skew = int_val;
		}
	}

	/* set object values */
	snd_config_for_each(i, next, cfg) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_id(n, &id) < 0)
			continue;

		if (!strcmp(id, "ctrl_id")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			ctrl_id = int_val;
		}

		if (!strcmp(id, "mic_a_enable")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			mic_a_enable = int_val;
		}

		if (!strcmp(id, "mic_b_enable")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			mic_b_enable = int_val;
		}

		if (!strcmp(id, "polarity_a")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			polarity_a = int_val;
		}

		if (!strcmp(id, "polarity_b")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			polarity_b = int_val;
		}

		if (!strcmp(id, "clk_edge")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			clk_edge = int_val;
		}

		if (!strcmp(id, "skew")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			skew = int_val;
		}
	}

	return dmic_set_pdm_params(tplg_pp, ctrl_id, mic_a_enable, mic_b_enable, polarity_a,
				   polarity_b, clk_edge, skew);
}

static int set_mic_data(struct tplg_pre_processor *tplg_pp, snd_config_t *cfg)
{
	snd_config_iterator_t i, next;
	snd_config_t *class_cfg;
	long int_val;
	int snr;
	int sensitivity;
	snd_config_t *n;
	const char *id;
	int ret;

	/* get default values */
	ret = snd_config_search(tplg_pp->input_cfg, "Class.Base.mic_extension", &class_cfg);
	if (ret < 0)
		return -EINVAL;

	/* set default values */
	snd_config_for_each(i, next, class_cfg) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_id(n, &id) < 0)
			continue;

		if (!strcmp(id, "snr")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			snr = int_val;
		}

		if (!strcmp(id, "sensitivity")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			sensitivity = int_val;
		}
	}

	/* set object values */
	snd_config_for_each(i, next, cfg) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_id(n, &id) < 0)
			continue;

		if (!strcmp(id, "snr")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			snr = int_val;
		}

		if (!strcmp(id, "sensitivity")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			sensitivity = int_val;
		}
	}

	return dmic_set_ext_params(tplg_pp, snr, sensitivity);
}

static int set_vendor_mic_data(struct tplg_pre_processor *tplg_pp, snd_config_t *cfg)
{
	snd_config_iterator_t i, next;
	snd_config_t *class_cfg;
	int mic_id;
	int mic_type;
	int location;
	int speaker_position_distance;
	int horizontal_offset;
	int vertical_offset;
	int frequency_low_band;
	int frequency_high_band;
	int direction_angle;
	int elevation_angle;
	int vertical_angle_begin;
	int vertical_angle_end;
	int horizontal_angle_begin;
	int horizontal_angle_end;
	snd_config_t *n;
	const char *id;
	long int_val;
	int ret;

	/* get default values */
	ret = snd_config_search(tplg_pp->input_cfg, "Class.Base.vendor_mic_config", &class_cfg);
	if (ret < 0)
		return -EINVAL;

	/* set default values */
	snd_config_for_each(i, next, class_cfg) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_id(n, &id) < 0)
			continue;

		if (!strcmp(id, "mic_id")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			mic_id = int_val;
		}

		if (!strcmp(id, "mic_type")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			mic_type = int_val;
		}

		if (!strcmp(id, "location")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			location = int_val;
		}

		if (!strcmp(id, "speaker_position_distance")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			speaker_position_distance = int_val;
		}

		if (!strcmp(id, "horizontal_offset")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			horizontal_offset = int_val;
		}

		if (!strcmp(id, "vertical_offset")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			vertical_offset = int_val;
		}

		if (!strcmp(id, "frequency_low_band")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			frequency_low_band = int_val;
		}

		if (!strcmp(id, "frequency_high_band")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			frequency_high_band = int_val;
		}

		if (!strcmp(id, "direction_angle")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			direction_angle = int_val;
		}

		if (!strcmp(id, "elevation_angle")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			elevation_angle = int_val;
		}

		if (!strcmp(id, "vertical_angle_begin")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			vertical_angle_begin = int_val;
		}

		if (!strcmp(id, "vertical_angle_end")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			vertical_angle_end = int_val;
		}

		if (!strcmp(id, "horizontal_angle_begin")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			horizontal_angle_begin = int_val;
		}

		if (!strcmp(id, "horizontal_angle_end")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			horizontal_angle_end = int_val;
		}
	}

	/* set object values */
	snd_config_for_each(i, next, cfg) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_id(n, &id) < 0)
			continue;

		if (!strcmp(id, "mic_id")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			mic_id = int_val;
		}

		if (!strcmp(id, "mic_type")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			mic_type = int_val;
		}

		if (!strcmp(id, "location")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			location = int_val;
		}

		if (!strcmp(id, "speaker_position_distance")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			speaker_position_distance = int_val;
		}

		if (!strcmp(id, "horizontal_offset")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			horizontal_offset = int_val;
		}

		if (!strcmp(id, "vertical_offset")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			vertical_offset = int_val;
		}

		if (!strcmp(id, "frequency_low_band")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			frequency_low_band = int_val;
		}

		if (!strcmp(id, "frequency_high_band")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			frequency_high_band = int_val;
		}

		if (!strcmp(id, "direction_angle")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			direction_angle = int_val;
		}

		if (!strcmp(id, "elevation_angle")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			elevation_angle = int_val;
		}

		if (!strcmp(id, "vertical_angle_begin")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			vertical_angle_begin = int_val;
		}

		if (!strcmp(id, "vertical_angle_end")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			vertical_angle_end = int_val;
		}

		if (!strcmp(id, "horizontal_angle_begin")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			horizontal_angle_begin = int_val;
		}

		if (!strcmp(id, "horizontal_angle_end")) {
			if (snd_config_get_integer(n, &int_val))
				return -EINVAL;
			horizontal_angle_end = int_val;
		}
	}

	return dmic_set_mic_params(tplg_pp, mic_id, mic_type, location, speaker_position_distance,
				   horizontal_offset, vertical_offset, frequency_low_band,
				   frequency_high_band, direction_angle, elevation_angle,
				   vertical_angle_begin, vertical_angle_end, horizontal_angle_begin,
				   horizontal_angle_end);
}

static int set_bytes_data(struct tplg_pre_processor *tplg_pp,
			  snd_config_t *cfg)
{
	snd_config_iterator_t i, next;
	snd_config_t *n;
	const char *bytes;
	const char *id;

	if (snd_config_get_id(cfg, &id) < 0)
		return -EINVAL;

	if (strcmp(id, "fir_coeffs"))
		return 0;

	snd_config_for_each(i, next, cfg) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_string(n, &bytes))
			return -EINVAL;

		tplg_pp_debug("set fir_coeffs for dmic: '%s' ...", bytes);
	}

	return 0;
}

/* init dmic parameters, should be called before parsing dais */
int nhlt_dmic_init_params(struct tplg_pre_processor *tplg_pp)
{
	return dmic_init_params(tplg_pp);
}

/* get dmic endpoint count */
int nhlt_dmic_get_ep_count(struct tplg_pre_processor *tplg_pp)
{
	return dmic_get_vendor_blob_count(tplg_pp);
}

int nhlt_dmic_get_ep(struct tplg_pre_processor *tplg_pp, struct endpoint_descriptor **eps,
		     int index)
{
	struct endpoint_descriptor ep;
	struct mic_array_device_specific_config mic_s_conf;
	struct mic_array_device_specific_vendor_config mic_v_conf;
	struct mic_snr_sensitivity_extension mic_ext;
	struct mic_vendor_config mic_conf;
	struct formats_config f_conf;
	struct format_config f_conf1;
	uint8_t *ep_target;
	size_t blob_size;
	int ret;
	int i;

	size_t mic_config_size;
	uint32_t sample_rate;
	uint16_t channel_count;
	uint32_t bits_per_sample;
	uint8_t array_type;
	uint8_t extension;
	uint8_t num_mics;
	uint32_t snr;
	uint32_t sensitivity;

	uint8_t type;
	uint8_t panel;
	uint32_t speaker_position_distance;
	uint32_t horizontal_offset;
	uint32_t vertical_offset;
	uint8_t frequency_low_band;
	uint8_t frequency_high_band;
	uint16_t direction_angle;
	uint16_t elevation_angle;
	uint16_t vertical_angle_begin;
	uint16_t vertical_angle_end;
	uint16_t horizontal_angle_begin;
	uint16_t horizontal_angle_end;

	/*
	 * nhlt dmic structure:
	 *
	 * endpoint_descriptor, sizeof(struct endpoint_descriptor)
	 *
	 * device_specific_config (mic), sizeof(mic_array_device_specific_config)
	 * or
	 * device_specific_config (mic), sizeof(mic_array_device_specific_vendor_config)
	 *
	 * formats_config (formats_count), sizeof(struct formats_config)
	 * format_config (waveex), sizeof(struct format_config)
	 * vendor_blob sizeof(vendor_blob)
	 */

	/* dmic ep */
	ep.link_type = NHLT_LINK_TYPE_PDM;
	ep.instance_id = 0;
	ep.vendor_id = NHLT_VENDOR_ID_INTEL;
	ep.device_id = NHLT_DEVICE_ID_INTEL_PDM_DMIC;
	ep.revision_id = 0;
	ep.subsystem_id = 0;
	ep.device_type = 0;
	ep.direction = NHLT_ENDPOINT_DIRECTION_CAPTURE;
	ep.virtualbus_id = 0;

	ret = dmic_get_params(tplg_pp, index, &sample_rate, &channel_count, &bits_per_sample,
			      &array_type, &num_mics, &extension, &snr, &sensitivity);

	if (ret) {
		fprintf(stderr, "nhlt_dmic_get_ep: dmic_get_params failed\n");
		return ret;
	}

	if (array_type == NHLT_MIC_ARRAY_TYPE_VENDOR_DEFINED) {
		mic_v_conf.config.capabilities_size = 4 + num_mics *
			sizeof(struct mic_vendor_config);
		mic_v_conf.device_config.virtual_slot = 0; /* always 0 for dmic */
		mic_v_conf.device_config.config_type = NHLT_DEVICE_CONFIG_TYPE_MICARRAY;
		mic_v_conf.number_of_microphones = num_mics;
		mic_v_conf.array_type_ex = array_type;
		/* presense of extension struct is coded into lower 4 bits of array_type */
		if (extension) {
			mic_v_conf.array_type_ex = (array_type & ~0x0F) | (0x01 & 0x0F);
			mic_v_conf.config.capabilities_size +=
				sizeof(struct mic_snr_sensitivity_extension);
		}
	} else {
		mic_s_conf.config.capabilities_size = 3;
		mic_s_conf.device_config.virtual_slot = 0; /* always 0 for dmic */
		mic_s_conf.device_config.config_type = NHLT_DEVICE_CONFIG_TYPE_MICARRAY;
		mic_s_conf.array_type_ex = array_type;
		/* presense of extension struct coded into lower 4 bits of array_type */
		if (extension) {
			mic_s_conf.array_type_ex = (array_type & ~0x0F) | (0x01 & 0x0F);
			mic_s_conf.config.capabilities_size +=
				sizeof(struct mic_snr_sensitivity_extension);
		}
	}

	/* formats_config */
	f_conf.formats_count = 1;

	/* fill in wave format extensible types */
	f_conf1.format.wFormatTag = 0;
	f_conf1.format.nSamplesPerSec = sample_rate;
	f_conf1.format.nChannels = channel_count;
	f_conf1.format.wBitsPerSample = bits_per_sample;
	f_conf1.format.nBlockAlign = channel_count * bits_per_sample / 8;
	f_conf1.format.nAvgBytesPerSec = f_conf1.format.nSamplesPerSec * f_conf1.format.nBlockAlign;

	/* bytes after this value in this struct */
	f_conf1.format.cbSize = 22;
	/* actual bits in container */
	f_conf1.format.wValidBitsPerSample = 0;
	/* channel map not used at this time */
	f_conf1.format.dwChannelMask = 0;
	/* WAVE_FORMAT_PCM guid (0x0001) ? */
	f_conf1.format.SubFormat[0] = 0;
	f_conf1.format.SubFormat[1] = 0;
	f_conf1.format.SubFormat[2] = 0;
	f_conf1.format.SubFormat[3] = 0;

	ret = dmic_get_vendor_blob_size(tplg_pp, &blob_size);
	if (ret) {
		fprintf(stderr, "nhlt_dmic_get_ep: dmic_get_vendor_blob_size failed\n");
		return ret;
	}

	f_conf1.vendor_blob.capabilities_size = blob_size;

	if (array_type == NHLT_MIC_ARRAY_TYPE_VENDOR_DEFINED)
		mic_config_size = sizeof(struct mic_array_device_specific_vendor_config) +
			num_mics * sizeof(struct mic_vendor_config);

	else
		mic_config_size = sizeof(struct mic_array_device_specific_config);

	if (extension)
		mic_config_size = sizeof(struct mic_snr_sensitivity_extension);

	ep.length = sizeof(struct endpoint_descriptor) +
		mic_config_size +
		sizeof(struct formats_config) +
		sizeof(struct format_config) +
		blob_size;

	/* allocate the final variable length ep struct */
	ep_target = calloc(ep.length, sizeof(uint8_t));
	if (!ep_target)
		return -ENOMEM;

	*eps = (struct endpoint_descriptor *)ep_target;

	/* copy all parsed sub arrays into the top level array */
	memcpy(ep_target, &ep, sizeof(struct endpoint_descriptor));

	ep_target += sizeof(struct endpoint_descriptor);

	if (array_type == NHLT_MIC_ARRAY_TYPE_VENDOR_DEFINED) {
		memcpy(ep_target, &mic_v_conf,
		       sizeof(struct mic_array_device_specific_vendor_config));
		ep_target += sizeof(struct mic_array_device_specific_vendor_config);
		for (i = 0; i < num_mics; i++) {
			ret = dmic_get_mic_params(tplg_pp, i, &type,
						  &panel, &speaker_position_distance,
					     &horizontal_offset, &vertical_offset,
						  &frequency_low_band, &frequency_high_band,
						  &direction_angle, &elevation_angle,
						  &vertical_angle_begin, &vertical_angle_end,
						  &horizontal_angle_begin, &horizontal_angle_end);

			if (ret) {
				fprintf(stderr, "nhlt_dmic_get_ep: dmic_get_mic_params failed\n");
				return ret;
			}

			mic_conf.type = type;
			mic_conf.panel = panel;
			mic_conf.speaker_position_distance = speaker_position_distance;
			mic_conf.horizontal_offset = horizontal_offset;
			mic_conf.vertical_offset = vertical_offset;
			mic_conf.frequency_low_band = frequency_low_band;
			mic_conf.frequency_high_band = frequency_high_band;
			mic_conf.direction_angle = direction_angle;
			mic_conf.elevation_angle = elevation_angle;
			mic_conf.vertical_angle_begin = vertical_angle_begin;
			mic_conf.vertical_angle_end = vertical_angle_end;
			mic_conf.horizontal_angle_begin = horizontal_angle_begin;
			mic_conf.horizontal_angle_end = horizontal_angle_end;

			memcpy(ep_target, &mic_conf, sizeof(struct mic_vendor_config));
			ep_target += sizeof(struct mic_vendor_config);
		}
	} else {
		memcpy(ep_target, &mic_s_conf, sizeof(struct mic_array_device_specific_config));
		ep_target += sizeof(struct mic_array_device_specific_config);
	}

	if (extension) {
		mic_ext.snr = snr;
		mic_ext.sensitivity = sensitivity;
		memcpy(ep_target, &mic_ext, sizeof(struct mic_snr_sensitivity_extension));
		ep_target += sizeof(struct mic_snr_sensitivity_extension);
	}

	memcpy(ep_target, &f_conf, sizeof(struct formats_config));
	ep_target += sizeof(struct formats_config);

	memcpy(ep_target, &f_conf1, sizeof(struct format_config));
	ep_target += sizeof(struct format_config);

	ret = dmic_get_vendor_blob(tplg_pp, ep_target);
	if (ret) {
		fprintf(stderr, "nhlt_dmic_get_ep: dmic_get_vendor_blob failed\n");
		return ret;
	}

	return 0;
}

/*
 * Set dmic parameters from topology for dmic coefficient calculation.
 *
 * Coefficients are recalculated in case of multiple DAIs in topology and might affect each other.
 *
 * You can see an example of topology v2 config of dmic below. In this example the default
 * object parameters are spelled out for clarity. General parameters like clk_min are parsed with
 * set_dmic_data and pdm object data with set_pdm_data. Number of pdm's can vary from 1 to 2. Values
 * are saved into intermediate structs and the vendor specific blob is calculated at the end of
 * parsing with dmic_calculate.
 *
 *	DMIC."0" {
 *		name NoCodec-6
 *		id 6
 *		index 0
 *		driver_version		1
 *		io_clk                  38400000
 *		clk_min			500000
 *		clk_max			4800000
 *		duty_min		40
 *		duty_max		60
 *		sample_rate		48000
 *		fifo_word_length	16
 *		unmute_ramp_time_ms	200
 *		num_pdm_active          2
 *
 *		# PDM controller config
 *		Object.Base.pdm_config."0" {
 *			ctrl_id	0
 * 			mic_a_enable	1
 *			mic_b_enable	1
 *			polarity_a	0
 *			polarity_b	0
 *			clk_edge	0
 *			skew		0
 *		}
 *      }
 */
int nhlt_dmic_set_params(struct tplg_pre_processor *tplg_pp,
			 snd_config_t *cfg, snd_config_t *parent)
{
	snd_config_t *items;
	int ret;
	snd_config_iterator_t i, next;
	snd_config_t *n;
	const char *id;

	/* set basic dmic data */
	ret = set_dmic_data(tplg_pp, cfg);
	if (ret < 0)
		return ret;

	/* we need to have at least one pdm object */
	ret = snd_config_search(cfg, "Object.Base.pdm_config", &items);
	if (ret < 0)
		return ret;

	snd_config_for_each(i, next, items) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_id(n, &id) < 0)
			continue;

		ret = set_pdm_data(tplg_pp, n);
		if (ret < 0)
			return ret;
	}

	/* check for microphone parameter configuration */
	ret = snd_config_search(cfg, "Object.Base.mic_extension", &items);
	if (!ret) {
		snd_config_for_each(i, next, items) {
			n = snd_config_iterator_entry(i);

			if (snd_config_get_id(n, &id) < 0)
				continue;

			ret = set_mic_data(tplg_pp, n);
			if (ret < 0)
				return ret;
		}
	}

	/* check for microphone parameter configuration */
	ret = snd_config_search(cfg, "Object.Base.vendor_mic_config", &items);
	if (!ret) {
		snd_config_for_each(i, next, items) {
			n = snd_config_iterator_entry(i);

			if (snd_config_get_id(n, &id) < 0)
				continue;

			set_vendor_mic_data(tplg_pp, n);
		}
	}

	/* check for optional filter coeffs */
	ret = snd_config_search(cfg, "Object.Base.data", &items);
	if (!ret) {
		snd_config_for_each(i, next, items) {
			n = snd_config_iterator_entry(i);

			if (snd_config_get_id(n, &id) < 0)
				continue;

			set_bytes_data(tplg_pp, n);
		}
	}

	ret = dmic_calculate(tplg_pp);

	return ret;
}
