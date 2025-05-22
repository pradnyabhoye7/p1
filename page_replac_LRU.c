#include <stdio.h>

void lru(int page_numbers[], int n, int frame_count) {
    int frames[frame_count];
    int time[frame_count];
    int page_faults = 0, page_hits = 0;

    for (int i = 0; i < frame_count; i++) {
        frames[i] = -1;
        time[i] = -1;
    }

    printf("\nPage Frame Status:\n");

    for (int i = 0; i < n; i++) {
        int flag = 0;

        for (int j = 0; j < frame_count; j++) {
            if (frames[j] == page_numbers[i]) {
                flag = 1;
                page_hits++;
                time[j] = i;
                break;
            }
        }

        if (flag == 0) {
            int min_time = i, min_index = 0;
            for (int j = 0; j < frame_count; j++) {
                if (time[j] < min_time) {
                    min_time = time[j];
                    min_index = j;
                }
            }

            frames[min_index] = page_numbers[i];
            time[min_index] = i;
            page_faults++;

            printf("Page Fault: ");
        } else {
            printf("Page Hit:   ");
        }

        for (int k = 0; k < frame_count; k++) {
            if (frames[k] != -1)
                printf("%d ", frames[k]);
            else
                printf("- ");
        }
        printf("\n");
    }

    printf("\n--- Summary ---\n");
    printf("Total Page Faults: %d\n", page_faults);
    printf("Total Page Hits: %d\n", page_hits);
    printf("Page Fault Rate: %.2f\n", (float)page_faults / n);
    printf("Page Hit Rate: %.2f\n", (float)page_hits / n);
}

int main() {
    int n, frame_count, page_size;

    printf("Enter the number of logical addresses: ");
    scanf("%d", &n);

    int logical_addresses[n];

    printf("Enter the logical addresses:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &logical_addresses[i]);
    }

    printf("Enter the page size (in bytes): ");
    scanf("%d", &page_size);

    printf("Enter the number of frames: ");
    scanf("%d", &frame_count);

    int page_numbers[n];

    printf("\nLogical Address Breakdown:\n");
    for (int i = 0; i < n; i++) {
        int page_number = logical_addresses[i] / page_size;
        int offset = logical_addresses[i] % page_size;
        page_numbers[i] = page_number;
        printf("Logical Addr: %d => Page: %d, Offset: %d\n", logical_addresses[i], page_number, offset);
    }

    lru(page_numbers, n, frame_count);

    return 0;
}
