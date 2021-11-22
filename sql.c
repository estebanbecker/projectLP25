//
// Created by flassabe on 16/11/2021.
//

#include "sql.h"

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>

char *get_sep_space(char *sql) {
    return sql;
}

char *get_sep_space_and_char(char *sql, char c) {
    return sql;
}

char *get_keyword(char *sql, char *keyword) {
    return sql;
}

char *get_field_name(char *sql, char *field_name) {
    return sql;
}

bool has_reached_sql_end(char *sql) {
    return false;
}

char *parse_fields_or_values_list(char *sql, table_record_t *result) {
    return sql;
}

char *parse_create_fields_list(char *sql, table_definition_t *result) {
    return sql;
}

char *parse_equality(char *sql, field_record_t *equality) {
    return sql;
}

char *parse_set_clause(char *sql, table_record_t *result) {
    return sql;
}

char *parse_where_clause(char *sql, filter_t *filter) {
    return sql;
}

query_result_t *parse(char *sql, query_result_t *result) {
    return NULL;
}

query_result_t *parse_select(char *sql, query_result_t *result) {
    return NULL;
}

query_result_t *parse_create(char *sql, query_result_t *result) {
    return NULL;
}

query_result_t *parse_insert(char *sql, query_result_t *result) {
    return NULL;
}

query_result_t *parse_update(char *sql, query_result_t *result) {
    return NULL;
}

query_result_t *parse_delete(char *sql, query_result_t *result) {
    return NULL;
}

query_result_t *parse_drop_db(char *sql, query_result_t *result) {
    return NULL;
}

query_result_t *parse_drop_table(char *sql, query_result_t *result) {
    return NULL;
}
