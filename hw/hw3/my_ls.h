#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef MY_LS_
#define MY_LS_

void  do_ls(char *dirName,int track);
char * compare_mode(char c);
char *  change_mode(int mode,int filetype);
int check_if_file(char *path);
char * pathname(char *path, char * filename);
void check_for_recursion(char *argument, char *filename, int track);
void print_info(char *argument,char *filename,int filetype,int track,int required_path);
void indent(int track);
void ls_directory(char *argument, DIR *dirp,int track);
char *getUserName(uid_t uid);
char *getGroupName(gid_t gid);
void directory_recursion(char *temp,int track, char *directory_name);
void free_memory(char *mode, char *pathname_file, char *modification_time,int require_path);
void print_directory_name(char *argument);
#endif
