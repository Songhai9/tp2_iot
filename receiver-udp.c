#include "contiki.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include <string.h>
#include <stdio.h>
#include "sys/log.h"
#include "project-conf.h"

#define LOG_MODULE "TP2"
#define LOG_LEVEL LOG_LEVEL_INFO

#define PORT_SENDER 1111
#define PORT_RECV 2222

PROCESS(udp_receiver, "Receiver UDP");
AUTOSTART_PROCESSES(&udp_receiver);

static struct simple_udp_connection udp_conn;

// Callback for receiving UDP messages
void udp_rx_callback(struct simple_udp_connection *c,
                     const uip_ipaddr_t *src,
                     uint16_t src_port,
                     const uip_ipaddr_t *dest,
                     uint16_t dest_port,
                     const uint8_t *data,
                     uint16_t datalen)
{
  LOG_INFO("Received %d bytes from ", datalen);
  LOG_INFO_6ADDR(src);
  LOG_INFO_(": %.*s\n", datalen, (char *)data);
}

PROCESS_THREAD(udp_receiver, ev, data)
{
  PROCESS_BEGIN();

  // Initialize UDP connection
  simple_udp_register(&udp_conn,
                      PORT_RECV,
                      NULL,
                      PORT_SENDER,
                      udp_rx_callback);

  LOG_INFO("UDP Receiver Initialized\n");

  PROCESS_END();
}
