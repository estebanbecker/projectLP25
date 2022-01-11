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
            execute_update(&query->query_content.update_query);
            break;

        case QUERY_DELETE:
            execute_delete(&query->query_content.delete_query);
            break;
        case QUERY_NONE:
        
        default:
            break;
    }
}

/*!
 * @brief function execute_create creates a table
 * @param query query to be executed
 */
void execute_create(create_query_t *query) {
    create_table(query);
}

void execute_insert(insert_query_t *query) {
     for (int i = 0; i < query->fields_values.fields_count; i++)
    {   
        if(query->fields_values.fields[i].field_type == TYPE_PRIMARY_KEY){
            unsigned long long next_key = get_next_key(query->table_name);
            if( query->fields_values.fields[i].field_value.primary_key_value > next_key){
                update_key(query->table_name, query->fields_values.fields[i].field_value.primary_key_value);
            } else {
                query->fields_values.fields[i].field_value.primary_key_value = next_key;
                update_key(query->table_name, next_key++);
            }
        }  
    }
    add_row_to_table(query->table_name, &query->fields_values);
}

void execute_select(update_or_select_query_t *query) {
    char table_name[TEXT_LENGTH];
        char cwd[4096];
    record_list_t *record_list = malloc(sizeof(record_list_t));
    
    get_filtered_records(
            query->table_name,
            &query->set_clause, 
            &query->where_clause,
            record_list);
            
    display_table_record_list(record_list);
    free(record_list);
}

/*!
 * @brief function execute_update updates records in the database
 * @param query query to be executed
 */
void execute_update(update_or_select_query_t *query) {
    FILE *idx = open_index_file(query->table_name, "rb");

    if (idx){
        index_record_t index_record;
        table_record_t record;

        table_definition_t table_definition;
        get_table_definition(query->table_name, &table_definition);

        //read index to look through all active records
        while (fread(&index_record.is_active, sizeof(uint8_t), 1, idx) == 1) {
            if (index_record.is_active) {
                fread(&index_record.record_offset, sizeof(uint32_t), 1, idx);
                fseek(idx, 3, SEEK_CUR);

                get_table_record(query->table_name, index_record.record_offset, &table_definition, &record);

                if (is_matching_filter(&record, &query->where_clause)) {

                    for (int field_set = 0; field_set < query->set_clause.fields_count; ++field_set) {
                        for (int field_record = 0; field_record < record.fields_count; ++field_record) {
                            if (strcmp(query->set_clause.fields[field_set].column_name, record.fields[field_record].column_name) == 0){
                                record.fields[field_record] = query->set_clause.fields[field_set];
                            }
                        }
                    }
                    write_record(query->table_name, index_record.record_offset, &table_definition, &record);

                }
            } else {
                fseek(idx, 7, SEEK_CUR);
            }
        }

        fclose(idx);
    }
}


/*!
 * @brief function execute_delete deletes records in a database
 * @param query query to be executed
 */
void execute_delete(delete_query_t *query) {

    table_definition_t table_definition;
    get_table_definition(query->table_name, &table_definition);

    FILE *idx = open_index_file(query->table_name, "rb+");

    if (idx){
        index_record_t index_record;
        table_definition_t table_definitions;
        table_record_t record;
        uint8_t empty = 0;

        get_table_definition(query->table_name, &table_definitions);

        //read index to look through all active records
        while (fread(&index_record.is_active, sizeof(uint8_t), 1, idx) == 1) {
            if (index_record.is_active) {

                fread(&index_record.record_offset, sizeof(uint32_t), 1, idx);
                get_table_record(query->table_name, index_record.record_offset, &table_definitions, &record);

                if (is_matching_filter(&record, &query->where_clause)) {
                    fseek(idx, -5, SEEK_CUR);

                    fwrite(&empty, sizeof(uint8_t), 1, idx);

                    fseek(idx, 7, SEEK_CUR);
                }else{
                    fseek(idx, 3, SEEK_CUR);
                }

            } else {
                fseek(idx, 7, SEEK_CUR);
            }
        }

        fclose(idx);
    }
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
