#include "string.h"
#include "filehelper.h"

void ChangeFileType(char* file_name, const char* dest_file_type, int size){

    char* dot_position = strchr(file_name, '.');
    if(dot_position == NULL){
        dot_position = file_name + strlen(file_name);
    }

    int file_name_length = strlen(file_name);
    (void)file_name_length;

    int file_type_len = strlen(dest_file_type);
    if((dot_position + file_type_len) - file_name < size){
        strcpy(dot_position, dest_file_type);
        dot_position[file_type_len] = '\0';
    }else {
        strcpy(file_name + size - file_type_len, dest_file_type);
        file_name[size] = '\0';
    }

}