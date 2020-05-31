#include "dynPer.h"
#include "generate_graph.h"
#include "generate_stimulus.h"
#include <math.h>

#define VIVEK_DEBUG 0
/*
  dynPer.c
  Dynamic Perceptron simulator
  20200101
  Vivek George
*/

char edge_path[] = "/home/vivek/research/dynamic_perceptron/working_ross/ross-dynamic-perceptron/edge_input.793";
char vertex_path[] = "/home/vivek/research/dynamic_perceptron/working_ross/ross-dynamic-perceptron/vertex_input.793";
char stim_path[] = "/home/vivek/research/dynamic_perceptron/working_ross/ross-dynamic-perceptron/stim_input.74.5";
//struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
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

  long int vertex = (long int) lp->gid;
  extern struct Graph* graph;
  // printGraph(graph);

  extern struct Stim* stim;
  struct StimNode* stimPtr = stim->head[vertex];

  // Initial values of vertices
  tw_event *e;
  airport_message *m;
  s->last_fired_time = -10; // set this to null to begin
  s->current_amplitude = 0.0;
  s->last_evaluation_time = 0.0; // set this to null

  int numOutEdges=0;
  int numOutEdges1=0;

  // Initialize the activators information
  for(int i=0;i<10;i++)
    for(int j=0;j<3;j++)
      s->activators_info[i][j]=-1;
  // End initializing activators information

  // Getting Graph information
  s->id = graph->head[vertex]->nid;
  s->max_refractory_period = graph->head[vertex]->ref_per;
  s->firing_threshold = graph->head[vertex]->threshold;
  // s->number_of_edge_parameters = 2;

  struct Node* ptr = graph->head[vertex];
  while (ptr != NULL)
  {
    ptr = ptr->next;
    numOutEdges++;
  }

#if VIVEK_DEBUG
  printf("Node %li Parameters:\n",vertex);
  printf("\tNumber of outgoing edges: %d\n", numOutEdges);
#endif
  s->number_of_outgoing_edges = numOutEdges;
  if(s->number_of_outgoing_edges == 0 || graph->head[vertex]->dest == -1 )
  {
    // do nothing
    s->number_of_outgoing_edges = 0;
  }
  else
  {
    s->outgoing_edge_info_dst = tw_calloc(TW_LOC,"oe_dest",sizeof(tw_stime *), s->number_of_outgoing_edges);
    s->outgoing_edge_info_dly = tw_calloc(TW_LOC,"oe_dly",sizeof(tw_stime *), s->number_of_outgoing_edges);
    s->outgoing_edge_info_wgt = tw_calloc(TW_LOC,"oe_wgt",sizeof(tw_stime *), s->number_of_outgoing_edges);
    // s->outgoing_edge_info_amplitude = tw_calloc(TW_LOC,"oe_amp",sizeof(tw_stime *), s->number_of_outgoing_edges);

    struct Node* ptr1 = graph->head[vertex];
    while (ptr1 != NULL)
    {
      s->outgoing_edge_info_dst[numOutEdges1] = ptr1->dest;
      s->outgoing_edge_info_dly[numOutEdges1] = ptr1->delay;
      s->outgoing_edge_info_wgt[numOutEdges1] = ptr1->weight;
      // s->outgoing_edge_info_amplitude[numOutEdges1] = 0; // initial edge signal amplitude
      ptr1 = ptr1->next;
      numOutEdges1++;

#if VIVEK_DEBUG
      printf("\tNode %li to Node %li Edge Delay : %f Edge Weight: %f \n", vertex, s->outgoing_edge_info_dst[numOutEdges1], s->outgoing_edge_info_dly[numOutEdges1], s->outgoing_edge_info_wgt[numOutEdges1]);
#endif
    }
  }

// tw_event_new(destination logical process, delay , sender logical process)
//  if(lp->gid <= 100)
//  {
//    e = tw_event_new(lp->gid, 0 , lp);
//    m = tw_event_data(e);
//    m->type = ARRIVING;
//    m->edge_sig_amplitude = 1;
//    tw_event_send(e);
//  }
  while (stimPtr != NULL && stimPtr->amplitude != 0)
    {
      e = tw_event_new(lp->gid, stimPtr->delay, lp);
      m = tw_event_data(e);
      m->type = ARRIVING;
      m->edge_sig_amplitude = stimPtr->amplitude;
      tw_event_send(e);
      stimPtr = stimPtr->next;
#if VIVEK_DEBUG
      printf("Stimulus nid: %li\n",vertex);
      printf("Stimulus Amplitude: %Lf\n",stimPtr->amplitude);
      printf("Stimulus Time: %Lf\n",stimPtr->delay);
      printf("created new event\n");
      printf("created new message of event\n");
#endif
    }

}


void
event_handler(airport_state * s, tw_bf * bf, airport_message * msg, tw_lp * lp)
{
  tw_lpid dst_lp;
  tw_stime ts;
  tw_event *e;
  airport_message *m;
  int i;
  double sig_diff;
  double decay_value;
  s->current_time = tw_now(lp);

  switch(msg->type)
    {
      // This the integration phase of the node
      case ARRIVING:
        {
          sig_diff = s->current_time - s->last_fired_time;
#if VIVEK_DEBUG
          printf("\t%s %li\n", "Signal Arrived to Node: ", lp->gid);
          printf("\t%s %i\n", "Signal Arrived from Node: ", msg->signal_origin);
          printf("\t\t%s %lf\n", "At Time: ", s->current_time);
          printf("\t\t%s %lf\n", "Node Last Fired At: ", s->last_fired_time);
          printf("\t\t%s %lf\n", "Signal Difference: ", sig_diff);
          printf("\t\t%s %lf\n", "Max Refractory Period: ", s->max_refractory_period);
#endif
          // if the node is listening
          // if(s->remaining_refractory_period >= s->max_refractory_period)
          if(sig_diff >= s->max_refractory_period)
          {
            // keeping track of activators list
            s->activators_info[s->num_activators][0] = msg->signal_origin;
            s->activators_info[s->num_activators][1] = msg->signal_origin_time;
            s->activators_info[s->num_activators][2] = msg->edge_sig_amplitude;
            s->num_activators++;
            //end keepig track of activators list

            msg->prev_remaining_refractory_period = s->remaining_refractory_period;
            s->remaining_refractory_period = 0;
            msg->previous_evaluation_time = s->last_evaluation_time;
            ts = msg->previous_evaluation_time - s->current_time;
            decay_value = exp(ts);
            msg->prev_current_amplitude = s->current_amplitude;
            s->current_amplitude = msg->edge_sig_amplitude * msg->edge_weight + s->current_amplitude * decay_value; //applying simple decay

            //applying gabe decay
            // s->current_amplitude = msg->edge_sig_amplitude *
#if VIVEK_DEBUG
            printf("\t\t%s %11.11lf\n", "Incoming Signal Amplitude: ", msg->edge_sig_amplitude);
            printf("\t\t%s %11.11lf\n", "Incoming Edge Weight: ", msg->edge_weight);
            printf("\t\t%s %11.11lf\n", "Decay value: ", decay_value);
            printf("\t\t%s %11.11lf\n", "Previous Amplitude: ", msg->prev_current_amplitude);
            printf("\t\t%s %11.11lf\n", "Firing Threshold: ", s->firing_threshold);
            printf("\t\t%s %11.11lf\n", "Current Amplitude: ", s->current_amplitude);
            printf("\t\t%s %11.11lf\n", "Remaining Refractory Period: ", s->remaining_refractory_period);
#endif
            s->last_evaluation_time = s->current_time;

            if(s->current_amplitude >= s->firing_threshold)
            {

              // e = tw_event_new(lp->gid, tw_now(lp), lp);
              // Printing out the activators inforamtion
              printf("\t\t Activators List Info: \n");
              for (i=0; i<s->num_activators ; i++)
              {
                printf("\t\t Target Node: %i, Target Node Act Time: %lf, Source Node: %lf, Source Activation Time: %lf, Source Signal Amplitude: %lf\n", s->id, s->last_evaluation_time, s->activators_info[i][0],s->activators_info[i][1],s->activators_info[i][2]);
                // Setting back the activators list to the initialized values after firing
                for(int j=0; j<3;j++)
                {
                  s->activators_info[i][j]=-1;
                }
              }
              s->num_activators = 0; // clear the number of activators
              e = tw_event_new(lp->gid, 0 ,lp);
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
            // Print dropped Signal information
            printf("\t\t Dropped Signal Information: \n");
            printf("\t\t Target Node: %i, Target Node Act Time: %lf, Source Node: %i, Source Activation Time: %lf, Source Signal Amplitude: %lf\n", s->id, s->last_evaluation_time, msg->signal_origin,msg->signal_origin_time,msg->edge_sig_amplitude);
            // end dropped signal information


            // e = tw_event_new(lp->gid, tw_now(lp), lp);
            e = tw_event_new(lp->gid, 0 ,lp);
            m = tw_event_data(e);
            m->type = DROPPING;
            m->last_fired_time = s->last_fired_time;
            m->signal_origin = msg->signal_origin;
            m->prev_remaining_refractory_period = s->remaining_refractory_period;
            tw_event_send(e);
          }
          break;
        }

      case DEPARTING:
        {
#if VIVEK_DEBUG
          printf("\t%s %li\n", "Node Fired ID: ", lp->gid);
          // printf("\t%s %i\n", "Node ID Fired: ", s->id);
          printf("\t\t%s %11.11lf\n", "At Time: ", s->current_time);
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
                ts      = s->outgoing_edge_info_dly[i];
                e       = tw_event_new(dst_lp, ts, lp);
                m       = tw_event_data(e);
                m->signal_origin_time = s->last_fired_time;
                m->type = ARRIVING;
                m->edge_sig_amplitude = 1;
                m->edge_weight = s->outgoing_edge_info_wgt[i];
                m->signal_origin = lp->gid;
                tw_event_send(e);
#if VIVEK_DEBUG
                printf("\t\t%s %lu\n", "Sending Outbound Signal to: ", dst_lp);
                // printf("\t\t%s %11.11lf\n", "Outbound Signal Delay: ", ts);
                printf("\t\t%s %lf\n", "Outbount Signal Delay: ", ts);
#endif
              }
          }
          else
#if VIVEK_DEBUG
            printf("\t\t No outgoing signals.\n");
#endif

          break;
        }

      case DROPPING:
        {
          s->remaining_refractory_period = s->max_refractory_period - (s->current_time - msg->last_fired_time);
          s->current_amplitude = 0;
#if VIVEK_DEBUG
          printf("\n\n \t Node Refractory, Dropped Packet\n");
          printf("\t%s %li\n", "Node Droping Signal ID: ", lp->gid);
          printf("\t%s %i\n", "Signal Arrived from Node: ", msg->signal_origin);
          printf("\t\t%s %11.11lf\n", "Remaining Refractory Period: ", s->remaining_refractory_period);
          printf("\t%s %i\n", "Signal Arrived from Node: ", msg->signal_origin);
#endif
          break;
        }
      }
}

void
rc_event_handler(airport_state * s, tw_bf * bf, airport_message * msg, tw_lp * lp)
{
#if VIVEK_DEBUG
  printf("\n %s \n", "DOING REVERSE COMPUTATION");
#endif
  switch(msg->type)
  {
    case ARRIVING:
#if VIVEK_DEBUG
      printf("\n %s \n", "DOING REVERSE COMPUTATION ARRIVING");

      printf("\t%s %d\n", "Current node: ", s->id);
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
#if VIVEK_DEBUG
      printf("\n %s \n", "DOING REVERSE COMPUTATION DEPARTING");

      printf("\t%s %d\n", "Current node: ", s->id);
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
#if VIVEK_DEBUG
      printf("\n %s \n", "DOING REVERSE COMPUTATION DROPPING");

      printf("\t%s %d\n", "Current node: ", s->id);
      printf("\t\t%s %lf\n", "At Time: ", tw_now(lp));

      printf("\t%s %i\n", "Signal Arrived from Node: ", msg->signal_origin);

      printf("\t%s %lf\n", "Current Amplitude: ", s->current_amplitude);
      printf("\t%s %lf\n\n", "Previous Amplitude: ", msg->prev_current_amplitude);

      printf("\t%s %lf\n", "Current Refractory Period: ", s->remaining_refractory_period);
      printf("\t%s %lf\n\n", "Previous Refractory Period: ", msg->prev_remaining_refractory_period);
#endif

      s->remaining_refractory_period = msg->prev_remaining_refractory_period;
      s->current_amplitude = msg->prev_current_amplitude;
      break;
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
  g_tw_ts_end = 10; // Sets the simulation end time:w
  printf("\n\t\t%s %lf\n", "Simulation end time: ", g_tw_ts_end);

  g_tw_lookahead = lookahead;

  tw_define_lps(nlp_per_pe, sizeof(airport_message));

  for(i = 0; i < g_tw_nlp; i++)
  {
    tw_lp_settype(i, &airport_lps[0]);
  }

  printf("\n Start Simulation \n");
  tw_run();
  printf("\n End Simulation \n");

  if(tw_ismaster())
  {
    printf("\nDyn Per Model Statistics:\n");
    printf("\t%-50s %11.4lf\n", "Average Waiting Time", wait_time_avg);
//    printf("\t%-50s %11lu\n", "Number of nodes",
//      nlp_per_pe * g_tw_npe * tw_nnodes());
    printf("\t%-50s %d\n", "ROSS do not print value ", ROSS_DO_NOT_PRINT);
  }

  tw_end();

  return 0;
}
