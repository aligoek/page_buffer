#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THRESHOLD 5
#define BUFFER_SIZE 4
#define MAX_PAGE_LEN 100

typedef struct Node
{
    int counter;
    char page[MAX_PAGE_LEN];
    struct Node *prev;
    struct Node *next;
} Node;

int clearBuffer(Node **head)
{
    Node *current = *head;
    Node *next_node;
    while (current != NULL)
    {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    *head = NULL;
    printf("Buffer cleared.\n");
    return 1;
}

void printBuffer(Node *head)
{
    Node *current = head;
    if (current == NULL) {
        printf("Buffer is empty.\n");
        return;
    }
    while (current != NULL)
    {
        printf("%s,%d", current->page, current->counter);
        if (current->next != NULL)
        {
            printf(" <-> ");
        }
        current = current->next;
    }
    printf("\n");
}

int checkBufferSize(Node *head)
{
    int size = 0;
    Node *current = head;
    while (current != NULL)
    {
        size++;
        current = current->next;
    }
    return size;
}

int isPageInBuffer(Node *head, const char *page)
{
    Node *current = head;
    while (current != NULL)
    {
        if (strcmp(current->page, page) == 0)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int isCountAboveThreshold(int count, int threshold)
{
    return count > threshold;
}

int removeLastNode(Node **head)
{
    if (*head == NULL) return 0;

    if ((*head)->next == NULL)
    {
        free(*head);
        *head = NULL;
        return 1;
    }

    Node *current = *head;
    while (current->next->next != NULL)
    {
        current = current->next;
    }
    free(current->next);
    current->next = NULL;
    return 1;
}

int addPageToBuffer(Node **head, const char *page)
{
    Node *current = *head;
    while (current != NULL) {
        if (strcmp(current->page, page) == 0) {
            current->counter++;

            if (isCountAboveThreshold(current->counter, THRESHOLD)) {
                printf("Threshold passed. Moving to front.\n");
                if (current->prev != NULL) {
                    current->prev->next = current->next;
                }
                if (current->next != NULL) {
                    current->next->prev = current->prev;
                }
                current->prev = NULL;
                current->next = *head;
                if (*head != NULL) {
                   (*head)->prev = current;
                }
                *head = current;
            }
            printBuffer(*head);
            return 1;
        }
        current = current->next;
    }

    if (checkBufferSize(*head) == BUFFER_SIZE)
    {
        printf("Buffer full. Removing last node.\n");
        removeLastNode(head);
    }

    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) {
        perror("Node allocation failed");
        return 0;
    }
    strncpy(newNode->page, page, MAX_PAGE_LEN - 1);
    newNode->page[MAX_PAGE_LEN - 1] = '\0';
    newNode->counter = 1;

    newNode->prev = NULL;
    newNode->next = *head;
    if (*head != NULL)
    {
        (*head)->prev = newNode;
    }
    *head = newNode;

    printBuffer(*head);
    return 1;
}

void printMenu()
{
    printf("\n1. Add page to buffer\n");
    printf("2. Clear buffer\n");
    printf("3. Print buffer\n");
    printf("4. Exit\n");
}

int getUserChoice()
{
    int input;
    printf("Enter your choice: ");
    scanf("%d", &input);
    while (getchar() != '\n');
    return input;
}

int main()
{
    Node *buffer = NULL;
    int input_mode;
    char page_buffer[MAX_PAGE_LEN];

    printf("Welcome to the buffer management program.\n");
    printf("Enter 1 for file input, 2 for user input: ");
    scanf("%d", &input_mode);
    while (getchar() != '\n');
    printf("\n");

    if (input_mode == 1)
    {
        FILE *file = fopen("PAGES.txt", "r");
        if (file == NULL)
        {
            perror("Error opening file");
            return 1;
        }

        while (fgets(page_buffer, MAX_PAGE_LEN, file) != NULL)
        {
            page_buffer[strcspn(page_buffer, "\n")] = 0;
            printf("Adding %s to buffer.\n", page_buffer);
            addPageToBuffer(&buffer, page_buffer);
            printf("\n");
        }
        fclose(file);
    }
    else if (input_mode == 2)
    {
        int choice = 0;
        while (choice != 4)
        {
            printMenu();
            choice = getUserChoice();
            switch (choice)
            {
            case 1:
                printf("Enter page address: ");
                fgets(page_buffer, MAX_PAGE_LEN, stdin);
                page_buffer[strcspn(page_buffer, "\n")] = 0;
                addPageToBuffer(&buffer, page_buffer);
                printf("\n");
                break;
            case 2:
                clearBuffer(&buffer);
                break;
            case 3:
                printBuffer(buffer);
                break;
            case 4:
                printf("Program ended.\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
                break;
            }
        }
    }
    else
    {
        printf("Invalid input mode. Program ended.\n");
    }

    clearBuffer(&buffer);

    return 0;
}
