/*! \file graph_algos.c
    \brief algorithmes fondamentaux
*/
/*
      Michel Couprie, septembre 1999

      janvier 2005 : DepthTree, MaxDiameterTree, Lee
      février 2005 : LCA (non efficace)
*/
#include "graphes.h"
#define L_INFINITY LONG_MAX

/* ====================================================================== */
/* ====================================================================== */
/* OPERATEURS DE BASE SUR LES GRAPHES */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn int Degre(graphe * g, int s)
    \param g (entrée) : un graphe
    \param s (entrée) : un sommet
    \return un entier
    \brief retourne le degré du sommet s dans le graphe graphe g (nombre de successeurs)
    \warning seule la représentation 'gamma' est utilisée
*/
int Degre(graphe * g, int s)
/* ====================================================================== */
{
  int d = 0;
  pcell p;
  for (p = g->gamma[s]; p != NULL; p = p->next) d++;
  return d;
} /* Degre() */

/* ====================================================================== */
/*! \fn graphe * FermetureSymetrique(graphe * g)
    \param g (entrée) : un graphe
    \return un graphe
    \brief construit et retourne la fermeture symétrique du graphe g (algorithme linéaire)
    \warning seule la représentation 'gamma' est utilisée
*/
graphe * FermetureSymetrique(graphe * g)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "FermetureSymetrique"
{
  graphe *gs;
  int nsom, narc, al_arcs, k, i, j;
  pcell p;

  nsom = g->nsom;
  narc = g->narc;
  gs = InitGraphe(nsom, 2 * narc);
  for (i = 0; i < nsom; i++) /* pour tout i sommet de g */
  {
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout j successeur de i */
      j = p->som;
      AjouteArcValue(gs, i, j, p->v_arc);
      AjouteArcValue(gs, j, i, p->v_arc);
    } // for p
  } // for i

  return gs;
} /* FermetureSymetrique() */

/* ====================================================================== */
/*! \fn void CompFortConnexe(graphe * g, graphe *g_1, int a, boolean * Ca)
    \param g (entrée) : un graphe
    \param g_1 (entrée) : le graphe symétrique de g
    \param a (entrée) : un sommet du graphe g
    \param Ca (sortie) : un sous-ensemble des sommets de g (tableau de booléens)
    \brief retourne dans Ca la composante fortement connexe de g contenant a 
          (sous la forme d'un tableau de booléens)
    \warning Ca doit avoir été alloué correctement (pas de vérification)
*/
void CompFortConnexe(graphe * g, graphe *g_1, int a, boolean * Ca)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "CompFortConnexe"
{
  boolean * D;   /* pour les "descendants" (successeurs a N niveaux) */
  boolean * A;   /* pour les "ascendants" (predecesseurs a N niveaux) */
  Lifo * T;   /* liste temporaire geree en pile (Last In, First Out) */
  int i, n, s;
  pcell p;

  n = g->nsom;
  T = CreeLifoVide(n);
  A = EnsembleVide(n);
  D = EnsembleVide(n);
  memset(Ca, 0, n);   // Ca := vide

  /* calcule les descendants de a */
  LifoPush(T, a);  
  while (!LifoVide(T))
  {
    i = LifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout sommet s successeur de i */
      s = p->som;
      if (!D[s]) 
      {
        D[s] = TRUE;
        LifoPush(T, s);
      }
    } // for p
  } // while (!LifoVide(T))

  /* calcule les ascendants de a */
  LifoPush(T, a);
  while (!LifoVide(T))
  {
    i = LifoPop(T);
    for (p = g_1->gamma[i]; p != NULL; p = p->next) 
    { /* pour tout s successeur de i */
      s = p->som;
      if (!A[s]) 
      {
        A[s] = TRUE;
        LifoPush(T, s);
      }
    } // for p
  } // while (!LifoVide(T))

  /* intersection de D et de A, union { a } */
  for (i = 0; i < n; i++) Ca[i] = (D[i] && A[i]);
  Ca[a] = TRUE;
    
  free(A);
  free(D);
  LifoTermine(T);
} /* CompFortConnexe() */

/* ====================================================================== */
/*! \fn boolean ExisteCircuit(graphe * g, int a)
    \param g (entrée) : un graphe
    \param a (entrée) : un sommet du graphe g
    \return booléen
    \brief teste l'existence d'un circuit dans g contenant a 
*/
boolean ExisteCircuit(graphe * g, int a)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "ExisteCircuit"
{
  boolean * D;   /* pour les "descendants" (successeurs a N niveaux) */
  Lifo * T;   /* liste temporaire geree en pile (Last In, First Out) */
  int i, n, s;
  pcell p;

  n = g->nsom;
  T = CreeLifoVide(n);
  D = EnsembleVide(n);

  /* calcule les descendants de a */
  LifoPush(T, a);  
  while (!LifoVide(T))
  {
    i = LifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout sommet s successeur de i */
      s = p->som;
      if (s == a) { free(D); LifoTermine(T); return TRUE; }
      if (!D[s]) 
      {
        D[s] = TRUE;
        LifoPush(T, s);
      }
    } // for p
  } // while (!LifoVide(T))

  free(D);
  LifoTermine(T);
  return FALSE;
} /* ExisteCircuit() */

/* ====================================================================== */
/*! \fn void CompConnexe(graphe * g, graphe *g_1, int a, boolean * Ca)
    \param   g (entrée) : un graphe
    \param g_1 (entrée) : le graphe symétrique de g
    \param a (entrée) : un sommet du graphe g
    \return un sous-ensemble de sommets de g (tableau de booléens)
    \brief retourne la composante connexe de g contenant a
          (sous la forme d'un tableau de booléens)
*/
void CompConnexe(graphe * g, graphe *g_1, int a, boolean * Ca)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "CompConnexe"
{
  Lifo * T;   /* liste temporaire geree en pile (Last In, First Out) */
  int i, n, s;
  pcell p;

  n = g->nsom;
  T = CreeLifoVide(n);
  memset(Ca, 0, n); // Ca := vide

  Ca[a] = TRUE;
  LifoPush(T, a);  
  while (!LifoVide(T))
  {
    i = LifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s successeur de i */
      s = p->som;
      if (!Ca[s]) 
      {
        Ca[s] = TRUE;
        LifoPush(T, s);
      }
    } // for p
    for (p = g_1->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s successeur de i dans g_1 */
      s = p->som;
      if (!Ca[s]) 
      {
        Ca[s] = TRUE;
        LifoPush(T, s);
      }
    } // for p
  } // while (!LifoVide(T))

  LifoTermine(T);
} /* CompConnexe() */

/* ====================================================================== */
/*! \fn int Connexe(graphe * g, graphe *g_1)
    \param g (entrée) : un graphe
    \param g_1 (entrée) : le graphe symétrique de g
    \return booléen
    \brief retourne 1 si le graphe est connexe, 0 sinon
*/
boolean Connexe(graphe * g, graphe *g_1)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "Connexe"
{
  int i;
  boolean ret = TRUE;
  int n = g->nsom;
  boolean * C = EnsembleVide(n);
  CompConnexe(g, g_1, 0, C);
  for (i = 0; i < n; i++) if (!C[i]) { ret = FALSE; break; }
  free(C);
  return ret;
} /* Connexe() */

/* ====================================================================== */
/*! \fn boolean CircuitNiveaux(graphe * g)
    \param g (entrée) : un graphe
    \return booléen
    \brief si le graphe possède au moins un circuit, retourne FALSE ;
    sinon, calcule les niveaux des sommets du graphes (dans v_sommets) et retourne TRUE
*/
boolean CircuitNiveaux(graphe * g, int a)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "CircuitNiveaux"
{
  int * D;   /* pour les degrés intérieurs */
  int i, n, s;
  pcell p;

  n = g->nsom;

  // A FINIR

  free(D);
  return FALSE;
} /* CircuitNiveaux() */

/* ====================================================================== */
/* ====================================================================== */
/* ARBRES */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn void DepthTree(graphe * g, graphe *g_1, int a, TYP_VARC *depth, int *farthest)
    \param g (entrée) : un graphe (arbre), symétrique ou non
    \param a (entrée) : un sommet du graphe g
    \param depth (sortie) : la profondeur de l'arbre de racine a
    \param farthest (sortie) : un sommet le plus éloigné de a 
    \brief calcule la profondeur de l'arbre de racine a
*/
void DepthTree(graphe * g, int a, TYP_VARC *depth, int *farthest)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "DepthTree"
{
  Lifo * T;   /* liste temporaire geree en pile (Last In, First Out) */
  int i, f, n, s;
  pcell p;
  TYP_VARC* dist;
  TYP_VARC d;

  n = g->nsom;
  dist = (TYP_VARC *)malloc(n * sizeof(TYP_VARC));
  T = CreeLifoVide(n);
  for (i = 0; i < n; i++) dist[i] = -1;

  d = dist[a] = 0;
  LifoPush(T, a);  
  while (!LifoVide(T))
  {
    i = LifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s successeur de i */
      s = p->som;
      if (dist[s] == -1) 
      {
        dist[s] = dist[i] + p->v_arc;
	if (dist[s] > d) { d = dist[s]; f = s; }
        LifoPush(T, s);
      }
    } // for p
  } // while (!LifoVide(T))

  free(dist);
  LifoTermine(T);
  *depth = d;
  *farthest = f;
} /* DepthTree() */

/* ====================================================================== */
/*! \fn graphe * RootTree(graphe * g, graphe * g_1, int i)
    \param g (entrée) : un graphe
    \param g_1 (entrée) : le graphe symétrique du graphe g
    \param i (entrée) : un sommet de \b g
    \brief retourne un arbre orienté (arboresence) de racine i
    dont les arcs correspondent aux arêtes du graphe non-orienté représenté par g et g_1.
    Les distances (nombre d'arcs) de la racine aux sommets sont de plus stockés dans les champs v_sommets.
*/
graphe * RootTree(graphe * g, graphe * g_1, int i)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "RootTree"
{
  int k, s, n;
  pcell p;
  Fifo * T;
  graphe * tree;

  if ((g == NULL) || (g_1 == NULL))
  {
    fprintf(stderr,"%s: input graph not available\n", F_NAME);
    exit(0);
  }
  n = g->nsom;

  T = CreeFifoVide(n);
  if (T == NULL)
  {
    fprintf(stderr,"%s: CreeFifoVide failed\n", F_NAME);
    exit(0);
  }

  tree = InitGraphe(n, n-1);
  if (tree == NULL)
  {
    fprintf(stderr,"%s: InitGraphe failed\n", F_NAME);
    exit(0);
  }

  for (k = 0; k < n; k++) tree->v_sommets[k] = -1;
  tree->v_sommets[i] = 0;
  FifoPush(T, i);

  while (!FifoVide(T))
  {
    i = FifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s successeur de i */
      s = p->som;
      if (tree->v_sommets[s] == -1) 
      {
	AjouteArcValue(tree, i, s, p->v_arc);
	tree->v_sommets[s] = tree->v_sommets[i] + 1;
	FifoPush(T, s);
      }
    } // for p
    for (p = g_1->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s prédécesseur de i */
      s = p->som;
      if (tree->v_sommets[s] == -1) 
      {
	AjouteArcValue(tree, i, s, p->v_arc);
	tree->v_sommets[s] = tree->v_sommets[i] + 1;
	FifoPush(T, s);
      }
    } // for p
  } // while (!FifoVide(T))

  FifoTermine(T);
  return tree;
} /* RootTree() */

/* ====================================================================== */
/*! \fn void DistRoot(graphe * g, int r)
    \param g (entrée) : un graphe (arbre orienté ou arborescence)
    \param r (entrée) : racine de \b g
    \brief calcule, pour chaque sommet x de g, la longueur (somme des poids d'arcs) d'un plus court
           chemin de r vers x. Cette longueur est stockée dans le champ
           \b v_sommets de \b g
*/
void DistRoot(graphe * g, int r)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "DistRoot"
{
  int k, s, n = g->nsom;
  pcell p;
  Fifo * T = CreeFifoVide(n);

  for (k = 0; k < n; k++) g->v_sommets[k] = -1;
  g->v_sommets[r] = 0;
  FifoPush(T, r);

  while (!FifoVide(T))
  {
    r = FifoPop(T);
    for (p = g->gamma[r]; p != NULL; p = p->next)
    { /* pour tout s successeur de r */
      s = p->som;
      if (g->v_sommets[s] == -1) 
      {
        g->v_sommets[s] = g->v_sommets[r] + p->v_arc;
        FifoPush(T, s);
      }
    } // for p
  } // while (!FifoVide(T))

  FifoTermine(T);
} /* DistRoot() */

/* ====================================================================== */
/* ====================================================================== */
/* PLUS COURTS CHEMINS */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn void Lee(graphe * g, int i)
    \param g (entrée) : un graphe
    \param i (entrée) : un sommet de \b g
    \brief calcule, pour chaque sommet x de g, la longueur (nombre d'arcs) d'un plus court
           chemin de i vers x. Cette longueur est stockée dans le champ
           \b v_sommets de \b g
*/
void Lee(graphe * g, int i)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "Lee"
{
  int k, s, n = g->nsom;
  pcell p;
  Fifo * T = CreeFifoVide(n);

  for (k = 0; k < n; k++) g->v_sommets[k] = -1;
  g->v_sommets[i] = 0;
  FifoPush(T, i);

  while (!FifoVide(T))
  {
    i = FifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s successeur de i */
      s = p->som;
      if (g->v_sommets[s] == -1) 
      {
        g->v_sommets[s] = g->v_sommets[i] + 1;
        FifoPush(T, s);
      }
    } // for p
  } // while (!FifoVide(T))

  FifoTermine(T);
} /* Lee() */

/* ====================================================================== */
/*! \fn void LeeNO(graphe * g, graphe * g_1, int i)
    \param g (entrée) : un graphe
    \param g_1 (entrée) : le graphe symétrique du graphe g
    \param i (entrée) : un sommet de \b g
    \brief calcule, pour chaque sommet x de g, la longueur (nombre d'arêtes) 
    d'un plus court chemin de i vers x dans le graphe non orienté représenté 
    par (g, g_1). Cette longueur est stockée dans le champ \b v_sommets de \b g
*/
void LeeNO(graphe * g, graphe * g_1, int i)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "LeeNO"
{
  int k, s, n = g->nsom;
  pcell p;
  Fifo * T = CreeFifoVide(n);

  for (k = 0; k < n; k++) g->v_sommets[k] = -1;
  g->v_sommets[i] = 0;
  FifoPush(T, i);

  while (!FifoVide(T))
  {
    i = FifoPop(T);
    for (p = g->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s successeur de i */
      s = p->som;
      if (g->v_sommets[s] == -1) 
      {
        g->v_sommets[s] = g->v_sommets[i] + 1;
        FifoPush(T, s);
      }
    } // for p
    for (p = g_1->gamma[i]; p != NULL; p = p->next)
    { /* pour tout s prédécesseur de i */
      s = p->som;
      if (g->v_sommets[s] == -1) // sic: g
      {
        g->v_sommets[s] = g->v_sommets[i] + 1;  // sic: g
        FifoPush(T, s);
      }
    } // for p
  } // while (!FifoVide(T))

  FifoTermine(T);
} /* LeeNO() */

/* ====================================================================== */
/* ====================================================================== */
/* PROGRAMME DE TEST */
/* ====================================================================== */
/* ====================================================================== */

#ifdef TEST
int main(int argc, char **argv)
{
  graphe * g, *g_1, *a;
  int s1, s2, na, ns;
  boolean *Cs;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s <nombre sommets> <nombre arcs>\n", argv[0]);
    exit(0);
  }

  ns = atoi(argv[1]);
  na = atoi(argv[2]);
  
  g = GrapheAleatoire(ns, na);
  AfficheSuccesseurs(g);
  AfficheArcs(g);
  g_1 = Symetrique(g);
  AfficheSuccesseurs(g_1);
  AfficheArcs(g_1);

  if (Connexe(g, g_1))
  {
    printf("graphe connexe\n");
    a = Kruskal2(g, g_1);
    AfficheSuccesseurs(a);
    AfficheArcs(a);
    TermineGraphe(a);
  }
  else printf("graphe NON connexe\n");

  do
  {
    printf("entrer un sommet : ");
    scanf("%d", &s1);
    if (s1 >= 0) 
    {
      Cs = CompFortConnexe(g, g_1, s1);
      AfficheEnsemble(Cs, g->nsom);
      free(Cs);
    }
  } while (s1 >= 0);

  do
  {
    printf("entrer un sommet : ");
    scanf("%d", &s1);
    if (s1 >= 0) 
    {
      Cs = CompConnexe(g, g_1, s1);
      AfficheEnsemble(Cs, g->nsom);
      free(Cs);
    }
  } while (s1 >= 0);

  TermineGraphe(g);
  TermineGraphe(g_1);

} /* main() */
#endif
