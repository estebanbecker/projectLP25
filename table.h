//
// Created by flassabe on 19/11/2021.
//

#ifndef _TABLE_H
#define _TABLE_H

#include <stdint.h>
#include <stdio.h>

#include "sql.h"
#include "record_list.h"

typedef struct __attribute__((__packed__)) { // attribute allows to directly fread from file into structure
    uint8_t is_active;
    uint32_t record_offset;
    uint16_t record_length;
} index_record_t;

FILE *open_definition_file(char *table_name, char *mode);
FILE *open_index_file(char *table_name, char *mode);
FILE *open_content_file(char *table_name, char *mode);
FILE *open_key_file(char *table_name, char *mode);

void create_table(create_query_t *table_definition);
void drop_table(char *table_name);
table_definition_t *get_table_definition(char *table_name, table_definition_t *result);
uint16_t compute_record_length(table_definition_t *definition);
uint32_t find_first_free_record(char *table_name);

void add_row_to_table(char *table_name, table_record_t *record);
char *format_row(char *table_name, char *buffer, table_definition_t *table_definition, table_record_t *record);
void update_key(char *table_name, unsigned long long value);
unsigned long long get_next_key(char *table_name);

field_record_t *find_field_in_table_record(char *field_name, table_record_t *record);
bool is_matching_filter(table_record_t *record, filter_t *filter);
record_list_t *get_filtered_records(char *table_name, table_record_t *required_fields, filter_t *filter, record_list_t *result);
table_record_t *get_table_record(char *table_name, uint32_t offset, table_definition_t *def, table_record_t *result);

#endif //_TABLE_H