#include <stdio.h>

typedef struct {
    int pid;
    int at;
    int bt;
    int rt;
    int ct;
    int tat;
    int wt;
    int done;
} Process;

int main() {
    int n, tq;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];

    printf("Enter Arrival Time and Burst Time for each process:\n");
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("P%d - AT and BT: ", p[i].pid);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].rt = p[i].bt;
        p[i].done = 0;
    }

    printf("Enter Time Quantum: ");
    scanf("%d", &tq);

    int time = 0, completed = 0;
    int gantt_time[1000], gantt_pid[1000], gc = 0;

    int queue[100], front = 0, rear = 0;
    int visited[n];
    for (int i = 0; i < n; i++) visited[i] = 0;

    // Initial enqueue of processes arriving at time 0
    for (int i = 0; i < n; i++) {
        if (p[i].at == 0) {
            queue[rear++] = i;
            visited[i] = 1;
        }
    }

    while (completed < n) {
        if (front == rear) {
            // CPU is idle
            gantt_pid[gc] = -1;
            gantt_time[gc++] = time;
            time++;
            // Check for any new arrivals
            for (int i = 0; i < n; i++) {
                if (p[i].at == time && !visited[i]) {
                    queue[rear++] = i;
                    visited[i] = 1;
                }
            }
            continue;
        }

        int idx = queue[front++];
        gantt_pid[gc] = p[idx].pid;
        gantt_time[gc++] = time;

        int exec_time = (p[idx].rt <= tq) ? p[idx].rt : tq;
        time += exec_time;
        p[idx].rt -= exec_time;

        // Enqueue processes arriving during this time slice
        for (int i = 0; i < n; i++) {
            if (p[i].at > gantt_time[gc - 1] && p[i].at <= time && !visited[i]) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }

        if (p[idx].rt > 0) {
            queue[rear++] = idx; // Requeue
        } else if (!p[idx].done) {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            p[idx].done = 1;
            completed++;
        }
    }

    gantt_time[gc] = time;

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
    float total_tat = 0, total_wt = 0;
    printf("\n\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        total_tat += p[i].tat;
        total_wt += p[i].wt;
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }

    printf("\nAverage Turnaround Time: %.2f", total_tat / n);
    printf("\nAverage Waiting Time   : %.2f\n", total_wt / n);

    return 0;
}
