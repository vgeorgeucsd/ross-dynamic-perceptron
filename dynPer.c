#include "dynPer.h"
#include <math.h>
/*
  dynPer.c
  Dynamic Perceptron simulator
  20200101
  Vivek George
*/

tw_peid
mapping(tw_lpid gid)
{
	return (tw_peid) gid / g_tw_nlp;
}

void
init(airport_state * s, tw_lp * lp)
{
  tw_event *e;
  airport_message *m;
  s->last_fired_time = -10; // set this to null to begin
  s->current_amplitude = 0.0;
  s->last_evaluation_time = 0.0; // set this to null

  switch(lp->gid) // Set the outgoing connections
    {

    case 0:
      {
        s->id = 0; // this is the node id
	// node 0 is connected to node 1
        s->max_refractory_period = 1.0;
	s->firing_threshold = 1.0;
	s->number_of_outgoing_edges = 1;
        s->number_of_edge_parameters = 2;

        s->outgoing_edge_info_dst = tw_calloc(TW_LOC,"oe_dest",sizeof(tw_stime *), s->number_of_outgoing_edges);
        s->outgoing_edge_info_dly = tw_calloc(TW_LOC,"oe_dly",sizeof(tw_stime *), s->number_of_outgoing_edges);
        s->outgoing_edge_info_wgt = tw_calloc(TW_LOC,"oe_wgt",sizeof(tw_stime *), s->number_of_outgoing_edges);

        s->outgoing_edge_info_dst[0] = 1;
        s->outgoing_edge_info_dly[0] = 1.0;
        s->outgoing_edge_info_wgt[0] = 1;
        printf("\t Node 0 Parameters\n");
        printf("\t%s %lf\n", " ED Node 0 to Node 1: ", s->outgoing_edge_info_dst[0]);

	break;
      }
    case 1:
      {
        s->id = 1;
	s->max_refractory_period = 1.0;
	s->firing_threshold = 1.0;
	s->number_of_outgoing_edges = 2;
        s->number_of_edge_parameters = 2;

        s->outgoing_edge_info_dst = tw_calloc(TW_LOC,"oe_dest",sizeof(tw_stime *), s->number_of_outgoing_edges);
        s->outgoing_edge_info_dly = tw_calloc(TW_LOC,"oe_dly",sizeof(tw_stime *), s->number_of_outgoing_edges);
        s->outgoing_edge_info_wgt = tw_calloc(TW_LOC,"oe_wgt",sizeof(tw_stime *), s->number_of_outgoing_edges);

        s->outgoing_edge_info_dst[0] = 2;
        s->outgoing_edge_info_dly[0] = 1.0;
        s->outgoing_edge_info_wgt[0] = 1;
        s->outgoing_edge_info_dst[1] = 0;
        s->outgoing_edge_info_dly[1] = 1.0;
        s->outgoing_edge_info_wgt[1] = 1;

        printf("\t Node 1 Parameters \n");
        // printf("\t%s %11.11f\n", " ED Node 1 to Node 2: ", s->outgoing_edge_info[0][1]);
        // printf("\t%s %11.11f\n", " ED Node 1 to Node 0: ", s->outgoing_edge_info[1][1]);
        printf("\t%s %lf\n", " ED Node 1 to Node 2: ", s->outgoing_edge_info_dst[0]);
        printf("\t%s %lf\n", " ED Node 1 to Node 0: ", s->outgoing_edge_info_dst[1]);
	break;
      }
    case 2:
      {
        s->id = 2;
       	s->max_refractory_period = 1.0;
	s->firing_threshold = 1.0;
	s->number_of_outgoing_edges = 1;
        s->number_of_edge_parameters = 2;

        s->outgoing_edge_info_dst = tw_calloc(TW_LOC,"oe_dest",sizeof(tw_stime *), s->number_of_outgoing_edges);
        s->outgoing_edge_info_dly = tw_calloc(TW_LOC,"oe_dly",sizeof(tw_stime *), s->number_of_outgoing_edges);
        s->outgoing_edge_info_wgt = tw_calloc(TW_LOC,"oe_wgt",sizeof(tw_stime *), s->number_of_outgoing_edges);

        s->outgoing_edge_info_dst[0] = 0;
        s->outgoing_edge_info_dly[0] = 1.5;
        s->outgoing_edge_info_wgt[0] = 1;

        printf("\t Node 2 Parameters \n");
        printf("\t%s %lf\n", " ED Node 2 to Node 0: ", s->outgoing_edge_info_dly[0]);
	break;
      }
    }

    // tw_event_new(destination logical process, delay , sender logical process)
    if(lp->gid == 0)
    {
      e = tw_event_new(lp->gid, 0 , lp);
      m = tw_event_data(e);
      m->type = ARRIVING;
      m->edge_weight = 1;
      tw_event_send(e);
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

  switch(msg->type)
    {
      // This the integration phase of the node
      case ARRIVING:
        {
          printf("\t%s %li\n", "Signal Arrived to Node: ", lp->gid);
          printf("\t%s %i\n", "Signal Arrived from Node: ", msg->signal_origin);
          printf("\t\t%s %lf\n", "At Time: ", tw_now(lp));
          printf("\t\t%s %lf\n", "Node Last Fired At: ", s->last_fired_time);

          sig_diff = tw_now(lp) - s->last_fired_time;

          printf("\t\t%s %lf\n", "Signal Difference: ", sig_diff);
          printf("\t\t%s %lf\n", "Max Refractory Period: ", s->max_refractory_period);
          // if the node is listening
          // if(s->remaining_refractory_period >= s->max_refractory_period)
          if(sig_diff >= s->max_refractory_period)
          {
            msg->prev_remaining_refractory_period = s->remaining_refractory_period;
            s->remaining_refractory_period = 0;
            msg->previous_evaluation_time = s->last_evaluation_time;
            ts = msg->previous_evaluation_time - tw_now(lp);
            printf("\t\t%s %11.11lf\n", "Edge Weight: ", msg->edge_weight);
            printf("\t\t%s %11.11lf\n", "Decay value: ", exp(ts));
            printf("\t\t%s %11.11lf\n", "Previous Amplitude: ", s->current_amplitude);
            msg->prev_current_amplitude = s->current_amplitude;
            s->current_amplitude = msg->edge_weight + s->current_amplitude * exp(ts); //applying decay

            printf("\t\t%s %11.11lf\n", "Firing Threshold: ", s->firing_threshold);
            printf("\t\t%s %11.11lf\n", "Current Amplitude: ", s->current_amplitude);
            printf("\t\t%s %11.11lf\n", "Remaining Refractory Period: ", s->remaining_refractory_period);
            s->last_evaluation_time = tw_now(lp);

            if(s->current_amplitude >= s->firing_threshold)
            {
              // e = tw_event_new(lp->gid, tw_now(lp), lp);
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
          printf("\t%s %li\n", "Node Fired ID: ", lp->gid);
          // printf("\t%s %i\n", "Node ID Fired: ", s->id);
          printf("\t\t%s %11.11lf\n", "At Time: ", tw_now(lp));
          msg->prev_current_amplitude = s->current_amplitude;
          s->current_amplitude = 0;
          msg->prev_last_fired_time = s->last_fired_time;
          s->last_fired_time = tw_now(lp);

          // send outgoing signals to the other nodes
          // need to do this for each outgoing edge
          for(i=0; i < s->number_of_outgoing_edges; i++)
          {
            dst_lp = s->outgoing_edge_info_dst[i];
            ts     = s->outgoing_edge_info_dly[i];
            e = tw_event_new(dst_lp, ts, lp);
            m = tw_event_data(e);
            m->type = ARRIVING;
            // m->edge_weight = s->outgoing_edge_info[i][2];
            m->edge_weight = s->outgoing_edge_info_wgt[i];
            m->signal_origin = lp->gid;
            tw_event_send(e);
            printf("\t\t%s %lu\n", "Sending Outbound Signal to: ", dst_lp);
            // printf("\t\t%s %11.11lf\n", "Outbound Signal Delay: ", ts);
            printf("\t\t%s %lf\n", "Outbount Signal Delay: ", ts);
          }
          break;
        }

      case DROPPING:
        {
          printf("\n\n \t Node Refractory, Dropped Packet \n\n");
          printf("\t%s %li\n", "Node Droping Signal ID: ", lp->gid);
          printf("\t%s %i\n", "Signal Arrived from Node: ", msg->signal_origin);
          s->remaining_refractory_period = s->max_refractory_period - (tw_now(lp) - msg->last_fired_time);
          s->current_amplitude = 0;
          printf("\t\t%s %11.11lf\n", "Remaining Refractory Period: ", s->remaining_refractory_period);
          printf("\t%s %i\n", "Signal Arrived from Node: ", msg->signal_origin);
          break;
        }
      }
}

void
rc_event_handler(airport_state * s, tw_bf * bf, airport_message * msg, tw_lp * lp)
{
  printf("\n %s \n", "DOING REVERSE COMPUTATION");
  switch(msg->type)
  {
    case ARRIVING:
      printf("\n %s \n", "DOING REVERSE COMPUTATION ARRIVING");

      printf("\t%s %d\n", "Current node: ", s->id);
      printf("\t\t%s %lf\n", "At Time: ", tw_now(lp));

      printf("\t%s %lf\n", "Last eval time: ", s->last_evaluation_time);
      printf("\t%s %lf\n\n", "Previous eval time: ", msg->previous_evaluation_time);

      printf("\t%s %lf\n", "Current Refractory Period: ", s->remaining_refractory_period);
      printf("\t%s %lf\n\n", "Previous Refractory Period: ", msg->prev_remaining_refractory_period);
      printf("\t%s %lf\n", "Current Amplitude: ", s->current_amplitude);
      printf("\t%s %lf\n\n", "Previous Amplitude: ", msg->prev_current_amplitude);


      s->remaining_refractory_period = msg->prev_remaining_refractory_period;
      s->current_amplitude = msg->prev_current_amplitude;
      s->last_evaluation_time = msg->previous_evaluation_time;
      break;

    case DEPARTING:
      printf("\n %s \n", "DOING REVERSE COMPUTATION DEPARTING");

      printf("\t%s %d\n", "Current node: ", s->id);
      printf("\t\t%s %lf\n", "At Time: ", tw_now(lp));

      printf("\t%s %lf\n", "Last eval time: ", s->last_evaluation_time);

      printf("\t%s %lf\n", "Last fired time: ", s->last_fired_time);
      printf("\t%s %lf\n\n", "Previous fired time: ", msg->prev_last_fired_time);

      printf("\t%s %lf\n", "Current Amplitude: ", s->current_amplitude);
      printf("\t%s %lf\n\n", "Previous Amplitude: ", msg->prev_current_amplitude);


      s->current_amplitude = msg->prev_current_amplitude;
      s->last_fired_time   = msg->prev_last_fired_time;
      break;

    case DROPPING:
      printf("\n %s \n", "DOING REVERSE COMPUTATION DROPPING");

      printf("\t%s %d\n", "Current node: ", s->id);
      printf("\t\t%s %lf\n", "At Time: ", tw_now(lp));

      printf("\t%s %i\n", "Signal Arrived from Node: ", msg->signal_origin);

      printf("\t%s %lf\n", "Current Amplitude: ", s->current_amplitude);
      printf("\t%s %lf\n\n", "Previous Amplitude: ", msg->prev_current_amplitude);

      printf("\t%s %lf\n", "Current Refractory Period: ", s->remaining_refractory_period);
      printf("\t%s %lf\n\n", "Previous Refractory Period: ", msg->prev_remaining_refractory_period);

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
//		printf("\t%-50s %11lu\n", "Number of nodes",
//			nlp_per_pe * g_tw_npe * tw_nnodes());
                printf("\t%-50s %d\n", "ROSS do not print value ", ROSS_DO_NOT_PRINT);
	}

	tw_end();

	return 0;
}
