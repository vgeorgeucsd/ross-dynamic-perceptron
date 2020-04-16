#include <igraph.h>

int main(){
  igraph_t g;
  int i;
  igraph_bool_t simple;

  /* G(n,p) */
  igraph_erdos_renyi_game(&g, IGRAPH_ERDOS_RENYI_GNP, 10, 0.999999, IGRAPH_DIRECTED, IGRAPH_LOOPS);

  printf("%s %d \n","Number of Nodes: ",igraph_vcount(&g));
  printf("%s %d \n","Number of edges: ",igraph_ecount(&g));
  printf("%s %d \n", "Test for directedness: ", igraph_is_directed(&g));
  igraph_destroy(&g);
}
