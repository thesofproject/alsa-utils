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

#ifndef __INTEL_NHLT_H
#define __INTEL_NHLT_H

#define MIN(a, b) ({		\
	typeof(a) __a = (a);	\
	typeof(b) __b = (b);	\
	__a > __b ? __b : __a;	\
})
#define MAX(a, b) ({		\
	typeof(a) __a = (a);	\
	typeof(b) __b = (b);	\
	__a < __b ? __b : __a;	\
})

#define BIT(b)                  (1UL << (b))
#define MASK(b_hi, b_lo)        (((1ULL << ((b_hi) - (b_lo) + 1ULL)) - 1ULL) << (b_lo))
#define SET_BIT(b, x)           (((x) & 1) << (b))
#define SET_BITS(b_hi, b_lo, x) (((x) & ((1ULL << ((b_hi) - (b_lo) + 1ULL)) - 1ULL)) << (b_lo))

struct intel_nhlt_params {
	void *dmic_params;
};

#endif /* __INTEL_NHLT_H */
