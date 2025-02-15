#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

#define MAX_FILENAME_LENGTH 100
#define MAX_TABLE_SIZE 4096

void getUserInput(char* inputFilename) {
    printf("Enter the input file path: ");
    fgets(inputFilename, MAX_FILENAME_LENGTH, stdin);
    inputFilename[strcspn(inputFilename, "\n")] = '\0'; 
}

void compressTextFile(const char* inputFilename) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    FILE* inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        printf("Error opening input file.\n");
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    char outputFilename[MAX_FILENAME_LENGTH];
    strcpy(outputFilename, inputFilename);

    char* extension = strrchr(outputFilename, '.');
    if (extension != NULL) {
        *extension = '\0'; // ??? ????? ????
    }

    strcat(outputFilename, ".txt-Compressed");

    FILE* outputFile = fopen(outputFilename, "wb");
    if (outputFile == NULL) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        printf("Error creating output file.\n");
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        fclose(inputFile);
        return;
    }

    unsigned int currentCode = 256;
    unsigned int tableSize = 256;
    unsigned int* codesTable = (unsigned int*)malloc(MAX_TABLE_SIZE * sizeof(unsigned int));
    char* currentPhrase = (char*)malloc(MAX_TABLE_SIZE * sizeof(char));
    char currentChar;
    char nextChar;
    unsigned int i;

    for (i = 0; i < tableSize; i++) {
        codesTable[i] = i;
    }

    fscanf(inputFile, "%c", &currentChar);
    currentPhrase[0] = currentChar;
    i = 1;

    while (fscanf(inputFile, "%c", &nextChar) != EOF) {
        currentPhrase[i] = nextChar;

        unsigned int j;
        for (j = 0; j < tableSize; j++) {
            if (strncmp(currentPhrase, (const char*)&codesTable[j], i + 1) == 0) {
                break;
            }
        }

        if (j < tableSize) {
            i++;
            continue;
        }

        fwrite(&codesTable[currentPhrase[0]], sizeof(unsigned int), 1, outputFile);

        if (currentCode < MAX_TABLE_SIZE) {
            memcpy(&codesTable[tableSize], currentPhrase, i + 1);
            tableSize++;
            currentCode++;
        }

        i = 0;
        currentPhrase[0] = nextChar;
    }

    fwrite(&codesTable[currentPhrase[0]], sizeof(unsigned int), 1, outputFile);

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("File compressed successfully. Output file: %s\n", outputFilename);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    free(currentPhrase);
    free(codesTable);
    fclose(inputFile);
    fclose(outputFile);
}

void decompressTextFile(const char* inputFilename) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    FILE* inputFile = fopen(inputFilename, "rb");
    if (inputFile == NULL) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        printf("Error opening input file.\n");
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return;
    }

    char outputFilename[MAX_FILENAME_LENGTH];
    strcpy(outputFilename, inputFilename);

    char* extension = strrchr(outputFilename, '-');
    if (extension != NULL) {
        *extension = '\0'; // ??? ????? ???? ????? ????
    }

    FILE* outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        printf("Error creating output file.\n");
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        fclose(inputFile);
        return;
    }

    unsigned int currentCode = 256;
    unsigned int tableSize = 256;
    unsigned int* codesTable = (unsigned int*)malloc(MAX_TABLE_SIZE * sizeof(unsigned int));
    char* currentPhrase = (char*)malloc(MAX_TABLE_SIZE * sizeof(char));
    unsigned int currentPhraseLength = 0;
    unsigned int previousCode;
    unsigned int currentCodeIndex = 0;
    unsigned int i;

    for (i = 0; i < tableSize; i++) {
        codesTable[i] = i;
    }

    fread(&previousCode, sizeof(unsigned int), 1, inputFile);
    currentPhrase[currentPhraseLength] = previousCode;
    currentPhraseLength++;
    fprintf(outputFile, "%c", previousCode);

    while (fread(&currentCode, sizeof(unsigned int), 1, inputFile) != 0) {
        if (currentCode < tableSize) {
            currentCodeIndex = currentCode;
        } else {
            currentPhrase[currentPhraseLength] = previousCode;
            currentCodeIndex = previousCode;
        }

        while (currentCodeIndex >= 256) {
            fprintf(outputFile, "%c", codesTable[currentCodeIndex]);
            currentCodeIndex = codesTable[currentCodeIndex];
        }

        fprintf(outputFile, "%c", currentCodeIndex);

        if (previousCode < MAX_TABLE_SIZE) {
            codesTable[tableSize] = previousCode;
            tableSize++;
        }

        previousCode = currentCode;
        currentPhraseLength = 0;
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    printf("File decompressed successfully. Output file: %s\n", outputFilename);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    free(currentPhrase);
    free(codesTable);
    fclose(inputFile);
    fclose(outputFile);
}


void displayHelpMenu() {
    printf("Options:\n");
    printf("1. Compress a text file(This option compresses the file whose address you enter).\n");
    printf("2. Decompress a text file(This option decompresses the file whose address you enter).\n");
    printf("3. Exit.\n");
}

int main() {
    int option;
    char inputFilename[MAX_FILENAME_LENGTH];

    while (1) {
        displayHelpMenu();
        printf("Enter an option: ");
        scanf("%d", &option);
        getchar(); 
        
        switch (option) {
            case 1:
                getUserInput(inputFilename);
                compressTextFile(inputFilename);
                break;
            case 2:
                getUserInput(inputFilename);
                decompressTextFile(inputFilename);
                break;
            case 3:
                return 0;
            default:
                printf("Invalid option.\n");
                break;
        }

        printf("\n");
    }
}