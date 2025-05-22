#include <stdio.h>
#include <stdlib.h>

void fifo(int virtual_addresses[], int count, int page_size, int frame_count) {
    int pages[count];
    int frames[frame_count];
    int page_faults = 0, page_hits = 0, oldest = 0;

    // Convert logical addresses to page numbers
    printf("\nVirtual Address to Page Number Mapping:\n");
    printf("Address\t-> Page Number (Address / Page Size)\n");
    for (int i = 0; i < count; i++) {
        pages[i] = virtual_addresses[i] / page_size;
        printf("%d\t-> %d ( %d / %d )\n", virtual_addresses[i], pages[i], virtual_addresses[i], page_size);
    }

    // Initialize frames
    for (int i = 0; i < frame_count; i++) {
        frames[i] = -1;
    }

    // FIFO algorithm
    printf("\nPage Frame Status (FIFO):\n");
    for (int i = 0; i < count; i++) {
        int hit = 0;
        for (int j = 0; j < frame_count; j++) {
            if (frames[j] == pages[i]) {
                hit = 1;
                page_hits++;
                break;
            }
        }

        if (hit) {
            printf("Page Hit\t| Page: %d | Frames: ", pages[i]);
        } else {
            frames[oldest] = pages[i];
            oldest = (oldest + 1) % frame_count;
            page_faults++;
            printf("Page Fault\t| Page: %d | Frames: ", pages[i]);
        }

        for (int j = 0; j < frame_count; j++) {
            if (frames[j] != -1)
                printf("%d ", frames[j]);
            else
                printf("- ");
        }
        printf("\n");
    }

    // Output results
    printf("\nSummary:\n");
    printf("Total Logical Addresses: %d\n", count);
    printf("Total Page Faults: %d\n", page_faults);
    printf("Total Page Hits: %d\n", page_hits);
    printf("Page Fault Rate: %.2f\n", (float)page_faults / count);
    printf("Page Hit Rate: %.2f\n", (float)page_hits / count);
}

int main() {
    int count, frame_count, page_size;

    // Input virtual address details
    printf("Enter the number of virtual/logical addresses: ");
    scanf("%d", &count);

    int virtual_addresses[count];
    printf("Enter each virtual/logical address (integer values):\n");
    for (int i = 0; i < count; i++) {
        scanf("%d", &virtual_addresses[i]);
    }

    printf("Enter the page size (in bytes): ");
    scanf("%d", &page_size);

    printf("Enter the number of page frames in memory: ");
    scanf("%d", &frame_count);

    fifo(virtual_addresses, count, page_size, frame_count);

    return 0;
}
