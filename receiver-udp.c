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

/*---------------------------------------------------------------------------*/
PROCESS(udp_receiver, "Receiver UDP");
AUTOSTART_PROCESSES(&udp_receiver);
/*---------------------------------------------------------------------------*/
// fonction de réception des messages
void udp_rx_callback(struct simple_udp_connection *udp_con,
		     const uip_ipaddr_t *src,
		     uint16_t sport,
		     const uip_ipaddr_t *dest,
		     uint16_t dport,
		     const uint8_t *data,
		     uint16_t size)
{
  LOG_INFO("PASS");
}


           //%x:%x:%x:%x:%x:%x:%x:%x", udp_con.remote_addr.u16[0], udp_con.remote_addr.u16[1],
           //udp_con.remote_addr.u16[2] ,udp_con.remote_addr.u16[3],udp_con.remote_addr.u16[4],
           //udp_con.remote_addr.u16[5],udp_con.remote_addr.u16[6],udp_con.remote_addr.u16[7]);


/*---------------------------------------------------------------------------*/
struct simple_udp_connection udp_conn;


PROCESS_THREAD(udp_receiver, ev, data)
{
  PROCESS_BEGIN();

  uip_ip6addr_t send_ip;
  uip_ip6addr(&send_ip, 0xfe80, 0, 0, 0, 0xf6ce, 0x3654, 0x4752, 0x8eb5);


  LOG_INFO("UDP CONNECTION with : ");
  LOG_INFO_6ADDR(&send_ip);

   // Initialize UDP connection
  simple_udp_register(&udp_conn,
                     PORT_RECV,              
                     &send_ip,                   
                     PORT_SENDER,            
                     udp_rx_callback);

  LOG_INFO("PASSSSSS\n");
    
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

