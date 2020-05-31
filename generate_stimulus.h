#include <stdio.h>
#include <stdlib.h>
// #include <ross.h>

// Define maximum number of vertices in the graph
#define S 793
#define StimParms 3

// Data structure to store graph
struct Stim {
  // An array of pointers to Node to represent adjacency list
  struct StimNode* head[S];
};

// A data structure to store adjacency list nodes of the graph
struct StimNode {
  long double amplitude,delay;
  struct StimNode* next;
};

// Function to create an adjacency list from specified edges
struct Stim* createStim(FILE* reads, struct Stim* graph)
{
  unsigned i;


  // initialize head pointer for all vertices
  for (i = 0; i < S; i++)
  {
      graph->head[i] = NULL;
  }

  // add edges to the directed graph one by one
  int src;
  long double amplitude, delay;
  while(fscanf(reads,"%d %Lf %Lf", &src, &amplitude, &delay)==StimParms)
  {
    // allocate new node of Adjacency List from src
    struct StimNode* newNode = (struct StimNode*)malloc(sizeof(struct StimNode));
    newNode->amplitude = amplitude;
    newNode->delay = delay;

    // point new node to current head
    newNode->next = graph->head[src];

    // point head pointer to new node
    graph->head[src] = newNode;
    printf("Reading Stim: %d (wgt: %Lf, dly: %Lf)\n", src, graph->head[src]->amplitude,graph->head[src]->delay);
  }

  return graph;
}

// Function to print adjacency list representation of graph
void printStim(struct Stim* graph)
{
  int i,k=0;
  for (i = 0; i < S; i++)
  {
    // print current vertex and all ts neighbors
    struct StimNode* ptr = graph->head[i];
    while (ptr != NULL)
    {
      printf("%d (wgt: %Lf, dly: %Lf)\t", i, ptr->amplitude,ptr->delay);
      ptr = ptr->next;
      k++;
    }
    printf("\n");
    printf("Number of outgoing edges: %d \n",k);
    k=0;
  }
}


// Weighted Directed Graph Implementation in C
// int generate_graph(void)

struct Stim* generate_stim(char stim_path[])
{

  struct Stim* graph_int = (struct Stim*)malloc(sizeof(struct Stim));
  FILE* reads = fopen(stim_path, "r");
  if (reads==NULL)
  {
    perror("Error: did not find stim input file");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("Found edge input file \n");
    // construct graph from given edges
    graph_int = createStim(reads,graph_int);
    fclose(reads);

    // print adjacency list representation of graph
    printStim(graph_int);

  }
  return graph_int;
}
