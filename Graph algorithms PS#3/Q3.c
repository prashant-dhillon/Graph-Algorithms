/* Author: Prashant Dhillon
    Subject: Graph and Algorithm */

#include <stdlib.h>
#include <stdio.h>
#include "graphaux.h"
#include "graphes.h"

void print_result(int init_vertex, graphe *g)
{
    printf("\n");
    printf("shortest path from source vertex %d to each vertex is : \n", init_vertex);

    for (int i = 0; i < g->nsom; i++)
    {
        if (g->v_sommets[i] > 50000)
        {
            printf("Distance from  %d --> %d = vertex not reachable \n", init_vertex, i);
        }
        else
        {
            printf("Distance from  %d --> %d = %ld\n", init_vertex, i, g->v_sommets[i]);
        }
    }
    printf("\n");
}

boolean check_empty(boolean *v_new_traversable, boolean *v_new, graphe *g)
{
    for (int i = 0; i < g->nsom; i++)
    {
        if (v_new_traversable[i] == TRUE && v_new[i] == TRUE)
        {
            return FALSE;
        }
    }

    return TRUE;
}

void dijkstra(graphe *g, int init_vertex)
{
    int v_count, v_count_succ;
    int last_visited[g->nsom];
    pcell p;
    boolean *v_traversed = EnsembleVide(g->nsom);
    boolean *v_new = EnsembleVide(g->nsom);
    boolean *v_new_traversable = EnsembleVide(g->nsom);

    for (int k = 0; k < g->nsom; k++)
    {
        g->v_sommets[k] = LONG_MAX;
        v_new[k] = TRUE;
        last_visited[k] = -10;
    }
    last_visited[init_vertex] = -1;

    v_new_traversable[init_vertex] = TRUE;
    g->v_sommets[init_vertex] = 0;
    ListeChainee T = listeVide();
    ajouteElmt(&T, init_vertex);

    while (!check_empty(v_new, v_new_traversable, g))
    {
        int temp = -10;
        long min_dist = LONG_MAX;
        LCell *cur = T;
        int x;
        while (cur != NULL)
        {
            x = cur->elmt;
            cur = cur->next;

            if (g->v_sommets[x] < min_dist && v_new[x] == TRUE)
            {
                min_dist = g->v_sommets[x];
                temp = x;
            }
        }
        v_count = temp;

        if (v_traversed[v_count] == TRUE)
        {
            continue;
        }

        for (p = g->gamma[v_count]; p != NULL; p = p->next)
        {
            v_count_succ = p->som;

            if (g->v_sommets[v_count] + p->v_arc < g->v_sommets[v_count_succ])
            {
                if (g->v_sommets[v_count_succ] == LONG_MAX)
                {
                    ajouteElmt(&T, v_count_succ);
                }
                g->v_sommets[v_count_succ] = g->v_sommets[v_count] + p->v_arc;
                last_visited[v_count_succ] = v_count;
            }
            v_new_traversable[v_count_succ] = TRUE;
        }

        v_traversed[v_count] = TRUE;
        v_new[v_count] = FALSE;
    }
    print_result(init_vertex, g);
}

int main(int argc, char **argv)
{
    graphe *g = NULL;
    char buf[256];

    g = ReadGraphe(argv[1]);
    if (g == NULL)
    {
        printf("Graphe file is empty, please try again");
        exit(0);
    }

    printf("Enter a initial vertex and press enter to compute shortest path....\n");
    long int init_vertex;
    scanf("%ld", &init_vertex);

    dijkstra(g, init_vertex);

    TermineGraphe(g);

    return 0;
}