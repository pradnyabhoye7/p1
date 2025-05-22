#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

int V = 0;
int adj[MAX][MAX];
char* indexToName[MAX];
char nameMap[MAX][50];

int getIndex(char* name) {
    for (int i = 0; i < V; i++) {
        if (strcmp(nameMap[i], name) == 0) return i;
    }
    strcpy(nameMap[V], name);
    indexToName[V] = nameMap[V];
    return V++;
}

void addEdge(char* from, char* to) {
    int u = getIndex(from);
    int v = getIndex(to);
    adj[u][v] = 1;
}

int isCyclicUtil(int v, int* visited, int* recStack) {
    visited[v] = 1;
    recStack[v] = 1;

    for (int u = 0; u < V; u++) {
        if (adj[v][u]) {
            if (!visited[u] && isCyclicUtil(u, visited, recStack))
                return 1;
            else if (recStack[u])
                return 1;
        }
    }

    recStack[v] = 0;
    return 0;
}

int detectDeadlock() {
    int visited[MAX] = {0};
    int recStack[MAX] = {0};

    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            if (isCyclicUtil(i, visited, recStack)) {
                printf("\nCycle detected involving node: %s\n", indexToName[i]);
                return 1;
            }
        }
    }
    return 0;
}

int main() {
    int numProcesses, numResources;
    printf("Enter number of processes: ");
    scanf("%d", &numProcesses);
    printf("Enter number of resources: ");
    scanf("%d", &numResources);

    char name[50];
    printf("\nEnter names of processes:\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("Process %d: ", i + 1);
        scanf("%s", name);
        getIndex(name);
    }

    printf("\nEnter names of resources:\n");
    for (int i = 0; i < numResources; i++) {
        printf("Resource %d: ", i + 1);
        scanf("%s", name);
        getIndex(name);
    }

    int edges;
    printf("\nEnter number of edges (allocation or request): ");
    scanf("%d", &edges);
    printf("Enter edges in format: FROM TO (example: P1 R1 or R2 P2)\n");

    char from[50], to[50];
    for (int i = 0; i < edges; i++) {
        scanf("%s %s", from, to);
        addEdge(from, to);
    }

    if (detectDeadlock())
        printf("\nDeadlock detected!\n");
    else
        printf("\nNo deadlock detected. System is safe.\n");

    return 0;
}
