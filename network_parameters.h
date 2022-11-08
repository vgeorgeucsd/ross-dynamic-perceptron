// #define N 1288
// #define N 838
// #define N 988
#define N TOTALNUMBEROFNODES // this is set by the bash script
#define INNODES 784 // this is used for printing out the cutoffs on a per node basis
#define EdgeParms 4
#define VertexParms 3
#define StimParms 3
#define SIM_TIME_LIMIT 2.1
#define maxActivatorsList 500000
#define maxActivatorsParms 6
// Network dynamics parameters
#define INIT_EDGE_SPEED 0.5 // This is the initial speed for all edges in the network
#define outgoing_sig_amplitude 1 // This is the amplitude of the outgoing signal
                                // multiplied by the weight is the total signal contribution
                                // to the target node upon signal arrival
#define simple_decay 1
#define simple_decay_exp_const 0.0093 // this is the exponential decay time constant
#define gabe_decay 0
#define factor 10

// STDP parameters
#define stdp_weight_switch 1  // turn on and off weight stdp
#define stdp_speed_switch 0  // turn on and off weight stdp
#define OLDHEBBIAN 0 // turn on (1) or off (0) the original hebbian stdp scheme
#define KLEBERGSTDP 1 // turn on or off Kleberg (Vogels) paper STDP
#define STRONG_OFF 0 // turn off STDP facilitation only
#define WEAK_OFF 0 // turn off STDP depression only
#define STRONG_MAX 0 // turn STDP facilitation to maximum always
#define WEAK_MAX 0 // turn STDP depression to maximum always


#define stdp_learning_rate 0.05
#define stdp_delta_change_params_weak_a -0.00014
//pow(10,tw_rand_unif(lp->rng))/-1000
#define stdp_delta_change_params_weak_b 10.0
#define stdp_delta_change_params_str_a 0.00008
#define stdp_delta_change_params_str_b 7.0
#define stdp_weight_min -2.0
#define stdp_weight_max 2.0
#define stdp_speed_min 0.0
#define stdp_speed_max 200.0

// STDP Parameters from Kleberg etal 2014
// Excitatory and inhibitory STDP jointly tune feedforward
// neural circuits to selectively propagate correlated spiking
// activity

// These parameters are from the body of the text
#define stdp_alpha_LTP_exc 1.0 // LTP coefficient for excitatory synapse
#define stdp_alpha_LTD_exc -0.5 // LTP coefficient for excitatory synapse


// These parameters are from table 1
#define stdp_sfc_tau_LTD_pre_exc 0.034 // seconds, eSTDP window time constant
#define stdp_sfc_tau_LTP_post_exc 0.017 // seconds, eSTDP window time constant
#define stdp_sfc_tau_pre_inh 0.030 // seconds
#define stdp_sfc_tau_post_inh 0.030 // seconds
#define stdp_sfc_w0 0.065 // weight units
#define stdp_sfc_eta_exc 0.02535 // w0*0.39 excitatory learning rate
#define stdp_sfc_eta_inh 0.075 // inhibitory learning rate
#define stdp_sfc_alpha 0.2 // inhibitory presynaptic single-spike contribution
#define stdp_sfc_C_LTD 5.0 // long term depression scaling for eSTDP
#define stdp_sfc_C_LTP 50.0 // long term potentiation scaling for eSTDP

#define stdp_ffc_w0 0.08 // equilibrium weight for eSTDP onto output neuron
#define stdp_ffc_eta_exc 0.0624 // excitatory learning rate
#define stdp_ffc_eta_inh 0.02 // inhibitory learning rate
#define stdp_ffc_tau_pre_inh 0.020 // seconds, iSTDP window time constant
#define stdp_ffc_tau_post_inh 0.020 // seconds, iSTDP window time constant
#define stdp_ffc_w0_inh 0.16 // equilibrium weight for eSTDP onto interneurons

// these parameters are from table 2
#define stdp_hebbian_p -1.0 // iSTDP hebbian window parameter
#define stdp_hebbian_q 1.5 // iSTDP hebbian window parameter
#define stdp_anti_hebbian_p 1.5 // iSTDP anti-hebbian window parameter
#define stdp_anti_hebbian_q -1.0 // iSTDP anti-hebbian window parameter
#define stdp_symmetric_p 1.5 // iSTDP symmetric window parameter
#define stdp_symmetric_q 1.5 // iSTDP symmetric window parameter
#define stdp_symmetric_Equal_p 0.25 // iSTDP symmetric equal LTP/LTD window parameter
#define stdp_symmetric_Equal_q 0.25 // iSTDP symmetric window LTP/LTD parameter


// Outputs
#define VIVEK_DEBUG 0
#define VIVEK_DEBUGx 0
#define VIVEK_DEBUG1 0
#define printActivatorList_to_File 0
#define OUTPUT_FILENAME_LENGTH 200
#define STRING_SIZE_OF_NODE_IDS 10
#define OUTPUT_EDGE_WEIGHTS_TO_STDOUT 0
#define OUTPUT_ACT_LST_TO_STDOUT 0
#define CUTOFF_TIMES 0.1 // this and the next variable need to be checked to be on the same scale
#define OUTPUT_MSG_CUTOFFS 1
