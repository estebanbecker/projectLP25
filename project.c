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

    char text[150] = "test";
    create_query_t *test = NULL;
    strcpy(test->table_name, text);
    create_table(test);

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
