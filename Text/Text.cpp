#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <clocale>
#include <ctype.h>

typedef struct Word {
    char* value;
    int count;
    int length;
    struct Word* next;
} Word;


Word* createNode(const char* word) {
    Word* newNode = (Word*)malloc(sizeof(Word));
    if (newNode != NULL) {
        newNode->value = _strdup(word);
        newNode->count = 0;
        newNode->next = NULL;
    }
    return newNode;
}


void pushWord(Word** head, const char* word) {
    Word* newNode = (Word*)malloc(sizeof(Word));
    newNode->value = _strdup(word);
    newNode->length = strlen(word);
    newNode->next = NULL;
    if (*head == NULL) {
        *head = newNode;
    }
    else {
        Word* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

Word* loadWordsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return NULL;
    }

    Word* head = NULL;
    char buffer[256];
    while (fscanf(file, "%s", buffer) == 1) {
        char* token = strtok(buffer, " \t\r\n");

        while (token != NULL) {
            char* word = _strdup(token);
            char* punct = NULL;
            int wordLen = strlen(word);

            if (ispunct(word[wordLen - 1])) {
                punct = _strdup(&word[wordLen - 1]);
                word[wordLen - 1] = '\0';
            }

            pushWord(&head, word);

            if (punct != NULL) {
                pushWord(&head, punct);
                free(punct);
            }

            token = strtok(NULL, " \t\r\n");
        }
    }

    fclose(file);
    return head;
}

void freeList(Word* head) {
    Word* current = head;
    while (current != NULL) {
        Word* temp = current;
        current = current->next;
        free(temp->value);
        free(temp);
    }
}


void displayList(Word* head) {
    Word* current = head;
    while (current != NULL) {
        printf("%s", current->value);
      
        if (current->next != NULL && ispunct(current->next->value[0])) {
           
            if (current->next->next != NULL && ispunct(current->next->next->value[0])) {
                printf(" ");
            }
        }
        else {
            printf(" ");
        }

        current = current->next;
    }
    printf("\n");
}



Word* sortListByLength( Word* head) {
    if (head == NULL || head->next == NULL) {
        return head; 
    }
    Word* sortedList = NULL;  
    const Word* current = head;  

    while (current != NULL) {
        const Word* nextNode = current->next;  


        if (sortedList == NULL || strlen(current->value) >= strlen(sortedList->value)) {
            Word* newNode = createNode(current->value);
            newNode->next = sortedList;
            sortedList = newNode;
        }
        else {
            Word* temp = sortedList;
            while (temp->next != NULL && strlen(current->value) < strlen(temp->next->value)) {
                temp = temp->next;
            }
            Word* newNode = createNode(current->value);
            newNode->next = temp->next;
            temp->next = newNode;
        }

        current = nextNode; 
    }

    return sortedList;
}

Word* findWordInList(Word* head, const char* word) {
    Word* current = head;
    while (current != NULL) {
        if (strcmp(current->value, word) == 0) {
            return current;  // Слово найдено
        }
        current = current->next;
    }

    return NULL;  // Слово не найдено
}

Word* countWordOccurrences(const Word* head) {
    if (head == NULL) {
        return NULL; // Список пустой
    }
    Word* occurrencesList = NULL;  // Список вхождений слов
    const Word* current = head;    // Указатель на текущий элемент

    while (current != NULL) {
        const char* word = current->value;
        Word* node = findWordInList(occurrencesList, word);

        if (node != NULL) {
            node->count++;  // Увеличиваем счетчик вхождений
        }
        else {
            Word* newNode = createNode(word);
            newNode->count = 1;
            newNode->next = occurrencesList;
            occurrencesList = newNode;
        }

        current = current->next;  // Переходим к следующему элементу
    }

    return occurrencesList;
}

void displayWordOccurrences(const Word* head) {
    const Word* current = head;
    while (current != NULL) {
        printf("Word: %s, Count: %d, Length: %lu\n", current->value, current->count, strlen(current->value));
        current = current->next;
    }
}

int main() {


    Word* head = loadWordsFromFile("D:\\text.txt");
    

    displayList(head);

    //Word* head2 = sortListByLength(head);
    
    Word* head3 = countWordOccurrences(head);
    
    //displayList(head2);

    displayWordOccurrences(head3);

    freeList(head3);
    //freeList(head2);
    freeList(head);

    return 0;
}
