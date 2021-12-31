#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#include "utils.h"
#include "database.h"
#include "sql.h"
#include "table.h"
#include "check.h"
#include "record_list.h"
#include "expand.h"


#define SQL_COMMAND_MAX_SIZE 1500

int main(int argc, char *argv[]) {

    // Here: check parameters with getopt
    // -d database_name
    // -l directory of the parent database folder (default: current directory)

    char buffer[SQL_COMMAND_MAX_SIZE];
    query_result_t query;
    
    do {
        printf("> ");
        fflush(stdin);
        if (fgets(buffer, SQL_COMMAND_MAX_SIZE, stdin) == NULL)
            continue;
        buffer[strlen(buffer)-1] = '\0';
        if (strcmp(buffer, "exit") == 0)
            break;
        
        printf("%s\n", buffer);

        if(parse(buffer, &query) == NULL) {
            continue;
        }
        //here check if the query is valid

        expand(&query);

        //here execute the query


    } while (true);

    
    return 0;
}
