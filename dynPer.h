#ifndef INC_dynPer_h
#define INC_dynPer_h

#include <ross.h>

#define ROSS_DO_NOT_PRINT 1

typedef enum airport_event_t airport_event_t;
typedef struct airport_state airport_state;
typedef struct airport_message airport_message;

enum airport_event_t
{
	ARRIVING = 1,
	DEPARTING,
	DROPPED
};

struct airport_state
{
        int             id; // node id
	double		firing_threshold; // this is the threshold for node activations
	double		current_amplitude; // this is the current amplitude of the node
	double		max_refractory_period; // length of the refractory periods

        tw_stime        remaining_refractory_period; // the time which remains in the nodes refractory period
	tw_stime        last_fired_time;
	tw_stime	last_evaluation_time;
        int             number_of_edge_parameters;
	int		number_of_outgoing_edges; // this is the number of outoging edges
        tw_stime        **outgoing_edge_info;
       	// tw_stime	outgoing_edge_info[5000][3]; // contains the gid of the outgoing edges,
                                                 // the 1st dimension contains the destination lp
                                                 // the 2nd dim. contains the delay
                                                 // Need to TODO
                                                 // the 4rd dim. contains the inhibitory or excitatory nature of the edge
                                                 // the 3th dim. weight of the edge
                                                 // We would like to split the delay into the signaling speed and the distance so that we can implement STDP on the signaling speed
};

struct airport_message
{
	airport_event_t	 type;

	tw_stime	 previous_evaluation_time; // Reverse computation, last time the node was evaluated
        double           node_activation_amplitude; // Amplitude of the node potential at node activation
        tw_stime        last_fired_time; // For reverse computation
        double          edge_weight;
        double          prev_remaining_refractory_period;
        double          prev_current_amplitude;
        tw_stime        prev_last_fired_time;
        int             signal_origin;
};

tw_stime lookahead = 0.00000001;
// static tw_stime lookahead = 0;
static tw_lpid	 nlp_per_pe = 3; // Vivek: number of nodes
static tw_stime	 wait_time_avg = 0.0;

#endif
