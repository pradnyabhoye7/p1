#include <stdio.h>
#include <stdlib.h>

int find_closest(int requests[], int n, int head) {
    int min_distance = abs(requests[0] - head);
    int index = 0;
    for (int i = 1; i < n; i++) {
        int distance = abs(requests[i] - head);
        if (distance < min_distance) {
            min_distance = distance;
            index = i;
        }
    }
    return index;
}

int main() {
    int size, n, head, step = 1;
    float seek_time_unit, rotational_latency;

    // Inputs
    printf("Enter the total disk size (number of tracks): ");
    scanf("%d", &size);

    printf("Enter the number of disk requests: ");
    scanf("%d", &n);

    int requests[n];
    printf("Enter the disk request sequence (track numbers):\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &requests[i]);
    }

    printf("Enter the initial head position: ");
    scanf("%d", &head);

    printf("Enter the seek time per track (e.g., 0.1 ms): ");
    scanf("%f", &seek_time_unit);

    printf("Enter the rotational latency per access (e.g., 2.0 ms): ");
    scanf("%f", &rotational_latency);

    // SSTF scheduling
    printf("\nSeek Sequence (SSTF):\n");
    printf("Step\tFrom\tTo\tTracks Moved\tSeek Time (ms)\n");

    float total_seek_time = 0;

    while (n > 0) {
        int index = find_closest(requests, n, head);
        int distance = abs(requests[index] - head);
        float seek_time = distance * seek_time_unit + rotational_latency;

        printf("%d\t%d\t%d\t%d\t\t%.2f\n", step++, head, requests[index], distance, seek_time);

        total_seek_time += seek_time;
        head = requests[index];

        // Remove the processed request
        for (int i = index; i < n - 1; i++) {
            requests[i] = requests[i + 1];
        }
        n--;
    }

    printf("\nTotal Seek Time: %.2f ms\n", total_seek_time);
    printf("Average Seek Time per request: %.2f ms\n", total_seek_time / (step - 1));

    return 0;
}
