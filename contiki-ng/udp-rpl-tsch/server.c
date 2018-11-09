#include "contiki.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uiplib.h"

////////////////////////////////////////////////////////////////////////////

#include "decoder.h"

#include "net/mac/tsch/tsch-asn.h"
uint32_t ref;
uint32_t event_offset;
struct tsch_asn_t event_ASN;

////////////////////////////////////////////////////////////////////////////


#define WITH_SERVER_REPLY  0
#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678

static struct simple_udp_connection udp_conn;

PROCESS(udp_server_process, "UDP server");
AUTOSTART_PROCESSES(&udp_server_process);
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
  printf("Received request '%.*s' from ", datalen, (char *) data);
  uiplib_ipaddr_print(sender_addr);
  printf("\n");

  /*-------------------------------------------*/
  /* RECEIVER                                  */
  /*-------------------------------------------*/
  
 // decoder_receiver(data);
  
  /*-------------------------------------------*/
  /* END RECEIVER                              */
  /*-------------------------------------------*/

//#if WITH_SERVER_REPLY
  /* send back the same string to the client as an echo reply */
  //printf("Sending response.\n");
  //simple_udp_sendto(&udp_conn, data, datalen, sender_addr);
//#endif /* WITH_SERVER_REPLY */


}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_server_process, ev, data)
{

 // static struct etimer print_timer;
  PROCESS_BEGIN();

  /* Initialize DAG root */
  NETSTACK_ROUTING.root_start();

  /* Initialize UDP connection */
  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL,
                      UDP_CLIENT_PORT, udp_rx_callback);

  /* Print timer */
//  etimer_set(&print_timer, CLOCK_SECOND * 30);
  /*while(1) {
    PROCESS_WAIT_EVENT();

    if (etimer_expired(&print_timer)) {

      etimer_reset(&print_timer);

  */
      
      /*-------------------------------------------*/
      /* RECIEVER                                  */
      /*-------------------------------------------*/
      
     /* decoder_process();
      */
      /*-------------------------------------------*/
      /* END RECIEVER                              */
      /*-------------------------------------------*/
  /*  }
  }
  */

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
