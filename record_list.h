//
// Created by flassabe on 30/11/2021.
//

#ifndef _RECORD_LIST_H
#define _RECORD_LIST_H

#include "utils.h"

typedef struct _record_list_node {
    table_record_t record;
    struct _record_list_node *next;
    struct _record_list_node *previous;
} record_list_node_t;

typedef struct {
    record_list_node_t *head;
    record_list_node_t *tail;
} record_list_t;

void clear_list(record_list_t *record_list);
void add_record(record_list_t *record_list, table_record_t *record);

int field_record_length(field_record_t *field_record);
void display_table_record_list(record_list_t *record_list);

#endif //_RECORD_LIST_H
