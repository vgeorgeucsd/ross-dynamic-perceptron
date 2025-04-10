// #define N 1288
// #define N 838
// #define N 988
#define N TOTALNUMBEROFNODES // this is set by the bash script
#define INNODES 784 // this is used for printing out the cutoffs on a per node basis
#define EdgeParms 4
#define VertexParms 3
#define StimParms 3
#define SIM_TIME_LIMIT 2.0
#define maxActivatorsList 500
#define maxActivatorsParms 4
// Network dynamics parameters
#define INIT_EDGE_SPEED 0.5 // This is the initial speed for all edges in the network
#define outgoing_sig_amplitude 1 // This is the amplitude of the outgoing signal
                                // multiplied by the weight is the total signal contribution
                                // to the target node upon signal arrival
#define simple_decay 1
#define simple_decay_exp_const 0.1 // this is the exponential decay time constant
#define gabe_decay 0

// STDP parameters
#define stdp_weight_switch 1  // turn on and off weight stdp
#define stdp_speed_switch 1  // turn on and off weight stdp
#define stdp_learning_rate 0.05
#define stdp_delta_change_params_weak_a -0.6
#define stdp_delta_change_params_weak_b 8.0
#define stdp_delta_change_params_str_a 0.3
#define stdp_delta_change_params_str_b 5.0
#define stdp_weight_min -2.0
#define stdp_weight_max 2.0
#define stdp_speed_min 0.0
#define stdp_speed_max 2.0

// Outputs
#define VIVEK_DEBUG 0
#define VIVEK_DEBUGx 0
#define printActivatorList_to_File 0
#define OUTPUT_FILENAME_LENGTH 200
#define STRING_SIZE_OF_NODE_IDS 10
#define OUTPUT_EDGE_WEIGHTS_TO_STDOUT 0
#define OUTPUT_ACT_LST_TO_STDOUT 0
#define CUTOFF_TIMES 0.1 // this and the next variable need to be checked to be on the same scale
#define OUTPUT_MSG_CUTOFFS 1
