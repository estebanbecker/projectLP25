//
// Created by flassabe on 23/11/2021.
//

#include "expand.h"

/**
 * @brief Expand the query result to have all the fiel of the table
 * 
 * @param query Querry to expand
 * @author @estebanbecker
 */
void expand(query_result_t *query) {
    if (query->query_type == QUERY_SELECT) {
        expand_select(&query->query_content.select_query);
    } else if (query->query_type == QUERY_INSERT) {
        expand_insert(&query->query_content.insert_query);
    }
}

/**
 * @brief Exand a select query to have all the fields of the table
 * 
 * @param query Select query to expand
 * @author @estebanbecker
 */
void expand_select(update_or_select_query_t *query) {
    if(query->set_clause.fields_count == 1 && query->set_clause.fields[0].field_type == TYPE_TEXT && strcmp(query->set_clause.fields[0].field_value.text_value,"*") == 0) {
        query->set_clause.fields_count = 0;
        table_definition_t table;
        
        get_table_definition(query->table_name, &table);

        for (int i = 0; i < table.fields_count; i++) {
            strcpy(query->set_clause.fields[i].field_value.text_value , table.definitions[i].column_name);
            strcpy(query->set_clause.fields[i].column_name , table.definitions[i].column_name);
            query->set_clause.fields[i].field_type = table.definitions[i].column_type;
            
            query->set_clause.fields_count++;
        }
    }
}

/**
 * @brief Expand an insert query to have all the fields of the table
 * 
 * @param query Insert query to expand
 * @author @estebanbecker
 */
void expand_insert(insert_query_t *query) {

    table_definition_t table;
    
    get_table_definition(query->table_name, &table);

    if(query->fields_names.fields_count == 1 && query->fields_names.fields[0].field_type == TYPE_TEXT && strcmp(query->fields_names.fields[0].field_value.text_value,"*") == 0) {

        for (int i = 0; i < table.fields_count; i++) {
            strcpy(query->fields_values.fields[i].column_name , table.definitions[i].column_name);
        }

    }else{
        for (int i = 0; i < table.fields_count; i++) {
            if(!is_field_in_record(&query->fields_values, table.definitions[i].column_name)) {
                strcpy(query->fields_values.fields[query->fields_names.fields_count].column_name , table.definitions[i].column_name);
                query->fields_values.fields[query->fields_names.fields_count].field_type = table.definitions[i].column_type;
                make_default_value(&query->fields_values.fields[query->fields_names.fields_count],query->table_name);
                query->fields_values.fields_count++;
            }
        }
    }
}

/**
 * @brief 
 * 
 * @param record record to check
 * @param field_name field name to search
 * @return true is in the record
 * @return false is not in the record
 * @author @estebanbecker
 */
bool is_field_in_record(table_record_t *record, char *field_name) {
    for (int i = 0; i < record->fields_count; i++) {
        if (strcmp(record->fields[i].column_name, field_name) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Put the default value of the field in the record
 * 
 * @param field field to modify
 * @param table_name table name
 * @author @estebanbecker
 */
void make_default_value(field_record_t *field, char *table_name) {
    switch (field->field_type) {
        case TYPE_INTEGER:
            field->field_value.int_value = 0;
            break;
        case TYPE_TEXT:
            strcpy(field->field_value.text_value, "");
            break;
        
        case TYPE_FLOAT:
            field->field_value.float_value = 0;
            break;
        case TYPE_PRIMARY_KEY:
            field->field_value.primary_key_value = get_next_key(table_name);
            break;
    }
}

