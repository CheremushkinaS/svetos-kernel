#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <sys/stat.h>
#include <stdint.h>

// Структура заголовка CPIO
struct cpio_header {
    char c_magic[6];
    char c_ino[8];
    char c_mode[8];
    char c_uid[8];
    char c_gid[8];
    char c_nlink[8];
    char c_mtime[8];
    char c_filesize[8];
    char c_devmajor[8];
    char c_devminor[8];
    char c_rdevmajor[8];
    char c_rdevminor[8];
    char c_namesize[8];
    char c_check[8];
};

// Вспомогательная функция для безопасного форматирования 8-байтных полей
void format_cpio_field(char *field, size_t field_size, const char *format, unsigned long value) {
    char tmp[9];
    snprintf(tmp, sizeof(tmp), format, value);
    strncpy(field, tmp, field_size - 1);
    field[field_size - 1] = '\0';
}

// Функция для добавления файла в архив CPIO
void add_file_to_cpio(FILE *cpio_archive, const char *filename, const char *dest_path) {
    struct stat st;
    if (stat(filename, &st) != 0) {
        fprintf(stderr, "Error: Cannot stat file %s\n", filename);
        return;
    }

    // Читаем содержимое файла
    FILE *f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return;
    }

    char *data = malloc(st.st_size);
    if (fread(data, 1, st.st_size, f) != st.st_size) {
        fprintf(stderr, "Error: Failed to read file %s\n", filename);
        fclose(f);
        free(data);
        return;
    }
    fclose(f);

    // Формируем заголовок CPIO
    struct cpio_header hdr;
    memset(&hdr, 0, sizeof(hdr));
    
    strcpy(hdr.c_magic, "070701");
    format_cpio_field(hdr.c_mode, sizeof(hdr.c_mode), "%08lo", (unsigned long)0100644);
    format_cpio_field(hdr.c_uid, sizeof(hdr.c_uid), "%08lo", 0UL);
    format_cpio_field(hdr.c_gid, sizeof(hdr.c_gid), "%08lo", 0UL);
    format_cpio_field(hdr.c_nlink, sizeof(hdr.c_nlink), "%08lo", 1UL);
    format_cpio_field(hdr.c_mtime, sizeof(hdr.c_mtime), "%08lo", 0UL);
    format_cpio_field(hdr.c_filesize, sizeof(hdr.c_filesize), "%08lo", (unsigned long)st.st_size);
    format_cpio_field(hdr.c_devmajor, sizeof(hdr.c_devmajor), "%08lo", 0UL);
    format_cpio_field(hdr.c_devminor, sizeof(hdr.c_devminor), "%08lo", 0UL);
    format_cpio_field(hdr.c_rdevmajor, sizeof(hdr.c_rdevmajor), "%08lo", 0UL);
    format_cpio_field(hdr.c_rdevminor, sizeof(hdr.c_rdevminor), "%08lo", 0UL);
    format_cpio_field(hdr.c_namesize, sizeof(hdr.c_namesize), "%08lo", (unsigned long)strlen(dest_path) + 1);
    format_cpio_field(hdr.c_check, sizeof(hdr.c_check), "%08lo", 0UL);

    // Записываем заголовок
    fwrite(&hdr, 1, sizeof(hdr), cpio_archive);

    // Записываем имя файла
    fwrite(dest_path, 1, strlen(dest_path) + 1, cpio_archive);

    // Выравнивание до 4 байт
    int padding = (4 - (sizeof(hdr) + strlen(dest_path) + 1) % 4) % 4;
    for (int i = 0; i < padding; i++) {
        fputc(0, cpio_archive);
    }

    // Записываем данные файла
    fwrite(data, 1, st.st_size, cpio_archive);

    // Выравнивание данных до 4 байт
    padding = (4 - st.st_size % 4) % 4;
    for (int i = 0; i < padding; i++) {
        fputc(0, cpio_archive);
    }

    free(data);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s --output-initrd=filename [--add=module1,module2,...]\n", argv[0]);
        return 1;
    }

    const char *output_filename = NULL;
    const char *modules_to_add = NULL;

    // Парсим аргументы командной строки
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--output-initrd=", 16) == 0) {
            output_filename = argv[i] + 16;
        } else if (strncmp(argv[i], "--add=", 6) == 0) {
            modules_to_add = argv[i] + 6;
        }
    }

    if (!output_filename) {
        fprintf(stderr, "Error: Output filename not specified\n");
        return 1;
    }

    // Создаем CPIO архив
    FILE *cpio = fopen(output_filename, "wb");
    if (!cpio) {
        fprintf(stderr, "Error: Cannot create output file %s\n", output_filename);
        return 1;
    }

    // Добавляем файлы модулей
    if (modules_to_add) {
        char *modules = strdup(modules_to_add);
        char *token = strtok(modules, ",");
        
        while (token) {
            char filename[256];
            // Исправляем пути к модулям
            if (strcmp(token, "timer") == 0) {
                snprintf(filename, sizeof(filename), "build/modules/drivers/core/timer/timer.o");
            } else if (strcmp(token, "keyboard") == 0) {
                snprintf(filename, sizeof(filename), "build/modules/drivers/core/keyboard/keyboard.o");
            } else if (strcmp(token, "vga") == 0) {
                snprintf(filename, sizeof(filename), "build/modules/drivers/core/vga/vga.o");
            } else if (strcmp(token, "terminal") == 0) {
                snprintf(filename, sizeof(filename), "build/modules/sys/terminal/terminal.o");
            } else {
                snprintf(filename, sizeof(filename), "build/modules/%s.o", token);
            }
            
            char dest_path[256];
            snprintf(dest_path, sizeof(dest_path), "%s.o", token);
            
            add_file_to_cpio(cpio, filename, dest_path);
            token = strtok(NULL, ",");
        }
        
        free(modules);
    }

    // Добавляем конец архива
    struct cpio_header end_hdr;
    memset(&end_hdr, 0, sizeof(end_hdr));
    strcpy(end_hdr.c_magic, "070701");
    format_cpio_field(end_hdr.c_namesize, sizeof(end_hdr.c_namesize), "%08lo", 11UL);
    format_cpio_field(end_hdr.c_filesize, sizeof(end_hdr.c_filesize), "%08lo", 0UL);
    
    fwrite(&end_hdr, 1, sizeof(end_hdr), cpio);
    fwrite("TRAILER!!!\0", 1, 11, cpio);
    
    // Выравнивание
    int padding = (4 - (sizeof(end_hdr) + 11) % 4) % 4;
    for (int i = 0; i < padding; i++) {
        fputc(0, cpio);
    }

    fclose(cpio);
    printf("Created initrd: %s\n", output_filename);
    
    return 0;
}
