#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

//--------------------------------------------
// NAME: Emiliqn Sokolov
// CLASS: XI b
// NUMBER: 15
// PROBLEM: shell
// FILE NAME: Emiliqn_Sokolov_15.c
// FILE PURPOSE:
// This file contains the program
//--------------------------------------------
char *str = NULL;
//--------------------------------------------
// FUNCTION: exec_func
// Implementing fork and execv functions
// PARAMETERS:
// char** arr_of_str - array of strings
//---------------------------------------------
int exec_func(char** arr_of_str)
{
		// Fork process
	pid_t pid = fork();

		// Error
	if (pid == -1)
	{
		char* error = strerror(errno);
		printf("%s\n", error);
		return 1;
	}

		// Child process
	else if (pid == 0) 
	{
			// Execute command
		execv(arr_of_str[0],arr_of_str);

			// Error occurred
		char* error = strerror(errno);
		printf("%s: %s\n", arr_of_str[0], error);
		exit(1);
	}
	// Parent process
	else 
	{
			// Wait for child process to finish
		int childStatus;
		pid = waitpid(pid, &childStatus, 0);
		if (pid < 0)
		{
			char* error = strerror(errno);
			printf("%s\n", error);
			return 1;
		}
	}
}

//--------------------------------------------
// FUNCTION: parse_cmdline
// Splits the cmdline string by spaces
// PARAMETERS:
// const char* cmdline - string that contains the info from the standart input
//--------------------------------------------
char** parse_cmdline(const char* cmdline)
{
	str = malloc(strlen(cmdline));
	char** arr_of_str = NULL;    //array of strings definition
	int k = 2;
	int word_count = 0;	//word_counter
	strcpy(str,cmdline);		//copying the const cmdline variable to str
	char *pch = strtok(str, " ");	// call the strtok with str as 1st arg
	while(pch != NULL)	// check if we got a token.
	{
		arr_of_str = realloc(arr_of_str,k*sizeof(char*));
		arr_of_str[word_count++] = pch;  //add the string 
		pch = strtok(NULL, " ");
		k++;
	}
	if(word_count == 0)
	{
		arr_of_str = malloc(2*sizeof(char*));
		arr_of_str[0] = "";
		arr_of_str[1] = NULL;
	}
	else
	{
		arr_of_str[word_count] = NULL;
	}
	
	return arr_of_str;	//return the array of strings
}

int main()
{
	char *cmdline = NULL;
    	size_t size = 0, chr = 0;
    	int index;
    	char** arr_of_str;
	char c;
	while((c = getchar()) != EOF) //check each character
	{	
		if(size <= chr)
		{
			size += 1;
			cmdline = realloc(cmdline,size);
			index = 0;
		}

		if(c == '\n') //check for new lines
		{
			cmdline[chr] = '\0';  //terminate the string
			chr = 0;
			size = 0;
			index = 1;
			arr_of_str = parse_cmdline(cmdline); //calling the parse_cmdline function
			exec_func(arr_of_str);	//calling the exec_func function
			free(arr_of_str);
			free(str);			//free the memory allocation
			free(cmdline);		
			cmdline = NULL;
		}
		else
		{
			cmdline[chr++] = c;
			
		}
	}
	if(index == 0)
	{
		free(cmdline);	//free the memory allocation
	}
	return 0;
}
