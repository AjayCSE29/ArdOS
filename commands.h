#ifndef COMMANDS_H
#define COMMANDS_H

void cmd_help();
void cmd_info();
void cmd_clear();
void cmd_ls();
void cmd_mkdir(char name[]);
void cmd_touch(char name[]);
void cmd_cd(char name[]);
void cmd_pwd();
void cmd_write(char filename[]);
void cmd_cat(char filename[]);

#endif