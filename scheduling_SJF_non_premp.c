#include <stdio.h>
#include <limits.h>

typedef struct {
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int completed;
} Process;

int findNextProcess(Process p[], int n, int current_time) {
    int idx = -1;
    int min_bt = INT_MAX;

    for (int i = 0; i < n; ++i) {
        if (!p[i].completed && p[i].at <= current_time) {
            if (p[i].bt < min_bt || (p[i].bt == min_bt && p[i].at < p[idx].at)) {
                min_bt = p[i].bt;
                idx = i;
            }
        }
    }

    return idx;
}

int main() {
    int n;
    float total_tat = 0, total_wt = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process p[n];

    // Take user input
    for (int i = 0; i < n; ++i) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time and Burst Time for Process P%d: ", p[i].pid);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].completed = 0;
    }

    int completed = 0, current_time = 0;

    int gantt_pid[100], gantt_time[100], gc = 0;

    while (completed < n) {
        int idx = findNextProcess(p, n, current_time);

        if (idx == -1) {
            // CPU Idle
            current_time++;
        } else {
            gantt_pid[gc] = p[idx].pid;
            gantt_time[gc++] = current_time;

            current_time += p[idx].bt;
            p[idx].ct = current_time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            p[idx].completed = 1;
            completed++;

            total_tat += p[idx].tat;
            total_wt += p[idx].wt;
        }
    }

    gantt_time[gc] = current_time;

    // Print Gantt Chart
    printf("\n\nGantt Chart:\n ");
    for (int i = 0; i < gc; ++i) {
        printf(" P%d  ", gantt_pid[i]);
    }
    printf("\n");
    for (int i = 0; i <= gc; ++i) {
        printf("%-5d", gantt_time[i]);
    }

    // Output process table
    printf("\n\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; ++i) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    // Print averages
    printf("\nAverage Turnaround Time = %.2f", total_tat / n);
    printf("\nAverage Waiting Time    = %.2f\n", total_wt / n);

    return 0;
}
