#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void create_directory_if_not_exists(const char *dir_name) {
    struct stat st = {0};
    if (stat(dir_name, &st) == -1) {
        mkdir(dir_name, 0700);
    }
}

void move_file(const char *file_name, const char *target_folder) {
    char command[512];
    snprintf(command, sizeof(command), "mv \"%s\" \"%s/\"", file_name, target_folder);
    system(command);  // Use system to execute the mv command
}

const char* get_file_type_folder(const char *file_name) {
    const char *ext = strrchr(file_name, '.');
    if (ext == NULL) {
        return "Others";
    }
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".png") == 0 || strcmp(ext, ".jpeg") == 0) {
        return "Images";
    } else if (strcmp(ext, ".pdf") == 0 || strcmp(ext, ".docx") == 0 || strcmp(ext, ".txt") == 0) {
        return "Documents";
    } else if (strcmp(ext, ".mp4") == 0 || strcmp(ext, ".mkv") == 0) {
        return "Videos";
    } else {
        return "Others";
    }
}

void organize_files(const char *directory) {
    DIR *d;
    struct dirent *dir;
    d = opendir(directory);

    if (d == NULL) {
        printf("Failed to open directory %s\n", directory);
        return;
    }

    while ((dir = readdir(d)) != NULL) {
        if (dir->d_type == DT_REG) {  // Only consider regular files
            const char *file_name = dir->d_name;
            const char *folder = get_file_type_folder(file_name);
            create_directory_if_not_exists(folder);
            move_file(file_name, folder);
            printf("Moved %s to %s folder\n", file_name, folder);
        }
    }

    closedir(d);
}

int main() {
    char directory[256];

    // Get the target directory from user input
    printf("Enter the directory path to organize: ");
    scanf("%s", directory);

    organize_files(directory);

    return 0;
}
