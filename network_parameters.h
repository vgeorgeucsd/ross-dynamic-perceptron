// #define N 1288
// #define N 838
// #define N 988
#define N 3
#define EdgeParms 4
#define VertexParms 3
#define StimParms 3
#define SIMTIMELIMIT 10
#define maxActivatorsList 100
#define maxActivatorsParms 4
// Network dynamics parameters
#define INIT_EDGE_SPEED 1 // This is the initial speed for all edges in the network
#define outgoing_sig_amplitude 1 // This is the amplitude of the outgoing signal
                                // multiplied by the weight is the total signal contribution
                                // to the target node upon signal arrival

// STDP parameters
#define stdp_weight_switch 0  // turn on and off weight stdp
#define stdp_speed_switch 0  // turn on and off weight stdp
#define stdp_learning_rate 0.05
#define stdp_delta_change_params_weak_a 0.6
#define stdp_delta_change_params_weak_b 8
#define stdp_delta_change_params_str_a 0.3
#define stdp_delta_change_params_str_b 5
#define stdp_weight_min -2
#define stdp_weight_max 2
#define stdp_speed_min 0
#define stdp_speed_max 2

// Outputs
#define VIVEK_DEBUG 0
#define printEdgeValues_to_File 1
#define printActivatorList_to_File 1
#define OUTPUT_FILENAME_LENGTH 200
#define STRING_SIZE_OF_NODE_IDS 10
#define OUTPUT_EDGE_WEIGHTS_TO_STDOUT 1
#define OUTPUT_ACT_LST_TO_STDOUT 1
