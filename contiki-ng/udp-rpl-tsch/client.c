/**
*   @authors MoteSync - Proyecto RSI - IIE - FIng - Udelar
*   Basado en $(CONTIKI)/examples/6tisch/simple-node/
*   y $(CONTIKI)/examples/rpl-udp/udp-{client,server}.c
*/
#include "contiki.h"
//#include "net/routing/routing.h"
#include "random.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "dev/user-sensor.h"
#include "net/ipv6/uiplib.h"
#include "sys/node-id.h"
#include "net/mac/tsch/tsch.h"



#define WITH_SERVER_REPLY  1
#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678

/*---------------------------------------------------------------------------*/

#include "data.h"

/* Create a structure and pointer to store the data to be sent as payload */
static struct my_msg_t msg;
static struct my_msg_t *msgPtr = &msg;

#include "net/mac/tsch/tsch-asn.h"
uint32_t ref;
uint32_t event_offset;
struct tsch_asn_t event_ASN;

/*---------------------------------------------------------------------------*/

static struct simple_udp_connection udp_conn;

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
  //static char str[32];
  uip_ipaddr_t dest_ipaddr;
  
  PROCESS_BEGIN();
  event_offset = 0;
  event_ASN.ls4b = 0;
  event_ASN.ms1b = 0;
  /* Initialize UDP connection */
  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
                      UDP_SERVER_PORT, udp_rx_callback);
  SENSORS_ACTIVATE(user_sensor);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &user_sensor);
    event_offset = ((uint32_t) user_sensor.value(0)) - ref;
    //debug printf("reftime %u",ref_time);
    if(NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr)) 
    {
      /*-------------------------------------------*/
      /*            Send to DAG root               */
      /*-------------------------------------------*/
      
      msg.id = node_id;
      msg.x_pos = 0;
      msg.y_pos = 0;
      
      msg.event_asn_ls4b = event_ASN.ls4b;
      msg.event_asn_ms1b = event_ASN.ms1b;
      
      msg.event_offset = event_offset;
      

      printf("Sending request %u to ", count);
      uiplib_ipaddr_print(&dest_ipaddr);
      printf("\n");
      count++;

      printf("1 ID: %#06x, X pos: %lu cm, Y pos: %lu cm, ASN: %lu, Offset: %lu ticks.\n", 
        msg.id, msg.x_pos, msg.y_pos, msg.event_asn_ls4b, msg.event_offset);
      
      simple_udp_sendto(&udp_conn, msgPtr, sizeof(msg), &dest_ipaddr);
      
      /*
      msg.id = 16;
      msg.x_pos = 0;
      msg.y_pos = 324;
      
      msg.event_asn_ls4b = 0;
      msg.event_asn_ms1b = 0;
      
      msg.event_offset = 255;
      

      printf("Sending request %u to ", count);
      uiplib_ipaddr_print(&dest_ipaddr);
      printf("\n");
      count++;

      printf("ID: %d, X pos: %lu cm, Y pos: %lu cm, ASN: %lu, Offset: %lu ticks.\n",
        msg.id, msg.x_pos, msg.y_pos, msg.event_asn_ls4b, msg.event_offset);
      
      simple_udp_sendto(&udp_conn, msgPtr, sizeof(msg), &dest_ipaddr);
      msg.id = 17;
      msg.x_pos = 500;
      msg.y_pos = 0;
      
      msg.event_asn_ls4b = 1;
      msg.event_asn_ms1b = 0;
      
      msg.event_offset = 95;
      

      printf("Sending request %u to ", count);
      uiplib_ipaddr_print(&dest_ipaddr);
      printf("\n");
      count++;
      printf("ID: %d, X pos: %lu cm, Y pos: %lu cm, ASN: %lu, Offset: %lu ticks.\n",
        msg.id, msg.x_pos, msg.y_pos, msg.event_asn_ls4b, msg.event_offset);
      
      simple_udp_sendto(&udp_conn, msgPtr, sizeof(msg), &dest_ipaddr);      
      */

      /*-------------------------------------------*/
      /* END SENDER                                */
      /*-------------------------------------------*/

    } 
    else 
    {
      printf("Not reachable yet\n");
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/