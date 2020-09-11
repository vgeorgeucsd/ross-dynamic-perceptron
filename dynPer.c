#include "dynPer_TOTALNUMBEROFNODES.h"
#include "generate_graph_TOTALNUMBEROFNODES.h"
#include "generate_stimulus_TOTALNUMBEROFNODES.h"
#include "network_parameters_TOTALNUMBEROFNODES.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
/*
  dynPer.c
  Dynamic Perceptron simulator
  20200101
  Vivek George
*/

// char edge_path[] = "/home/vivek/research/dynamic_perceptron/working_ross/ross-dynamic-perceptron/edge_input.793";
// char vertex_path[] = "/home/vivek/research/dynamic_perceptron/working_ross/ross-dynamic-perceptron/vertex_input.793";
//char edge_path[] = "/home/vivek/research/dynamic_perceptron/working_ross/ross-dynamic-perceptron/edge_input.original";
//char vertex_path[] = "/home/vivek/research/dynamic_perceptron/working_ross/ross-dynamic-perceptron/vertex_input.original";
//char stim_path[] = "/home/vivek/research/dynamic_perceptron/working_ross/ross-dynamic-perceptron/stim_input.original";
// char edge_path[] = "/home/vivek/research/dynamic_perceptron/working_ross/ross-dynamic-perceptron/edge_info_source_200.out";
// char vertex_path[] = "/home/vivek/research/dynamic_perceptron/working_ross/ross-dynamic-perceptron/vertex_info_source_200.out";


// char stim_path[]   = "/home/vivek/research/dynamic_perceptron/working_ross/fresh_pull/ross-dynamic-perceptron/stim_input.74.5";
// char edge_path[]   = "/home/vivek/research/dynamic_perceptron/working_ross/fresh_pull/ross-dynamic-perceptron/edge_info_source_TOTALNUMBEROFNODES.out";
// char vertex_path[] = "/home/vivek/research/dynamic_perceptron/working_ross/fresh_pull/ross-dynamic-perceptron/vertex_info_source_TOTALNUMBEROFNODES.out";
// char output_dir[]  = "/home/vivek/research/dynamic_perceptron/working_ross/fresh_pull/ross-dynamic-perceptron/temp_outputs/";

char stim_path[] = "TOS_STIM_PATH";
char edge_path[]   = "TOS_EDGE_PATH";
char vertex_path[] = "TOS_VERTEX_PATH";
char output_dir[]  = "TOS_OUTPUT_PATH";


// char stim_path[] = "/g/g20/george39/ross-dynamic-perceptron/stim_input.74.5";
// char edge_path[] = "/g/g20/george39/ross-dynamic-perceptron/edge_info_source_TOTALNUMBEROFNODES.out";
// char vertex_path[] = "/g/g20/george39/ross-dynamic-perceptron/vertex_info_source_TOTALNUMBEROFNODES.out";
// char output_dir[] = "/g/g20/george39/ross-dynamic-perceptron/temp_outputs/";

//char edge_path[] = "/g/g20/george39/ross-dynamic-perceptron/edge_input.original";
//char vertex_path[] = "/g/g20/george39/ross-dynamic-perceptron/vertex_input.original";
//char stim_path[] = "/g/g20/george39/ross-dynamic-perceptron/stim_input.original";
//char output_dir[] = "/g/g20/george39/ross-dynamic-perceptron/temp_outputs/";

struct Graph* graph;
struct Stim* stim;

tw_peid
mapping(tw_lpid gid)
{
  return (tw_peid) gid / g_tw_nlp;
}

void
init(airport_state * s, tw_lp * lp)
{

  unsigned long int self_id = (unsigned long int) lp->gid;
  extern struct Graph* graph;
  extern struct Stim* stim;

  // Initial values of vertices
  tw_event *e;
  airport_message *m;
  s->last_fired_time = -10; // set this to null to begin
  s->current_amplitude = 0.0;
  s->last_evaluation_time = 0.0; // set this to null

  unsigned long int numOutEdges=0;
  unsigned long int numOutEdges1=0;

  // Initialize the activators information
  int i,j;
  for(i=0;i<maxActivatorsList;i++)
    for(j=0;j<maxActivatorsParms;j++)
      s->activators_info[i][j]=-1;
  // End initializing activators information

  // Getting Graph information
  s->id = graph->head[self_id]->nid;

  s->max_refractory_period = graph->head[self_id]->ref_per;

  s->firing_threshold = graph->head[self_id]->threshold;

  s->activation_counter = 0;
  // s->number_of_edge_parameters = 2;

  struct Node* ptr = graph->head[self_id];
  while (ptr != NULL)
  {
    ptr = ptr->next;
    numOutEdges++;
  }

#if VIVEK_DEBUG1
  printf("Node %lu Parameters:\n",self_id);
  printf("\tNumber of outgoing edges: %lu\n", numOutEdges);
#endif

  s->number_of_outgoing_edges = numOutEdges;
  if(s->number_of_outgoing_edges == 0 || graph->head[self_id]->dest == -1 )
  {
    // do nothing
    s->number_of_outgoing_edges = 0;
    free(graph->head[self_id]);
    graph->head[self_id] = NULL;
  }
  else
  {
    s->outgoing_edge_info_dst = tw_calloc(TW_LOC,"oe_dest",sizeof(tw_stime *), s->number_of_outgoing_edges);
    s->outgoing_edge_info_dist = tw_calloc(TW_LOC,"oe_dist",sizeof(tw_stime *), s->number_of_outgoing_edges);
    s->outgoing_edge_info_speed = tw_calloc(TW_LOC,"oe_speed",sizeof(tw_stime *), s->number_of_outgoing_edges);
    s->outgoing_edge_info_weight = tw_calloc(TW_LOC,"oe_weight",sizeof(tw_stime *), s->number_of_outgoing_edges);

    struct Node* ptr1;
    while(graph->head[self_id] != NULL)
    {
      ptr1 = graph->head[self_id];
      s->outgoing_edge_info_dst[numOutEdges1] = ptr1->dest;
      s->outgoing_edge_info_dist[numOutEdges1] = ptr1->dist;
      s->outgoing_edge_info_speed[numOutEdges1] = INIT_EDGE_SPEED;
      s->outgoing_edge_info_weight[numOutEdges1] = ptr1->weight;
      // s->outgoing_edge_info_amplitude[numOutEdges1] = 0; // initial edge signal amplitude

#if OUTPUT_EDGE_WEIGHTS_TO_STDOUT
      tw_output(lp,"tw: Edge from Node %lu to Node %lu ; Edge Speed : %11.17lf ; Edge Weight: %11.17lf ; Modified at Current Time: 0.0 \n", self_id, s->outgoing_edge_info_dst[numOutEdges1], s->outgoing_edge_info_speed[numOutEdges1], s->outgoing_edge_info_weight[numOutEdges1]);
#endif
#if VIVEK_DEBUG
      printf("Source nid: %lu\n",self_id);
      printf("Sim Var Target nid: %lu\n", s->outgoing_edge_info_dst[numOutEdges1]);
      printf("Sim Var Edge Dist : %11.30lf\n", s->outgoing_edge_info_dist[numOutEdges1]);
      printf("In Var Edge Dist : %11.30lf\n", ptr1->dist);

#endif
      graph->head[self_id] = graph->head[self_id]->next;
      free(ptr1);
      ptr1 = NULL;
      numOutEdges1++;
    }
  }
  free(graph->head[self_id]);
  graph->head[self_id] = NULL;

// tw_event_new(destination logical process, delay , sender logical process)
//  if(lp->gid <= 100)
//  {
//    e = tw_event_new(lp->gid, 0 , lp);
//    m = tw_event_data(e);
//    m->type = ARRIVING;
//    m->edge_sig_amplitude = 1;
//    tw_event_send(e);
//  }

  // struct StimNode* stimPtr = stim->head[self_id];

  struct StimNode* stimPtr;
  while (stim->head[self_id] != NULL && stim->head[self_id]->amplitude != 0){
    stimPtr = stim->head[self_id];
    e = tw_event_new(self_id, stimPtr->delay, lp);
    m = tw_event_data(e);
    m->type = ARRIVING;
    m->edge_sig_amplitude = stimPtr->amplitude;
    m->edge_weight = 1;
    m->edge_speed = 1;
    m->signal_origin = self_id;
    m->signal_origin_time = 0;
    tw_event_send(e);
#if VIVEK_DEBUG
    printf("Stimulus event\n");
    printf("Stimulus nid: %lu\n",self_id);
    printf("Target nid: %lu\n", self_id);
    printf("Stimulus Amplitude: %Lf\n",stimPtr->amplitude);
    printf("Stimulus Delay: %Lf\n",stimPtr->delay);
#endif
    stim->head[self_id] = stim->head[self_id]->next;
    free(stimPtr);
    stimPtr = NULL;
  }
  free(stim->head[self_id]);
  stim->head[self_id] = NULL;

#if OUTPUT_MSG_CUTOFFS
  if(self_id>=INNODES){
    tw_stime dly = 0.0;
    for(;dly<=SIM_TIME_LIMIT;dly+=CUTOFF_TIMES){
      e = tw_event_new(self_id, dly, lp);
      m = tw_event_data(e);
      m->type = PRINT_EDGE_WEIGHTS_SPEEDS;
      tw_event_send(e);
      // printf("Target nid: %lu; Delay Time: %lf\n", self_id, dly);
    }
  }
#endif
}


void
event_handler(airport_state * s, tw_bf * bf, airport_message * msg, tw_lp * lp)
{
  tw_lpid dst_lp;
  tw_stime ts;
  tw_event *e;
  airport_message *m;
  unsigned long int i;
  tw_stime sig_diff, decay_value;
  s->current_time = tw_now(lp);

  switch(msg->type)
    {
      // This the integration phase of the node
      case ARRIVING:
        {
          sig_diff = s->current_time - s->last_fired_time;
#if VIVEK_DEBUG
          printf("\nSignal Arrived\n");
          printf("\t%s %li\n", "Signal Arrived from Node: ", msg->signal_origin);
          printf("\t%s %li\n", "Signal Arrived to Node: ", lp->gid);
          printf("\t\t%s %11.22lf\n", "At Time: ", s->current_time);
          // printf("\t\t%s %lf\n", "Node Last Fired At: ", s->last_fired_time);
          // printf("\t\t%s %lf\n", "Signal Difference: ", sig_diff);
          // printf("\t\t%s %lf\n", "Max Refractory Period: ", s->max_refractory_period);
#endif
          // if the node is listening
          // if(s->remaining_refractory_period >= s->max_refractory_period)
          if(sig_diff > s->max_refractory_period)
          {
            // keeping track of activators list
            s->activators_info[s->num_activators][0] = msg->signal_origin;
            s->activators_info[s->num_activators][1] = msg->signal_origin_time;
            s->activators_info[s->num_activators][2] = msg->edge_sig_amplitude;
            s->activators_info[s->num_activators][3] = s->current_time;
            s->num_activators++;
            //end keepig track of activators list
#if gabe_decay
            msg->prev_remaining_refractory_period = s->remaining_refractory_period;
            s->remaining_refractory_period = 0;
            msg->previous_evaluation_time = s->last_evaluation_time;
            ts = msg->previous_evaluation_time - s->current_time;
//            decay_value = exp(ts);

            msg->prev_current_amplitude = s->current_amplitude;
//            printf("\t\t%s %11.11lf\n", "PrevCurrentAmplitude: ", msg->prev_current_amplitude);
            double numerator,super_decay,max_value,min_value;
            super_decay = (msg->prev_current_amplitude * (1 + (0.8 * exp(0.03 * ts))))/(1 + exp(0.03 * ts));
//            printf("\t\t%s %11.11lf\n", "Super_decay: ", super_decay);
            max_value = (msg->prev_current_amplitude * (1 + (0.8 * exp(0.03 * 0))))/(1 + exp(0.03 * 0));
//            printf("\t\t%s %11.11lf\n", "Max_value: ", max_value);
            min_value = (msg->prev_current_amplitude * (1 + (0.8 * exp(0.03 * 10000))))/(1 + exp(0.03 * 10000));
//            printf("\t\t%s %11.11lf\n", "Min_value: ", min_value);
            numerator = super_decay - min_value;
//            printf("\t\t%s %11.11lf\n", "Numerator: ", numerator);
            if(max_value - min_value <= 0)
            {
               decay_value = 0;
            }
            else
            {
               decay_value = numerator / abs(max_value - min_value);
            }
//            printf("\t\t%s %11.11lf\n", "Decay_value: ", decay_value);

            s->current_amplitude = msg->edge_sig_amplitude * msg->edge_weight + s->current_amplitude * decay_value; //applying simple decay
#endif

#if simple_decay
            msg->prev_remaining_refractory_period = s->remaining_refractory_period;
            s->remaining_refractory_period = 0;
            msg->previous_evaluation_time = s->last_evaluation_time;
            ts = msg->previous_evaluation_time - s->current_time;
            decay_value = exp(simple_decay_exp_const*ts);
            msg->prev_current_amplitude = s->current_amplitude;
            s->current_amplitude = msg->edge_sig_amplitude * msg->edge_weight + s->current_amplitude * decay_value; //applying simple decay
#endif

            s->last_evaluation_time = s->current_time;

            if(s->current_amplitude >= s->firing_threshold)
            {
              (s->activation_counter)++;
              // Printing out the activators inforamtion
              // End setting p the File printing information
              for (i=0; i<s->num_activators ; i++)
              {

#if OUTPUT_ACT_LST_TO_STDOUT
                tw_output(lp,"tw: Activation Target Node: %lu, Target Node Act Time: %11.21lf, Source Node: %lf, Source Activation Time: %11.21lf, Source Signal Amplitude: %11.21lf, Source Signal Arrival Time: %11.21lf\n", s->id, s->last_evaluation_time, s->activators_info[i][0],s->activators_info[i][1],s->activators_info[i][2], s->activators_info[i][3]);
#endif

                // Sending STDP signals for strengthening edge weight
                e = tw_event_new(s->activators_info[i][0], 0.0 ,lp);
                m = tw_event_data(e);
                m->type = STDP_STRONG;
                m->signal_origin = s->activators_info[i][0];
                m->signal_origin_time = s->activators_info[i][1];
                m->signal_arrival_time = s->activators_info[i][3];
                m->last_fired_time = s->current_time; // this is when the target vertex fired
                m->signal_current_node = lp->gid;  // sends the activated nodes id
                m->prev_edge_weight = msg->edge_weight;
                m->prev_edge_speed = msg->edge_speed;
                tw_event_send(e);

                // End STDP code

                // Setting back the activators list to the initialized values after firing
                int j;
                for(j=0; j<maxActivatorsParms ; j++)
                {
                  s->activators_info[i][j]=-1;
                }
              }
              s->num_activators = 0; // clear the number of activators

              // Getting message ready for sending singals to outgoing edges
              e = tw_event_new(lp->gid, 0.0 ,lp);
              m = tw_event_data(e);
              m->type = DEPARTING;
              m->last_fired_time = s->last_fired_time;
              m->node_activation_amplitude = s->current_amplitude;
              m->prev_current_amplitude = msg->prev_current_amplitude;
              tw_event_send(e);
            }
          }
          else
          {
            // else drop the packet
#if VIVEK_DEBUG
            printf("\nNode is refractory \n");
            // Print dropped Signal information
            printf("\t\t Dropped Signal Information:\n");
            printf("\t\t Target Node: %li, Target Node Act Time: %lf, Source Node: %li, Source Activation Time: %lf, Source Signal Amplitude: %lf\n", s->id, s->last_evaluation_time, msg->signal_origin,msg->signal_origin_time,msg->edge_sig_amplitude);
            // end dropped signal information
#endif

            e = tw_event_new(lp->gid, 0.0 ,lp);
            m = tw_event_data(e);
            m->type = DROPPING;
            m->last_fired_time = s->last_fired_time;
            m->signal_origin = msg->signal_origin;
            m->signal_origin_time = msg->signal_origin_time;
            m->signal_arrival_time = s->current_time;
            m->prev_remaining_refractory_period = s->remaining_refractory_period;
            m->prev_edge_weight = msg->edge_weight;
            m->prev_edge_speed = msg->edge_speed;
            tw_event_send(e);

          }
          break;
        }

      case DEPARTING:
        {
#if VIVEK_DEBUG
          printf("\nSignals Departing\n");
          printf("\t%s %lu\n", "From Node: ", lp->gid);
          // printf("\t%s %li\n", "Node ID Fired: ", s->id);
          printf("\t%s %11.22lf\n", "At Time: ", s->current_time);
#endif
          msg->prev_current_amplitude = s->current_amplitude;
          s->current_amplitude = 0;
          msg->prev_last_fired_time = s->last_fired_time;
          s->last_fired_time = s->current_time;

          // send outgoing signals to the other nodes
          // need to do this for each outgoing edge
          if(s->number_of_outgoing_edges != 0)
          {
            for(i=0; i < s->number_of_outgoing_edges; i++)
              {
                dst_lp  = s->outgoing_edge_info_dst[i];
                ts      = s->outgoing_edge_info_dist[i]/s->outgoing_edge_info_speed[i];
                e       = tw_event_new(dst_lp, ts, lp);
                m       = tw_event_data(e);
                m->signal_origin_time = s->last_fired_time;
                m->type = ARRIVING;
                m->edge_sig_amplitude = outgoing_sig_amplitude;
                m->edge_weight = s->outgoing_edge_info_weight[i];
                m->edge_speed = s->outgoing_edge_info_speed[i];
                m->signal_origin = lp->gid;
                tw_event_send(e);
#if VIVEK_DEBUG
                printf("\t%s %lu\n", "To Node: ", dst_lp);
                // printf("\t\t%s %11.11lf\n", "Outbound Signal Delay: ", ts);
                printf("\t%s %11.22lf\n", "Outbount Signal Delay: ", ts);
                printf("\t%s %11.22lf\n", "Outbount Signal Speed: ", s->outgoing_edge_info_speed[i]);
                printf("\t%s %11.22lf\n", "Outbount Edge Distance : ", s->outgoing_edge_info_dist[i]);
#endif
              }
          }
#if VIVEK_DEBUG1
          else
            printf("\tNo outgoing signals.\n");
#endif

          break;
        }

      case DROPPING:
        {
          s->remaining_refractory_period = s->max_refractory_period - (s->current_time - msg->last_fired_time);
          s->current_amplitude = 0;
#if VIVEK_DEBUG
          printf("\n\n Node Refractory, Dropped Packet!\n");
          printf("\t%s %li\n", "Signal Arrived from Node: ", msg->signal_origin);
          printf("\t%s %11.17lf\n", "Which Fired At Time: ", msg->signal_origin_time);
          printf("\t%s %li\n", "Target Node Droping Signal: ", lp->gid);
          printf("\t%s %11.17lf\n", "Remaining Refractory Period: ", s->remaining_refractory_period);
          printf("\t%s %11.17lf\n", "Signal Arrival Time: ", s->current_time);
#endif
          // Sending STDP signals for weakening edge weight
          e = tw_event_new(msg->signal_origin, 0 ,lp);
          m = tw_event_data(e);
          m->type = STDP_WEAK;
          m->signal_origin = msg->signal_origin;
          m->signal_origin_time = msg->signal_origin_time;
          m->signal_arrival_time = msg->signal_arrival_time;
          m->last_fired_time = s->current_time; // this is when the target vertex fired
          m->signal_current_node = lp->gid;
          m->prev_edge_weight = msg->prev_edge_weight;
          m->prev_edge_speed = msg->prev_edge_speed;
          tw_event_send(e);
          // End STDP code

          break;
        }
      case STDP_STRONG:
        {

#if VIVEK_DEBUG
          printf("\n%s\n\n", "Doing STDP Weight Increase");
          printf("\t%s%li\n","Signal Source Node: ",msg->signal_origin);
          printf("\t%s%f\n","Which Fired at Time: ",msg->signal_origin_time);
          printf("\t%s%lu\n","Signal Target Node: ", msg->signal_current_node);
          printf("\t%s%f\n","Which Fired at Time: ", msg->last_fired_time);
#endif

          // defining some variables to calculate stdp changes
          // Old STDP rule based on origin node firing time
          // tw_stime delta_t = msg->signal_origin_time - msg->last_fired_time;
          // New STDP rule based on origin node signal arrival time, ie. EPSP time
          tw_stime delta_t = msg->signal_arrival_time - msg->last_fired_time;
          tw_stime delta_change = stdp_delta_change_params_str_a * powl(M_E,delta_t/stdp_delta_change_params_str_b); //the first argument of powl is eulers number

          for(i=0; i < s->number_of_outgoing_edges; i++)
          {
            if(s->outgoing_edge_info_dst[i]==msg->signal_current_node)
            {
#if VIVEK_DEBUG1
              printf("\t%s%lf","Old Edge Weight: ", s->outgoing_edge_info_weight[i]);
#endif

              // Modify the edge weight by whatever rules
              if(stdp_weight_switch)
              {
                s->outgoing_edge_info_weight[i] = stdp_update_edge_parm(delta_change,s->outgoing_edge_info_weight[i], 0); // for the weight stdp toggle
              }
#if VIVEK_DEBUG1
              printf("\t%s%lf\n"," New Edge Weight: ", s->outgoing_edge_info_weight[i]);

              // Speed Modifications
              printf("\t%s%lf","Old Edge Speed: ", s->outgoing_edge_info_speed[i]);
#endif
              // Modify the edge speed by whatever rules
              if(stdp_speed_switch)
              {
                s->outgoing_edge_info_speed[i] = stdp_update_edge_parm(delta_change,s->outgoing_edge_info_speed[i], 1); // 1 is for the speed stdp toggle
              }
#if VIVEK_DEBUG1
              printf("\t%s%lf\n"," New Edge Speed: ", s->outgoing_edge_info_speed[i]);
              printf("Node %li connected to Node %li Edge Speed : %f Edge Weight: %f Current Time: %lf \n", msg->signal_origin, msg->signal_current_node, s->outgoing_edge_info_speed[i], s->outgoing_edge_info_weight[i], msg->last_fired_time);
#endif

#if OUTPUT_EDGE_WEIGHTS_TO_STDOUT
              tw_output(lp,"tw: STDP_STR Edge from Node %lu to Node %lu ; Edge Speed : %11.22lf ; Edge Weight: %11.22lf ; Modified at Current Time: %11.22lf \n", msg->signal_origin, msg->signal_current_node, s->outgoing_edge_info_speed[i], s->outgoing_edge_info_weight[i], msg->last_fired_time);
#endif

            }
          }
          break;
        }

      case STDP_WEAK:
        {
#if VIVEK_DEBUG
          printf("\n%s\n\n", "Doing STDP Weight Decrease");
          printf("\t%s%li\n","Signal Source Node: ",msg->signal_origin);
          printf("\t%s%f\n","Which Fired at Time: ",msg->signal_origin_time);
          printf("\t%s%lu\n","Signal Target Node: ", msg->signal_current_node);
          printf("\t%s%f\n","Which Fired at Time: ", msg->last_fired_time);
#endif

          // defining some variables to calculate stdp changes
          // Old STDP based on origin node firing time
          // tw_stime delta_t = msg->signal_origin_time - msg->last_fired_time;
          // New STDP based on origin node signal arrival time
          tw_stime delta_t = msg->signal_arrival_time - msg->last_fired_time;
          tw_stime delta_change = stdp_delta_change_params_weak_a * powl(M_E,delta_t/stdp_delta_change_params_weak_b); //the first argument of powl is eulers number


          for(i=0; i < s->number_of_outgoing_edges; i++)
          {
            if(s->outgoing_edge_info_dst[i]==msg->signal_current_node)
            {
#if VIVEK_DEBUG1
              printf("\t%s%lf","Old Edge Weight: ", s->outgoing_edge_info_weight[i]);
#endif
              // Modify the edge weight based on whatever rule

              if(stdp_weight_switch)
              {
                s->outgoing_edge_info_weight[i] = stdp_update_edge_parm(delta_change,s->outgoing_edge_info_weight[i],0); // for the weight stdp toggle
              }
#if VIVEK_DEBUG1
              printf("\t%s%lf\n"," New Edge Weight: ", s->outgoing_edge_info_weight[i]);

              printf("\t%s%lf","Old Edge Speed: ", s->outgoing_edge_info_speed[i]);
#endif
              if(stdp_speed_switch)
              {
                s->outgoing_edge_info_speed[i] = stdp_update_edge_parm(delta_change,s->outgoing_edge_info_speed[i],1); // for the weight stdp toggle
              }
#if VIVEK_DEBUG1
              printf("\t%s%lf\n"," New Edge Speed: ", s->outgoing_edge_info_speed[i]);
#endif

#if OUTPUT_EDGE_WEIGHTS_TO_STDOUT
              tw_output(lp,"tw: STDP_WKN Edge From Node %lu to Node %lu ; Edge Speed : %11.22lf ; Edge Weight: %11.22lf ; Modified at Current Time: %11.22lf\n", msg->signal_origin, msg->signal_current_node, s->outgoing_edge_info_speed[i], s->outgoing_edge_info_weight[i], msg->last_fired_time);
#endif

            }
          }

          break;
        }
      case PRINT_EDGE_WEIGHTS_SPEEDS:
        {
          tw_output(lp,"xtw: Number of activations of Node %lu: %ld\n", s->id, s->activation_counter);
          if(s->number_of_outgoing_edges != 0){
            for(i=0; i < s->number_of_outgoing_edges; i++){
                tw_output(lp,"tw: Edge From Node %lu to Node %lu ; Edge Speed : %11.22lf ; Edge Weight: %11.22lf ; Current Time: %11.22lf\n", s->id, s->outgoing_edge_info_dst[i], s->outgoing_edge_info_speed[i], s->outgoing_edge_info_weight[i], s->current_time);
              }
          }
          break;
        }
      }
}

void
rc_event_handler(airport_state * s, tw_bf * bf, airport_message * msg, tw_lp * lp)
{
  int i;
#if VIVEK_DEBUG1
  printf("\n %s \n", "DOING REVERSE COMPUTATION");
#endif
  switch(msg->type)
  {
    case ARRIVING:
#if VIVEK_DEBUG1
      printf("\n %s \n", "DOING REVERSE COMPUTATION ARRIVING");

      printf("\t%s %lu\n", "Current node: ", s->id);
      printf("\t\t%s %lf\n", "At Time: ", tw_now(lp));

      printf("\t%s %lf\n", "Last eval time: ", s->last_evaluation_time);
      printf("\t%s %lf\n\n", "Previous eval time: ", msg->previous_evaluation_time);

      printf("\t%s %lf\n", "Current Refractory Period: ", s->remaining_refractory_period);
      printf("\t%s %lf\n\n", "Previous Refractory Period: ", msg->prev_remaining_refractory_period);
      printf("\t%s %lf\n", "Current Amplitude: ", s->current_amplitude);
      printf("\t%s %lf\n\n", "Previous Amplitude: ", msg->prev_current_amplitude);
#endif

      s->remaining_refractory_period = msg->prev_remaining_refractory_period;
      s->current_amplitude = msg->prev_current_amplitude;
      s->last_evaluation_time = msg->previous_evaluation_time;
      break;

    case DEPARTING:
#if VIVEK_DEBUG1
      printf("\n %s \n", "DOING REVERSE COMPUTATION DEPARTING");

      printf("\t%s %lu\n", "Current node: ", s->id);
      printf("\t\t%s %lf\n", "At Time: ", tw_now(lp));

      printf("\t%s %lf\n", "Last eval time: ", s->last_evaluation_time);

      printf("\t%s %lf\n", "Last fired time: ", s->last_fired_time);
      printf("\t%s %lf\n\n", "Previous fired time: ", msg->prev_last_fired_time);

      printf("\t%s %lf\n", "Current Amplitude: ", s->current_amplitude);
      printf("\t%s %lf\n\n", "Previous Amplitude: ", msg->prev_current_amplitude);
#endif

      s->current_amplitude = msg->prev_current_amplitude;
      s->last_fired_time   = msg->prev_last_fired_time;
      break;

    case DROPPING:
#if VIVEK_DEBUG1
      printf("\n %s \n", "DOING REVERSE COMPUTATION DROPPING");

      printf("\t%s %lu\n", "Current node: ", s->id);
      printf("\t\t%s %lf\n", "At Time: ", tw_now(lp));

      printf("\t%s %li\n", "Signal Arrived from Node: ", msg->signal_origin);

      printf("\t%s %lf\n", "Current Amplitude: ", s->current_amplitude);
      printf("\t%s %lf\n\n", "Previous Amplitude: ", msg->prev_current_amplitude);

      printf("\t%s %lf\n", "Current Refractory Period: ", s->remaining_refractory_period);
      printf("\t%s %lf\n\n", "Previous Refractory Period: ", msg->prev_remaining_refractory_period);
#endif

      s->remaining_refractory_period = msg->prev_remaining_refractory_period;
      s->current_amplitude = msg->prev_current_amplitude;
      break;

    case STDP_STRONG:
    {
#if VIVEK_DEBUG1
      printf("\n %s \n", "DOING REVERSE COMPUTATION: STDP STRONG");
      printf("\t%s %lu\n", "Current node: ", s->id);
      printf("\t%s %lf\n", "Current Time: ", tw_now(lp));
      printf("\tEdge from Node: %lu, to Node: %li\n",s->id, msg->signal_current_node);
      printf("\tReversed Edge Weight: %f, Reversed Edge Speed: %f\n", msg->prev_edge_weight, msg->prev_edge_speed);
#endif
      for(i=0; i<s->number_of_outgoing_edges; i++)
      {
        if(s->outgoing_edge_info_dst[i]==msg->signal_current_node)
        {
#if VIVEK_DEBUG1
      printf("\tForward Edge Weight:  %f, Forward Edge Speed:  %f\n", s->outgoing_edge_info_weight[i],s->outgoing_edge_info_speed[i]);
#endif
          s->outgoing_edge_info_weight[i] = msg->prev_edge_weight;
          s->outgoing_edge_info_speed[i] = msg->prev_edge_speed;
        }
      }
      break;
    }

    case STDP_WEAK:
    {
#if VIVEK_DEBUG1
      printf("\n %s \n", "DOING REVERSE COMPUTATION: STDP WEAK");
      printf("\t%s %lu\n", "Current node: ", s->id);
      printf("\t%s %lf\n", "Current Time: ", tw_now(lp));
      printf("\tEdge from Node: %lu, to Node: %li\n",s->id, msg->signal_current_node);
      printf("\tReversed Edge Weight: %f, Reversed Edge Speed: %f\n", msg->prev_edge_weight, msg->prev_edge_speed);
#endif
      for(i=0; i<s->number_of_outgoing_edges; i++)
      {
        if(s->outgoing_edge_info_dst[i]==msg->signal_current_node)
        {
#if VIVEK_DEBUG1
      printf("\tForward Edge Weight:  %f, Forward Edge Speed:  %f\n", s->outgoing_edge_info_weight[i],s->outgoing_edge_info_speed[i]);
#endif
          s->outgoing_edge_info_weight[i] = msg->prev_edge_weight;
          s->outgoing_edge_info_speed[i] = msg->prev_edge_speed;
        }
      }
      break;
    }
  }
  return;
}

void
final(airport_state * s, tw_lp * lp)
{
  return;
}

tw_lptype airport_lps[] =
{
  {
    (init_f) init,
    (pre_run_f) NULL,
    (event_f) event_handler,
    (revent_f) rc_event_handler,
    (commit_f) NULL,
    (final_f) final,
    (map_f) mapping,
    sizeof(airport_state),
  },
  {0},
};

const tw_optdef app_opt [] =
{
  TWOPT_GROUP("Airport Model"),
        TWOPT_STIME("lookahead", lookahead, "lookahead for events"),
  TWOPT_END()
};

tw_stime stdp_update_edge_parm(tw_stime delta_w, tw_stime edge_parm, int parm_option)
{
  tw_stime update;
  tw_stime stdp_parm_max, stdp_parm_min;

  if(parm_option==0)
  {
    stdp_parm_max = stdp_weight_max;
    stdp_parm_min = stdp_weight_min;
  }
  else if(parm_option==1)
  {
    stdp_parm_max = stdp_speed_max;
    stdp_parm_min = stdp_speed_min;
  }
  if(delta_w > 0)
  {
    update = stdp_learning_rate * delta_w * (stdp_parm_max - edge_parm);
  }
  else
  {
    update = stdp_learning_rate * delta_w * (edge_parm - stdp_parm_min);
  }

  edge_parm += update;

  if(edge_parm < stdp_parm_min)
  {
    edge_parm = stdp_parm_min;
  }
  if(edge_parm > stdp_parm_max)
  {
    edge_parm = stdp_parm_max;
  }
  return edge_parm;
}

// declarations of some functions used by the simulator
int
main(int argc, char **argv, char **env)
{
  int i;

// This is to generate the structural graph of the network

  extern char edge_path[];
  extern char vertex_path[];
  extern struct Graph* graph;
  extern struct Stim* stim;
  graph = generate_graph(edge_path, vertex_path);
  stim = generate_stim(stim_path);

  tw_opt_add(app_opt);
  tw_init(&argc, &argv);

  // nlp_per_pe /= (tw_nnodes() * g_tw_npe); // Vivek: commented this out and added next line
  // nlp_per_pe = g_tw_nlp;
  printf("\n %s%li\n", "Number of possible nodes: ", g_tw_nlp);
  printf("\n\t\t%s %u\n", "Number of nodes/processors defined: ", tw_nnodes());
  g_tw_ts_end = SIM_TIME_LIMIT; // Sets the simulation end time:w
  printf("\n\t\t%s %lf\n", "Simulation end time: ", g_tw_ts_end);
  printf("\tEvents Per Processing Element: %d\n",g_tw_events_per_pe);

  g_tw_lookahead = lookahead;

  tw_define_lps(nlp_per_pe, sizeof(airport_message));

  for(i = 0; i < g_tw_nlp; i++)
  {
    tw_lp_settype(i, &airport_lps[0]);
  }

  printf("\n Start Simulation \n");
  tw_run();

  if(tw_ismaster())
  {
    printf("\nDyn Per Model Statistics:\n");
    printf("\t%-50s %11.4lf\n", "Average Waiting Time", wait_time_avg);
//    printf("\t%-50s %11lu\n", "Number of nodes",
//      nlp_per_pe * g_tw_npe * tw_nnodes());
    printf("\t%-50s %d\n", "ROSS do not print value ", ROSS_DO_NOT_PRINT);

    printf("\n End Simulation \n");
  }

  tw_end();

  return 0;
}
