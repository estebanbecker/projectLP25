#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "query_exec.h"
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
    
    int opt;
    char *database_name = NULL;
    char *database_path = NULL;


    while ((opt = getopt(argc, argv, "d:l:")) != -1) {
        switch (opt) {
            case 'd':
                database_name = optarg;
                break;
            case 'l':
                database_path = optarg;
                break;
            default:
                printf("Usage: %s -d database_name -l database_path\n", argv[0]);
                return 1;
        }
    }

    if (database_name == NULL) {
        printf("Usage: %s -d database_name -l database_path\n", argv[0]);
        return 1;
    }
    if(database_path != NULL){

        printf("Error: database path is not valid\n");
        return 1;

    }

    create_db_directory(database_name);

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
        if(!check_query(&query)){
            continue;
        }

        expand(&query);

        execute(&query);


    } while (true);

    
    return 0;
}
