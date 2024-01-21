#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>
#define SIZE 256
#define BUFFER_SIZE 4096

typedef struct {
    char *patterns[SIZE]; // array of patterns
    char *patternFiles[SIZE]; // array of pattern files
    char *files[SIZE]; // array of files
    char finalPattern[BUFFER_SIZE]; // single pattern
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

void parseOptions(int argc, char **argv, options *currentOptionsSet);
void printDataInfo(char **data);
void printDebugInfo(options currentOptionsSet);
void parseFlagE(options *currentOptionsSet, char **argv, int *i, int j, int *patternsCount);
void parseFlagF(options *currentOptionsSet, char **argv, int *i, int j, int *patternFilesCount);
void grep(options *currentOptionsSet); // get final pattern
void getPattern(options *currentOptionsSet);
void getPatternFiles(options *currentOptionsSet, int num);
void getFinalData(options *currentOptionsSet, char *line, char *fileName, int num);
void getAuxData(options *currentOptionsSet, char *fileName);
void processingWithO(regex_t regex, char *line, options *currentOptionsSet);



int main(int argc, char **argv) {
    options currentOptionsSet = {0};
    parseOptions(argc, argv, &currentOptionsSet);
    printDebugInfo(currentOptionsSet);
    grep(&currentOptionsSet);
    return 0;
}

void parseOptions(int argc, char **argv, options *currentOptionsSet) {
    int filesCount = 0;
    int patternsCount = 0;
    int patternFilesCount = 0;
    bool isValid = true;
    
    for (int i = 1; i < argc; i++) {
        
        if (isValid) {
            
            if (argv[i][0] == '-') { // check argument for being an option
                
                if (argv[i][1] == 'e') { // -e and -f are first priority to work with
                    parseFlagE(currentOptionsSet, argv, &i, 2, &patternsCount);
                } else if (argv[i][1] == 'f') {
                    parseFlagF(currentOptionsSet, argv, &i, 2, &patternFilesCount);
                } else {
                    
                    // parse other flags
                    // MARK: Check place for j
                    // int j = 0;
                    
                    switch (argv[i][1]) {
                        case 'e':
                            parseFlagE(currentOptionsSet, argv, &i, 2, &patternsCount);
                            break;
                        case 'f':
                            parseFlagF(currentOptionsSet, argv, &i, 2, &patternsCount);
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
                currentOptionsSet->patterns[patternsCount++] = argv[i];
                
            } else { // if there already template has been found let's read as the file
                currentOptionsSet->filesCount++;
                currentOptionsSet->files[filesCount++] = argv[i];
                printf("filesCount incrementation block is done\n");
            }
            
        } else {
            puts("Uncorrect options");
            filesCount = 0;
            break;
        }
    }
    printf("%d - current amount of files\n", currentOptionsSet->filesCount);
}

void printDataInfo(char **data) {
    int i = 0;
    while (data[i]) {
        putc('\t', stdout);
        puts(data[i]);
        i++;
    }
}

void printDebugInfo(options currentOptionsSet) {
    if (currentOptionsSet.filesCount) {
        printf("files count: %d\n", currentOptionsSet.filesCount);
        puts("patterns:"); // MARK: Why putc instead of printf?
        printDataInfo(currentOptionsSet.patterns);
        puts("patterns files:");
        printDataInfo(currentOptionsSet.patternFiles);
        puts("files:");
        printDataInfo(currentOptionsSet.files);
    }
}

void parseFlagE(options *currentOptionsSet, char **argv, int *i, int j, int *patternsCount) {
    currentOptionsSet->e = true;
    if (argv[*i][j] != '\0') {
        // strcat - contacenation of the strings which mean: "add content of string №2 to string №1"
        char buffer[BUFFER_SIZE] = "";
        strcat(buffer, argv[*i] + j);
        currentOptionsSet->patterns[*patternsCount++] = buffer;
    } else {
        currentOptionsSet->patterns[*patternsCount++] = argv[++(*i)];
    }
}

void parseFlagF(options *currentOptionsSet, char **argv, int *i, int j, int *patternFilesCount) {
    currentOptionsSet->f = true;
    if (argv[*i][j] != '\0') {
        char buffer[BUFFER_SIZE] = "";
        strcat(buffer, argv[*i] + j);
        currentOptionsSet->patternFiles[(*patternFilesCount)++] = buffer;
    } else {
        currentOptionsSet->patternFiles[(*patternFilesCount)++] = argv[++(*i)];
    }
}

// ./s21_grep -e temp2 -f patfile.txt file1.txt file2.txt file3.txt - test case
// grep -e temp2 -f patfile.txt file1.txt file2.txt file3.txt
void grep(options *currentOptionsSet) {
    
    if (currentOptionsSet->filesCount == 0) {
        fprintf(stderr, "No files specified.\n");
        return;
    }
    
    getPattern(currentOptionsSet);
    puts("Final pattern:"); // you can just use printf or delete the whole line
    puts(currentOptionsSet->finalPattern);
    
    for (int i = 0; i < currentOptionsSet->filesCount; i++) {
        
        FILE *file = fopen(currentOptionsSet->files[i], "r");
        
        if (file != NULL) {
            // printf("Error check 4 right after opening the first file\n");
            
            char buffer[BUFFER_SIZE];
            int num = 1;
            
            while (fgets(buffer, BUFFER_SIZE, file)) {
                
                int lenght = strlen(buffer); // work with last string
                
                if (buffer[lenght - 1] != '\n') {
                    buffer[lenght + 1] = 0;
                    buffer[lenght] = '\n';
                }
                getFinalData(currentOptionsSet, buffer, currentOptionsSet->files[i], num);
                num++;
            }
            getAuxData(currentOptionsSet, currentOptionsSet->files[i]); // RENAME THIS SHIT
            fclose(file);
            
        } else if (!currentOptionsSet->s) {
            fprintf(stderr, "s21_grep: %s: No such file of directory", currentOptionsSet->files[i]);
        } else {
            fprintf(stderr, "something went wrong. Printed by fprintf\n");
            printf("something wrong here. Printed by printf\n");
        }
    }
}

void getPattern(options *currentOptionsSet) {
    for (int i = 0; currentOptionsSet->patterns[i]; i++) {
        // strlen - determines the lenght of string excluding '\0' character
        strcat(strcat(currentOptionsSet->finalPattern, !strlen(currentOptionsSet->finalPattern) ? "" : "|"),
               // "." - empty pattern. Mean random char in regular expressions
               !strlen(currentOptionsSet->patterns[i]) ? "." : currentOptionsSet->patterns[i]);
        for (int i = 0; currentOptionsSet->patternFiles[i]; i++) {
            getPatternFiles(currentOptionsSet, i);
        }
    }
}

//void cook_pattern_files(options *s_opt, int num) {
//  char buf[BUF_SIZE];
//  FILE *fp = fopen(s_opt->pattern_files[num], "r");
//  if (fp) {
//    while (fgets(buf, BUF_SIZE, fp)) {
//      char *c = buf + strlen(buf) - 1;
//      if (*c == '\n') *c = '\0';
//      strcat(strcat(s_opt->pattern, !strlen(s_opt->pattern) ? "" : "|"),
//             !strlen(buf) ? "." : buf);
//    }
//  } else
//    fprintf(stderr, "s21_grep: %s: No such file or directory\n",
//            s_opt->pattern_files[num]);
//}

void getPatternFiles(options *currentOptionsSet, int num) {
    char buffer[BUFFER_SIZE];
    FILE *file = fopen(currentOptionsSet->patternFiles[num], "r");
    
    if (file != NULL) {
        // read patterns line by line and add it to the final pattern
        while (fgets(buffer, BUFFER_SIZE, file)) {
            char *content = buffer + strlen(buffer) - 1;
            printf("%s - another string\n", content);
            printf("%c - another char\n", *content);
            
            if (*content == '\n') { // MARK: ERROR 1. This code won't work
                printf("%s - found wrong string\n", content);
                 *content = '\0';
            }
            strcat(strcat(currentOptionsSet->finalPattern, !strlen(currentOptionsSet->finalPattern) ? "" : "|"),
                   !strlen(buffer) ? "." : buffer);
        }
    } else {
        // fprintf let us choose output stream for printing the data
        // stderr it's a type of output specially made for errors
        fprintf(stderr, "s21_grep: %s: No such file or directory\n", currentOptionsSet->patternFiles[num]);
    }
}

void getFinalData(options *currentOptionsSet, char *line, char *fileName, int num) {
    regex_t regex;
    
    if (!regcomp(&regex, currentOptionsSet->finalPattern, currentOptionsSet->i ? REG_ICASE : REG_EXTENDED)) {
        
        if (regexec(&regex, line, 0, NULL, 0) == currentOptionsSet->v) {
            currentOptionsSet->matchesCount++;
            
            if ((currentOptionsSet->c || currentOptionsSet->l) == 0) {
                
                if (currentOptionsSet->filesCount > 1 && !currentOptionsSet->h) {
                    printf("%s:", fileName);
                }
                
                if (currentOptionsSet->n) {
                    printf("%d:", num);
                }
                
                if (!currentOptionsSet->o) {
                    fputs(line, stdout);
                } else {
                    processingWithO(regex, line, currentOptionsSet); // RENAME THIS SHIT
                }
            }
            regfree(&regex);
        }
    }
}

void getAuxData(options *currentOptionsSet, char *fileName) {
    
    if (currentOptionsSet->c) {
        
        if (currentOptionsSet->l) {
            currentOptionsSet-> filesCount > 1 ? printf("%s:1\n", fileName) : printf("1\n");
        } else {
            
            if (currentOptionsSet->filesCount > 1) {
                printf("%s:", fileName);
            }
            printf("%d\n", currentOptionsSet->matchesCount);
        }
    }
    
    if (currentOptionsSet->l && currentOptionsSet->matchesCount) {
        printf("%s\n", fileName);
    }
}

void processingWithO(regex_t regex, char *line, options *currentOptionsSet) {
    
    while (!regexec(&regex, line, 0, NULL, 0)) { // as long as we have matches in the line, do:
        
        char buffer[BUFFER_SIZE] = "";
        strcpy(buffer, line);
        int left = 0; // index of the first left element in the matching line
        int right = strlen(buffer); // index of the last right element in the matching line
        
        while (!regexec(&regex, buffer + left, 0, NULL, 0)) {
            left++; // as long as we have matches in the line, let's push left index
        }
        left--; // if we accidently made it out of the line, let's back
        
        while(!regexec(&regex, buffer + left, 0, NULL, 0)) { // after push the right index and remove unmatching elements in parallel
            right--;
            buffer[right] = 0;
        }
        buffer[right] = line[right];
        right++; // if we made it out of the line let's back
        
        if (!currentOptionsSet->v) {
            printf("%s\n", buffer + left); // display the matching elements
        }
        int lenght = right - left + 1;
        
        for (int i = 0; i < lenght; i++) { // remove matching elements from the line
            line[left++] = line[right++];
        }
    }
    
    if (currentOptionsSet->v) {
        printf("%s", line); // if there is flag "v" let program to invert the output (bring only unmatching lines)
    }
}
