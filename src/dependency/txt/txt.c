#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TXT_READ_SIZE_DEFAULT 1024

void txtRead(const char *path, char *recv_string, size_t buffer_size) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        // 文件打开失败
    }

    size_t total_read = 0;
    char temp_buffer[1024];

    while (fgets(temp_buffer, sizeof(temp_buffer), fp)) {
        size_t len = strlen(temp_buffer);

        if (total_read + len >= buffer_size - 1) {
            // Buffer size exceeded
            break;
        }

        strcat(recv_string, temp_buffer);
        total_read += len;
    }

    fclose(fp);
}