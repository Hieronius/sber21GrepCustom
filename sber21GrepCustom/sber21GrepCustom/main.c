//
//  main.c
//  sber21GrepCustom
//
//  Created by Арсентий Халимовский on 07.01.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

void parseOptions(int argc, char **argv, options *sOpt);

int main(int argc, char **argv) {
    options currentOptionsSet = {0};
    parseOptions(argc, argv, &currentOptionsSet);
    
    printf("Hello, World!\n");
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
                    // parse -e
                } else if (*(*(argv + i) + 1) == 'f') {
                    // parse -f
                } else {
                    
                    // parse other flags
                    switch (*(*(argv + i))) {
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
                        case 'e':
                            currentOptionsSet->e = true;
                            // case e
                            break;
                        case 'f':
                            currentOptionsSet->f = true;
                            // case f
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
