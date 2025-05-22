#include <stdio.h>
#include <stdbool.h>

int main() {
    int P, R;
    printf("Banker's Algorithm\n\n");

    // Input
    printf("Enter number of processes: ");
    scanf("%d", &P);
    printf("Enter number of resource types: ");
    scanf("%d", &R);

    int alloc[P][R], max[P][R], need[P][R], avail[R];
    bool finish[P];
    int safeSeq[P], count = 0;

    // Allocation Matrix
    printf("\nEnter Allocation Matrix (%d x %d):\n", P, R);
    for (int i = 0; i < P; i++) {
        printf("Allocation for Process %d: ", i);
        for (int j = 0; j < R; j++) {
            scanf("%d", &alloc[i][j]);
        }
    }

    // Max Matrix
    printf("\nEnter Maximum Matrix (%d x %d):\n", P, R);
    for (int i = 0; i < P; i++) {
        printf("Max demand for Process %d: ", i);
        for (int j = 0; j < R; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    // Available Resources
    printf("\nEnter Available Resources (%d):\n", R);
    for (int i = 0; i < R; i++) {
        scanf("%d", &avail[i]);
    }

    // Need Matrix Calculation
    printf("\nCalculated Need Matrix:\n");
    for (int i = 0; i < P; i++) {
        printf("Need for Process %d: ", i);
        for (int j = 0; j < R; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }

    // Initialization
    for (int i = 0; i < P; i++)
        finish[i] = false;

    printf("\n=== Step-by-Step Safe Sequence Check ===\n");

    while (count < P) {
        bool found = false;

        for (int i = 0; i < P; i++) {
            if (!finish[i]) {
                bool canAllocate = true;

                printf("\nChecking if Process %d can be allocated: ", i);
                for (int j = 0; j < R; j++) {
                    if (need[i][j] > avail[j]) {
                        canAllocate = false;
                        break;
                    }
                }

                if (canAllocate) {
                    printf("Yes\nAllocating resources to Process %d.\n", i);
                    printf("Available before execution: ");
                    for (int j = 0; j < R; j++) printf("%d ", avail[j]);
                    printf("\n");

                    for (int j = 0; j < R; j++)
                        avail[j] += alloc[i][j];

                    finish[i] = true;
                    safeSeq[count++] = i;
                    found = true;

                    printf("Process %d has completed.\n", i);
                    printf("Updated Available Resources: ");
                    for (int j = 0; j < R; j++) printf("%d ", avail[j]);
                    printf("\n");
                } else {
                    printf("No (Not enough resources available)\n");
                }
            }
        }

        if (!found) {
            printf("\nSystem is in an UNSAFE state! No safe sequence possible.\n");
            return 1;
        }
    }

    // Safe sequence found
    printf("\nSystem is in a SAFE state.\nSafe Sequence: ");
    for (int i = 0; i < P; i++) {
        printf("P%d", safeSeq[i]);
        if (i != P - 1) printf(" -> ");
    }
    printf("\n");

    return 0;
}
