#include <stdio.h>
#include <limits.h>

typedef struct {
    int pid;
    int at;
    int bt;
    int rt;
    int ct;
    int tat;
    int wt;
} Process;

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time and Burst Time for P%d: ", p[i].pid);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].rt = p[i].bt;
    }

    int time = 0, complete = 0, prev = -1;
    int gantt_pid[1000], gantt_time[1000], gc_index = 0;

    while (complete < n) {
        int idx = -1, min_rt = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0 && p[i].rt < min_rt) {
                min_rt = p[i].rt;
                idx = i;
            }
        }

        if (idx == -1) {
            // CPU is idle
            if (prev != -2) {
                gantt_pid[gc_index] = -1;
                gantt_time[gc_index++] = time;
                prev = -2;
            }
            time++;
            continue;
        }

        if (prev != idx) {
            gantt_pid[gc_index] = p[idx].pid;
            gantt_time[gc_index++] = time;
            prev = idx;
        }

        p[idx].rt--;
        time++;

        if (p[idx].rt == 0) {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            complete++;
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

    // Print Table
    printf("\n\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    float total_tat = 0, total_wt = 0;

    for (int i = 0; i < n; i++) {
        total_tat += p[i].tat;
        total_wt += p[i].wt;
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    printf("\nAverage Turnaround Time: %.2f", total_tat / n);
    printf("\nAverage Waiting Time   : %.2f\n", total_wt / n);

    return 0;
}
