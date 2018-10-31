#ifndef __PROJECT_CONF_H__
#define __PROJECT_CONF_H__

#undef  NETSTACK_CONF_RADIO
#define NETSTACK_CONF_RADIO         cc1200_driver
#define NETSTACK_CONF_RDC           nullrdc_driver
#define CC1200_CONF_USE_GPIO2       1
#define CC1200_CONF_USE_RX_WATCHDOG 0
#define ANTENNA_SW_SELECT_DEF_CONF  ANTENNA_SW_SELECT_SUBGHZ
#define RPL_MRHOF_CONF_MAX_LINK_METRIC 10000

#if WITH_TSCH == 1
/*---------------------------------------------------------------------------*/
/* Enable RPL non-storing mode.
 */

/* No need for routes */
#undef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES 0

/* Mode of operation */
#undef RPL_CONF_MOP
#define RPL_CONF_MOP RPL_MOP_NON_STORING

/*---------------------------------------------------------------------------*/
/* Netstack configuration for TSCH
 *
 * The netstack configuration is typically overridden by the platform
 * configuration, as defined in contiki-conf.h
 */

/* NETSTACK_CONF_FRAMER specifies the over-the-air frame format used
   by Contiki radio packets. For IEEE 802.15.4 radios, use the
   framer_802154 driver. */
#undef NETSTACK_CONF_FRAMER
#define NETSTACK_CONF_FRAMER framer_802154

/* NETSTACK_CONF_RDC specifies the Radio Duty Cycling (RDC) layer. The
   nullrdc_driver never turns the radio off and is compatible with all
   radios, but consumes a lot of power. The contikimac_driver is
   highly power-efficent and allows sleepy routers, but is not
   compatible with all radios. */
#undef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC nullrdc_driver

/* NETSTACK_CONF_MAC specifies the Medium Access Control (MAC)
   layer. The nullmac_driver does not provide any MAC
   functionality. The csma_driver is the default CSMA MAC layer, but
   is not compatible with all radios. */
#undef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC tschmac_driver



/*---------------------------------------------------------------------------*/
/* TSCH configuration options.
 */

/* If you are running with RPL, it is recommended to enable the `tsch-rpl` module. */
/* TSCH and RPL callbacks */
#define RPL_CALLBACK_PARENT_SWITCH tsch_rpl_callback_parent_switch
#define RPL_CALLBACK_NEW_DIO_INTERVAL tsch_rpl_callback_new_dio_interval
#define TSCH_CALLBACK_JOINING_NETWORK tsch_rpl_callback_joining_network
#define TSCH_CALLBACK_LEAVING_NETWORK tsch_rpl_callback_leaving_network


/* On CC2420-based platforms, enable SFD timestamps: */
/* Disable DCO calibration (uses timerB) */
/*#undef DCOSYNCH_CONF_ENABLED
#define DCOSYNCH_CONF_ENABLED 0*/
/* Enable SFD timestamps (uses timerB) */
/*#undef CC2420_CONF_SFD_TIMESTAMPS
#define CC2420_CONF_SFD_TIMESTAMPS 1*/

/*---------------------------------------------------------------------------*/
/* TSCH configuration
 */

/* Start TSCH automatically after init? If not, the upper layers
 * must call NETSTACK_MAC.on() to start it. Useful when the
 * application needs to control when the nodes are to start
 * scanning or advertising.*/
#define TSCH_CONF_AUTOSTART 0

/*---------------------------------------------------------------------------*/
/* TSCH log configuration */

/* TSCH log levels:
 * 0: no log
 * 1: basic PRINTF enabled
 * 2: basic PRINTF enabled and tsch-log module enabled */
#define TSCH_LOG_CONF_LEVEL 0

/*---------------------------------------------------------------------------*/
/* TSCH schedule configuration */

/* Initializes TSCH with a 6TiSCH minimal schedule */
#define TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL 1

/* 6TiSCH Minimal schedule slotframe length.
 * Larger values result in less frequent active slots: reduces capacity and saves energy. */
#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH 3

/*---------------------------------------------------------------------------*/
/*MODIFICAR ESTO EN LA TAREA 2 PARTE 3*/

/* TSCH conf configuration */

/* Default hopping sequence, used in case hopping sequence ID == 0
 * Hopping sequence options:
 *   TSCH_HOPPING_SEQUENCE_16_16 (uint8_t[]){ 16, 17, 23, 18, 26, 15, 25, 22, 19, 11, 12, 13, 24, 14, 20, 21 }
 *   TSCH_HOPPING_SEQUENCE_4_16 (uint8_t[]){ 20, 26, 25, 26, 15, 15, 25, 20, 26, 15, 26, 25, 20, 15, 20, 25 }
 *   TSCH_HOPPING_SEQUENCE_4_4 (uint8_t[]){ 15, 25, 26, 20 }
 *   TSCH_HOPPING_SEQUENCE_2_2 (uint8_t[]){ 20, 25 }
 *   TSCH_HOPPING_SEQUENCE_1_1 (uint8_t[]){ 20 }
 */
#define TSCH_CONF_DEFAULT_HOPPING_SEQUENCE TSCH_HOPPING_SEQUENCE_2_2

/* Hopping sequence used for joining (scan channels) */
#define TSCH_CONF_JOIN_HOPPING_SEQUENCE TSCH_DEFAULT_HOPPING_SEQUENCE

/* Keep radio always on within TSCH timeslot (1) or turn it off between packet and ACK? (0) */
#define TSCH_CONF_RADIO_ON_DURING_TIMESLOT 0

/* How long to scan each channel in the scanning phase */
#define TSCH_CONF_CHANNEL_SCAN_DURATION CLOCK_SECOND

/*---------------------------------------------------------------------------*/
/* 802.15.4 frame configuration
 */

/* IEEE802.15.4 frame version */
#undef FRAME802154_CONF_VERSION
#define FRAME802154_CONF_VERSION FRAME802154_IEEE802154E_2012


/*---------------------------------------------------------------------------*/
/* Other system configuration options.
 */

/* Save some space to fit the limited RAM of the z1 */

#undef UIP_CONF_TCP
#define UIP_CONF_TCP 0

#undef QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM 3

#undef RPL_NS_CONF_LINK_NUM
#define RPL_NS_CONF_LINK_NUM  8

#undef NBR_TABLE_CONF_MAX_NEIGHBORS
#define NBR_TABLE_CONF_MAX_NEIGHBORS 8

#undef UIP_CONF_ND6_SEND_NS
#define UIP_CONF_ND6_SEND_NS 0

#undef SICSLOWPAN_CONF_FRAG
#define SICSLOWPAN_CONF_FRAG 0

#endif /* WITH_TSCH */

#endif /* __PROJECT_CONF_H__ */
