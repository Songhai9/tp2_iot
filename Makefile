CONTIKI_PROJECT = sender-udp receiver-udp
all: $(CONTIKI_PROJECT)

CONTIKI = ../

MAKE_MAC = MAKE_MAC_CSMA
MAKE_NET = MAKE_NET_IPV6
MAKE_ROUTING = MAKE_ROUTING_NULLROUTING

include $(CONTIKI)/Makefile.include
include $(CONTIKI/Makefile.identify-target)