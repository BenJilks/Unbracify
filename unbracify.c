#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define LINE_BUFFER_SIZE 1024

static char* s_line = NULL;
static size_t s_line_buffer_size = LINE_BUFFER_SIZE;
static size_t s_last_line_length = 0;
static size_t s_last_line_indent = 0;
static size_t s_line_count = 0;

static bool s_peek_current_line = false;
static bool s_ignore_indent_next_line = false;

ssize_t next_line(FILE* file):
    if (s_peek_current_line):
        s_peek_current_line = false;
        return s_last_line_length;

    size_t length = getline(&s_line, &s_line_buffer_size, file);
    s_last_line_length = length;
    s_line_count += 1;
    return length;

size_t trim_end(char* string, size_t length):
    for (int i = length - 1; i >= 0; --i):
        if (!isspace(string[i])):
            return i + 1;
    return 0;

size_t count_indent(char const* line, size_t length):
    for (size_t i = 0; i < length; ++i):
        if (!isspace(line[i])):
            return i;
    return 0;

void unbracify_block(FILE* file, size_t last_indent):
    size_t length = 0;
    size_t current_indent = 0;
    bool current_indent_set = false;

    while ((length = next_line(file)) != -1):
        length = trim_end(s_line, length);
        if (length == 0):
            printf("\n");
            continue;

        size_t indent = count_indent(s_line, length);
        if (!current_indent_set):
            current_indent = indent;
            current_indent_set = true;

            if (last_indent != 0 && current_indent <= last_indent):
                fprintf(stderr, "Error line %lu: Expected indent\n", \
                    s_line_count);
                exit(1);

        if (s_ignore_indent_next_line):
            s_ignore_indent_next_line = false;
            indent = s_last_line_indent;
        else:
            s_last_line_indent = indent;

            if (indent > current_indent):
                fprintf(stderr, "Error line %lu: Unexpected indent\n", \
                    s_line_count);
                exit(1);

            if (indent < current_indent):
                s_peek_current_line = true;
                break;

        if (s_line[length - 1] == ':'):
            char* original_line = malloc(length);
            memcpy(original_line, s_line, length);

            printf("%.*s\n", (int)length - 1, original_line);

            printf("%.*s{\n", (int)indent, original_line);
            unbracify_block(file, current_indent);
            printf("%.*s}\n", (int)indent, original_line);

            free(original_line);
        else if (s_line[length - 1] == '\\'):
            s_ignore_indent_next_line = true;
            printf("%.*s\n", (int)length - 1, s_line);
        else:
            printf("%.*s\n", (int)length, s_line);

void unbracify_file(FILE* file):
    s_line = malloc(LINE_BUFFER_SIZE);
    s_line_buffer_size = LINE_BUFFER_SIZE;
    unbracify_block(file, 0);
    free(s_line);

void usage(char const* executable):
    printf("Usage: %s <input_file>\n", executable);
    printf("\n");

int main(int argc, char const** argv):
    if (argc <= 1):
        usage(argv[0]);
        fprintf(stderr, "error: no input file given\n");
        return 1;

    char const* input_file_path = argv[1];
    FILE* input_file = fopen(input_file_path, "r");
    if (!input_file):
        fprintf(stderr, "error: could not open input file: %s\n", \
            strerror(errno));
        return 1;

    unbracify_file(input_file);
    fclose(input_file);
    return 0;
