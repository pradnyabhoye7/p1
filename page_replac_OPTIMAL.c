#include <stdio.h>

int findOptimalPage(int pages[], int n, int frames[], int frameCount, int currentIndex) {
    int farthest = currentIndex;
    int pageToReplace = -1;

    for (int i = 0; i < frameCount; i++) {
        int j;
        for (j = currentIndex; j < n; j++) {
            if (pages[j] == frames[i]) {
                if (j > farthest) {
                    farthest = j;
                    pageToReplace = i;
                }
                break;
            }
        }

        // If page not found in future, it's best to replace it
        if (j == n) {
            return i;
        }
    }

    return (pageToReplace == -1) ? 0 : pageToReplace;
}

int main() {
    int n, frameCount, pageSize;

    printf("Enter the number of logical addresses: ");
    scanf("%d", &n);

    int logicalAddresses[n];
    printf("Enter the logical addresses:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &logicalAddresses[i]);
    }

    printf("Enter the page size (in bytes): ");
    scanf("%d", &pageSize);

    printf("Enter the number of frames: ");
    scanf("%d", &frameCount);

    int pageNumbers[n];
    printf("\nLogical Address Breakdown:\n");
    for (int i = 0; i < n; i++) {
        pageNumbers[i] = logicalAddresses[i] / pageSize;
        int offset = logicalAddresses[i] % pageSize;
        printf("Logical Addr: %d => Page: %d, Offset: %d\n", logicalAddresses[i], pageNumbers[i], offset);
    }

    int frames[frameCount];
    for (int i = 0; i < frameCount; i++) {
        frames[i] = -1;
    }

    int pageFaults = 0, pageHits = 0;

    printf("\nPage Frame Status:\n");
    for (int i = 0; i < n; i++) {
        int page = pageNumbers[i];
        int isHit = 0;

        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == page) {
                isHit = 1;
                pageHits++;
                break;
            }
        }

        if (isHit) {
            printf("Page Hit:   ");
        } else {
            pageFaults++;
            int pageToReplace = -1;

            // Find an empty frame first
            for (int j = 0; j < frameCount; j++) {
                if (frames[j] == -1) {
                    pageToReplace = j;
                    break;
                }
            }

            // No empty frame: use optimal replacement
            if (pageToReplace == -1) {
                pageToReplace = findOptimalPage(pageNumbers, n, frames, frameCount, i + 1);
            }

            frames[pageToReplace] = page;
            printf("Page Fault: ");
        }

        for (int j = 0; j < frameCount; j++) {
            if (frames[j] != -1)
                printf("%d ", frames[j]);
            else
                printf("X ");
        }
        printf("\n");
    }

    printf("\n--- Summary ---\n");
    printf("Total Page Faults: %d\n", pageFaults);
    printf("Total Page Hits: %d\n", pageHits);
    printf("Page Fault Rate: %.2f\n", (float)pageFaults / n);
    printf("Page Hit Rate: %.2f\n", (float)pageHits / n);

    return 0;
}
