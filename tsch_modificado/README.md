## README

Modificaciones respecto al original:

Agrego las siguientes variables que se definen en sender.c y receiver.c segun corresponda: 
```c
                  extern rtimer_clock_t ti;
                  extern struct asn_t ASync;
```

Tambien agrego las siguientes lineas para copiar inicio de slotframe y ASN. (linea 955)

```c
        TSCH_DEBUG_SLOT_START();
        ti = RTIMER_NOW();
	ASync = current_asn;
```

Con esto vi que por la cantidad de ticks entre cada inicio lo que estoy detectando es el inicio de slotframe (Lo probe con varios tamanos de slotframe).
Lo proximo a probar es poner esta misma linea al comienzo del while(tsch_is_associated).

