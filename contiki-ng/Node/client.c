/**
*   @authors MoteSync - Proyecto RSI - IIE - FIng - Udelar
*   Basado en $(CONTIKI)/examples/6tisch/simple-node/
*   y $(CONTIKI)/examples/rpl-udp/udp-{client,server}.c
*/
#include "contiki.h"
#include "net/routing/routing.h"
#include "random.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "dev/user-sensor.h"
#include "net/ipv6/uiplib.h"
#include "sys/node-id.h"
#include "net/mac/tsch/tsch-asn.h"
#include <stdio.h>
#include "dev/user-sensor.h"
#include "sys/rtimer.h"

#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

/*---------------------------------------------------------------------------*/

#include "data.h"


/*---------------------------------------------------------------------------*/
/* Create a structure and pointer to store the data to be sent as payload */
/*
static struct my_msg_t msg;
static struct my_msg_t *msgPtr = &msg;
*/

/*---------------------------------------------------------------------------*/

static struct simple_udp_connection udp_conn;

struct data_sensor_t data_user_sensor;

/*---------------------------------------------------------------------------*/

PROCESS(udp_client_process, "UDP client");
AUTOSTART_PROCESSES(&udp_client_process);

/*---------------------------------------------------------------------------*/

static void
udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  printf("Received response '%.*s' from ", datalen, (char *) data);
  uiplib_ipaddr_print(sender_addr);
  printf("\n");

}

/*---------------------------------------------------------------------------*/

PROCESS_THREAD(udp_client_process, ev, data)
{
  static unsigned count;
  static char str[32];
  uip_ipaddr_t dest_ipaddr;
  
  PROCESS_BEGIN();

  /* Initialize UDP connection */
  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
                      UDP_SERVER_PORT, udp_rx_callback);
  SENSORS_ACTIVATE(user_sensor);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &user_sensor);
    data_user_sensor = get_data_sensor();

    if(NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) {
      /* Send to DAG root */
      printf("Sending request %u to ", count);
      uiplib_ipaddr_print(&dest_ipaddr);
      printf("\n");
	/**** Envia string****/
      snprintf(str, sizeof(str), "hello %d", count); 
      simple_udp_sendto(&udp_conn, str, strlen(str), &dest_ipaddr);
      count++;
    } else {
      printf("Not reachable yet\n");
    }
  }
  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
