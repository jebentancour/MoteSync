all: sender receiver

CONTIKI = /home/user/contiki
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

# Linker optimizations
SMALL = 1

# To reduce memory
CFLAGS += -ffunction-sections
LDFLAGS += -Wl,--gc-sections,--undefined=_reset_vector__,--undefined=InterruptVectors,--undefined=_copy_data_init__,--undefined=_clear_bss_init__,--undefined=_end_of_init__

CONTIKI_WITH_IPV6 = 1

ifeq ($(WITH_TSCH),1)
MODULES += core/net/mac/tsch
CFLAGS += -DWITH_TSCH=1
endif

#ifeq ($(MAKE_WITH_PERIODIC_ROUTES_PRINT),1)
#CFLAGS += -DWITH_PERIODIC_ROUTES_PRINT=1
#endif

include $(CONTIKI)/Makefile.include
