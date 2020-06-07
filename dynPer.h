#ifndef INC_dynPer_h
#define INC_dynPer_h

#include <ross.h>
#include "network_parameters_TOTALNUMBEROFNODES.h"
// #include "generate_graph.h"

#define ROSS_DO_NOT_PRINT 1

typedef enum airport_event_t airport_event_t;
typedef struct airport_state airport_state;
typedef struct airport_message airport_message;

enum airport_event_t
{
  ARRIVING = 1,
  DEPARTING,
  DROPPING,
  STDP_STRONG, // stdp increase weight
  STDP_WEAK    // stdp decrease weight
};

struct airport_state
{
  long int        id; // node id
  double          firing_threshold; // this is the threshold for node activations
  double          current_amplitude; // this is the current amplitude of the node
  double          max_refractory_period; // length of the refractory periods
  tw_stime        remaining_refractory_period; // the time which remains in the nodes refractory period
  tw_stime        last_fired_time;
  tw_stime        last_evaluation_time;
  tw_stime        current_time;
  // int             number_of_edge_parameters;
  int             number_of_outgoing_edges; // this is the number of outoging edges
  // tw_stime        **outgoing_edge_info;
  long int        *outgoing_edge_info_dst;
  tw_stime        *outgoing_edge_info_dist;
  tw_stime        *outgoing_edge_info_speed;
  tw_stime        *outgoing_edge_info_wgt;
  int             num_activators;
  tw_stime        activators_info[maxActivatorsList][maxActivatorsParms];  // this is the list of activators when the node fires
                                          // the number of rows is dependent on network parameters
                                          // each row represents the information for an incoming signal
                                          // 1st column represents source node
                                          // 2nd column represents source activation time
                                          // 3rd column represents source signal amplitude
  // tw_stime        *outgoing_edge_info_amplitude;

  // tw_stime     outgoing_edge_info[5000][3]; // contains the gid of the outgoing edges,
                                               // the 1st dimension contains the destination lp
                                               // the 2nd dim. contains the delay
                                               // Need to TODO
                                               // the 4rd dim. contains the inhibitory or excitatory nature of the edge
                                               // the 3th dim. weight of the edge
                                               // We would like to split the delay into the signaling speed and the distance so that we can implement STDP on the signaling speed
};

struct airport_message
{
  airport_event_t type;

  tw_stime        previous_evaluation_time; // Reverse computation, last time the node was evaluated
  double          node_activation_amplitude; // Amplitude of the node potential at node activation
  tw_stime        last_fired_time; // For reverse computation
  double          edge_sig_amplitude;
  double          edge_weight;
  double          prev_remaining_refractory_period;
  double          prev_current_amplitude;
  tw_stime        prev_last_fired_time;
  long int        signal_origin; // node id of the source signal
  tw_stime        signal_origin_time;  // time of firing of source node
  long int        signal_current_node; // node which is activated by the signal
};

// tw_stime lookahead = 0.00000001;
tw_stime lookahead = 3;
// static tw_stime lookahead = 0;
static tw_lpid	 nlp_per_pe = N; // Vivek: number of nodes
static tw_stime	 wait_time_avg = 0.0;

tw_stime stdp_update_edge_parm(tw_stime delta_t, tw_stime parm, int parm_type);

#endif
