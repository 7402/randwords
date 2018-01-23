//
//  main.c
//  randwords
//
// Copyright (C) 2018 Michael Budiansky. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this list of conditions
// and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice, this list of conditions
// and the following disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
// WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "words.h"

bool print_words(int from_index, int to_index, int words_per_row, int word_count);
void license(void);
void man_page_source(void);

int main(int argc, const char * argv[]) {
    int max_index = word_count() - 1;
    int from_index = 0;
    int to_index = max_index;
    bool error = false;
    bool silent = false;
    int words_per_row = 5;
    int word_count = 35;
    
    for (int index = 1; index < argc && !error; index++) {
        //  -f from
        if (strcmp(argv[index], "-f") == 0 && index + 1 < argc) {
            from_index = atoi(argv[++index]) - 1;
            error = from_index < 0 || from_index > max_index || from_index >= to_index;
            
        //  -t to
        } else if (strcmp(argv[index], "-t") == 0 && index + 1 < argc) {
            to_index = atoi(argv[++index]) - 1;
            error = to_index < 0 || to_index > max_index || from_index >= to_index;
            
        //  -c words_per_row
        } else if (strcmp(argv[index], "-c") == 0 && index + 1 < argc) {
            words_per_row = atoi(argv[++index]);
            error = words_per_row < 1 || words_per_row > 100;
            
        //  -n number_of_words
        } else if (strcmp(argv[index], "-n") == 0 && index + 1 < argc) {
            word_count = atoi(argv[++index]);
            error = word_count < 0 || word_count > 5000;
            
        //  -v --version       print version
        } else if ((strcmp(argv[index], "--version") == 0) ||
                   (strcmp(argv[index], "-v") == 0)) {
            printf("v0.9\n");
            silent = true;

        //  -h --help       print help for randwords
        } else if ((strcmp(argv[index], "--help") == 0) ||
                   (strcmp(argv[index], "-h") == 0)) {
            printf("Generates a series of random words.\n"
                   "Usage:\n"
                   "  randwords [-c <words_per_row] [-n <number_of_words>]\n"
                   "            [-f <from_word_number>] [-t <to_word_number>]\n"
                   "  randwords -h | --help\n"
                   "  randwords -v | --version\n"
                   "  randwords --license\n"
                   "  randwords --man-page\n"
                   "\n"
                   "Options:\n"
                   "  -c <words_per_row>     Number of words per row [default: 5]\n"
                   "  -n <number_of_words>   Number of words to print [default: 35]\n"
                   "  -f <from_word_number>  Include words from list beginning at this number [default: 1]\n"
                   "  -t <to_word_number>    Include words from list up to this number [default: 5000]\n"
                   "  -h --help     Show this screen.\n"
                   "  --version     Show version.\n"
                   "  --license     Show software copyright and license\n"
                   "  --man_page    Show source for man page\n");
            silent = true;
            
        //  --man-page      print source for man page
        } else if (strcmp(argv[index], "--man-page") == 0) {
            man_page_source();
            silent = true;
            
        //  --licence       print copyright and license
        } else if (strcmp(argv[index], "--license") == 0) {
            license();
            silent = true;;
            
        } else {
            error = true;
        }
    }
    
    if (silent) {
        
    } else if (error) {
        fprintf(stderr, "Error: invalid options\n");
        
    } else {
        if (print_words(from_index, to_index, words_per_row, word_count)) {
            printf("error: unable to print\n");
        }
    }
    
    return error ? 1 : 0;
}

struct KeyIndex {
    short rand_key;
    short words_index;
};
typedef struct KeyIndex KeyIndex;

int compare (const void* elem1, const void* elem2);
int compare (const void* elem1, const void* elem2)
{
    return ((KeyIndex *)elem1)->rand_key - ((KeyIndex *)elem2)->rand_key;
}


bool print_words(int from_index, int to_index, int words_per_row, int word_count)
{
    bool error = false;
    
    int key_index_size = to_index - from_index + 1;;
    KeyIndex *key_index = (KeyIndex *)calloc(key_index_size, sizeof(KeyIndex));
    
    if (key_index != NULL) {
        bool needs_shuffle = true;
        int next_key_index = 0;
        
        srand((unsigned int)time(NULL));
        for (int k = 0; k < key_index_size; k++) key_index[k].words_index = (short)(from_index + k);
        
        for (int word_num = 1; word_num <= word_count; word_num++) {
            if (needs_shuffle) {
                for (int k = 0; k < key_index_size; k++) key_index[k].rand_key = (short)rand();
                qsort(key_index, key_index_size, sizeof(KeyIndex), compare);
                next_key_index = 0;
                needs_shuffle = false;
            }
            
            printf("%s", word(key_index[next_key_index++].words_index));

            if (word_num % words_per_row == 0 || word_num == word_count) {
                printf("\n");
                
            } else {
                printf(" ");
            }

            if (next_key_index >= key_index_size) needs_shuffle = true;
        }

        free(key_index);
        key_index = NULL;
    }
    
    return error;
}

void license(void)
{
    printf("Copyright (C) 2018 Michael Budiansky. All rights reserved.\n"
           "\n"
           "Redistribution and use in source and binary forms, with or without modification, are permitted\n"
           "provided that the following conditions are met:\n"
           "\n"
           "Redistributions of source code must retain the above copyright notice, this list of conditions\n"
           "and the following disclaimer.\n"
           "\n"
           "Redistributions in binary form must reproduce the above copyright notice, this list of conditions\n"
           "and the following disclaimer in the documentation and/or other materials provided with the\n"
           "distribution.\n"
           "\n"
           "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR\n"
           "IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND\n"
           "FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR\n"
           "CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL\n"
           "DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,\n"
           "DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,\n"
           "WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY\n"
           "WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n"
           "\n"
           "The word list in this software is derived from manipulation of data from Google Research\n"
           "<http://storage.googleapis.com/books/ngrams/books/datasetsv2.html>, licensed by Google\n"
           "under a Creative Commons Attribution 3.0 Unported License\n"
           "<http://creativecommons.org/licenses/by/3.0/>.\n"
           );

}

void man_page_source(void)
{
    printf(".TH randwords 1\n"
           "\n"
           ".SH NAME\n"
           "randwords \\- send series of random words to standard output\n"
           "\n"
           ".SH SYNOPSIS\n"
           ".nf\n"
           "\\fBrandwords\\fR [\\fB\\-c\\fR \\fIWORDS_PER_ROW\\fR] [\\fB\\-n\\fR \\fINUMBER_OF_WORDS\\fR]\n"
           "          [\\fB\\-f\\fR \\fIFROM_WORD_NUMBER\\fR] [\\fB\\-t\\fR \\fITO_WORD_NUMBER\\fR]\n"
           ".fi\n"
           "\n"
           ".SH DESCRIPTION\n"
           "Command\\-line tool that generates a series of random words for Morse code practice,\n"
           "and prints to standard output. Words are chosen from a selected subset of 5000 common\n"
           "English words.\n"
           "\n"
           ".SH OPTIONS\n"
           "\n"
           ".TP\n"
           ".BR \\-c \" \" \\fIWORDS_PER_ROW\\fR\n"
           "Number of words per row. Default is 5.\n"
           "\n"
           ".TP\n"
           ".BR \\-n \" \" \\fINUMBER_OF_WORDS\\fR\n"
           "Total number of words to send. Default is 35.\n"
           "\n"
           ".TP\n"
           ".BR \\-f \" \" \\fIFROM_WORD_NUMBER\fR\n"
           "Include words from list beginning at this number, e.g., \\fB-f 200\\fR means start\n"
           "using words from the 200th most common word. Default is 1.\n"
           "\n"
           ".TP\n"
           ".BR \\-t \" \" \\fITO_WORD_NUMBER\\fR\n"
           "Include words from list up to this number, e.g., \\fB-t 500\\fR means use words up to\n"
           "the 500th most common word. Default is 5000.\n"
           "\n"
           ".TP\n"
           ".BR \\-\\-version\n"
           "Show version.\n"
           "\n"
           ".TP\n"
           ".BR \\-h \", \" \\-\\-help\\fR\n"
           "Show help message.\n"
           "\n"
           ".TP\n"
           ".BR \\-\\-license\n"
           "Show software copyright and license.\n"
           "\n"
           ".TP\n"
           ".BR \\-\\-man\\-page\n"
           "Show source for this man page\n"
           "\n"
           ".SH EXAMPLES\n"
           "Print 50 words selected from the most common 100 English words, with 10 words per row:\n"
           ".PP\n"
           ".nf\n"
           ".RS\n"
           "\\fB randwords -t 100 -n 50 -c 10\\fR\n"
           ".RE\n"
           ".fi\n"
           ".PP\n"
           "\n"
           "Play Morse code for 80 words selected from the 100th to 200th most common English words:\n"
           ".PP\n"
           ".nf\n"
           ".RS\n"
           "\\fB randwords -f 100 -t 200 -n 80 | mbeep -i /dev/stdin -c\\fR\n"
           ".RE\n"
           ".fi\n"
           ".PP\n"
           "\n"
           ".SH SEE ALSO\n"
           ".BR mbeep (1)\n"
           "\n"
           ".SH AUTHOR\n"
           "Michael Budiansky \\fIhttps://www.7402.org/email\\fR\n");
}
