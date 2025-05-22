#include <stdio.h>

typedef struct {
    int pid;    // Process ID
    int at;     // Arrival Time
    int bt;     // Burst Time
    int prio;   // Priority (lower value = higher priority)
    int ct;     // Completion Time
    int tat;    // Turnaround Time
    int wt;     // Waiting Time
    int done;   // Process completion flag
} Process;

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process p[n];

    // Input Section
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time, Burst Time and Priority for P%d: ", p[i].pid);
        scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].prio);
        p[i].done = 0;
    }

    int time = 0, completed = 0;
    int gantt_pid[100], gantt_time[100], gc = 0;

    // Scheduling Logic
    while (completed < n) {
        int idx = -1;
        int min_prio = 1e9;

        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].at <= time) {
                if (p[i].prio < min_prio || (p[i].prio == min_prio && p[i].at < p[idx].at)) {
                    min_prio = p[i].prio;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            // CPU is idle
            gantt_pid[gc] = -1;
            gantt_time[gc++] = time;
            time++;
            continue;
        }

        gantt_pid[gc] = p[idx].pid;
        gantt_time[gc++] = time;

        time += p[idx].bt;
        p[idx].ct = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        p[idx].done = 1;
        completed++;
    }
    gantt_time[gc] = time; // Final timestamp

    // Gantt Chart
    printf("\nGantt Chart:\n ");
    for (int i = 0; i < gc; i++) {
        if (gantt_pid[i] == -1)
            printf(" IDLE ");
        else
            printf(" P%d  ", gantt_pid[i]);
    }
    printf("\n");
    for (int i = 0; i <= gc; i++) {
        printf("%-5d", gantt_time[i]);
    }

    // Process Table
    printf("\n\nPID\tAT\tBT\tPR\tCT\tTAT\tWT\n");
    float total_tat = 0, total_wt = 0;
    for (int i = 0; i < n; i++) {
        total_tat += p[i].tat;
        total_wt += p[i].wt;
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].prio, p[i].ct, p[i].tat, p[i].wt);
    }

    // Averages
    printf("\nAverage Turnaround Time: %.2f", total_tat / n);
    printf("\nAverage Waiting Time   : %.2f\n", total_wt / n);

    return 0;
}
