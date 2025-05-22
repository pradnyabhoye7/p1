#include <stdio.h>
#include <limits.h>

typedef struct {
    int pid;    // Process ID
    int at;     // Arrival Time
    int bt;     // Burst Time
    int rt;     // Remaining Time
    int prio;   // Priority (lower value = higher priority)
    int ct;     // Completion Time
    int tat;    // Turnaround Time
    int wt;     // Waiting Time
} Process;

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];

    // Input: AT, BT, Priority
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time, Burst Time and Priority for P%d: ", p[i].pid);
        scanf("%d %d %d", &p[i].at, &p[i].bt, &p[i].prio);
        p[i].rt = p[i].bt;
    }

    int time = 0, complete = 0, prev = -1;
    int gantt_pid[1000], gantt_time[1000], gc_index = 0;

    // Scheduling simulation (per unit time)
    while (complete < n) {
        int idx = -1;
        int min_prio = INT_MAX;

        // Find highest priority process that has arrived
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0 && p[i].prio < min_prio) {
                min_prio = p[i].prio;
                idx = i;
            }
        }

        if (idx == -1) {
            // No process ready; CPU idle
            if (prev != -2) {
                gantt_pid[gc_index] = -1;
                gantt_time[gc_index++] = time;
                prev = -2;
            }
            time++;
            continue;
        }

        // If process has changed, log transition
        if (prev != idx) {
            gantt_pid[gc_index] = p[idx].pid;
            gantt_time[gc_index++] = time;
            prev = idx;
        }

        // Execute 1 unit of process
        p[idx].rt--;
        time++;

        // If process is done
        if (p[idx].rt == 0) {
            complete++;
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
        }
    }

    gantt_time[gc_index] = time;

    // Print Gantt Chart
    printf("\n\nGantt Chart:\n ");
    for (int i = 0; i < gc_index; i++) {
        if (gantt_pid[i] == -1)
            printf(" IDLE ");
        else
            printf(" P%d  ", gantt_pid[i]);
    }
    printf("\n");
    for (int i = 0; i <= gc_index; i++) {
        printf("%-5d", gantt_time[i]);
    }

    // Print Process Table
    printf("\n\nPID\tAT\tBT\tPR\tCT\tTAT\tWT\n");
    float total_tat = 0, total_wt = 0;
    for (int i = 0; i < n; i++) {
        total_tat += p[i].tat;
        total_wt += p[i].wt;
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].prio, p[i].ct, p[i].tat, p[i].wt);
    }

    printf("\nAverage Turnaround Time: %.2f", total_tat / n);
    printf("\nAverage Waiting Time   : %.2f\n", total_wt / n);

    return 0;
}
