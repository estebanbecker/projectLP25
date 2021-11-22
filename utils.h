//
// Created by flassabe on 16/11/2021.
//

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef _UTILS_H
#define _UTILS_H

#define TEXT_LENGTH 150
#define MAX_FIELDS_COUNT 16

#define MAX_TABLE_RECORD_SIZE TEXT_LENGTH*MAX_FIELDS_COUNT

typedef enum {
    TYPE_UNKNOWN,
    TYPE_PRIMARY_KEY,
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_TEXT
} field_type_t;

typedef struct {
    char column_name[TEXT_LENGTH];
    field_type_t column_type;
} field_definition_t;

typedef struct {
    int fields_count;
    field_definition_t definitions[MAX_FIELDS_COUNT];
} table_definition_t;

typedef struct {
    char column_name[TEXT_LENGTH];
    field_type_t field_type;
    union {
        double float_value;
        long long int_value;
        unsigned long long primary_key_value;
        char text_value[TEXT_LENGTH];
    } field_value;
} field_record_t;

typedef struct {
    int fields_count;
    field_record_t fields[MAX_FIELDS_COUNT];
} table_record_t;

typedef enum {
    OP_OR,
    OP_AND,
    OP_ERROR
} operator_t;

typedef struct {
    table_record_t values;
    operator_t logic_operator;
} filter_t;

char *make_full_path(char *path, char *basename);
bool directory_exists(char *path);

#endif //_UTILS_H
