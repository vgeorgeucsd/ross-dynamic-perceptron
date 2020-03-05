#ifndef INC_dynPer_h
#define INC_dynPer_h

#include <ross.h>

#define ROSS_DO_NOT_PRINT 1

typedef enum airport_event_t airport_event_t;
typedef struct airport_state airport_state;
typedef struct airport_message airport_message;

enum airport_event_t
{
	ARRIVAL = 1,
	FIRING,
	REFRACTORY
};

struct airport_state
{
        int             id; // node id
	double		firing_threshold; // this is the threshold for node activations
	double		current_amplitude; // this is the current amplitude of the node
	int		number_of_outgoing_edges; // this is the number of outoging edges
	int		max_refractory_period; // length of the refractory periods

        tw_stime        remaining_refractory_period; // the time which remains in the nodes refractory period
	tw_stime        last_fired_time;
	tw_stime	last_evaluation_time;
	float		outgoing_edge_info[][2]; // contains the gid of the outgoing edges, 
                                                 // the 1st dimension contains the destination lp
                                                 // the 2nd dim. contains the delay
                                                 // Need to TODO
                                                 // the 3rd dim. contains the inhibitory or excitatory nature of the edge
                                                 // the 4th dim. weight of the edge
                                                 // We would like to split the delay into the signaling speed and the distance so that we can implement STDP on the signaling speed

};

struct airport_message
{
	airport_event_t	 type;

	tw_stime	 previous_evaluation_time; // This stores the last time the node was evaluated
};

static tw_stime lookahead = 0.00000001;
static tw_lpid	 nlp_per_pe = 3; // Vivek: number of nodes


static tw_stime	 wait_time_avg = 0.0;

#endif
