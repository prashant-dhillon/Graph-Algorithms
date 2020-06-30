#include "graphes.h"

boolean EstBipartite(graphe *g, int x, int Z[])
#undef F_NAME
#define F_NAME "CompFortConnexe"
{

  Lifo *X;
  int y, z, n;
  pcell p;

  n = g->nsom;
  X = CreeLifoVide(n);

  LifoPush(X, x);
  Z[x] = 1;
  g->v_sommets[x] = Z[z] + 1;
  while (!LifoVide(X))
  {
    y = LifoPop(X);
    for (p = g->gamma[y]; p != NULL; p = p->next)
    {
      z = p->som;
      if (Z[z] == -1)
      {
        Z[z] = 1 - Z[y];
        g->v_sommets[z] = Z[z] + 1;

        LifoPush(X, z);
      }
      else if (Z[z] == Z[y])
        return FALSE;
    }
  }
  LifoTermine(X);
  return TRUE;
}

int main(int argc, char **argv)

{
  graphe *g;
  char buf[256];

  boolean result;

  g = ReadGraphe(argv[1]);

  if (!g)
  {
    printf("Cannot read graph\n");
    exit(1);
  }

  int Z[g->nsom];
  for (int k = 0; k < g->nsom; k++)
  {
    Z[k] = -1;
  }

  for (int i = 0; i < g->nsom; i++)
  {
    if (Z[i] == -1)
    {
      result = EstBipartite(g, i, Z);
      if (!result)
        break;
    }
  }
  if (result)
  {
    printf("Possible to place friends in two car with the given combination\n People in CAR1 = [Joe, Janice, Racheal]\n People in CAR2 = [Monica, Ross, Chandler, Phoebe]\n Graph with two colors corresponding two cars is saved with 'friends.graph.eps' filename\n");
  }
  else
  {
    printf("Not possible to place friends in two cars\n");
  }

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

  free(g);
  return 0;
}
