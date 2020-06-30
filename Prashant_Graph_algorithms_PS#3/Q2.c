/* Author: Prashant Dhillon
    Subject: Graph and Algorithm */

#include <stdlib.h>
#include <stdio.h>
#include "graphaux.h"
#include "graphes.h"

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

void dijkstra_shortestpath(graphe *g, int init_vertex, int terminate_vertex)
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

    while (!check_empty(v_new, v_new_traversable, g))
    {
        int temp = -7;
        long min_dist = LONG_MAX;
        for (int i = 0; i < g->nsom; i++)
        {
            if (g->v_sommets[i] < min_dist && v_new[i] == TRUE)
            {
                min_dist = g->v_sommets[i];
                temp = i;
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

            if (g->v_sommets[v_count] != LONG_MAX && g->v_sommets[v_count] + p->v_arc < g->v_sommets[v_count_succ])
            {
                g->v_sommets[v_count_succ] = g->v_sommets[v_count] + p->v_arc;
                last_visited[v_count_succ] = v_count;
            }
            v_new_traversable[v_count_succ] = TRUE;
        }

        v_traversed[v_count] = TRUE;
        v_new[v_count] = FALSE;
        if (v_traversed[terminate_vertex] == TRUE)
        {
            continue;
        }
    }

    if (g->v_sommets[terminate_vertex] == LONG_MAX)
    {
        printf("\nNo path found between %d to %d.\n\n", init_vertex, terminate_vertex);
        exit(0);
    }

    Lifo *traceroute = CreeLifoVide(g->nsom);
    int on_vertex = terminate_vertex;

    while (on_vertex != -1)
    {
        LifoPush(traceroute, on_vertex);
        on_vertex = last_visited[on_vertex];
    }
    int route;
    printf("\n");
    printf("Shortest Path distance from %d to %d is %ld:\n", init_vertex, terminate_vertex, g->v_sommets[terminate_vertex]);
    printf("For the above distance calculation path followed is:\n");
    while (!LifoVide(traceroute))
    {
        route = LifoPop(traceroute);
        printf(" %d ->", route);
    }
    printf("\n");
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

    printf("Enter a initial vertex and press enter to proceed....\n");
    long int init_vertex;
    scanf("%ld", &init_vertex);
    printf("Enter a terminating vertex and press enter to compute shortest path between initial and terminate point....\n");
    long int terminate_vertex;
    scanf("%ld", &terminate_vertex);

    dijkstra_shortestpath(g, init_vertex, terminate_vertex);

    TermineGraphe(g);

    return 0;
}
