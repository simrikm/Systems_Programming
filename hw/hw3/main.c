#include "my_ls.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	DIR *dirp;
	char *argument=argc>1?argv[1]:(char*)".";
	if(!(dirp=opendir(argument)))
		print_info(argument,(char*)"",1,0,0);
	else
		do_ls(argument,0);
	closedir(dirp);
	return 0;

}
