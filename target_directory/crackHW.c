#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

const int PASS_LEN = 20;
const int HASH_LEN = 33;

char *tryWord(char *plaintext, char *hashFilename) {
    char *hash = md5(plaintext, strlen(plaintext));
    FILE *file = fopen(hashFilename, "r");
    if (!file) {
        perror("Failed to open hash file");
        free(hash);
        return NULL;
    }

    char line[HASH_LEN];
    while (fgets(line, HASH_LEN, file)) {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, hash) == 0) {
            fclose(file);
            return hash;
        }
    }

    fclose(file);
    free(hash);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    FILE *dictFile = fopen(argv[2], "r");
    if (!dictFile) {
        perror("Failed to open dictionary file");
        exit(1);
    }

    char word[PASS_LEN];
    int crackedCount = 0;

    while (fgets(word, PASS_LEN, dictFile)) {
        word[strcspn(word, "\n")] = 0;
        char *foundHash = tryWord(word, argv[1]);
        if (foundHash) {
            printf("%s %s\n", foundHash, word);
            crackedCount++;
            free(foundHash);
        }
    }

    fclose(dictFile);
    printf("%d hashes cracked!\n", crackedCount);
    return 0;
}
