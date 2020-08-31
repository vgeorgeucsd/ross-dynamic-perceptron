#include <stdio.h>
#include <stdlib.h>

// Define maximum number of vertices in the graph
#define N 3
#define EdgeParms 4
#define VertexParms 3

// Data structure to store graph
struct Graph {
  // An array of pointers to Node to represent adjacency list
  struct Node* head[N];
};

// A data structure to store adjacency list nodes of the graph
struct Node {
  unsigned long int dest;
  float dist,weight;
  unsigned long int nid;
  float ref_per;
  float threshold;
  struct Node* next;
};

// Function to create an adjacency list from specified edges
struct Graph* createGraph(FILE* reads)
{
  unsigned long int i;

  // allocate memory for graph data structure
  struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));

  // initialize head pointer for all vertices
  for (i = 0; i < N; i++)
    graph->head[i] = NULL;

  // add edges to the directed graph one by one
  unsigned long int src, dest;
  float weight;
  float dist;
  while(fscanf(reads,"%d %d %d %f", &src, &dest, &weight, &dist)==EdgeParms)
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
  }

  return graph;
}

// Function to print adjacency list representation of graph
struct Graph* addVertexInfo(struct Graph* graph, FILE* reads)
{

  unsigned long int nid;
  float ref_per;
  float threshold;
  printf("Reading vertex info file\n");
  while(fscanf(reads,"%d %f %f", &nid, &ref_per, &threshold)==VertexParms)
  {
    printf("Reading Vertex: %d\n",nid);
    graph->head[nid]->nid = nid;
    graph->head[nid]->ref_per = ref_per;
    graph->head[nid]->threshold = threshold;
  }

  return graph;

}

// Function to print adjacency list representation of graph
void printGraph(struct Graph* graph)
{
  unsigned long int i,j;
  for (i = 0; i < N; i++)
  {
    // print current vertex and all ts neighbors
    struct Node* ptr = graph->head[i];
    while (ptr != NULL)
    {
      printf("%d -> %d (wgt: %f, dly: %f)\t", i, ptr->dest, ptr->weight, ptr->dist);
      ptr = ptr->next;
    }

    printf("\n");
  }

  printf("Printing Vertex Info: \n");

  for (j = 0; j < N; j++)
  {
    // print current vertex info
    printf("NID: %d, ref per: %f, thresh: %f\n", graph->head[j]->nid, graph->head[j]->ref_per, graph->head[j]->threshold);
  }

}


// Weighted Directed Graph Implementation in C
int main(void)
{
  FILE* reads = fopen("edge_input", "r");
  if (reads==NULL)
  {
    perror("Error: did not find edge input file");
    return 1;
  }
  else
  {
    printf("Found edge input file \n");
    // construct graph from given edges
    struct Graph *graph = createGraph(reads);
    fclose(reads);

    // add the vertex information addVertexInfo()
    FILE* reads = fopen("vertex_input", "r");
    if (reads==NULL)
    {
      perror("Error: Did not find vertex input file");
      return 1;
    }
    else
    {
      printf("Found vertex input file \n");
      graph = addVertexInfo(graph,reads);
    }

    // print adjacency list representation of graph
    printGraph(graph);

  }
  return 0;
}
