#include "contiki.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include <string.h>
#include <stdio.h>
#include "sys/log.h"
#include "../arch/platform/nrf52840/common/temperature-sensor.h"
#include "project-conf.h"
#include "sys/clock.h"

#define LOG_MODULE "TP2"
#define LOG_LEVEL LOG_LEVEL_INFO


#define PORT_SENDER 1111
#define PORT_RECV 2222



/*---------------------------------------------------------------------------*/
PROCESS(udp_sender, "Sender UDP");
AUTOSTART_PROCESSES(&udp_sender);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_sender, ev, data)
{
  PROCESS_BEGIN();
  static struct simple_udp_connection udp_conn;

  uip_ipaddr_t ipaddr;
  // ipaddr = fe80::fc6e:3680:3988:62bf
  uip_ip6addr(&ipaddr, 0xfe80, 0x0000, 0x0000, 0x0000, 0xf6ce, 0x3688, 0x3988, 0x62bf);
  LOG_INFO("Sender UDP");
  LOG_INFO_6ADDR(&ipaddr);

  // Initialize UDP connection
  simple_udp_register(&udp_conn,
                     PORT_SENDER,            
                     &ipaddr,         
                     PORT_RECV,              
                     NULL);

  static struct etimer timer;
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    LOG_INFO("Send\n");
    simple_udp_send(&udp_conn, "Maiga", 6);
    etimer_reset(&timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
