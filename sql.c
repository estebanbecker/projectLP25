//
// Created by flassabe on 16/11/2021.
//

#include "sql.h"

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Get the first occurrence of a non space character in a string.
 * @example "   hello" -> pointer to "hello"
 * 
 * @param sql Pointer to a position in the sql query.
 * @return char* Return the pointer to the first occurrence of a non space character.
 * @author @estebanbecker
 */
char *get_sep_space(char *sql) {
    /*if(!isspace(*sql)) {
        printf("Use less \n");
    }*/
    while(isspace(*sql)) {
        sql++;
    }
    return sql;
}

/**
 * @brief Get the first occurrence of a character after spaces and one time the parameter "c".
 * @example "  ,  id WHERE data = 2 , id = 1",',' -> pointer to "id WHERE data = 2 , id = 1"
 * @example "  a  data , id",'a' -> pointer to "data , id"
 * @example "  ,,  id WHERE data = 2 , id = 1",',' -> pointer to ",  id WHERE data = 2 , id = 1"
 * @example "id WHERE data = 2 , id = 1",',' ->NULL
 *  
 * @param sql Pointer to a position in the sql query.
 * @param c Character to skip
 * @return char* Pointer to the position in the query after the character.
 * @author @estebanbecker
 */
char *get_sep_space_and_char(char *sql, char c) {
    sql = get_sep_space(sql);
    if (*sql == c) {
        sql++;
    }else{
        return NULL;
    }
    sql=get_sep_space(sql);
    return sql;
}

/**
 * @brief Check that the keyword is at the beginning of the string. The keyword is case insensitive.
 * @example "SELECT * FROM table WHERE id = 1","select" -> " * FROM table WHERE id = 1"
 * @example "SELECT * FROM table WHERE id = 1","from" -> NULL
 * 
 * @param sql Pointer to a position in the sql query.
 * @param keyword Keyword to check.
 * @return char* Pointer to the position in the query after the keyword.
 * @author @estebanbecker
 */
char *get_keyword(char *sql, char *keyword) {
    int valid = 1;
    sql=get_sep_space(sql);
    while(valid && *keyword != '\0') {
        if (tolower(*sql) != tolower(*keyword)) {
            valid = 0;
        }
        sql++;
        keyword++;
    }
    sql=get_sep_space(sql);
    if (valid) {
        return sql;
    }else{
        return NULL;
    }
}

/**
 * @brief Get the field name object.
 * @example "'my name is' text)" -> " text" and field_name = "my name is"
 * @example "my name is text)" -> " name is text" and field_name = "my"
 * 
 * @param sql Pointer to a position in the sql query.
 * @param field_name Point to the field name to remplace.
 * @return char* Pointer to the position in the query after the field name.
 * @author @estebanbecker
 */
char *get_field_name(char *sql, char *field_name) {
    int i = 0;
    sql=get_sep_space(sql);
    if (*sql== '\'') {
        i++;
        sql++;
        while(*sql != '\'') {
            field_name[i-1] = *sql;
            i++;
            sql++;
        }
        field_name[i-1] = '\0';
        return ++sql;
    }else{
        while(isalnum(*sql)) {
            field_name[i] = *sql;
            i++;
            sql++;
        }
        field_name[i] = '\0';
        return sql;
    }
}

/**
 * @brief Check if it is the end of the query by looking if there are only spaces left.
 * 
 * @param sql Pointer to a position in the sql query.
 * @return true If it is the end of the query.
 * @return false If it is not the end of the query.
 * @author @estebanbecker
 */
bool has_reached_sql_end(char *sql) {
    while (*sql == ' ') {
        sql++;
    }
    if (*sql == '\0' || *sql == ';') {
        return true;
    }else{
        return false;
    }
}

/**
 * @brief Get a list of value or fiels names.
 * @example " id, name, age WHERE id=2;" -> "WHERE id=2;" and result->fields_count = 3 result->fields[0] = TYPE_TEXT,"id" result->fields[1] = TYPE_TEXT,"name" result->fields[2] = TYPE_TEXT,"age"
 * @example "12, Hey, 2.5 WHERE id=2;" -> "WHERE id=2;" and result->fields_count = 3 result->fields[0] = TYPE_INTEGER,"12" result->fields[1] = TYPE_TEXT,"Hey" result->fields[2] = TYPE_FLOAT,"2.5"
 * 
 * @param sql Pointer to a position in the sql query.
 * @param result a pointer to the list of values or fields names in an organised structure to modificate.
 * @return char* Pointer to the position in the query after the list of values or fields names.
 * @author @estebanbecker
 */
char *parse_fields_or_values_list(char *sql, table_record_t *result) {
    
    char field[TEXT_LENGTH];
    bool continue_parsing = true;
    sql= get_sep_space(sql);

    int i = 0; //debugging

    if(*sql='(') {
        sql++;
    }

    while (continue_parsing)
    {
        i++;
        if (has_reached_sql_end(sql)) {
            continue_parsing = false;
        }else{
            sql= get_field_name(sql, field);
            if (is_int(field)) {
                result->fields[result->fields_count].field_type = TYPE_INTEGER;
                result->fields[result->fields_count].field_value.int_value = atoi(field);
            }else if (is_float(field)) {
                result->fields[result->fields_count].field_type = TYPE_FLOAT;
                result->fields[result->fields_count].field_value.float_value = atof(field);
            }else{
                result->fields[result->fields_count].field_type = TYPE_TEXT;
                strcpy(result->fields[result->fields_count].field_value.text_value, field);
            }
            result->fields_count++;
            if (get_sep_space_and_char(sql, ',') != NULL && get_sep_space_and_char(sql, ')') == NULL) {
                sql = get_sep_space_and_char(sql, ',');
            } else {
                continue_parsing = false;
            }
        }
        

    }
    
    if(get_sep_space_and_char(sql, ')') != NULL) {
        sql = get_sep_space_and_char(sql, ')');
    }

    return sql;
    

}

/**
 * @brief Get a definition of a table.
 * @example "(id primary key, name text, age float);" -> ";" and result->fields_count = 3 result->definitions[0] = TYPE_PRIMARY_KEY,"id" result->definitions[1] = TYPE_TEXT,"name" result->definitions[2] = TYPE_FLOAT,"age"
 * 
 * @param sql Pointer to a position in the sql query.
 * @param result Pointer to the table definition structure to modificate.
 * @return char* Pointer to the position in the query after the table definition.
 */
char *parse_create_fields_list(char *sql, table_definition_t *result) {
    sql = get_sep_space(sql);
    if (*sql != '(') {
        return NULL;
    }
    sql++;
    while (*sql != ')') {
        sql = get_field_name(sql, result->definitions[result->fields_count].column_name);
        if (sql == NULL) {
            return NULL;
        }else{
            sql = get_sep_space(sql);
            if (get_keyword(sql, "primary key") != NULL) {
                result->definitions[result->fields_count].column_type = TYPE_PRIMARY_KEY;
                sql = get_keyword(sql, "primary key");
                sql = get_sep_space(sql);
            }else if (get_keyword(sql, "text") != NULL) {
                result->definitions[result->fields_count].column_type = TYPE_TEXT;
                sql = get_keyword(sql, "text");
                sql = get_sep_space(sql);
            }else if (get_keyword(sql, "float") != NULL) {
                result->definitions[result->fields_count].column_type = TYPE_FLOAT;
                sql = get_keyword(sql, "float");
                sql = get_sep_space(sql);
            }else if (get_keyword(sql, "int") != NULL) {
                result->definitions[result->fields_count].column_type = TYPE_INTEGER;
                sql = get_keyword(sql, "int");
                sql = get_sep_space(sql);
            }else{
                return NULL;
            }
        }
        result->fields_count++;
        if (get_sep_space_and_char(sql, ',') != NULL ) {
            sql = get_sep_space_and_char(sql, ',');
        } else if (*sql != ')') {
            return NULL;
        }         
    }
    return ++sql;
}

/**
 * @brief function that extract an equality condition from a sql query.
 * 
 * @param sql Pointer to a position in the sql query.
 * @param equality Pointer to the equality condition structure to modificate.
 * @return char* Pointer to the position in the query after the equality condition.
 */
char *parse_equality(char *sql, field_record_t *equality) {

    char field1[TEXT_LENGTH], field2[TEXT_LENGTH];

    if (has_reached_sql_end(sql)) {
        return NULL;
    }

    sql = get_field_name(sql, field1);
    if (sql==NULL) {
        return NULL;
    }
    sql = get_sep_space(sql);

    if (get_keyword(sql, "=") != NULL) {
        sql = get_keyword(sql, "=");
        sql = get_sep_space(sql);
    }

    sql = get_field_name(sql, field2);
    if (sql==NULL) {
        return NULL;
    } 
    sql = get_sep_space(sql);

    strcpy(equality->field_value.text_value, field1);
    strcat(equality->field_value.text_value, "=");
    strcat(equality->field_value.text_value, field2);

    equality->field_type = TYPE_UNKNOWN;

    return sql;
}

/**
 * @brief extract a condition set from a sql query wich is a list of equality conditions.
 * 
 * @param sql Pointer to a position in the sql query.
 * @param result Pointer to the condition structure to modificate.
 * @return char* Pointer to the position in the query after the condition.
 */
char *parse_set_clause(char *sql, table_record_t *result) {
    if (has_reached_sql_end(sql)) {
        return NULL;
    }
    sql = get_sep_space(sql);

    while (!has_reached_sql_end(sql)) {
        sql = parse_equality(sql, &result->fields[result->fields_count]);
        if (sql == NULL) {
            return NULL;
        }
        result->fields_count++;
        if (get_sep_space_and_char(sql, ',') != NULL) {
            sql = get_sep_space_and_char(sql, ',');
        } else {
            return sql;
        }
    }
    return sql;    
}

/**
 * @brief exract a condition where from a sql query wich is a list of equality conditions.
 * 
 * @param sql Pointer to a position in the sql query.
 * @param filter Pointer to the condition structure to modificate.
 * @return char* Pointer to the position in the query after the condition.
 */
char *parse_where_clause(char *sql, filter_t *filter) {
    if (has_reached_sql_end(sql)) {
        return NULL;
    }

    while (!has_reached_sql_end(sql)){
        sql = get_sep_space(sql);
        
        sql = parse_equality(sql, &filter->values.fields[filter->values.fields_count]);
        filter->values.fields_count++;

        if (has_reached_sql_end(sql)) {
            return NULL;
        }else if (get_keyword(sql, "AND") != NULL) {
            
            sql = get_keyword(sql, "AND");
            
            if(filter->logic_operator == OP_OR && filter->values.fields_count > 1){
                filter->logic_operator = OP_ERROR;
            } else {
                filter->logic_operator = OP_AND;
            }
        } else if(get_keyword(sql, "OR") != NULL){
            
            sql = get_keyword(sql, "AND");
            
            if(filter->logic_operator == OP_AND && filter->values.fields_count > 1){
                filter->logic_operator = OP_ERROR;
            } else {
                filter->logic_operator = OP_OR;
            }

        }else{
            return NULL;
        }
    }
    
    return sql;
}

/**
 * @brief fuction that extract the data from a sql query.
 * 
 * @param sql Pointer to the sql query.
 * @param result Pointer to the data structure to modificate.
 * @return query_result_t*  Return the data of the query
 */
query_result_t *parse(char *sql, query_result_t *result) {
    if(sql == NULL) {
        return NULL;
    }
    if(has_reached_sql_end(sql)){
        return NULL;
    }

    sql= get_sep_space(sql);

    if (get_keyword(sql, "create table") != NULL) {
        parse_create(get_keyword(sql, "create table"), result);
    }else if (get_keyword(sql, "insert") != NULL) {
        parse_insert(get_keyword(sql, "insert"), result);
    }else if (get_keyword(sql, "select") != NULL) {
        parse_select(get_keyword(sql, "select"), result);
    }else if (get_keyword(sql, "update") != NULL) {
        parse_update(get_keyword(sql, "update"), result);
    }else if (get_keyword(sql, "delete") != NULL) {
        parse_delete(get_keyword(sql, "delete"), result);
    }else if (get_keyword(sql, "drop") != NULL) {
        sql=get_keyword(sql, "drop");
        sql= get_sep_space(sql);
        if (get_keyword(sql, "table") != NULL) {
            parse_drop_table(get_keyword(sql, "table"), result);
        }else if (get_keyword(sql, "database") != NULL) {
            parse_drop_db(get_keyword(sql, "database"), result);
        }else if (get_keyword(sql, "db") != NULL) {
            parse_drop_db(get_keyword(sql, "db"), result);
        }else{
            return NULL;
        }

    }else{
        return NULL;
    }
    return result;
}

/**
 * @brief function that extract the data from a sql select query.
 * 
 * @param sql Pointer to the sql select query without SELECT.
 * @param result Point to the data structure to modificate.
 * @return query_result_t* Return the data of the query
 */
query_result_t *parse_select(char *sql, query_result_t *result) {

    char *table_name[TEXT_LENGTH];

    //Get the colomns names
    result->query_type = QUERY_SELECT;
    if (has_reached_sql_end(sql)) {
        return NULL;
    }
    sql = get_sep_space(sql);
    if(get_keyword(sql, "*") != NULL){
        result->query_content.select_query.set_clause.fields_count = 1;
        result->query_content.select_query.set_clause.fields[0].field_type = TYPE_UNKNOWN;
        strcpy(result->query_content.select_query.set_clause.fields[0].field_value.text_value, "*");
    }else{
        sql = parse_fields_or_values_list(sql, &result->query_content.select_query.set_clause);
    }
    if (has_reached_sql_end(sql)) {
        return NULL;
    }
    sql = get_sep_space(sql);

    //Get the table name
    if (get_keyword(sql, "from") != NULL) {

        sql = get_keyword(sql, "from");
        sql = get_sep_space(sql);

        if (get_field_name(sql, table_name) != NULL) {
            sql = get_field_name(sql, table_name);
            strcpy(result->query_content.select_query.table_name, table_name);
        } else {
            return NULL;
        }
        
        strcpy(result->query_content.select_query.table_name, table_name);

    } else {
        return NULL;
    }

    //Get a where clause
    if(has_reached_sql_end(sql)){
        return result;
    }else if(get_keyword(sql, "where") != NULL){
        
        if(has_reached_sql_end(sql)){
            return NULL;
        }
        sql = get_keyword(sql, "where");
        sql = get_sep_space(sql);
        sql = parse_where_clause(sql, &result->query_content.select_query.where_clause);

    }else{
        return NULL;
    }

    return result;
}

/**
 * @brief function that extract the data from a sql create query.
 * 
 * @param sql Pointer to the sql create query without CREATE TABLE.
 * @param result Pointer to the data structure to modificate.
 * @return query_result_t* Return the data of the query
 */
query_result_t *parse_create(char *sql, query_result_t *result) {
    result->query_type = QUERY_CREATE_TABLE;
    if (has_reached_sql_end(sql)) {
        return NULL;
    }
    sql = get_field_name(sql, result->query_content.create_query.table_name);

    if (has_reached_sql_end(sql)) {
        return NULL;
    }
    sql = parse_create_fields_list(sql, &result->query_content.create_query.table_definition);
    if (has_reached_sql_end(sql)) {
        return result;
    }else{
        return NULL;
    }

}

/**
 * @brief function that extract the data from a sql insert query.
 * 
 * @param sql Pointer to the sql insert query without INSERT.
 * @param result Pointer to the data structure to modificate.
 * @return query_result_t* Return the data of the query
 */
query_result_t *parse_insert(char *sql, query_result_t *result) {
    char *table_name[TEXT_LENGTH];
    result->query_type = QUERY_INSERT;
    if (has_reached_sql_end(sql)) {
        return NULL;
    }
    sql = get_keyword(sql, "into");
    if (has_reached_sql_end(sql)) {
        return NULL;
    }
    sql= get_field_name(sql, result->query_content.insert_query.table_name);
    if (has_reached_sql_end(sql)) {
        return NULL;
    }
    sql = parse_fields_or_values_list(sql, &result->query_content.insert_query.fields_names);
    if (has_reached_sql_end(sql)) {
        return NULL;
    }
    sql = get_keyword(sql, "values");
    if (has_reached_sql_end(sql)) {
        return NULL;
    }
    sql = parse_fields_or_values_list(sql, &result->query_content.insert_query.fields_values);
    if (has_reached_sql_end(sql)) {
        return result;
    }else{
        return NULL;
    }
}

/**
 * @brief function that extract the data from a sql update query.
 * 
 * @param sql Pointer to the sql update query without UPDATE.
 * @param result Pointer to the data structure to modificate.
 * @return query_result_t* Return the data of the query
 */
query_result_t *parse_update(char *sql, query_result_t *result) {
    //TODO
    return NULL;
}

/**
 * @brief function that extract the data from a sql delete query.
 * 
 * @param sql Pointer to the sql delete query without DELETE.
 * @param result Pointer to the data structure to modificate.
 * @return query_result_t* Return the data of the query
 */
query_result_t *parse_delete(char *sql, query_result_t *result) {
    //TODO
    return NULL;
}

/**
 * @brief function that extract the data from a sql drop query.
 * 
 * @param sql Pointer to the sql drop query without DROP DB/DROP DATABASE.
 * @param result Pointer to the data structure to modificate.
 * @return query_result_t* Return the data of the query
 */
query_result_t *parse_drop_db(char *sql, query_result_t *result) {
    result->query_type = QUERY_DROP_DB;
    if (has_reached_sql_end(sql)) {
        return NULL;
    }
    sql = get_field_name(sql, result->query_content.database_name);
    if (has_reached_sql_end(sql)) {
        return result;
    }else{
        return NULL;
    }
}

/**
 * @brief function that extract the data from a sql drop table query.
 * 
 * @param sql Pointer to the sql drop table query without DROP TABLE.
 * @param result Pointer to the data structure to modificate.
 * @return query_result_t* Return the data of the query
 */
query_result_t *parse_drop_table(char *sql, query_result_t *result) {
    result->query_type = QUERY_DROP_TABLE;
    if (has_reached_sql_end(sql)) {
        return NULL;
    }

    sql = get_field_name(sql, result->query_content.table_name);
    if (has_reached_sql_end(sql)) {
        return result;
    }else{
        return NULL;
    }
    
}
