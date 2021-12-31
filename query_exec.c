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
            
        case QUERY_SELECT:
            execute_select(&query->query_content.select_query); 
            break;
        case QUERY_UPDATE:
            execute_update(&query);
            break;
        case QUERY_DELETE:
            execute_delete(&query->query_content.delete_query);
            break;
        case QUERY_NONE:
        
        default:
            break;
    }
}

void execute_create(create_query_t *query) {
    create_table(query);
}

void execute_insert(insert_query_t *query) {
    chdir(query->table_name);
    //insert_column(query);
    add_row_to_table(query->table_name, &query->fields_values);
    chdir("..");
}

void execute_select(update_or_select_query_t *query) {
    char table_name[TEXT_LENGTH];
    chdir(query->table_name);
    record_list_t *record = malloc(sizeof(record_list_t));
    display_table_record_list(
        get_filtered_records(
            query->table_name, 
            &query->set_clause, 
            &query->where_clause, 
            record)
        );
    free(record);
    chdir("..");
}

void execute_update(update_or_select_query_t *query) {
    chdir(query->table_name);
    record_list_t *record = malloc(sizeof(record_list_t));
    get_filtered_records(
            query->table_name, 
            &query->set_clause, 
            &query->where_clause, 
            record);
    if (record != NULL) {
        record_list_node_t *tmp = record->head;
        while (tmp) {
            tmp = tmp->next;
            
        }
        record->head = record->tail = NULL;
    }
    free(record);
    chdir("..");
}

void execute_delete(delete_query_t *query) {

    chdir(query->table_name);
    
    record_list_t *record = malloc(sizeof(record_list_t));
    table_definition_t *required_fields = malloc(sizeof(table_definition_t));
    char table_name[TEXT_LENGTH];
    strcpy(table_name, query->table_name);
    required_fields = get_table_definition(query->table_name, required_fields);
    filter_t *filter = malloc(sizeof(filter_t));

    //using get_filtered_records code

    *filter = query->where_clause;
    FILE *idx = open_index_file(table_name, "rb");

    if (idx){
        index_record_t index_record;
        table_definition_t table_definitions;
        table_record_t record;
        table_record_t result_record;

        get_table_definition(table_name, &table_definitions);

        //read index to look through all active records
        while (fread(&index_record.is_active, sizeof(uint8_t), 1, idx) == 1) {
            if (index_record.is_active) {
                fread(&index_record.record_offset, sizeof(uint32_t), 1, idx);
                get_table_record(table_name, index_record.record_offset, &table_definitions, &record);

                if (is_matching_filter(&record, &query->where_clause)) {
                    index_record.is_active = false;
                }
                fseek(idx, 3, SEEK_CUR);
            } else {
                fseek(idx, 7, SEEK_CUR);
            }
        }

        fclose(idx);
    }
    free(record);
    chdir("..");
}

/*!
 * @brief function execute_drop_table deletes a database's table
 * assuming working in database directory
 * @param db_name name of the table being dropped
 */
void execute_drop_table(char *table_name) {
    //recursive_rmdir(table_name);
    drop_table(table_name);
}

/*!
 * @brief function execute_drop_database deletes the whole database
 * assuming working in database directory
 * @param db_name name of the database being dropped
 */
void execute_drop_database(char *db_name) {
    chdir(".."); //switch to main directory for db
    recursive_rmdir(db_name);
}


void insert_column(insert_query_t *query){
    for(int field = 0; field < query->fields_names.fields_count; field++){
    strcpy(query->fields_values.fields[field].column_name, query->fields_names.fields[field].field_value.text_value);
    }
}