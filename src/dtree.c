#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>

#define MAX_PATH 4096
#define MAX_ENTRIES 1024

typedef struct {
    int show_hidden;
    int max_depth;
    int show_size;
} Options;

static int is_last_entry[MAX_PATH];

void format_size(off_t size, char *buf);
void print_indent(int depth);
int count_entries(const char *path, const Options *opts);
void print_entry(const char *name, int is_dir, int depth, off_t size, const Options *opts);
void process_directory(const char *path, int depth, const Options *opts);
void print_usage(const char *program_name);

void format_size(off_t size, char *buf) {
    const char *units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit_index = 0;
    double size_d = size;

    while (size_d >= 1024 && unit_index < 4) {
        size_d /= 1024;
        unit_index++;
    }

    if (unit_index == 0) {
        sprintf(buf, "%lld B", (long long)size);
    } else {
        sprintf(buf, "%.1f %s", size_d, units[unit_index]);
    }
}

void print_indent(int depth) {
    for (int i = 0; i < depth; i++) {
        if (is_last_entry[i]) {
            printf("    "); 
        } else {
            printf("│   ");
        }
    }
    
    if (depth > 0) {
        if (is_last_entry[depth - 1]) {
            printf("└── ");  // Last entry
        } else {
            printf("├── ");  // Middle entry
        }
    }
}

int count_entries(const char *path, const Options *opts) {
    DIR *dir = opendir(path);
    if (!dir) return 0;
    
    struct dirent *entry;
    int count = 0;
    
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (!opts->show_hidden && entry->d_name[0] == '.') {
            continue;
        }
        count++;
    }
    
    closedir(dir);
    return count;
}

void print_entry(const char *name, int is_dir, int depth, off_t size, const Options *opts) {
    print_indent(depth);
    
    if (opts->show_size && !is_dir) {
        char size_str[32];
        format_size(size, size_str);
        printf("[%s] ", size_str);
    }
    
    printf("%s%s\n", name, is_dir ? "/" : "");
}

void process_directory(const char *path, int depth, const Options *opts) {
    DIR *dir;
    struct dirent *entry;
    char full_path[MAX_PATH];
    struct stat st;
    struct dirent *entries[MAX_ENTRIES];
    int entry_count = 0;

    if (opts->max_depth != -1 && depth > opts->max_depth) {
        return;
    }

    dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "Error opening directory '%s': %s\n", path, strerror(errno));
        return;
    }

    while ((entry = readdir(dir)) != NULL && entry_count < MAX_ENTRIES) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (!opts->show_hidden && entry->d_name[0] == '.') {
            continue;
        }
        
        entries[entry_count] = malloc(sizeof(struct dirent));
        memcpy(entries[entry_count], entry, sizeof(struct dirent));
        entry_count++;
    }
    closedir(dir);

    for (int i = 0; i < entry_count; i++) {
        entry = entries[i];
        
        is_last_entry[depth] = (i == entry_count - 1);

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (lstat(full_path, &st) == -1) {
            fprintf(stderr, "Error getting info for '%s': %s\n", full_path, strerror(errno));
            continue;
        }

        print_entry(entry->d_name, S_ISDIR(st.st_mode), depth, st.st_size, opts);

        if (S_ISDIR(st.st_mode)) {
            process_directory(full_path, depth + 1, opts);
        }
        
        free(entries[i]);
    }
}

void print_usage(const char *program_name) {
    printf("Usage: %s [options] [directory]\n", program_name);
    printf("Options:\n");
    printf("  -a    Show hidden files\n");
    printf("  -d N  Maximum depth (N levels)\n");
    printf("  -s    Show file sizes\n");
    printf("  -h    Show this help message\n");
}

int main(int argc, char *argv[]) {
    Options opts = {
        .show_hidden = 0,
        .max_depth = -1,
        .show_size = 0
    };
    
    char *dir_path = "."; 
    int opt;

    memset(is_last_entry, 0, sizeof(is_last_entry));

    while ((opt = getopt(argc, argv, "ad:sh")) != -1) {
        switch (opt) {
            case 'a':
                opts.show_hidden = 1;
                break;
            case 'd':
                opts.max_depth = atoi(optarg);
                if (opts.max_depth < 0) {
                    fprintf(stderr, "Depth must be non-negative\n");
                    return 1;
                }
                break;
            case 's':
                opts.show_size = 1;
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    if (optind < argc) {
        dir_path = argv[optind];
    }

    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        fprintf(stderr, "Error opening directory '%s': %s\n", dir_path, strerror(errno));
        return 1;
    }
    closedir(dir);

    printf("%s\n", dir_path);
    
    process_directory(dir_path, 0, &opts);

    return 0;
}
