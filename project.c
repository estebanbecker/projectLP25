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

    
    char buffer[SQL_COMMAND_MAX_SIZE];
    do {
        printf("> ");
        fflush(stdin);
        if (fgets(buffer, SQL_COMMAND_MAX_SIZE, stdin) == NULL)
            continue;
        buffer[strlen(buffer)-1] = '\0';
        if (strcmp(buffer, "exit") == 0)
            break;
        
        printf("%s\n", buffer);
        table_record_t *record = NULL;
        record = (table_record_t *)malloc(sizeof(table_record_t));

        parse_fields_or_values_list(buffer, record);

        printf("fini");
        
        // Here: parse SQL, check query, execute query
    } while (true);

    return 0;
}
