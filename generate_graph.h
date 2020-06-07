#include <stdio.h>
#include <stdlib.h>
#include "network_parameters_TOTALNUMBEROFNODES.h"
// Define maximum number of vertices in the graph
// #define N 3
// #define EdgeParms 4
// #define VertexParms 3

// Data structure to store graph
struct Graph {
  // An array of pointers to Node to represent adjacency list
  struct Node* head[N];
};

// A data structure to store adjacency list nodes of the graph
struct Node {
  int dest;
  float weight,dist;
  int nid;
  float ref_per;
  float threshold;
  struct Node* next;
};

// Function to create an adjacency list from specified edges
struct Graph* createGraph(FILE* reads, struct Graph* graph)
{
  unsigned i;

  // allocate memory for graph data structure
  // struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));

  // initialize head pointer for all vertices
  for (i = 0; i < N; i++)
  {
//      struct Node* newNode0 = (struct Node*)malloc(sizeof(struct Node));
      graph->head[i] = NULL;
//      newNode0->dest = NULL;
//      newNode0->weight = 0;
//      newNode0->delay = 0;

    // point new node to current head
//    newNode0->next = graph->head[i];

    // point head pointer to new node
//      graph->head[i] = newNode0;
  }
  // add edges to the directed graph one by one
  int src;
  int dest;
  float weight;
  float dist;
  while(fscanf(reads,"%d %d %f %f", &src, &dest, &weight, &dist)==EdgeParms)
  {
    // allocate new node of Adjacency List from src to dest
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->dist = dist;

    // point new node to current head
    newNode->next = graph->head[src];

    // point head pointer to new node
    graph->head[src] = newNode;
    printf("Reading Edge: %d -> %d (wgt: %f, dist: %f)\n", src, graph->head[src]->dest, graph->head[src]->weight,graph->head[src]->dist);
  }

  return graph;
}

// Function to print adjacency list representation of graph
struct Graph* addVertexInfo(FILE* reads, struct Graph* graph)
{
  int nid;
  float ref_per;
  float threshold;
  printf("Reading vertex info file\n");
  while(fscanf(reads,"%d %f %f", &nid, &ref_per, &threshold)==VertexParms)
  {
    printf("Reading Vertex: %d\n",nid);
    if(graph->head[nid] == NULL)
    {
      printf("Initializing Vertex: %d\n", nid);
      struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
      newNode->nid = nid;
      newNode->ref_per = ref_per;
      newNode->threshold = threshold;
      newNode->dest = -1; // setting this to -1 so that we can indicate no outgoing edges
      newNode->weight = 0;
      newNode->dist = 0;
      graph->head[nid] = newNode;
      printf("NID: %d, ref per: %f, thresh: %f, dest: %d, wgt: %f, dist: %f \n", graph->head[nid]->nid, graph->head[nid]->ref_per, graph->head[nid]->threshold, graph->head[nid]->dest, graph->head[nid]->weight,graph->head[nid]->dist);
    }
    else
    {
      graph->head[nid]->nid = nid;
      graph->head[nid]->ref_per = ref_per;
      graph->head[nid]->threshold = threshold;
    }
    // graph->head[nid]->nid = nid;
    // graph->head[nid]->ref_per = ref_per;
    // graph->head[nid]->threshold = threshold;
  }

  return graph;

}

// Function to print adjacency list representation of graph
void printGraph(struct Graph* graph)
{
  int i,j,k=0;
  for (i = 0; i < N; i++)
  {
    // print current vertex and all ts neighbors
    struct Node* ptr = graph->head[i];
    while (ptr != NULL)
    {
      printf("%d -> %d (wgt: %f, dist: %f)\t", i, ptr->dest, ptr->weight,ptr->dist);
      ptr = ptr->next;
      k++;
    }
    printf("\n");
    printf("Number of outgoing edges: %d \n",k);
    k=0;
  }

  printf("Printing Vertex Info: \n");

  for (j = 0; j < N; j++)
  {
    // print current vertex info
    printf("NID: %d, ref per: %f, thresh: %f\n", graph->head[j]->nid, graph->head[j]->ref_per, graph->head[j]->threshold);
  }

}


// Weighted Directed Graph Implementation in C
// int generate_graph(void)

struct Graph* generate_graph(char edge_path[], char vertex_path[])
{

  struct Graph* graph_int = (struct Graph*)malloc(sizeof(struct Graph));
  FILE* reads = fopen(edge_path, "r");
  if (reads==NULL)
  {
    perror("Error: did not find edge input file");
    exit(EXIT_FAILURE);
  }
  else
  {
    printf("Found edge input file \n");
    // construct graph from given edges
    graph_int = createGraph(reads,graph_int);
    fclose(reads);

    // add the vertex information addVertexInfo()
   FILE* reads = fopen(vertex_path, "r");
   if (reads==NULL)
   {
     perror("Error: Did not find vertex input file");
     exit(EXIT_FAILURE);
   }
   else
   {
     printf("Found vertex input file \n");
     graph_int = addVertexInfo(reads,graph_int);
   }

    // print adjacency list representation of graph
    printGraph(graph_int);

  }
  return graph_int;
}
