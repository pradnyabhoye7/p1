#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    int disk_size, n, head, total_tracks_moved = 0, step = 1;
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
            printf("Invalid request: %d. Track numbers must be between 0 and %d.\n", requests[i], disk_size - 1);
            return 1;
        }
    }

    printf("Enter initial head position: ");
    scanf("%d", &head);
    if (head < 0 || head >= disk_size) {
        printf("Invalid head position. Must be between 0 and %d.\n", disk_size - 1);
        return 1;
    }

    printf("Enter rotational latency per track (ms): ");
    scanf("%f", &latency_per_track);

    int greater[n], lesser[n], g = 0, l = 0;

    for (int i = 0; i < n; i++) {
        if (requests[i] >= head)
            greater[g++] = requests[i];
        else
            lesser[l++] = requests[i];
    }

    sort(greater, g);
    sort(lesser, l);

    printf("\nSeek Sequence (C-SCAN):\n");
    printf("Step\tFrom\tTo\tSeek Distance\n");

    // Process all requests to the right (greater)
    for (int i = 0; i < g; i++) {
        printf("%d\t%d\t%d\t%d\n", step++, head, greater[i], abs(greater[i] - head));
        total_tracks_moved += abs(greater[i] - head);
        head = greater[i];
    }

    // Move to 0 from the last head position (wrap-around)
    if (g > 0 || l > 0) {
        printf("%d\t%d\t0\t%d\n", step++, head, 0, head);
        total_tracks_moved += head;
        head = 0;
    }

    // Process remaining requests to the left (lesser)
    for (int i = 0; i < l; i++) {
        printf("%d\t%d\t%d\t%d\n", step++, head, lesser[i], abs(lesser[i] - head));
        total_tracks_moved += abs(lesser[i] - head);
        head = lesser[i];
    }

    float total_seek_time = total_tracks_moved * latency_per_track;
    float avg_seek_time = total_seek_time / n;

    printf("\nTotal Seek Distance (tracks moved): %d\n", total_tracks_moved);
    printf("Total Seek Time: %.2f ms\n", total_seek_time);
    printf("Average Seek Time: %.2f ms\n", avg_seek_time);

    return 0;
}
