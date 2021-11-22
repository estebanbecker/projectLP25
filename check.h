//
// Created by flassabe on 19/11/2021.
//

#ifndef _CHECK_H
#define _CHECK_H

#include "sql.h"

bool check_query(query_result_t *query);

bool check_query_select(update_or_select_query_t *query);
bool check_query_update(update_or_select_query_t *query);
bool check_query_create(create_query_t *query);
bool check_query_insert(insert_query_t *query);
bool check_query_delete(delete_query_t *query);
bool check_query_drop_table(char *table_name);
bool check_query_drop_db(char *db_name);

bool check_fields_list(table_record_t *fields_list, table_definition_t *table_definition);
bool check_value_types(table_record_t *fields_list, table_definition_t *table_definition);

field_definition_t *find_field_definition(char *field_name, table_definition_t *table_definition);
bool is_value_valid(field_record_t *value, field_definition_t *field_definition);
bool is_int(char *value);
bool is_float(char *value);
bool is_key(char *value);

#endif //_CHECK_H
