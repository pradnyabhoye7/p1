#include <stdio.h>

typedef struct {
    int pid;        // Process ID
    int at;         // Arrival Time
    int bt;         // Burst Time
    int ct;         // Completion Time
    int tat;        // Turnaround Time
    int wt;         // Waiting Time
} Process;

void sortByArrivalTime(Process p[], int n) {
    for (int i = 0; i < n-1; ++i) {
        for (int j = 0; j < n-i-1; ++j) {
            if (p[j].at > p[j+1].at) {
                Process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process p[n];

    // Input
    for (int i = 0; i < n; ++i) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time and Burst Time for Process P%d (AT BT): ", p[i].pid);
        scanf("%d %d", &p[i].at, &p[i].bt);
    }

    // Sort processes by Arrival Time
    sortByArrivalTime(p, n);

    // FCFS Scheduling Logic
    int current_time = 0;
    float total_wt = 0, total_tat = 0;

    for (int i = 0; i < n; ++i) {
        if (current_time < p[i].at)
            current_time = p[i].at;

        p[i].ct = current_time + p[i].bt;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;

        total_wt += p[i].wt;
        total_tat += p[i].tat;
        current_time = p[i].ct;
    }

    // Display Table
    printf("\n%-5s %-10s %-10s %-10s %-15s %-10s\n", "PID", "Arrival", "Burst", "Complete", "Turnaround", "Waiting");
    for (int i = 0; i < n; ++i) {
        printf("P%-4d %-10d %-10d %-10d %-15d %-10d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    // Display Averages
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time   : %.2f\n", total_wt / n);

    // Gantt Chart
    printf("\nGantt Chart:\n");

    // Top border
    printf(" ");
    for (int i = 0; i < n; i++) {
        printf("--------");
    }
    printf("\n|");

    // Process sequence
    for (int i = 0; i < n; i++) {
        printf("  P%d   |", p[i].pid);
    }

    // Bottom border
    printf("\n ");
    for (int i = 0; i < n; i++) {
        printf("--------");
    }

    // Time labels
    printf("\n0");
    for (int i = 0; i < n; i++) {
        printf("      %d", p[i].ct);
    }

    printf("\n");

    return 0;
}
