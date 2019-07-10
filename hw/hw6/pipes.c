#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#define TRUE 1
#define FALSE 0


#define MAX 256
typedef char buffer[MAX];
typedef char * vector[MAX];
buffer arg_buffer[MAX];

typedef enum MODE
{
	PLAIN,
	BACKGROUND,
	PIPE
}Mode;


typedef struct COMMAND
{
	buffer name;
	vector argv;
	char * ifile;
	char * ofile;
	Mode mode;
	int is_last;
	char * full_path;
}Command;


char * path_values[MAX];

typedef struct PROCESS
{
	buffer cmd;
	int pid;
	Mode mode;
}Process;



// list (stack) of processes

Process process_stack[MAX];
int psp = -1;


// true only if we are in a pipeline

int in_pipe = FALSE;


// get_path reads the user's path variable into path_values

void get_path()
{
	char * path, * * pv;
	path = getenv("PATH");
	pv = path_values;
	for (;;)
	{
		*pv++ = path;
		while (*path && *path != ':') path++;
		if (!*path) break;
		*path++ = 0;
	}
	*pv = NULL;
}


// get_word gets a word from the string

const char delim[]="<>&|;";

char * get_word(char * word)
{
	int ch;
	char * p = word;
	for (;;)
	{
		if ((ch = getchar()) == EOF)
			exit(0);
		if (ch != ' ' && ch != '\t')
			break;
	}
	if (ch == '\n')
		return NULL;
	*p++ = ch;
	if (strchr(delim, ch) == NULL)
	{
		for (;;)
		{
			ch = getchar();
			if (isspace(ch) || strchr(delim, ch))
				break;
			*p++ = ch;
		}
		ungetc(ch, stdin);
    }
	*p = 0;
	return word;
}


// parse a command line into a cmd

void get_command_line(Command * cmd)
{
	char * * argv = cmd->argv;
	buffer * arg_buf_ptr = arg_buffer;
	cmd->ifile = NULL;
	cmd->ofile = NULL;
	cmd->mode = PLAIN;
	cmd->is_last = FALSE;
	if (!get_word(cmd->name))
	{
		cmd->name[0] = 0;
		cmd->is_last = TRUE;
		return;
	}
	*argv++ = cmd->name;
	for (; (*argv = get_word(*arg_buf_ptr)); argv++, arg_buf_ptr++)
		if (!strcmp(*argv, "<"))
		{
			if (!(cmd->ifile = get_word(*arg_buf_ptr)))
				printf("Redirect w/o file\n");
			argv--;
		}
		else if (!strcmp(*argv, ">"))
		{
			if (!(cmd->ofile = get_word(*arg_buf_ptr)))
				printf("Redirect w/o file\n");
			argv--;
		}
		else if (!strcmp(*argv,"&") ||
			!strcmp(*argv, "|") || !strcmp(*argv, ";"))
		{
			if (*argv[0] == '&')
				cmd->mode = BACKGROUND;
			else if (*argv[0]=='|')
				cmd->mode=PIPE;
			*argv = NULL;
			return;
		}
	cmd->is_last = TRUE;
}


// check_children looks for terminated background jobs

int check_children()
{
	int status, pid, i;;
	pid = waitpid(-1,&status,WNOHANG)/* wait for a child without blocking (hanging) */;
	if (pid <= 0) return -1;
	for (i = 0; i <= psp; i++)
		if (process_stack[i].pid == pid) break;
	if (i <= psp)
	{
		if (process_stack[i].mode == BACKGROUND)
			printf("[%d] Done %s\n", i, process_stack[i].cmd);
		process_stack[i] = process_stack[psp--];
	}
	return pid;
}


// readable and writable check for respective operation

int readable(char * file_name)
{
	struct stat buf;
	if (stat(file_name, &buf) < 0) return FALSE;
	return buf.st_mode & S_IRUSR;
}

int writable(char * file_name)
{ // actually, this subroutine is wrong, but it will work for now
	struct stat buf;
	if (stat(file_name, &buf) < 0) return TRUE;
	return buf.st_mode & S_IWUSR;
}


//  expand_path looks for an executable in users path

char * expand_path(char * name)
{
	char * * pv;
	static buffer path_part;	
	for (pv = path_values; *pv; pv++)
	{
        sprintf(path_part, "%s/%s", *pv, name);
		if (readable(path_part))
			return path_part;
	}
	return NULL;
}


// verify command line checks to besure pipes make sense

int verify_command_line(Command * cmd)
{
	if ((cmd->mode == PIPE && in_pipe
			&&(cmd->ifile || cmd->ofile))
		|| (cmd->mode == PIPE && !in_pipe && cmd->ofile)
		|| (cmd->mode != PIPE && in_pipe && cmd->ifile))
	{
			printf("Can't redirect in pipeline\n");
			return FALSE;
	}
	if (cmd->ifile && !readable(cmd->ifile))
	{
		printf("Unable to open input file '%s'\n", cmd->ifile);
		return FALSE;
	}
	if (cmd->ofile && !writable(cmd->ofile))
	{
		printf("Unable to open output file '%s'\n", cmd->ofile);
		return FALSE;
	}
	if (!(cmd->full_path = expand_path(cmd->name)))
	{
		printf("Unable to find command '%s'\n",  cmd->name);
		return FALSE;
	}
	return TRUE;
}


//  set_child_io redirects the input/output of a process

int set_child_io(Command * cmd, int * in, int * out)
{
	static int pass_through;
	int pipes[2];
	*in = 0;
	*out = 1;
	if (cmd->ifile)
		*in = open(cmd->ifile,O_RDONLY)/* open the cmd.ifile for reading */;
	if (cmd->ofile)
		*out = open(cmd->ofile,O_CREAT|O_WRONLY|O_TRUNC,0666) /* create cmd.ofile for writing */;
	if (cmd->mode == PIPE)
	{
		if (pipe(pipes)) /* create a pipeline in pipes */
		{
			printf("Unable to create pipes\n");
			return FALSE;
		}
		if (in_pipe) *in = pass_through;
		else in_pipe = TRUE;
		*out = pipes[1];
		pass_through = pipes[0];
	}
	else if (in_pipe)
	{
		in_pipe = FALSE;
		*in = pass_through;
	}
	return TRUE;
}


// main loop for Simple Unix Shell

int main(int argc, char * * argv, char * * envp)
{
	if(argc>0 && argv[0]!=NULL){
	Command cmd;
	int child_pid, in, out;
	get_path();
	cmd.is_last = TRUE;
	while (TRUE)
	{
		if (psp >= 0)
			while (check_children() >= 0)
				;
		if (cmd.is_last)
			printf("%% ");
		get_command_line(&cmd);
		if (!cmd.name[0])
			continue;
		if (!verify_command_line(&cmd))
			continue;
		if (!set_child_io(&cmd, &in, &out))
			continue;
		if ((child_pid = fork()))
		{
			// PARENT (shell) work
			if (cmd.mode == PLAIN)
				while (check_children() != child_pid)
					;
			else
			{
				process_stack[++psp].pid = child_pid;
				strcpy(process_stack[psp].cmd, cmd.name);
				process_stack[psp].mode = cmd.mode;
				if (cmd.mode == BACKGROUND)
					printf("[%d] %d\n", psp, child_pid);
			}
			if (in != 0)
				close(in) /* close the file descriptor for in */ ;
			if (out != 1)
				close(out) /* close the file descriptor for out */ ;
		}
		else
		{
			// CHILD work
			/* duplicate the I/O descriptor from in over our stdin */
			/* duplicate the I/O descriptor from out over our stdout */
			/* load the program in cmd.full_path into the current process */
			dup2(in,0);
			dup2(out,1);
			if(execve(cmd.full_path,cmd.argv,envp)==-1)
			printf("Unable to run '%s'\n",  cmd.full_path);
			exit(-1);
		}
	}
	}
    return 0;
}


