#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_BLOCK_SIZE 8
#define MAX_PROCESSES 100

typedef struct Buddy {
    int size;
    int is_free;
    int start_address;
    char process_name[10];
    struct Buddy *left;
    struct Buddy *right;
} Buddy;

Buddy* create_block(int size, int start_address) {
    Buddy *node = (Buddy *)malloc(sizeof(Buddy));
    node->size = size;
    node->is_free = 1;
    node->start_address = start_address;
    node->left = NULL;
    node->right = NULL;
    strcpy(node->process_name, "");
    return node;
}

int next_power_of_two(int n) {
    int power = MIN_BLOCK_SIZE;
    while (power < n)
        power *= 2;
    return power;
}

void split(Buddy *node, int size) {
    if (node->size <= MIN_BLOCK_SIZE || node->size / 2 < size)
        return;
    int half = node->size / 2;
    node->left = create_block(half, node->start_address);
    node->right = create_block(half, node->start_address + half);
}

int allocate(Buddy *node, int size, char *pname) {
    if (!node->is_free || node->size < size)
        return 0;
    if (node->size == size && node->left == NULL && node->right == NULL) {
        node->is_free = 0;
        strcpy(node->process_name, pname);
        printf("Allocated %s (%d bytes) at address %d\n", pname, size, node->start_address);
        return 1;
    }
    if (node->left == NULL && node->right == NULL)
        split(node, size);
    if (allocate(node->left, size, pname)) return 1;
    if (allocate(node->right, size, pname)) return 1;
    return 0;
}

int deallocate(Buddy *node, char *pname) {
    if (node == NULL)
        return 0;
    if (strcmp(node->process_name, pname) == 0 && node->left == NULL && node->right == NULL && !node->is_free) {
        node->is_free = 1;
        printf("Deallocated %s at address %d\n", pname, node->start_address);
        strcpy(node->process_name, "");
        return 1;
    }
    if (deallocate(node->left, pname) || deallocate(node->right, pname)) {
        if (node->left && node->right &&
            node->left->is_free && node->right->is_free &&
            strlen(node->left->process_name) == 0 &&
            strlen(node->right->process_name) == 0) {
            free(node->left);
            free(node->right);
            node->left = NULL;
            node->right = NULL;
            node->is_free = 1;
        }
        return 1;
    }
    return 0;
}

void print_tree(Buddy *node, int level) {
    if (node == NULL)
        return;
    for (int i = 0; i < level; i++)
        printf("  ");
    if (node->left == NULL && node->right == NULL) {
        printf("|-- [%d bytes] at %d => %s (%s)\n", node->size, node->start_address,
               node->is_free ? "Free" : "Used", node->is_free ? "" : node->process_name);
    } else {
        printf("|-- [%d bytes] at %d => Split\n", node->size, node->start_address);
    }
    print_tree(node->left, level + 1);
    print_tree(node->right, level + 1);
}

int main() {
    int total_memory;
    printf("Enter total memory size (multiple of 8): ");
    scanf("%d", &total_memory);
    if (total_memory < MIN_BLOCK_SIZE || (total_memory & (total_memory - 1)) != 0) {
        printf("Memory size must be a power of 2 and >= %d\n", MIN_BLOCK_SIZE);
        return 1;
    }

    Buddy *root = create_block(total_memory, 0);
    int choice;
    char pname[10];
    int size;

    while (1) {
        printf("\n--- Buddy System Menu ---\n");
        printf("1. Allocate Memory to Process\n");
        printf("2. Deallocate Memory of Process\n");
        printf("3. Show Memory Tree\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Enter process name (e.g., P1): ");
                scanf("%s", pname);
                printf("Enter size to allocate: ");
                scanf("%d", &size);
                if (size < MIN_BLOCK_SIZE || size > total_memory) {
                    printf("Invalid size. Must be between %d and %d bytes.\n", MIN_BLOCK_SIZE, total_memory);
                    break;
                }
                size = next_power_of_two(size);
                if (!allocate(root, size, pname))
                    printf("Allocation failed: Not enough memory or too fragmented.\n");
                break;
            case 2:
                printf("Enter process name to deallocate: ");
                scanf("%s", pname);
                if (!deallocate(root, pname))
                    printf("Deallocation failed: Process not found or already free.\n");
                break;
            case 3:
                printf("\nCurrent Memory Tree:\n");
                print_tree(root, 0);
                break;
            case 4:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}
