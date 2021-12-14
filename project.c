#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#include "utils.h"
#include "database.h"
#include "sql.h"
#include "table.h"
#include "check.h"

#define SQL_COMMAND_MAX_SIZE 1500

int main(int argc, char *argv[]) {

    // Here: check parameters with getopt
    // -d database_name
    // -l directory of the parent database folder (default: current directory)
    create_query_t table;
    strcpy(table.table_name, "test");
    table.table_definition.fields_count=2;
    strcpy(table.table_definition.definitions[0].column_name, "field");
    strcpy(table.table_definition.definitions[1].column_name, "field2");
    table.table_definition.definitions[0].column_type = TYPE_INTEGER;
    table.table_definition.definitions[1].column_type = TYPE_PRIMARY_KEY;
    create_query_t *ptable = &table;
    int size;

    create_table(ptable);

    table_definition_t result_table;
    table_definition_t *result_table_ptr;
    result_table_ptr = get_table_definition("test", &result_table);
    size = compute_record_length(result_table_ptr);
    printf("%d\n", size);

    char buffer[SQL_COMMAND_MAX_SIZE];
    do {
        printf("> ");
        fflush(stdin);
        if (fgets(buffer, SQL_COMMAND_MAX_SIZE, stdin) == NULL)
            continue;
        buffer[strlen(buffer)-1] = '\0';
        if (strcmp(buffer, "exit") == 0)
            break;
        // Here: parse SQL, check query, execute query
    } while (true);

    return 0;
}
