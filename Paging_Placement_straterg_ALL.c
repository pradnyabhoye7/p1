#include <stdio.h>
#include <stdlib.h>

void print_Block(int blockSize[], int m) {
    printf("Current Block Sizes: ");
    for (int i = 0; i < m; i++) {
        printf("%d ", blockSize[i]);
    }
    printf("\n");
}

int compute_ext_frag(int blockSize[], int m) {
    int ext_frag = 0;
    for (int i = 0; i < m; i++)
        ext_frag += blockSize[i];
    return ext_frag;
}

void First_Fit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];
    for (int i = 0; i < n; i++) allocation[i] = -1;

    printf("\n--- First Fit Allocation ---\n");

    for (int i = 0; i < n; i++) {
        printf("\nAllocating process %d of size %d: ", i + 1, processSize[i]);
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                printf("Allocated to block %d\n", j + 1);
                break;
            }
        }

        if (allocation[i] == -1)
            printf("Not allocated (external fragmentation may occur)\n");

        print_Block(blockSize, m);
    }

    int ext_frag = compute_ext_frag(blockSize, m);
    printf("External Fragmentation after First Fit: %d\n", ext_frag);
}

void Next_Fit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n], lastPos = 0;

    for (int i = 0; i < n; i++) allocation[i] = -1;

    printf("\n--- Next Fit Allocation ---\n");

    for (int i = 0; i < n; i++) {
        printf("\nAllocating process %d of size %d: ", i + 1, processSize[i]);
        int j = lastPos, count = 0;
        while (count < m) {
            if (blockSize[j] >= processSize[i]) {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                lastPos = j;
                printf("Allocated to block %d\n", j + 1);
                break;
            }
            j = (j + 1) % m;
            count++;
        }

        if (allocation[i] == -1)
            printf("Not allocated (external fragmentation may occur)\n");

        print_Block(blockSize, m);
    }

    int ext_frag = compute_ext_frag(blockSize, m);
    printf("External Fragmentation after Next Fit: %d\n", ext_frag);
}

void Best_Fit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];
    for (int i = 0; i < n; i++) allocation[i] = -1;

    printf("\n--- Best Fit Allocation ---\n");

    for (int i = 0; i < n; i++) {
        int bestIdx = -1;
        printf("\nAllocating process %d of size %d: ", i + 1, processSize[i]);

        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx])
                    bestIdx = j;
            }
        }

        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            blockSize[bestIdx] -= processSize[i];
            printf("Allocated to block %d\n", bestIdx + 1);
        } else {
            printf("Not allocated (external fragmentation may occur)\n");
        }

        print_Block(blockSize, m);
    }

    int frag = compute_ext_frag(blockSize, m);
    printf("External Fragmentation after Best Fit: %d\n", frag);
}

void Worst_Fit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n];
    for (int i = 0; i < n; i++) allocation[i] = -1;

    printf("\n--- Worst Fit Allocation ---\n");

    for (int i = 0; i < n; i++) {
        int worstIdx = -1;
        printf("\nAllocating process %d of size %d: ", i + 1, processSize[i]);

        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx])
                    worstIdx = j;
            }
        }

        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            blockSize[worstIdx] -= processSize[i];
            printf("Allocated to block %d\n", worstIdx + 1);
        } else {
            printf("Not allocated (external fragmentation may occur)\n");
        }

        print_Block(blockSize, m);
    }

    int frag = compute_ext_frag(blockSize, m);
    printf("External Fragmentation after Worst Fit: %d\n", frag);
}

int main() {
    int m, n;
    printf("Enter the number of memory blocks: ");
    scanf("%d", &m);

    int blockSize[m];
    printf("Enter the sizes of %d blocks:\n", m);
    for (int i = 0; i < m; i++)
        scanf("%d", &blockSize[i]);

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    int processSize[n];
    printf("Enter the sizes of %d processes:\n", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &processSize[i]);

    // Create separate copies of block sizes for each strategy
    int FF[m], BF[m], NF[m], WF[m];
    for (int i = 0; i < m; i++) {
        FF[i] = BF[i] = NF[i] = WF[i] = blockSize[i];
    }

    First_Fit(FF, m, processSize, n);
    Best_Fit(BF, m, processSize, n);
    Next_Fit(NF, m, processSize, n);
    Worst_Fit(WF, m, processSize, n);

    return 0;
}
