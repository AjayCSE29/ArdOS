#ifndef FILESYSTEM_H
#define FILESYSTEM_H

void fs_init();

#define MAX_FILE_SIZE 128

bool fs_createDirectory(const char name[]);
bool fs_createFile(const char name[]);

void fs_list();
bool fs_changeDirectory(const char name[]);
void fs_printWorkingDirectory();
bool fs_writeFile(const char filename[], const char text[]);
bool fs_readFile(const char filename[]);
bool fs_deleteFile(const char name[]);
void fs_format();

#endif