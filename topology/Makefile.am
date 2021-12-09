bin_PROGRAMS = \
	alsatplg

if USE_RST2MAN
man_MANS = alsatplg.1
endif

%.1: %.rst
	rst2man $< > $@

alsatplg_SOURCES = topology.c pre-processor.c pre-process-class.c pre-process-object.c \
		pre-process-dapm.c pre-process-dai.c nhlt/nhlt-processor.c \
		nhlt/intel/dmic-nhlt.c nhlt/intel/dmic/dmic-process.c nhlt/intel/dmic/dmic-debug.c \
		nhlt/intel/ssp-nhlt.c nhlt/intel/ssp/ssp-process.c nhlt/intel/ssp/ssp-debug.c

noinst_HEADERS = topology.h pre-processor.h \
		nhlt/nhlt-processor.h nhlt/nhlt.h nhlt/intel/intel-nhlt.h \
		nhlt/intel/dmic-nhlt.h nhlt/intel/dmic/dmic-process.h \
		nhlt/intel/dmic/dmic-intel.h nhlt/intel/dmic/dmic-internal.h \
		nhlt/intel/dmic/pdm-decim-fir.h nhlt/intel/dmic/dmic-debug.h \
		nhlt/intel/ssp-nhlt.h nhlt/intel/ssp/ssp-process.h \
		nhlt/intel/ssp/ssp-intel.h nhlt/intel/ssp/ssp-internal.h \
		nhlt/intel/ssp/ssp-debug.h

AM_CPPFLAGS = \
         -Wall -I$(top_srcdir)/include

alsatplg_LDADD = $(ALSA_TOPOLOGY_LIBS)

EXTRA_DIST = alsatplg.rst

CLEANFILES = alsatplg.1
