#ifndef FILESYSTEM_H
#define FILESYSTEM_H

void fs_init();

bool fs_createDirectory(const char name[]);
bool fs_createFile(const char name[]);

void fs_list();
bool fs_changeDirectory(const char name[]);
void fs_printWorkingDirectory();

#endif