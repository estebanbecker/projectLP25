//
// Created by flassabe on 16/11/2021.
//

#include "utils.h"

#include <dirent.h>

/*!
 *  \brief make_full_path concatenates path and basename and returns the result
 *  \param path the path to the database directory basename, can be NULL (i.e. path is current directory)
 *  Path may end with '/' but it is not required.
 *  \param basename the database name.
 *
 *  \return a pointer to the full path. Its content must be freed by make_full_path caller.
 */
char *make_full_path(char *path, char *basename) {
    return NULL;
}

bool directory_exists(char *path) {
    DIR *my_dir = opendir(path);
    if (my_dir) {
        closedir(my_dir);
        return true;
    }
    return false;
}
