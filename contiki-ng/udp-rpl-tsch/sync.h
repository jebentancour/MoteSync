#ifndef SYNC_H_
#define SYNC_H_

/*-------------------------------- Includes ---------------------------------*/
#include "net/mac/tsch/tsch-asn.h"
/*---------------------------------------------------------------------------*/

/*------------------------------- Variables ---------------------------------*/
extern uint32_t ref;
extern struct tsch_asn_t event_ASN;
/*---------------------------------------------------------------------------*/


/*--------------------------------- Macros ----------------------------------*/

/** Este macro guarda en event_offset el inicio de slotframe y en event_ASN 
  * el ASN correspondiente 
  */
#define SYNC(ref_time, ref_ASN) do { \
    ref=ref_time; \
    event_ASN = ref_ASN; \
    } \
	while(0);

#endif /* SYNC_H*/