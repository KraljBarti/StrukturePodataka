#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50

struct _directory;
typedef struct _directory* PositionDirectory;
typedef struct _directory {
    char name[MAX_NAME_LENGTH];
    PositionDirectory subDirectories;
    PositionDirectory next;
} Directory;

struct _levelStack;
typedef struct _levelStack* PositionLevelStack;
typedef struct _levelStack {
    PositionDirectory directoryLevel;
    PositionLevelStack next;
} LevelStack;

// Function prototypes
PositionDirectory createDirectory(char name[MAX_NAME_LENGTH]);
PositionDirectory createSubdirectory(char name[MAX_NAME_LENGTH], PositionDirectory currentDirectory);
PositionDirectory changeDirectory(char name[MAX_NAME_LENGTH], PositionDirectory currentDirectory);
int listDirectoryContents(PositionDirectory currentDirectory);

PositionDirectory pop(PositionLevelStack headLevelStack);
void push(PositionLevelStack headLevelStack, PositionDirectory directoryLevel);
PositionLevelStack createNewLevelStackElement(PositionDirectory directoryLevel);

int main() {
    Directory headDirectory = {
        .name = {0},
        .subDirectories = NULL,
        .next = NULL
    };
    PositionDirectory rootDirectory = createDirectory("C:");
    headDirectory.next = rootDirectory;

    PositionDirectory currentDirectory = rootDirectory;

    LevelStack headLevelStack = {
        .directoryLevel = NULL,
        .next = NULL
    };
    push(&headLevelStack, currentDirectory);

    while (1) {
        printf("\nMenu:\n");
        printf("1 - md (Create Directory)\n");
        printf("2 - cd dir (Change Directory)\n");
        printf("3 - cd.. (Go Up)\n");
        printf("4 - dir (List Contents)\n");
        printf("5 - exit\n");

        char choice[10];
        printf("Enter your choice: ");
        scanf("%s", choice);

        if (strcmp(choice, "1") == 0) {
            char directoryName[MAX_NAME_LENGTH];
            printf("\033[0;32mEnter directory name: \033[0m");
            scanf("%s", directoryName);
            createSubdirectory(directoryName, currentDirectory);
        }
        else if (strcmp(choice, "2") == 0) {
            char directoryName[MAX_NAME_LENGTH];
            printf("\033[0;32mEnter directory name: \033[0m");
            scanf("%s", directoryName);
            PositionDirectory newDirectory = changeDirectory(directoryName, currentDirectory);
            if (newDirectory != currentDirectory) {
                push(&headLevelStack, currentDirectory);
                currentDirectory = newDirectory;
            }
        }
        else if (strcmp(choice, "3") == 0) {
            if (currentDirectory != rootDirectory) {
                currentDirectory = pop(&headLevelStack);
                printf("\033[0;32mCurrently in '%s' \033[0m\n", currentDirectory->name);
            }
            else {
                printf("Already in the root directory.\n");
            }
        }
        else if (strcmp(choice, "4") == 0) {
            listDirectoryContents(currentDirectory);
        }
        else if (strcmp(choice, "5") == 0) {
            printf("Exiting the program.\n");
            break;
        }
        else {
            printf("\033[0;31mInvalid choice. Please enter a valid option.\033[0m\n");
        }
    }

    // Free allocated memory
    free(rootDirectory);

    return 0;
}

/* Directory functions */

PositionDirectory createDirectory(char name[MAX_NAME_LENGTH]) {
    PositionDirectory newDirectory = (PositionDirectory)malloc(sizeof(Directory));
    if (!newDirectory) {
        printf("Can't allocate memory!\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newDirectory->name, name);
    newDirectory->subDirectories = NULL;
    newDirectory->next = NULL;
    return newDirectory;
}

PositionDirectory createSubdirectory(char name[MAX_NAME_LENGTH], PositionDirectory currentDirectory) {
    PositionDirectory newDirectory = createDirectory(name);
    newDirectory->next = currentDirectory->subDirectories;
    currentDirectory->subDirectories = newDirectory;
    return newDirectory;
}

PositionDirectory changeDirectory(char name[MAX_NAME_LENGTH], PositionDirectory currentDirectory) {
    PositionDirectory subdirectory = currentDirectory->subDirectories;
    while (subdirectory != NULL) {
        if (strcmp(subdirectory->name, name) == 0) {
            return subdirectory;
        }
        subdirectory = subdirectory->next;
    }
    printf("Directory '%s' not found.\n", name);
    return currentDirectory;
}

int listDirectoryContents(PositionDirectory currentDirectory) {
    printf("\033[0;32mContents of directory '%s':\033[0m\n", currentDirectory->name);
    PositionDirectory subdirectory = currentDirectory->subDirectories;
    while (subdirectory != NULL) {
        printf("\033[0;32m - %s\033[0m\n", subdirectory->name);
        subdirectory = subdirectory->next;
    }
    if (currentDirectory->subDirectories == NULL) {
        printf("\033[0;32m   (empty)\033[0m\n");
    }
    return EXIT_SUCCESS;
}

/* Stack functions */

PositionDirectory pop(PositionLevelStack headLevelStack) {
    PositionLevelStack toDelete = headLevelStack->next;
    if (!toDelete) {
        printf("Stack is empty! Nothing to pop!\n");
        return headLevelStack->directoryLevel;
    }

    headLevelStack->next = toDelete->next;
    PositionDirectory directoryLevel = toDelete->directoryLevel;
    free(toDelete);

    return directoryLevel;
}

void push(PositionLevelStack headLevelStack, PositionDirectory directoryLevel) {
    PositionLevelStack newLevelStackElement = createNewLevelStackElement(directoryLevel);
    newLevelStackElement->next = headLevelStack->next;
    headLevelStack->next = newLevelStackElement;
}

PositionLevelStack createNewLevelStackElement(PositionDirectory directoryLevel) {
    PositionLevelStack newLevelStackElement = (PositionLevelStack)malloc(sizeof(LevelStack));
    if (!newLevelStackElement) {
        perror("Can't allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    newLevelStackElement->directoryLevel = directoryLevel;
    newLevelStackElement->next = NULL;

    return newLevelStackElement;
}
