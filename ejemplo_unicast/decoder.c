#include "decoder.h"

#include "stdio.h"
#include "math.h"

#define TICKS_PER_S 32768
#define TIMESLOTS_PER_S 100

#define PROPAGATION_CM_S 33100

struct my_node_t {
  uint8_t event_valid;
  uint8_t id;
  uint32_t x_pos; // cm
  uint32_t y_pos; // cm
  uint32_t event_asn_ls4b;
  uint32_t event_asn_ms1b;
  uint32_t event_offset; // ticks
};

static struct my_node_t my_nodes[8];
static uint8_t node_count;
static uint32_t max_dist;
static uint32_t min_ticks;

void decoder_receiver(const uint8_t *data)
{
	/* Create a pointer to the received data, adjust to the expected structure */
  	struct my_msg_t *msgPtr = (struct my_msg_t *) data;

	printf("ID: %d, X pos: %lu cm, Y pos: %lu cm, ASN: %lu, Offset: %lu ticks.\n", 
		msgPtr->id, msgPtr->x_pos, msgPtr->y_pos, msgPtr->event_asn_ls4b, msgPtr->event_offset);
	
	if (node_count == 0){
		printf("First node!\n");
		struct my_node_t new_node;
		new_node.event_valid = 1;
		new_node.id = msgPtr->id;
		new_node.x_pos = msgPtr->x_pos;
		new_node.y_pos = msgPtr->y_pos;
		new_node.event_asn_ls4b = msgPtr->event_asn_ls4b;
		new_node.event_offset = msgPtr->event_offset;
		my_nodes[node_count] = new_node;
		node_count++;
	} else {
		uint8_t is_new;
		is_new = 1;
		int i;		
		for (i = 0; i < node_count; i++){
			if(my_nodes[i].id == msgPtr->id){
				is_new = 0;
                		printf("Updating values for node %d: ", my_nodes[i].id);
                
                		my_nodes[i].x_pos = msgPtr->x_pos;
                		my_nodes[i].y_pos = msgPtr->y_pos;
                		
				if ((msgPtr->event_asn_ls4b >= my_nodes[i].event_asn_ls4b) && (msgPtr->event_offset > my_nodes[i].event_offset)){
		        		
					uint32_t delta_ticks = TICKS_PER_S * (msgPtr->event_asn_ls4b - my_nodes[i].event_asn_ls4b) / TIMESLOTS_PER_S
								+  msgPtr->event_offset - my_nodes[i].event_offset;
		        		
					printf("delta_ticks = %lu, ", delta_ticks);
					
		        		if (delta_ticks >= min_ticks){
		            			printf("value correct\n");
		            
		            			my_nodes[i].event_asn_ls4b = msgPtr->event_asn_ls4b;
		            			my_nodes[i].event_offset = msgPtr->event_offset;
		            
		            			my_nodes[i].event_valid = 1;
		        		} else {
		            			printf("too soon\n");
		        		}
				} else {
					printf("old event\n");
				}
			}
		}
		if (is_new == 1){
			printf("New node!\n");
			struct my_node_t new_node;
			new_node.event_valid = 1;
			new_node.id = msgPtr->id;
			new_node.x_pos = msgPtr->x_pos;
			new_node.y_pos = msgPtr->y_pos;
			new_node.event_asn_ls4b = msgPtr->event_asn_ls4b;
			new_node.event_offset = msgPtr->event_offset;
			my_nodes[node_count] = new_node;
			node_count++;
		}        
	}
}

void decoder_process(void)
{ 
	if (node_count >= 3){
		uint8_t all_valid;
		all_valid = 1;
		
		printf("Calculating distance... ");

		max_dist = 0;
		uint8_t i;
		uint8_t j;
		for (i = 0; i < node_count; i++){
		    for (j = 0; j < node_count; j++){
		        uint32_t d2 = (my_nodes[i].x_pos - my_nodes[j].x_pos) * (my_nodes[i].x_pos - my_nodes[j].x_pos)
					 + (my_nodes[i].y_pos - my_nodes[j].y_pos) * (my_nodes[i].y_pos - my_nodes[j].y_pos);
			uint32_t d = sqrt(d2);
		        if (d > max_dist){
		            max_dist = d;
		        }
		    }
		    all_valid &= my_nodes[i].event_valid;            
		}

		min_ticks = TICKS_PER_S * max_dist / PROPAGATION_CM_S;

		printf("max_dist = %lu cm, min_ticks = %lu.\n", max_dist, min_ticks);
		
		if (all_valid){
		    printf("All data valid!\n");
		}
		
	} else {
        	max_dist = 0;
        	printf("Not enouth nodes!\n");
	}
}
