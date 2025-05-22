#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Bubble sort function
void sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
}

int main() {
    int disk_size, n, head, direction, total_tracks_moved = 0, step = 1;
    float latency_per_track;

    // Input section
    printf("Enter total disk size (number of tracks): ");
    scanf("%d", &disk_size);

    printf("Enter number of disk requests: ");
    scanf("%d", &n);

    int requests[n];
    printf("Enter disk request sequence (track numbers):\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &requests[i]);
        if (requests[i] < 0 || requests[i] >= disk_size) {
            printf("Invalid request: %d. Must be between 0 and %d.\n", requests[i], disk_size - 1);
            return 1;
        }
    }

    printf("Enter the initial head position: ");
    scanf("%d", &head);
    if (head < 0 || head >= disk_size) {
        printf("Invalid head position. Must be between 0 and %d.\n", disk_size - 1);
        return 1;
    }

    printf("Enter rotational latency per track (ms): ");
    scanf("%f", &latency_per_track);

    printf("Enter the direction (1 for right, 0 for left): ");
    scanf("%d", &direction);
    if (direction != 0 && direction != 1) {
        printf("Invalid direction. Enter 1 for right or 0 for left.\n");
        return 1;
    }

    sort(requests, n);

    // Processing SCAN logic
    printf("\nSeek Sequence (SCAN):\n");
    printf("Step\tFrom\tTo\tSeek Distance\n");

    if (direction == 1) {  // Moving right
        int i;
        for (i = 0; i < n; i++) {
            if (requests[i] >= head) {
                break;
            }
        }
        for (int j = i; j < n; j++) {
            int seek = abs(requests[j] - head);
            printf("%d\t%d\t%d\t%d\n", step++, head, requests[j], seek);
            total_tracks_moved += seek;
            head = requests[j];
        }
        // Go to end
        if (head != disk_size - 1) {
            printf("%d\t%d\t%d\t%d\n", step++, head, disk_size - 1, disk_size - 1 - head);
            total_tracks_moved += (disk_size - 1 - head);
            head = disk_size - 1;
        }
        // Now go left
        for (int j = i - 1; j >= 0; j--) {
            int seek = abs(requests[j] - head);
            printf("%d\t%d\t%d\t%d\n", step++, head, requests[j], seek);
            total_tracks_moved += seek;
            head = requests[j];
        }
    } else {  // Moving left
        int i;
        for (i = n - 1; i >= 0; i--) {
            if (requests[i] <= head) {
                break;
            }
        }
        for (int j = i; j >= 0; j--) {
            int seek = abs(requests[j] - head);
            printf("%d\t%d\t%d\t%d\n", step++, head, requests[j], seek);
            total_tracks_moved += seek;
            head = requests[j];
        }
        // Go to start
        if (head != 0) {
            printf("%d\t%d\t%d\t%d\n", step++, head, 0, head);
            total_tracks_moved += head;
            head = 0;
        }
        // Now go right
        for (int j = i + 1; j < n; j++) {
            int seek = abs(requests[j] - head);
            printf("%d\t%d\t%d\t%d\n", step++, head, requests[j], seek);
            total_tracks_moved += seek;
            head = requests[j];
        }
    }

    float total_seek_time = total_tracks_moved * latency_per_track;
    float avg_seek_time = total_seek_time / n;

    // Final results
    printf("\nTotal Tracks Moved (Seek Distance): %d\n", total_tracks_moved);
    printf("Total Seek Time: %.2f ms\n", total_seek_time);
    printf("Average Seek Time: %.2f ms\n", avg_seek_time);

    return 0;
}
