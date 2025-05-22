#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int disk_size, n, head, total_tracks_moved = 0, step = 1;
    float latency_per_track;

    // User Inputs
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

    // Processing Requests in FCFS Order
    printf("\nSeek Sequence (FCFS):\n");
    printf("Step\tFrom\tTo\tSeek Distance\n");

    for (int i = 0; i < n; i++) {
        int seek = abs(requests[i] - head);
        printf("%d\t%d\t%d\t%d\n", step++, head, requests[i], seek);
        total_tracks_moved += seek;
        head = requests[i];
    }

    // Output Calculations
    float total_seek_time = total_tracks_moved * latency_per_track;
    float avg_seek_time = total_seek_time / n;

    printf("\nTotal Tracks Moved (Seek Distance): %d\n", total_tracks_moved);
    printf("Total Seek Time: %.2f ms\n", total_seek_time);
    printf("Average Seek Time: %.2f ms\n", avg_seek_time);

    return 0;
}
