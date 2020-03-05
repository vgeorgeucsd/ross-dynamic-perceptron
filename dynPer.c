#include "dynPer.h"
#include <math.h>

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

  s->last_fired_time = -10.0;
  s->current_amplitude = 0.0;
  s->last_evaluation_time = 0.0;

 switch(lp->gid) // Set the outgoing connections
    {

    case 0:
      {
        s->id = 0; // this is the node id
	// node 0 is connected to node 1
	s->outgoing_edge_info[0][0] = 1;
        // node 0 has delay of 1 unit to node 1
	s->outgoing_edge_info[0][1] = 1.0;
	s->max_refractory_period = 1.0;
	s->firing_threshold = 1.0;
	s->number_of_outgoing_edges = 1;
        printf("\t Node 0 Parameters \n");
        printf("\t%s %f\n", " ED N0 to N1: ", s->outgoing_edge_info[0][1]);

	break;
      }
    case 1:
      {
        s->id = 1;
	// node 1 is connected to node 2
	s->outgoing_edge_info[0][0] = 2;
	s->outgoing_edge_info[0][1] = 1.0;
        // node 1 is connected back to node 0
        // with a delay of 2 time units
	s->outgoing_edge_info[1][0] = 0;
	s->outgoing_edge_info[1][1] = 1;
	s->max_refractory_period = 1.0;
	s->firing_threshold = 1.0;
	s->number_of_outgoing_edges = 2;
        printf("\t Node 1 Parameters \n");
        printf("\t%s %f\n", " ED N1 to N2: ", s->outgoing_edge_info[0][1]);
        printf("\t%s %f\n", " ED N1 to N0: ", s->outgoing_edge_info[1][1]);
	break;
      }
    case 2:
      {
        s->id = 2;
	// node 2 is connected to node 0
	s->outgoing_edge_info[0][0] = 0;
	s->outgoing_edge_info[0][1] = 1.0;
	s->max_refractory_period = 1.0;
	s->firing_threshold = 1.0;
	s->number_of_outgoing_edges = 1;
        printf("\t Node 2 Parameters \n");
        printf("\t%s %f\n", " ED N2 to N0: ", s->outgoing_edge_info[0][1]);
	break;
      }
    }
    // tw_event_new(destination logical process, delay , sender logical process)
    if(lp->gid == 0)
    {
      e = tw_event_new(lp->gid, 0 , lp);
      m = tw_event_data(e);
      m->type = ARRIVAL;
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
  float sig_diff = 0;

  switch(msg->type)
    {
      // This the integration phase of the node
      case ARRIVAL:
        {
          printf("\t%s %11.1li \n", "Signal Arrived to Node: ", lp->gid);
          printf("\t\t%s %11.11lf \n", "At Time: ", tw_now(lp));
          printf("\t\t%s %11.11lf \n", "Node Last Fired At: ", s->last_fired_time);
          s->remaining_refractory_period = tw_now(lp) - s->last_fired_time;
          sig_diff = tw_now(lp) - s->last_fired_time;

          // if the node is listening
          // if(s->remaining_refractory_period >= s->max_refractory_period)
          if(sig_diff >= s->max_refractory_period)
          {
            s->remaining_refractory_period = 0;
            if(s->current_amplitude == 0)
            {
              msg->previous_evaluation_time = tw_now(lp);
              s->current_amplitude = 1;
            }
            else
            {
              s->remaining_refractory_period = sig_diff;
              msg->previous_evaluation_time = s->last_evaluation_time;
              ts = msg->previous_evaluation_time - tw_now(lp);
              s->current_amplitude = s->current_amplitude - exp(ts);
            }
            printf("\t\t%s %11.11lf \n", "Firing Threshold: ", s->firing_threshold);
            printf("\t\t%s %11.11lf \n", "Current Amplitude: ", s->current_amplitude);
            printf("\t\t%s %11.11lf\n", "Remaining Refractory Period: ", s->remaining_refractory_period);
            s->last_evaluation_time = tw_now(lp);

            if(s->current_amplitude >= s->firing_threshold)
            {
              // e = tw_event_new(lp->gid, tw_now(lp), lp);
              e = tw_event_new(lp->gid, 0 ,lp);
              m = tw_event_data(e);
              m->type = FIRING;
              tw_event_send(e);
            }
          }
          else
          {
            // else drop the packet
            printf("\n\n \t Node Refractory, Dropped Packet \n\n");
          }
          break;
        }

      case FIRING:
        {
          printf("\t%s %li \n", "Node Fired ID: ", lp->gid);
          // printf("\t%s %i\n", "Node ID Fired: ", s->id);
          printf("\t\t%s %11.11lf\n", "At Time: ", tw_now(lp));

          s->current_amplitude = 0;
          s->last_fired_time = tw_now(lp);

          // send outgoing signals to the other nodes
          // need to do this for each outgoing edge
          for(i=0; i < s->number_of_outgoing_edges; i++){
            dst_lp = s->outgoing_edge_info[i][0];
            ts     = s->outgoing_edge_info[i][1];
            e = tw_event_new(dst_lp, ts, lp);
            m = tw_event_data(e);
            m->type = ARRIVAL;
            tw_event_send(e);
            printf("\t Sending Outgoing Signals \n");
            printf("\t\t%s %lu \n", "Outbound Signal to: ", dst_lp);
            printf("\t\t%s %11.11lf\n", "Outbound Signal Delay: ", ts);

          }
          break;
        }
      }
}

void
rc_event_handler(airport_state * s, tw_bf * bf, airport_message * msg, tw_lp * lp)
{
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
	g_tw_ts_end = 10; // Sets the simulation end time:w


        g_tw_lookahead = lookahead;

	tw_define_lps(nlp_per_pe, sizeof(airport_message));

	for(i = 0; i < g_tw_nlp; i++)
		tw_lp_settype(i, &airport_lps[0]);

	tw_run();

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
