#include "page_rank.h"

#include <stdlib.h>
#include <cmath>
#include <omp.h>
#include <utility>

#include "../common/CycleTimer.h"
#include "../common/graph.h"

// pageRank --
//
// g:           graph to process (see common/graph.h)
// solution:    array of per-vertex vertex scores (length of array is num_nodes(g))
// damping:     page-rank algorithm's damping parameter
// convergence: page-rank algorithm's convergence threshold
//
void pageRank(Graph g, double *solution, double damping, double convergence)
{

  // initialize vertex weights to uniform probability. Double
  // precision scores are used to avoid underflow for large graphs

  int numNodes = num_nodes(g);
  int converged = 0;
  double *score_old, *score_new;
  score_old = (double*)malloc(sizeof(double)*num_nodes(g));
  score_new = (double*)malloc(sizeof(double)*num_nodes(g));
  double equal_prob = 1.0 / numNodes;
  for (int i = 0; i < numNodes; ++i)
  {
    solution[i] = 0.0;
    score_old[i] = equal_prob;
  }
  while(!converged){
    for(int i = 0; i < numNodes; i++){/*raccoon:i < numNodes*/
      const Vertex* start = incoming_begin(g, i);
      const Vertex* end = incoming_end(g, i);
      for(const Vertex* j = start; j != end; j++){
        solution[i] += score_old[*j] / (double)outgoing_size(g, *j);
        //printf("%lf\n", score_new[i]);
      }
      solution[i] = (double)damping * solution[i] + (1.0 - damping) / (double)numNodes;
      //printf("%lf\n", score_new[i]);
      for(int k = 0; k < numNodes; k++){
      	if(outgoing_size(g, k) == 0)
      	  solution[i] += (double)damping * score_old[k] / (double)numNodes;
      }
    }
    double global_diff = 0;
    for(int i = 0; i < numNodes; i++)
      global_diff += abs(solution[i] - score_old[i]);
    converged = (global_diff < convergence);
  }
  free(score_old);
  free(score_new);
  /*
     For PP students: Implement the page rank algorithm here.  You
     are expected to parallelize the algorithm using openMP.  Your
     solution may need to allocate (and free) temporary arrays.

     Basic page rank pseudocode is provided below to get you started:

     // initialization: see example code above
     score_old[vi] = 1/numNodes;

     while (!converged) {

       // compute score_new[vi] for all nodes vi:
       score_new[vi] = sum over all nodes vj reachable from incoming edges
                          { score_old[vj] / number of edges leaving vj  }
       score_new[vi] = (damping * score_new[vi]) + (1.0-damping) / numNodes;

       score_new[vi] += sum over all nodes v in graph with no outgoing edges
                          { damping * score_old[v] / numNodes }

       // compute how much per-node scores have changed
       // quit once algorithm has converged

       global_diff = sum over all nodes vi { abs(score_new[vi] - score_old[vi]) };
       converged = (global_diff < convergence)
     }

   */
}
