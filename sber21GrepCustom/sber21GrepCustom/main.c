//
//  main.c
//  sber21GrepCustom
//
//  Created by Арсентий Халимовский on 07.01.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define SIZE 256
#define BUFFER_SIZE 4096

typedef struct {
    char *patterns[SIZE]; // array of patterns
    char *patternFiles[SIZE]; // array of pattern files
    char *files[SIZE]; // array of files
    char pattern[BUFFER_SIZE]; // single pattern
    int filesCount;
    int matchesCount;
    // flags
    bool e;
    bool i;
    bool v;
    bool c;
    bool l;
    bool n;
    bool h;
    bool s;
    bool f;
    bool o;
} options;

//void parse_e_flag(options *s_opt, char *argv[], int *i, int j,
//                  int *patterns_count) {

//void parse_f_flag(options *s_opt, char *argv[], int *i, int j,
//                  int *pattern_files_count) {

void parseOptions(int argc, char **argv, options *currentOptionsSet);
void printDataInfo(char **data);
void printDebugInfo(options currentOptionsSet);
void parseFlagE(options *currentOptionsSet, char **argv, int *i, int j, int *patternsCount);
void parseFlagF(options *currentOptionsSet, char **argv, int *i, int j, int *patternFilesCount);


int main(int argc, char **argv) {
    options currentOptionsSet = {0};
    parseOptions(argc, argv, &currentOptionsSet);
    printDebugInfo(currentOptionsSet);
    return 0;
}

void parseOptions(int argc, char **argv, options *currentOptionsSet) { // sOpt - current set of options
    int filesCount = 0;
    int patternsCount = 0;
    int patternFilesCount = 0;
    bool isValid = true;
    
    for (int i = 1; i < argc; i++) {
        
        if (isValid) {
            
            if (*(*argv + i) == '-') { // check argument for being an option
                
                if (*(*(argv + i) + 1) == 'e') { // -e and -f are first priority to work with
                    // parse_e_flag(s_opt, argv, &i, 2, &patterns_count);
                    parseFlagE(currentOptionsSet, argv, &i, 2, &patternsCount);
                } else if (*(*(argv + i) + 1) == 'f') {
                    parseFlagF(currentOptionsSet, argv, &i, 2, &patternFilesCount);
                } else {
                    
                    // parse other flags
                    // MARK: Check place for j
                    int j = 0;
                    
                    switch (*(*(argv + i)) + j) {
                        case 'e':
                            parseFlagE(currentOptionsSet, argv, &i, j + 1, &patternsCount);
                            break;
                        case 'f':
                            parseFlagF(currentOptionsSet, argv, &i, j + 1, &patternsCount);
                            currentOptionsSet->f = true;
                            // case f
                            break;
                        case 'o':
                            currentOptionsSet->o = true;
                            // case o
                            break;
                        case 'h':
                            currentOptionsSet->h = true;
                            // case h
                            break;
                        case 's':
                            currentOptionsSet->s = true;
                            // case s
                            break;
                        case 'v':
                            currentOptionsSet->v = true;
                            // case v
                            break;
                        case 'l':
                            currentOptionsSet->i = true;
                            // case i
                            break;
                        case 'i':
                            currentOptionsSet->i = true;
                            // case i
                            break;
                        case 'c':
                            currentOptionsSet->c = true;
                            // case c
                            break;
                        case 'n':
                            currentOptionsSet->n = true;
                            // case n
                            break;
                        default:
                            isValid = false;
                            break;
                    }
                }
                
                if (currentOptionsSet->o && // Check if there is incompatible flags
                   (currentOptionsSet->l || currentOptionsSet->v || currentOptionsSet->c)) {
                    currentOptionsSet->o = false;
                }
                
                if (isValid) {
                    printf("patterns: %d\n", patternsCount);
                    printf("patterns files: %d\n", patternFilesCount);
                }
                currentOptionsSet->filesCount = filesCount;
                
            } else if (!currentOptionsSet->e && !currentOptionsSet->f) { // if there is still no a template
                currentOptionsSet->e = true; // read as template
                currentOptionsSet->patterns[patternsCount++] = *(argv + i);
                
            } else { // if there already template has been found let's read as the file
                currentOptionsSet->files[filesCount++] = *(argv + i);
            }
            
        } else {
            puts("Uncorrect options");
            filesCount = 0;
            break;
        }
    }
}

void printDataInfo(char **data) {
    int i = 0;
    while (*(data + i)) {
        putc('\t', stdout);
        puts(*(data + i));
        i++;
    }
}

void printDebugInfo(options currentOptionsSet) {
    if (currentOptionsSet.filesCount) {
        printf("files count: %d\n", currentOptionsSet.filesCount);
        puts("patterns:"); // MARK: Why putc instead of printf?
        printDataInfo(currentOptionsSet.patterns);
        puts("patterns files:");
        printDataInfo(currentOptionsSet.patterns);
        puts("files:");
        printDataInfo(currentOptionsSet.files);
        puts("final pattern:");
        puts(currentOptionsSet.pattern);
    }
}

void parseFlagE(options *currentOptionsSet, char **argv, int *i, int j, int *patternsCount) {
    currentOptionsSet->e = true;
    if ((*(*(argv + *i)) + j) != '\0') {
        char buffer[BUFFER_SIZE] = "";
        strcat(buffer, *(argv + *i) + j);
        currentOptionsSet->patterns[(*patternsCount)++] = buffer;
    } else {
        currentOptionsSet->patterns[(*patternsCount)++] = *(argv + ++(*i)); // MARK: ++ mean "add one to i before use". Check for correct interpretation by pointers
    }
}

void parseFlagF(options *currentOptionsSet, char **argv, int *i, int j, int *patternFilesCount) {
    currentOptionsSet->f = true;
    if ((*(*(argv + *i)) + j) != '\0') {
        char buffer[BUFFER_SIZE] = "";
        strcat(buffer, *(argv + *i) + j);
        currentOptionsSet->patternFiles[(*patternFilesCount)++] = buffer;
    } else {
        currentOptionsSet->patternFiles[(*patternFilesCount)++] = *(argv + ++(*i));
    }
}
