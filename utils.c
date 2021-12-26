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
    
    if(basename){
        if(path){

            if(path[strlen(path)-1] != '/'){ //verify if path ends with '/'
                if(strlen(path)+1 < PATH_MAX){
                    sprintf(path, "%s/", path); //append '/' at the end of the path
                } else {
                    return NULL;
                }
            }

            if(directory_exists(path)){ //verify is path exists
                if(strlen(path)+strlen(basename) < PATH_MAX){ //verify that the full path is of correct size
                return strcat(path, basename);
                } else {
                    return NULL;
                }
            }

        } else {
            return basename; //if path is NULL then path is current directory
        }
    } else { 
        return NULL; //basename can't be NULL
    }
}

bool directory_exists(char *path) {
    DIR *my_dir = opendir(path);
    if (my_dir) {
        closedir(my_dir);
        return true;
    }
    return false;
}
