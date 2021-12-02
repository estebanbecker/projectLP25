//
// Created by flassabe on 22/11/2021.
//

#include "query_exec.h"

#include <dirent.h>
#include <unistd.h>

#include "database.h"
#include "table.h"

void execute(query_result_t *query) {
    switch (query->query_type) {
        case QUERY_CREATE_TABLE:
            execute_create(&query->query_content.create_query);
            break;
        case QUERY_INSERT:
            execute_insert(&query->query_content.insert_query);
            break;
        case QUERY_DROP_TABLE:
            execute_drop_table(query->query_content.table_name);
            break;
        case QUERY_DROP_DB:
            execute_drop_database(query->query_content.database_name);
            break;
            /* etc. */ 
    }
}

void execute_create(create_query_t *query) {
}

void execute_insert(insert_query_t *query) {
}

void execute_select(update_or_select_query_t *query) {}

void execute_update(update_or_select_query_t *query) {}

void execute_delete(delete_query_t *query) {
}

void execute_drop_table(char *table_name) {
}

void execute_drop_database(char *db_name) {
}
