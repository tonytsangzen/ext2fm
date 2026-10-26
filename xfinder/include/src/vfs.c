#include <stdio.h>
#include <path.h>

void* vfs_readfile(const char* filename, int* size){
    if (filename == NULL || size == NULL) {
        return NULL;
    }

    char full_path[1024];
    if (filename[0] != '/') {
        const char* exe_path = get_executable_path();
        if (exe_path == NULL) {
            return NULL;
        }
        snprintf(full_path, sizeof(full_path), "%s/%s", exe_path, filename);
    } else {
        strncpy(full_path, filename, sizeof(full_path) - 1);
        full_path[sizeof(full_path) - 1] = '\0';
    }

    FILE* file = fopen(full_path, "rb");
    if (file == NULL) {
        *size = 0;
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        *size = 0;
        return NULL;
    }

    long file_size = ftell(file);
    if (file_size == -1) {
        fclose(file);
        *size = 0;
        return NULL;
    }

    rewind(file);

    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        fclose(file);
        *size = 0;
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);
    if (bytes_read != (size_t)file_size) {
        free(buffer);
        fclose(file);
        *size = 0;
        return NULL;
    }

    buffer[file_size] = '\0';

    fclose(file);

    *size = (int)file_size;
    return buffer;
}

