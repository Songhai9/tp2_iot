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

PROCESS(udp_sender, "Sender UDP");
AUTOSTART_PROCESSES(&udp_sender);

static uint8_t seq_num = 0;  // Sequence number

PROCESS_THREAD(udp_sender, ev, data)
{
  PROCESS_BEGIN();
  static struct simple_udp_connection udp_conn;

  // Define the receiver's IP address
  uip_ipaddr_t receiver_ip;
  uip_ip6addr(&receiver_ip, 0xfe80, 0x0000, 0x0000, 0x0000, 0xf6ce, 0x3654, 0x4752, 0x8eb5); // Replace with your receiver's address

  LOG_INFO("UDP Sender Initialized\n");
  LOG_INFO("Receiver IP Address: ");
  LOG_INFO_6ADDR(&receiver_ip);
  LOG_INFO_("\n");

  // Initialize the UDP connection
  simple_udp_register(&udp_conn,
                      PORT_SENDER,
                      &receiver_ip,
                      PORT_RECV,
                      NULL);

  static struct etimer timer;
  etimer_set(&timer, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read temperature value
    int temperature = temperature_sensor.value(0);

    // Build the message
    char message[50];
    snprintf(message, sizeof(message), "Seq:%d Temp:%d Maiga", seq_num++, temperature);

    // Send the UDP message
    simple_udp_send(&udp_conn, message, strlen(message));
    LOG_INFO("Sent: %s to ", message);
    LOG_INFO_6ADDR(&receiver_ip);
    LOG_INFO_("\n");

    etimer_reset(&timer);
  }

  PROCESS_END();
}
