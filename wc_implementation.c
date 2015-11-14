#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

//--------------------------------------------
// NAME: Emiliqn Sokolov
// CLASS: XI b
// NUMBER: 15
// PROBLEM: wc
// FILE NAME: Emiliqn_Sokolov_15.c
// FILE PURPOSE:
// The file contains the whole program
//---------------------------------------------

char temp;
//-----------------------------------------------------------------------------------
// FUNCTION: st_input
// The purpose of the funcion is to read from the standard input
// PARAMETERS:
// *line count - Pointer counter that counts the lines in the standart input
// *word_count - Pointer counter that counts the words in the standart input
// *char_count - Pointer counter that counts the characters in the standart input
//-----------------------------------------------------------------------------------
void st_input(int *line_count, int *word_count,int *char_count) {
	char c;
	while(read(STDIN_FILENO, &c, 1) > 0) {   //checking each character 
		*char_count+=1;
		if(c == '\n') {      //checking for lines
			*line_count+=1;
		}
		if(c == ' ' || c == '\n' || c == '\t') {      //checking for words
			if(temp != ' ' && temp != '\n' && temp != '\t' && temp != '\0') {
				*word_count+=1;
			}
		}
	temp = c; //temp - temporary variable which gets the previous value of the variable c
	}
	temp = '\0';
}

int main(int argc, char *argv[]) {

int fd;
int argc_count;
char buff[1];
int status_read;
int line_count = 0;
int word_count = 0;
int char_count = 0;
int total_word_count = 0;
int total_line_count = 0;
int total_char_count = 0;
int close_status;
char src[100]; //variable for the print errors

for(argc_count = 1; argc_count < argc; argc_count++) { //if the program had arguments this loop checks each of them
	status_read = 1;
	line_count = 0;		
	word_count = 0;
	char_count = 0;
	
	if((*argv[argc_count] == '-') && (strlen(argv[argc_count]) == 1)) {
			st_input(&line_count,&word_count,&char_count); //function st_input is called which reads from the standart input
	}
	else {
		fd = open(argv[argc_count], O_RDONLY); //opening the file(argument name)
		if (fd == -1) {	//if the file cant be opened, has bad name, etc. then will be printed an error in the terminal
			strcpy(src, "wc: ");
			perror(strcat(src, argv[argc_count]));
			return 1;
		}

		while(status_read > 0) {                             // reading from file
			status_read = read(fd, buff, 1);          
			if (status_read < 0) {
				perror(strcpy(src, "wc: read()"));     //an error will be printed in the terminal if there is some issue with the reading
				return 1;
			}
			 
			if(status_read != 0) { //checks if the file is empty
				char_count++;
				if(buff[0] == '\n') { // checking for lines
					line_count++;
				} 
		 		if(buff[0] == ' ' || buff[0] == '\n' || buff[0] == '\t') {	//checking for words
					if(temp != ' ' && temp != '\n' && temp != '\t' && temp != '\0' ) {
						word_count++;
					}
				}
				temp = buff[0];
			}
		}
		temp = '\0';
		close_status = close(fd);   //closing the file
		if (close_status == -1)
		{	
			perror(strcpy(src, "wc: close()"));  //an error will be printed in the terminal if there is some issue with the closing
			return 1;
		}
	}

	printf("%d %d %d %s\n",line_count,word_count,char_count,argv[argc_count]); //printing the counters and the argument name
	total_word_count += word_count;
	total_line_count += line_count;
	total_char_count += char_count;
}
if(argc == 1) { // If there are no arguments then the program reads from the standart input
	st_input(&line_count,&word_count,&char_count); //function st_input is called which reads from the standart input
	printf("%d %d %d\n",line_count,word_count,char_count); //printing the counters
}
if(argc > 2) { //If there is more than one argument the program prints the total counters
	printf("%d %d %d total\n",total_line_count,total_word_count,total_char_count); //printing the total counters
}

return 0;
}
