//
// Created by flassabe on 19/11/2021.
//

#include "table.h"

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

/*!
 * @brief function open_definition_file opens the table key file
 * @param table_name the name of the table whose definition file must be opened
 * @param mode the opening mode (as in fopen)
 * @return and pointer to a FILE type, resulting from the fopen function
 */
FILE *open_definition_file(char *table_name, char *mode) {
    char file_path[TEXT_LENGTH];
    sprintf(file_path, "%s/%s.def", table_name, table_name);
    FILE *file = fopen(file_path, mode);
    if (file){
        return file;
    }
    return NULL;
}

/*!
 * @brief function open_index_file opens the table key file
 * @param table_name the name of the table whose index file must be opened
 * @param mode the opening mode (as in fopen)
 * @return and pointer to a FILE type, resulting from the fopen function
 */
FILE *open_index_file(char *table_name, char *mode) {
    char file_path[TEXT_LENGTH];
    sprintf(file_path, "%s/%s.idx", table_name, table_name);
    FILE *file = fopen(file_path, mode);
    if (file){
        return file;
    }
    return NULL;
}

/*!
 * @brief function open_content_file opens the table key file
 * @param table_name the name of the table whose content file must be opened
 * @param mode the opening mode (as in fopen)
 * @return and pointer to a FILE type, resulting from the fopen function
 */
FILE *open_content_file(char *table_name, char *mode) {
    char file_path[TEXT_LENGTH];
    sprintf(file_path, "%s/%s.data", table_name, table_name);
    FILE *file = fopen(file_path, mode);
    if (file){
        return file;
    }
    return NULL;
}

/*!
 * @brief function open_key_file opens the table key file
 * @param table_name the name of the table whose key file must be opened
 * @param mode the opening mode (as in fopen)
 * @return and pointer to a FILE type, resulting from the fopen function
 */
FILE *open_key_file(char *table_name, char *mode) {
    char file_path[TEXT_LENGTH];
    sprintf(file_path, "%s/%s.key", table_name, table_name);
    FILE *file = fopen(file_path, mode);
    if (file){
        return file;
    }
    return NULL;
}

/*!
 * @brief function create_table creates the files for a new table. It checks if the table doesn't already exist,
 * creates its directory, its definition file, whose content is written based on the definition. It creates both
 * index and content empty files, and if there is a primary key, it creates the primary key file with a stored value
 * of 1.
 * @param table_definition a pointer to the definition of the new table
 */
void create_table(create_query_t *table_definition) {
    if (!directory_exists(table_definition->table_name)){
        mkdir(table_definition->table_name, S_IRWXU);

        char path_file_extension[TEXT_LENGTH];
        char path_file[TEXT_LENGTH];

        //path to folder
        sprintf(path_file,"%s/%s", table_definition->table_name, table_definition->table_name);

        //creation of table_name/table_name.def
        sprintf(path_file_extension, "%s.def",path_file);
        FILE *def = fopen(path_file_extension, "w");

        for (int field = 0; field < table_definition->table_definition.fields_count; ++field) {
            //filling def file
            fprintf(def, "%u %s\n", table_definition->table_definition.definitions[field].column_type,
                    table_definition->table_definition.definitions[field].column_name);

            //creation of key table_name/table.key if needed
            if (table_definition->table_definition.definitions[field].column_type == TYPE_PRIMARY_KEY) {
                unsigned long long key_value=1;
                sprintf(path_file_extension, "%s.key", path_file);
                FILE *key = fopen(path_file_extension, "wb");
                fwrite(&key_value, sizeof(unsigned long long), 1, key);
                fclose(key);
            }
        }
        fclose(def);

        //creation of table_name/table_name.idx
        sprintf(path_file_extension, "%s.idx", path_file);
        FILE *idx = fopen(path_file_extension, "a");
        fclose(idx);

        //creation of table_name/table_name.data
        sprintf(path_file_extension, "%s.data", path_file);
        FILE *data = fopen(path_file_extension, "w");
        fclose(data);

    }else {
        printf("table already exists \n");
    }
}

/*!
 * @brief function drop_table removes all files and directory related to a table
 * @param table_name the name of the dropped table.
 */
void drop_table(char *table_name) {
    DIR *const directory = opendir(table_name);
    if (directory) {
        struct dirent *entry;
        while ((entry = readdir(directory))) {
            if (!strcmp(".", entry->d_name) || !strcmp("..", entry->d_name)) {
                continue;
            }
            char filename[strlen(table_name) + strlen(entry->d_name) + 2];
            sprintf(filename, "%s/%s", table_name, entry->d_name);
            remove(filename);
        }
        closedir(directory);
        rmdir(table_name);
    }
}

/*!
 * @brief function get_table_definition reads and returns a table definition
 * @param table_name the name of the target table
 * @param result a pointer to a previously allocated table_definition_t structure where the result will be stored.
 * @return the pointer to result, NULL if the function failed
 */
table_definition_t *get_table_definition(char *table_name, table_definition_t *result) {
    FILE *file = open_definition_file(table_name, "r");
    int field_count=0;
    char field_name[TEXT_LENGTH];
    char field_type[1];
    if (file) {
        while (fscanf(file, "%s %s", field_type, field_name) != EOF) {
            strcpy(result->definitions[field_count].column_name, field_name);
            result->definitions[field_count].column_type = field_type[0] - '0';

            ++field_count;
        }
        result->fields_count = field_count;
        return result;
    }
    return NULL;
}

/*!
 * @brief function compute_record_length computed the in-memory length of a table record. It sums all fields sizes
 * by their respective sizes. Text is always stored on TEXT_LENGTH chars so a text field in-memory size is always
 * TEXT_LENGTH, whatever its actual content strlen.
 * @param definition the table definition whose record length we need
 * @return the value of the record length.
 */
uint16_t compute_record_length(table_definition_t *definition) {
    uint16_t length = 0;
    for (int field_count = 0; field_count < definition->fields_count; ++field_count) {
        if (definition->definitions[field_count].column_type == TYPE_TEXT){
            length += 150;
        }else {
            length += 8;
        }
    }
    return length;
}

/*!
 * @brief function find_first_free_record finds the first free record in the index file. The first free record is:
 * - the first index_record_t occurrence in the index file whose active field is 0. Its active field must be put to 1
 * before ending the function (replace value inplace)
 * - if none exists: a new field_record_t added to the end of the file, with active set to 1, and others fields to 0.
 * @param table_name the name of the target table
 * @return the offset of the free index in the index file.
 */
uint32_t find_first_free_record(char *table_name) {
    index_record_t index_record = {.record_offset = 0, .record_length = 0};
    uint8_t activate = 1;
    uint8_t empty = 0;

    FILE *idx = open_index_file(table_name, "rb+");

    //read till end of file
    while (fread(&index_record.is_active, sizeof(uint8_t), 1, idx) == 1){
        //get offset
        fread(&index_record.record_offset, sizeof(uint32_t), 1, idx);

        //if free record found
        if (!index_record.is_active){
            fseek(idx, -5, SEEK_CUR);
            fwrite(&activate, sizeof(uint8_t), 1, idx);

            fclose(idx);
            return index_record.record_offset;
        }
        fread(&index_record.record_length, sizeof(uint16_t), 1, idx);

        //skip new line(1)
        fseek(idx, 1, SEEK_CUR);
    }

    //if no free record found create a new one -> "1000000\n"
    fwrite(&activate, sizeof(uint8_t), 1, idx);
    fwrite(&empty, sizeof(uint8_t), 6, idx);
    fwrite("\n", sizeof(char), 1, idx);

    fclose(idx);
    return index_record.record_offset + index_record.record_length;
}

/*!
 * @brief function add_row_to_table adds a row to a table. Relies on format_row, compute_record_length,
 * find_first_free_record, get_table_definition, open_content_file, open_index_file
 * @param table_name the name of the target table
 * @param record the record to add
 */
void add_row_to_table(char *table_name, table_record_t *record) {
    table_definition_t table_definition;
    index_record_t index_record;

    //get table_def, its size and a free record offset
    get_table_definition(table_name, &table_definition);
    index_record.record_length = compute_record_length(&table_definition);
    index_record.record_offset = find_first_free_record(table_name);

    //create buffer, empty it and format it
    char buffer[index_record.record_length];
    strcpy(buffer, "");

    if (format_row(buffer, &table_definition, record)) {
        FILE *data = open_content_file(table_name, "r+");
        if (data) {
            //go to offset and write buffer
            fseek(data, index_record.record_offset, SEEK_SET);
            fprintf(data, "%s", buffer);
            fclose(data);

            FILE *idx = open_index_file(table_name, "rb+");
            if (idx) {
                uint16_t last_size;

                fseek(idx, -3, SEEK_END);
                fread(&last_size, sizeof(uint16_t), 1, idx);

                //complete if new record was created
                if (last_size == 0) {
                    fseek(idx, -6, SEEK_CUR);
                    fwrite(&index_record.record_offset, sizeof(uint32_t), 1, idx);
                    fwrite(&index_record.record_length, sizeof(uint16_t), 1, idx);
                }
                fclose(idx);
            }else{
                printf("ERROR: could not open index file\n");
            }
        }else {
            printf("ERROR: could not open data file\n");
        }
    }else {
        printf("could not convert to buffer\n");
    }
}

/*!
 * @brief function format_row prepares the buffer to be written to the content file
 * @param table_name the name of the target table
 * @param buffer the buffer where the row content is prepared
 * @param table_definition the table definition for fields ordering and formatting
 * @param record the record to write
 * @return a pointer to buffer in case of success, NULL else.
 */
char *format_row(char *buffer, table_definition_t *table_definition, table_record_t *record) {
    for (int field = 0; field < table_definition->fields_count; ++field) {
        field_record_t *field_record = find_field_in_table_record(table_definition->definitions[field].column_name, record);
        switch (field_record->field_type) {
            case TYPE_PRIMARY_KEY:
                sprintf(buffer, "%s%8llo", buffer, field_record->field_value.primary_key_value);
                break;
            case TYPE_INTEGER:
                sprintf(buffer, "%s%8llo", buffer, field_record->field_value.int_value);
                break;
            case TYPE_FLOAT:
                sprintf(buffer, "%s%8of", buffer, field_record->field_value.float_value);
                break;
            case TYPE_TEXT:
                sprintf(buffer, "%s%s", buffer, field_record->field_value.text_value);
                break;
        }

    }
    return buffer;
}

/*!
 * @brief function update_key updates the key value in the key file. Key value is updated if and only if the new value
 * if higher than the stored value. The value sent to the function is the last value inserted into the table, so the
 * function must increment it before comparing to the key file content.
 * @param table_name the name of the table whose key file must be updated
 * @param value the new key value
 */
void update_key(char *table_name, unsigned long long value) {
    ++value;
    unsigned long long file_value = get_next_key(table_name);
    FILE *key;
    key = open_key_file(table_name, "wb");
    fwrite(file_value > value ? &file_value : &value, sizeof(unsigned long long), 1, key);
    fclose(key);
}

/*!
 * @brief function get_next_key extract the next key value from a table key file.
 * @param table_name the name of the table whose key value we need.
 * @return the next value of the key if it exists, 0 else
 */
unsigned long long get_next_key(char *table_name) {
    FILE *key = open_key_file(table_name, "rb");
    if (key){
        unsigned long long key_value;
        fread(&key_value, sizeof(unsigned long long), 1, key);
        fclose(key);
        return key_value;
    }
    return 0;
}

/*!
 * @brief function find_field_in_table_record looks up for a field by its name in a table_record_t structure
 * @param field_name the name of the field to look for
 * @param record a pointer to the table_record_t structure
 * @return a pointer to the matching field_record_t if found, NULL else
 */
field_record_t *find_field_in_table_record(char *field_name, table_record_t *record) {
    for (int field = 0; field < record->fields_count; ++field) {
        if (strcmp(field_name, record->fields[field].column_name) == 0){
            return &record->fields[field];
        }
    }
    return NULL; // Checking the query shall avoid this
}

/*!
 * @brief function is_matching_filter tests a table record against a filter to check if the record matches the filter,
 * i.e. conditions in the filter are verified by the record. The filter contains one or more equalities to check.
 * Tests are all following a single AND or OR logical operator.
 * @param record the record to be checked
 * @param filter the filter to check against (a NULL filter means no filtering must be done -> returns 0)
 * @return true if the record matches the filter, false else
 */
bool is_matching_filter(table_record_t *record, filter_t *filter) {
    //if filter is null
    if (filter == NULL){
        return 0;
    }

    bool match;
    for (int filter_field = 0; filter_field < filter->values.fields_count; ++filter_field) {
        for (int record_field = 0; record_field < record->fields_count; ++record_field) {
            //if same name and same type: check for same values
            if (strcmp(filter->values.fields[filter_field].column_name, record->fields[record_field].column_name) == 0
            && filter->values.fields[filter_field].field_type == record->fields[record_field].field_type) {
                //check for same value
                switch (filter->values.fields[filter_field].field_type) {

                    case TYPE_PRIMARY_KEY:
                        if (filter->values.fields[filter_field].field_value.primary_key_value ==
                            record->fields[record_field].field_value.primary_key_value) {
                            match = true;
                            record_field = record->fields_count;
                        } else {
                            match = false;
                        }
                        break;
                    case TYPE_INTEGER:
                        if (filter->values.fields[filter_field].field_value.int_value ==
                            record->fields[record_field].field_value.int_value) {
                            match = true;
                            record_field = record->fields_count;
                        } else {
                            match = false;
                        }
                        break;
                    case TYPE_FLOAT:
                        if (filter->values.fields[filter_field].field_value.float_value ==
                            record->fields[record_field].field_value.float_value) {
                            match = true;
                            record_field = record->fields_count;
                        } else {
                            match = false;
                        }
                        break;
                    case TYPE_TEXT:
                        if (strcmp(filter->values.fields[filter_field].field_value.text_value,
                                   record->fields[record_field].field_value.text_value) == 0) {
                            match = true;
                            record_field = record->fields_count;
                        } else {
                            match = false;
                        }
                        break;
                }
            } else {
                match = false;
            }
        }
        if (filter->logic_operator == OP_OR && match == true){
            return true;
        }else if (filter->logic_operator == OP_AND && match == false){
            return false;
        }
    }
    if (filter->logic_operator == OP_OR){
        return false;
    }else if (filter->logic_operator == OP_AND){
        return true;
    }
}

/*!
 * @brief function get_filtered_records gets a list (as a linked list) of table records limited to the fields
 * specified to the function and filtered by the specified WHERE clause.
 * @param table_name table from which to fetch data
 * @param required_fields the list of fields to be returned (cannot be NULL)
 * @param filter the WHERE clause filter. NULL if no filter must be applied
 * @param result a pointer to a previously allocated record_list_t. List shall be empty
 * @return a pointer to the first element of the resulting linked list. Shall be freed by the calling function
 */
record_list_t *get_filtered_records(char *table_name, table_record_t *required_fields, filter_t *filter, record_list_t *result) {
    return result;
}

/*!
 * @brief function get_table_record reads a table record from its content file
 * @param table_name the table whose record must be read from
 * @param offset the position of the beginning of the record in the content file (found in the index file)
 * @param def the table definition
 * @param result a pointer to a previously allocated table_record_t structure
 * @return the pointer to to result if succeeded, NULL else.
 */
table_record_t *get_table_record(char *table_name, uint32_t offset, table_definition_t *def, table_record_t *result) {
    return result;
}
