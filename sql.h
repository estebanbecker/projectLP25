//
// Created by flassabe on 16/11/2021.
//

#ifndef _SQL_H
#define _SQL_H

#include "utils.h"

#include <stdbool.h>

typedef enum {
    QUERY_NONE,
    QUERY_CREATE_TABLE,
    QUERY_DROP_TABLE,
    QUERY_SELECT,
    QUERY_UPDATE,
    QUERY_DELETE,
    QUERY_INSERT,
    QUERY_DROP_DB,
} query_type_t;

typedef struct {
    char table_name[TEXT_LENGTH];
    table_definition_t table_definition;
} create_query_t;

typedef struct {
    char table_name[TEXT_LENGTH];
    filter_t where_clause;
} delete_query_t;

typedef struct {
    char table_name[TEXT_LENGTH];
    table_record_t fields_names;
    table_record_t fields_values;
} insert_query_t;

typedef struct {
    char table_name[TEXT_LENGTH];
    table_record_t set_clause;
    filter_t where_clause;
} update_or_select_query_t;

typedef struct {
    query_type_t query_type;
    union {
        char table_name[TEXT_LENGTH];
        char database_name[TEXT_LENGTH];
        create_query_t create_query;
        delete_query_t delete_query;
        insert_query_t insert_query;
        update_or_select_query_t update_query;
        update_or_select_query_t select_query;
    } query_content;
} query_result_t;

query_result_t *parse(char *sql, query_result_t *result);
query_result_t *parse_select(char *sql, query_result_t *result);
query_result_t *parse_create(char *sql, query_result_t *result);
query_result_t *parse_insert(char *sql, query_result_t *result);
query_result_t *parse_update(char *sql, query_result_t *result);
query_result_t *parse_delete(char *sql, query_result_t *result);
query_result_t *parse_drop_db(char *sql, query_result_t *result);
query_result_t *parse_drop_table(char *sql, query_result_t *result);

char *get_sep_space(char *sql);
char *get_sep_space_and_char(char *sql, char c);
char *get_keyword(char *sql, char *keyword);
char *get_field_name(char *sql, char *field_name);
bool has_reached_sql_end(char *sql);
char *parse_fields_or_values_list(char *sql, table_record_t *result);
char *parse_create_fields_list(char *sql, table_definition_t *result);
char *parse_equality(char *sql, field_record_t *equality);
char *parse_set_clause(char *sql, table_record_t *result);
char *parse_where_clause(char *sql, filter_t *filter);

#endif //_SQL_H
