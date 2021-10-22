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

#include <errno.h>
#include <stdio.h>
#include <alsa/input.h>
#include <alsa/output.h>
#include <alsa/conf.h>
#include <alsa/error.h>
#include "pre-processor.h"
#include "nhlt-processor.h"
#include "nhlt.h"
#include "intel/dmic-nhlt.h"
#include "intel/ssp-nhlt.h"

#define MAX_ENDPOINT_COUNT 20

static int add_bytes_to_data_section(struct tplg_pre_processor *tplg_pp, char *src)
{
	const char *data_name = "nhlt";
	snd_config_t *top_data_section;
	snd_config_t *bytes;
	snd_config_t *nhlt;
	int ret;

	/* add the nhlt bytes as new config into existing SectionData*/
	ret = snd_config_search(tplg_pp->output_cfg, "SectionData", &top_data_section);
	if (ret < 0)
		return ret;

	ret = tplg_config_make_add(&nhlt, data_name, SND_CONFIG_TYPE_COMPOUND, top_data_section);
	if (ret < 0)
		return ret;

	ret = tplg_config_make_add(&bytes, "bytes", SND_CONFIG_TYPE_STRING, nhlt);
	if (ret < 0) {
		snd_config_delete(nhlt);
		return ret;
	}

	ret = snd_config_set_string(bytes, src);
	if (ret < 0) {
		snd_config_delete(nhlt);
		snd_config_delete(bytes);
		return ret;
	}

	return 0;
}

static int add_to_manifest(struct tplg_pre_processor *tplg_pp, char *src)
{
	const char *data_name = "nhlt";
	snd_config_t *manifest;
	snd_config_t *man_inst;
	snd_config_t *n;
	snd_config_t *cfg_new;
	snd_config_t *data;
	snd_config_iterator_t i, next;
	const char *id;
	char *id_name;
	int ret, id_num = 0;

	/* find manifest section and its named instance */
	ret = snd_config_search(tplg_pp->output_cfg, "SectionManifest", &manifest);
	if (ret < 0)
		return 0;

	snd_config_for_each(i, next, manifest) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_id(n, &id) < 0)
			continue;

		if (!strcmp(id, "sof_manifest"))
			man_inst = n;
	}

	/* if manifest not found, just exit */
	if (!man_inst)
		return 0;

	ret = add_bytes_to_data_section(tplg_pp, src);
	if (ret < 0)
		return ret;

	/* add the reference to the bytes into manifest data */
	ret = snd_config_search(man_inst, "data", &data);
	if (ret < 0)
		return ret;

	snd_config_for_each(i, next, data) {
		const char *name;

		n = snd_config_iterator_entry(i);
		if (snd_config_get_string(n, &name) < 0)
			continue;

		/* item already exists */
		if (!strcmp(name, data_name))
			return 0;
		id_num++;
	}

	/* add new item */
	id_name = tplg_snprintf("%d", id_num);
	if (!id_name)
		return -ENOMEM;

	ret = snd_config_make(&cfg_new, id_name, SND_CONFIG_TYPE_STRING);
	free(id_name);
	if (ret < 0)
		return ret;

	ret = snd_config_set_string(cfg_new, data_name);
	if (ret < 0)
		return ret;

	ret = snd_config_add(data, cfg_new);
	if (ret < 0)
		snd_config_delete(cfg_new);

	return 0;
}

static int print_nhlt_as_hex_bytes(struct nhlt *blob, struct endpoint_descriptor **eps, char *dst)
{
	uint8_t *top_p = (uint8_t *)blob;
	struct endpoint_descriptor *ep;
	uint8_t *ep_p;
	int i, j;

	for (i = 0; i < sizeof(struct nhlt); i++) {
		snprintf(dst, 6, "0x%02x,", *top_p);
		dst += 5;
		top_p++;
	}

	for (i = 0; i < blob->endpoint_count; i++) {
		ep = eps[i];
		ep_p = (uint8_t *)ep;
		printf("ep %i length is %u\n", i, ep->length);
		for (j = 0; j < ep->length; j++) {
			snprintf(dst, 6, "0x%02x,", *ep_p);
			dst += 5;
			ep_p++;
		}
	}

	/* remove the last comma... */
	dst--;
	*dst = '\0';

	return 0;
}

static bool check_for_nhlt(struct tplg_pre_processor *tplg_pp)
{
	snd_config_iterator_t i, next, i2, next2;
	snd_config_t *n, *n2, *manifest;
	const char *id;
	const char *nhlt;
	int ret;

	/* find manifest section and its named instance */
	ret = snd_config_search(tplg_pp->input_cfg, "Object.Base.manifest", &manifest);
	if (ret < 0)
		return false;

	snd_config_for_each(i, next, manifest) {
		n = snd_config_iterator_entry(i);

		if (snd_config_get_id(n, &id) < 0)
			continue;

		snd_config_for_each(i2, next2, n) {
			n2 = snd_config_iterator_entry(i2);

			if (snd_config_get_id(n2, &id) < 0)
				continue;

			if (!strcmp(id, "nhlt")) {
				if (snd_config_get_string(n2, &nhlt))
					return false;

				if (!strcmp(nhlt, "true"))
					return true;
			}
		}
	}

	return false;
}

/* called at the beginning of topology pre-processing */
int nhlt_init(struct tplg_pre_processor *tplg_pp)
{
	if (!tplg_pp)
		return -EINVAL;

	nhlt_dmic_init_params(tplg_pp);
	nhlt_ssp_init_params(tplg_pp);

	return 0;
}

/* called when exiting topology pre-processing */
int nhlt_delete(struct tplg_pre_processor *tplg_pp)
{
	if (!tplg_pp)
		return -EINVAL;

	if (tplg_pp->private_data)
		free(tplg_pp->private_data);

	return 0;
}

/* called at the end of topology pre-processing */
int nhlt_create(struct tplg_pre_processor *tplg_pp)
{
	struct endpoint_descriptor *eps[MAX_ENDPOINT_COUNT];
	struct intel_nhlt_params *nhlt;
	char *bytes_string_buffer;
	int eps_count = 0;
	struct nhlt blob;
	size_t nhlt_size;
	int ret;
	int i;

	nhlt = (struct intel_nhlt_params *)tplg_pp->private_data;
	if (!nhlt)
		return -EINVAL;

	if (!check_for_nhlt(tplg_pp))
		return 0;

	for (i = 0; i < MAX_ENDPOINT_COUNT; i++)
		eps[i] = NULL;

	/* we always have only 0 or 1 dmic ep */
	if (nhlt_dmic_get_ep_count(tplg_pp)) {
		/* the index is always 0 in dmic case */
		ret = nhlt_dmic_get_ep(tplg_pp, &eps[eps_count], 0);
		if (ret < 0)
			return ret;
		eps_count++;
	}

	/* we can have 0 to several ssp eps */
	for (i = 0; i < nhlt_ssp_get_ep_count(tplg_pp); i++) {
		ret = nhlt_ssp_get_ep(tplg_pp, &eps[eps_count], i);
		if (ret < 0)
			goto err;
		eps_count++;
	}

	/* we don't have endpoints */
	if (!eps_count)
		return 0;

	blob.efi_acpi.signature = 0;
	blob.efi_acpi.length = 0;
	blob.efi_acpi.revision = 0;
	blob.efi_acpi.checksum = 0;
	for (i = 0; i < 6; i++)
		blob.efi_acpi.oem_id[i] = 0;
	blob.efi_acpi.oem_table_id = 0;
	blob.efi_acpi.oem_revision = 0;
	blob.efi_acpi.creator_id = 0;
	blob.efi_acpi.creator_revision = 0;

	blob.endpoint_count = eps_count;

	/* get blob total size */
	nhlt_size = sizeof(struct nhlt);
	for (i = 0; i < eps_count; i++) {
		if (eps[i])
			nhlt_size += eps[i]->length;
	}

	/* add the total length to top level struct */
	blob.efi_acpi.length = nhlt_size;

	/* allocate the bytes string, every byte is alsa conf byte string format "0xA1," */
	bytes_string_buffer = calloc(nhlt_size * 5, sizeof(uint8_t));
	if (!bytes_string_buffer) {
		ret = -ENOMEM;
		goto err;
	}

	/* print the whole thing in hex characters */
	print_nhlt_as_hex_bytes(&blob, eps, bytes_string_buffer);

	ret = add_to_manifest(tplg_pp, bytes_string_buffer);

err:
	/* remove all enpoints */
	for (i = 0; i < eps_count; i++)
		free(eps[i]);

	free(bytes_string_buffer);

	return ret;
}

/* called from pre-processor deferred dai processing */
int nhlt_set_dai(struct tplg_pre_processor *tplg_pp,
		 snd_config_t *cfg, snd_config_t *parent)
{
	const char *id;
	int ret;

	if (!check_for_nhlt(tplg_pp))
		return 0;

	if (snd_config_get_id(cfg, &id) < 0)
		return -EINVAL;

	tplg_pp_debug("nhlt_set_dai id %s", id);

	/* set dai parameters here */
	if (!strncmp(id, "DMIC", 4)) {
		ret = nhlt_dmic_set_params(tplg_pp, cfg, parent);
		if (ret < 0)
			return ret;
	}

	if (!strncmp(id, "SSP", 3)) {
		ret = nhlt_ssp_set_params(tplg_pp, cfg, parent);
		if (ret < 0)
			return ret;
	}

	return 0;
}
