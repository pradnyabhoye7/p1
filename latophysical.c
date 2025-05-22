#include <stdio.h>

#define MAX 10

typedef struct {
    int base;
    int limit;
} Segment;

int main() {
    int method;
    printf("Choose Address Translation Method:\n");
    printf("1. Paging\n");
    printf("2. Segmentation\n");
    printf("Enter choice: ");
    scanf("%d", &method);

    if (method == 1) {
        // PAGING
        int frame_size, num_pages, logical_address;
        int page_table[MAX];

        printf("\n--- Paging ---\n");
        printf("Enter frame size (bytes): ");
        scanf("%d", &frame_size);

        printf("Enter number of pages: ");
        scanf("%d", &num_pages);

        printf("Enter the page table (frame number for each page):\n");
        for (int i = 0; i < num_pages; i++) {
            printf("Page %d -> Frame: ", i);
            scanf("%d", &page_table[i]);
        }

        printf("Enter logical address (in decimal): ");
        scanf("%d", &logical_address);

        int page_number = logical_address / frame_size;
        int offset = logical_address % frame_size;

        if (page_number >= num_pages) {
            printf("Invalid logical address: page number out of bounds!\n");
        } else {
            int physical_address = page_table[page_number] * frame_size + offset;
            printf("Physical Address = %d\n", physical_address);
        }

    } else if (method == 2) {
        // SEGMENTATION
        Segment segment_table[MAX];
        int num_segments;

        printf("\n--- Segmentation ---\n");
        printf("Enter number of segments: ");
        scanf("%d", &num_segments);

        for (int i = 0; i < num_segments; i++) {
            printf("Segment %d base: ", i);
            scanf("%d", &segment_table[i].base);
            printf("Segment %d limit: ", i);
            scanf("%d", &segment_table[i].limit);
        }

        int seg_no, offset;
        printf("Enter segment number: ");
        scanf("%d", &seg_no);
        printf("Enter offset: ");
        scanf("%d", &offset);

        if (seg_no >= num_segments) {
            printf("Invalid segment number!\n");
        } else if (offset >= segment_table[seg_no].limit) {
            printf("Invalid offset! Exceeds segment limit.\n");
        } else {
            int physical_address = segment_table[seg_no].base + offset;
            printf("Physical Address = %d\n", physical_address);
        }

    } else {
        printf("Invalid method selected!\n");
    }

    return 0;
}