#include "graphes.h"


boolean Estcycle(graphe *g, int vertex, boolean *visited_node, boolean *stack_count)
{


  if (visited_node[vertex] == FALSE)
  {
    visited_node[vertex] = TRUE;
    stack_count[vertex] = TRUE;
    g->v_sommets[vertex] = 2;

    for (int i = 0; i < g->nsom; i++)
    {
      if (EstSuccesseur(g, vertex, i))
      {
        if (!visited_node[i] && Estcycle(g, i, visited_node, stack_count))
        {
          return TRUE;
        }
        else if (stack_count[i])
        {
          return TRUE;
        }
      }
    }
  }
  stack_count[vertex] = FALSE;
  return FALSE;
}

boolean EstCyclic(graphe *g)
{
  boolean *visited_node;
  boolean *stack_count;
  visited_node = EnsembleVide(g->nsom);
  stack_count = EnsembleVide(g->nsom);

  for (int i = 0; i < g->nsom; i++)
  {
    visited_node[i] = FALSE;
    stack_count[i] = FALSE;
  }

  for (int i = 0; i < g->nsom; i++)
  {
    if (Estcycle(g, i, visited_node, stack_count))
    {  
      return TRUE;
    }
  }
  
  return FALSE;
}


int main(int argc, char **argv)
{
  graphe *g;
  char buf[256];

  g = ReadGraphe(argv[1]);


  if (EstCyclic(g) == TRUE)
  {
    printf("Graph contains odd Length Cycle\n * Note: Graph with odd cycle vertices in red color saved with 'input filename.eps'\n");
    PlongementCirculaire(g, 150);
  AutoNomsSommets(g, 1);
  sprintf(buf, "%s.eps", argv[1]);
  EPSGraphe(g,
            buf,
            10,
            5,
            60,
            1,
            0,
            1,
            0);
  }
  else
  {
    printf("Graph doesn't contains cycle\n");
  }

  

  return 0;
}
