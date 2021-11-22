//
// Created by flassabe on 19/11/2021.
//

#include "check.h"

#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>

#include "table.h"

/*!
 * @brief function check_query is the high level check function, which will call specialized check functions and
 * return their result.
 * @param query a pointer to a query result structure returned after parsing the SQL query (@see sql.h)
 * @return true if the query is valid, false else
 */
bool check_query(query_result_t *query) {
    switch (query->query_type) {
        case QUERY_SELECT:
            return check_query_select(&query->query_content.select_query);
            break;

        /* etc. */

        default:
            printf("Unsupported query code\n");
    }
    return false;
}

/*!
 * @brief check_query_select checks a select query
 * It relies on check_fields_list (list of fields to display and WHERE clause) and check_value_types (WHERE clause)
 * Also uses get_table_definition from table.h
 * @see check_fields_list
 * @see check_value_types
 * @see get_table_definition
 * @param query a pointer to the select query
 * @return true if valid, false if invalid
 */
bool check_query_select(update_or_select_query_t *query) {
    return false;
}

/*!
 * @brief check_query_update checks an update query
 * It relies on check_value_types and get_table_definition from table.h.
 * @see check_value_types
 * @see get_table_definition
 * @param query a pointer to the update query
 * @return true if valid, false if invalid
 */
bool check_query_update(update_or_select_query_t *query) {
    return false;
}

/*!
 * @brief check_query_create checks a create query. It checks if the table doesn't already exist
 * @param query a pointer to the create query
 * @return true if valid, false if invalid
 */
bool check_query_create(create_query_t *query) {
    return false;
}

/*!
 * @brief check_query_insert checks an insert query
 * It relies on check_value_types and get_table_definition from table.h.
 * Prior to checking the field/value pairs, you must join fields and values (extracted to different table_record_t
 * structs during the SQL parsing) by copying the values strings into the text_value member of the fields names
 * table_record_t structure.
 * @see check_value_types
 * @see get_table_definition
 * @param query a pointer to the insert query
 * @return true if valid, false if invalid
 */
bool check_query_insert(insert_query_t *query) {
    return false;
}

/*!
 * @brief check_query_delete checks a delete query
 * It relies on check_value_types (WHERE clause) and get_table_definition from table.h.
 * @see check_value_types
 * @see get_table_definition
 * @param query a pointer to the delete query
 * @return true if valid, false if invalid
 */
bool check_query_delete(delete_query_t *query) {
    return false;
}

/*!
 * @brief check_query_drop_table checks a drop table query: checks that the table exists
 * @param table_name the table name
 * @return true if valid, false if invalid
 */
bool check_query_drop_table(char *table_name) {
    return false;
}

/*!
 * @brief check_query_drop_db checks a drop database query: checks that the database exists
 * @param db_name the database name
 * @return true if valid, false if invalid
 */
bool check_query_drop_db(char *db_name) {
    return false;
}

/*!
 * @brief function check_fields_list checks if all fields from a fields list exist in a table definition
 * Uses find_field_definition
 * @see find_field_definition
 * @param fields_list list of the fields to check
 * @param table_definition table definition within which to check
 * @return true if all fields belong to table, false else
 */
bool check_fields_list(table_record_t *fields_list, table_definition_t *table_definition) {
    return false;
}

/*!
 * @brief function check_value_types checks if all fields in a record list belong to a table (through its definition)
 * AND if the corresponding value can be converted to the field type as specified in the table definition.
 * Uses find_field_definition and is_value_valid
 * @see find_field_definition
 * @see is_value_valid
 * @param fields_list list of the fields and their values (as text)
 * @param table_definition table definition
 * @return true if all fields belong to table and their value types are correct, false else
 */
bool check_value_types(table_record_t *fields_list, table_definition_t *table_definition) {
    return false;
}

/*!
 * @brief function find_field_definition looks up for a field name in a table definition.
 * @param field_name the field name to lookup for.
 * @param table_definition the table definition in which to search.
 * @return a pointer to the field definition structure if the field name exists, NULL if it doesn't.
 */
field_definition_t *find_field_definition(char *field_name, table_definition_t *table_definition) {
    return NULL;
}

/*!
 * @brief function is_value_valid checks if a field_record_t holding a field name and a text value matches a field definition:
 * it checks fields names, and tests if the text value can be converted into the table defined type.
 * /!\ If it succeeded, this function must modify the value in the field_record_t structure by converting and setting
 * the value to the proper type!
 * Uses is_int, is_float, is_key
 * @see is_int
 * @see is_float
 * @see is_key
 * @param value the field and value record
 * @param field_definition the field definition to test against
 * @return true if valid (and converted), false if invalid
 */
bool is_value_valid(field_record_t *value, field_definition_t *field_definition) {
    return false;
}

/*!
 * @brief function is_int tests if the string value is a text representation of an integer value.
 * You may use strtoll for this test.
 * @param value the text representation to test
 * @return true if value can be converted into an integer, false if it cannot
 */
bool is_int(char *value) {
    return false;
}

/*!
 * @brief function is_float tests if the string value is a text representation of a double value.
 * You may use strtod for this test.
 * @param value the text representation to test
 * @return true if value can be converted into a double, false if it cannot
 */
bool is_float(char *value) {
    return false;
}

/*!
 * @brief function is_int tests if the string value is a text representation of a key value.
 * You may use strtoull for this test.
 * @param value the text representation to test
 * @return true if value can be converted into a key, false if it cannot
 */
bool is_key(char *value) {
    return false;
}
