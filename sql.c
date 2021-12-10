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
    while(isspace(*sql)) {
        sql++;
    }
    if(*sql == c) {
        sql++;
    }else{
        return NULL;
    }
    while(isspace(*sql)) {
        sql++;
    }
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
    while(valid && *keyword != '\0') {
        if(tolower(*sql) != tolower(*keyword)) {
            valid = 0;
        }
        sql++;
        keyword++;
    }

    if(valid) {
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

    if(*sql== '\'') {
        i++;
        sql++;
        while(*sql != '\'') {
            field_name[i-1] = sql[i];
            i++;
            sql++;
        }
        field_name[i-1] = '\0';
        return sql++;
    }else{
        while(!isspace(*sql)) {
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
 */
char *parse_fields_or_values_list(char *sql, table_record_t *result) {
    char *cursor = get_sep_space(sql);
    char field[TEXT_LENGTH];
    bool continue_parsing = true;

    while (continue_parsing)
    {
        if(has_reached_sql_end(cursor)) {
            continue_parsing = false;
        }else if(get_keyword(cursor, "where") != NULL) {
            continue_parsing = false;
        }
    }
    
    //TO COMPLETE
    

}

/**
 * @brief Get a definition of a table.
 * @example "id integer primary key, name text, age float)" -> ")" and result->fields_count = 3 result->definitions[0] = TYPE_PRIMARY_KEY,"id" result->definitions[1] = TYPE_TEXT,"name" result->definitions[2] = TYPE_FLOAT,"age"
 * 
 * @param sql Pointer to a position in the sql query.
 * @param result Pointer to the table definition structure to modificate.
 * @return char* Pointer to the position in the query after the table definition.
 */
char *parse_create_fields_list(char *sql, table_definition_t *result) {
    
    return sql;
}

/**
 * @brief function that extract an equality condition from a sql query.
 * 
 * @param sql Pointer to a position in the sql query.
 * @param equality Pointer to the equality condition structure to modificate.
 * @return char* Pointer to the position in the query after the equality condition.
 */
char *parse_equality(char *sql, field_record_t *equality) {
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
    return NULL;
}

/**
 * @brief function that extract the data from a sql select query.
 * 
 * @param sql Pointer to the sql select query without SELECT.
 * @param result Point to the data structure to modificate.
 * @return query_result_t* Return the data of the query
 */
query_result_t *parse_select(char *sql, query_result_t *result) {
    return NULL;
}

/**
 * @brief function that extract the data from a sql create query.
 * 
 * @param sql Pointer to the sql create query without CREATE.
 * @param result Pointer to the data structure to modificate.
 * @return query_result_t* Return the data of the query
 */
query_result_t *parse_create(char *sql, query_result_t *result) {
    return NULL;
}

/**
 * @brief function that extract the data from a sql insert query.
 * 
 * @param sql Pointer to the sql insert query without INSERT.
 * @param result Pointer to the data structure to modificate.
 * @return query_result_t* Return the data of the query
 */
query_result_t *parse_insert(char *sql, query_result_t *result) {
    return NULL;
}

/**
 * @brief function that extract the data from a sql update query.
 * 
 * @param sql Pointer to the sql update query without UPDATE.
 * @param result Pointer to the data structure to modificate.
 * @return query_result_t* Return the data of the query
 */
query_result_t *parse_update(char *sql, query_result_t *result) {
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
    return NULL;
}

/**
 * @brief function that extract the data from a sql drop query.
 * 
 * @param sql Pointer to the sql drop query without DROP.
 * @param result Pointer to the data structure to modificate.
 * @return query_result_t* Return the data of the query
 */
query_result_t *parse_drop_db(char *sql, query_result_t *result) {
    return NULL;
}

/**
 * @brief function that extract the data from a sql drop table query.
 * 
 * @param sql Pointer to the sql drop table query without DROP TABLE.
 * @param result Pointer to the data structure to modificate.
 * @return query_result_t* Return the data of the query
 */
query_result_t *parse_drop_table(char *sql, query_result_t *result) {
    return NULL;
}
