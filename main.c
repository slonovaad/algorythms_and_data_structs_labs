#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int MAX_WORD_LEN = 100;
const int MAX_WORD_COUNT = 200;

int binsearch(int n, int* keys, int x) {
    if (n == 0) {
        return -1;
    }
    int l = 0;
    int r = n - 1;
    int mid = (l + r) / 2;
    while ((r - l) > 1) {
        if (keys[mid] > x) {
            r = mid;
        } else {
            l = mid;
        }
        mid = (l + r) / 2;
    }
    if (keys[l] == x) {
        return l;
    }
    if (keys[r] == x) {
        return r;
    }
    return -1;
}

void shaker_sort(int n, int* keys, char** values) {
    int swapped = 1, start = 0, end = n - 1;
    int t;
    char* t_str;
    while (swapped) {
        swapped = 0;
        for (int i = start; i < end; i++) {
            if (keys[i] > keys[i + 1]) {
                t = keys[i];
                keys[i] = keys[i + 1];
                keys[i + 1] = t;
                t_str = values[i];
                values[i] = values[i + 1];
                values[i + 1] = t_str;
                swapped = 1;
            }
        }
        end--;
        if (!swapped) {
            break;
        }
        swapped = 0;
        for (int i = end; i > start; i--) {
            if (keys[i] < keys[i - 1]) {
                t = keys[i];
                keys[i] = keys[i - 1];
                keys[i - 1] = t;
                t_str = values[i];
                values[i] = values[i - 1];
                values[i - 1] = t_str;
                swapped = 1;
            }
        }
        start++;
    }
}

void print_table(int* keys, char** values, int n) {
    for (int i = 0; i < n; i++) {
        printf("%-5d", keys[i]);
        printf("%s\n", values[i]);
    }
}

int main(void) {
    srand(time(NULL));
    char** words_1 = (char**)malloc(sizeof(char*) * MAX_WORD_COUNT);
    if (!words_1) {
        return 1;
    }
    char** words_2 = (char**)malloc(sizeof(char*) * MAX_WORD_COUNT);
    if (!words_2) {
        free(words_1);
        return 1;
    }char** words_3 = (char**)malloc(sizeof(char*) * MAX_WORD_COUNT);
    if (!words_3) {
        free(words_1);
        free(words_2);
        return 1;
    }
    for (int i = 0; i < MAX_WORD_COUNT; i++) {
        words_1[i] = (char*)malloc(sizeof(char) * MAX_WORD_LEN);
        if (!words_1[i]) {
            for (int j = 0; j < i; j++) {
                free(words_1[j]);
            }
            free(words_1);
            free(words_2);
            free(words_3);
            return 1;
        }
    }
    for (int i = 0; i < MAX_WORD_COUNT; i++) {
        words_2[i] = (char*)malloc(sizeof(char) * MAX_WORD_LEN);
        if (!words_2[i]) {
            for (int j = 0; j < i; j++) {
                free(words_2[j]);
            }
            for (int j = 0; j < MAX_WORD_COUNT; j++) {
                free(words_1[j]);
            }
            free(words_1);
            free(words_2);
            free(words_3);
            return 1;
        }
    }
    for (int i = 0; i < MAX_WORD_COUNT; i++) {
        words_3[i] = (char*)malloc(sizeof(char) * MAX_WORD_LEN);
        if (!words_3[i]) {
            for (int j = 0; j < i; j++) {
                free(words_3[j]);
            }
            for (int j = 0; j < MAX_WORD_COUNT; j++) {
                free(words_1[j]);
                free(words_2[j]);
            }
            free(words_1);
            free(words_2);
            free(words_3);
            return 1;
        }
    }
    int count_words = 0;

    FILE *file = fopen("input.txt", "r");
    if (!file) {
        for (int i = 0; i < MAX_WORD_COUNT; i++) {
            free(words_1[i]);
            free(words_2[i]);
            free(words_3[i]);
        }
        free(words_1);
        free(words_2);
        free(words_3);
        printf("error");
        return 1;
    }


    while (fscanf(file, "%99s", words_1[count_words]) == 1) {
        strcpy(words_2[count_words], words_1[count_words]);
        strcpy(words_3[count_words], words_1[count_words]);
        count_words++;
        if (count_words >= MAX_WORD_COUNT) {
            break;
        }
    }
    fclose(file);
    
    if (count_words != 0) {
        int* keys_1 = (int*)malloc(sizeof(int) * count_words);
        if (!keys_1) {
            for (int i = 0; i < MAX_WORD_COUNT; i++) {
                free(words_1[i]);
                free(words_2[i]);
                free(words_3[i]);
            }
            free(words_1);
            free(words_2);
            free(words_3);
            return 1;
        }
        int* keys_2 = (int*)malloc(sizeof(int) * count_words);
        if (!keys_2) {
            for (int i = 0; i < MAX_WORD_COUNT; i++) {
                free(words_1[i]);
                free(words_2[i]);
                free(words_3[i]);
            }
            free(words_1);
            free(words_2);
            free(words_3);
            free(keys_1);
            return 1;
        }
        int* keys_3 = (int*)malloc(sizeof(int) * count_words);
        if (!keys_3) {
            for (int i = 0; i < MAX_WORD_COUNT; i++) {
                free(words_1[i]);
                free(words_2[i]);
                free(words_3[i]);
            }
            free(words_1);
            free(words_2);
            free(words_3);
            free(keys_1);
            free(keys_2);
            return 1;
        }

        for (int i = 0; i < count_words; i++) {
            keys_1[i] = i;
        }
        for (int i = 0; i < count_words; i++) {
            keys_2[i] = count_words - 1 - i;
        }
        for (int i = 0; i < count_words; i++) {
            keys_3[i] = rand() % count_words;
        }
        printf("---table1---\n");
        printf("before sort:\n");
        print_table(keys_1, words_1, count_words);
        shaker_sort(count_words, keys_1, words_1);
        printf("\nafter sort:\n");
        print_table(keys_1, words_1, count_words);

        int x, ind;
        printf("enter key for search:\n");
        scanf("%d", &x);
        ind = binsearch(count_words, keys_1, x);
        if (ind == -1) {
            printf("not found\n");
        } else {
            printf("index: %d value: %s", ind, words_1[ind]);
        }
        
        printf("\n---table2---\n");
        printf("before sort:\n");
        print_table(keys_2, words_2, count_words);
        shaker_sort(count_words, keys_2, words_2);
        printf("\nafter sort:\n");
        print_table(keys_2, words_2, count_words);

        
        printf("enter key for search:\n");
        scanf("%d", &x);
        ind = binsearch(count_words, keys_2, x);
        if (ind == -1) {
            printf("not found\n");
        } else {
            printf("index: %d value: %s", ind, words_2[ind]);
        }
        
        printf("\n---table3---\n");
        printf("before sort:\n");
        print_table(keys_3, words_3, count_words);
        shaker_sort(count_words, keys_3, words_3);
        printf("\nafter sort:\n");
        print_table(keys_3, words_3, count_words);

        
        printf("enter key for search:\n");
        scanf("%d", &x);
        ind = binsearch(count_words, keys_3, x);
        if (ind == -1) {
            printf("not found\n");
        } else {
            printf("index: %d value: %s", ind, words_3[ind]);
        }

        free(keys_1);
        free(keys_2);
        free(keys_3);
    }
    for (int i = 0; i < MAX_WORD_COUNT; i++) {
        free(words_1[i]);
        free(words_2[i]);
        free(words_3[i]);
    }
    free(words_1);
    free(words_2);
    free(words_3);

    return 0;
}
