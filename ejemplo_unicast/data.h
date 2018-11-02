/*---------------------------------------------------------------------------*/
/* This data structure is used to store the packet content (payload) */
struct my_msg_t {
  uint8_t id;
  int16_t x_pos;
  int16_t y_pos;
  uint16_t event_asn;
  uint16_t event_offset;
};

/*---------------------------------------------------------------------------*/
