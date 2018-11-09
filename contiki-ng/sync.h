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
#define SYNC(log_type, init_code) do { \
    struct tsch_log_t *log = tsch_log_prepare_add(); \
    if(log != NULL) { \
      log->type = (log_type); \
      init_code; \
      tsch_log_commit(); \
    } \
} while(0);

#endif /* SYNC_H*/
