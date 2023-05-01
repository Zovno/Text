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

typedef struct Pair {
    char* A;
    char* B;
} Pair;


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
            return current;  
        }
        current = current->next;
    }

    return NULL;  
}

Word* countWordOccurrences(const Word* head) {
    if (head == NULL) {
        return NULL; 
    }
    Word* occurrencesList = NULL; 
    const Word* current = head;  

    while (current != NULL) {
        const char* word = current->value;
        Word* node = findWordInList(occurrencesList, word);

        if (node != NULL) {
            node->count++; 
        }
        else {
            Word* newNode = createNode(word);
            newNode->count = 1;
            newNode->next = occurrencesList;
            occurrencesList = newNode;
        }

        current = current->next; 
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


int isWordUsed(const Word* word, const Pair* pairs, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word->value, pairs[i].A) == 0 || strcmp(word->value, pairs[i].B) == 0) {
            return 1;
        }
    }
    return 0;
}


Pair* findWordPair(const Word* head, int* count) {
    const Word* wordA = head;
    const Word* wordB = head;
    int maxScore = 0;
    Pair* pairs = NULL;
    int i = 0;

    while (wordA != NULL) {
        wordB = wordA->next;
        while (wordB != NULL) {
            int score = (wordA->count * strlen(wordA->value) + wordB->count * strlen(wordB->value)) -
                (wordA->count * strlen(wordB->value) + wordB->count * strlen(wordA->value));

            if (score > maxScore) {

                if (!isWordUsed(wordA, pairs, i) && !isWordUsed(wordB, pairs, i)) {
                    Pair* new_arr = (Pair*)realloc(pairs, (sizeof(Pair) * (*count + 1)));
                    if (new_arr != NULL) {
                        pairs = new_arr;
                        (*count)++;
                    }
                    else {

                        continue;
                    }

                    pairs[i].A = _strdup(wordA->value);
                    pairs[i].B = _strdup(wordB->value);

                    maxScore = score;
                    i++;
                }
            }

            wordB = wordB->next;
        }

        wordA = wordA->next;
    }

    if (maxScore == 0) {
        printf("No word pair found.\n");
    }

    return pairs;
}



void printWordPairs(const Pair* pairs, int count) {
    if (count == 0) {
        printf("No word pairs found.\n");
        return;
    }

    printf("Word pairs:\n");
    for (int i = 0; i < count; i++) {
        printf("Pair %d: %s - %s\n", i + 1, pairs[i].A, pairs[i].B);
    }
}


void swapWordsInPair(Word* head, const Pair* pair) {
    if (head == NULL || pair == NULL) {
        return;
    }

    Word* current = head;
    while (current != NULL) {
        if (strcmp(current->value, pair->A) == 0) {
            current->value = _strdup(pair->B);
        }
        else if (strcmp(current->value, pair->B) == 0) {
            current->value = _strdup(pair->A);
        }
        current = current->next;
    }
}

void writeListToFile(const Word* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    const Word* current = head;
    while (current != NULL) {
        fprintf(file, "%s", current->value);

        if (current->next != NULL && ispunct(current->next->value[0])) {
            if (current->next->next != NULL && ispunct(current->next->next->value[0])) {
                fprintf(file, " ");
            }
        }
        else {
            fprintf(file, " ");
        }

        current = current->next;
    }

    fclose(file);
}

int main() {


    Word* head = loadWordsFromFile("D:\\text.txt");
    

    //displayList(head);


    
    Word* head3 = countWordOccurrences(head);
    
    //displayWordOccurrences(head3);
    int* count = new int;
    *(count) = 0;

    Pair* pairs = findWordPair(head3, count);

    printWordPairs(pairs, *count);
    swapWordsInPair(head, pairs);
    writeListToFile(head, "D:\\txt1.txt");
    swapWordsInPair(head, pairs);
    writeListToFile(head, "D:\\txt2.txt");
    free(pairs);
    free(count);
    freeList(head3);

    freeList(head);
    
    return 0;
}
