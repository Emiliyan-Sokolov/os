#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <string.h>
//--------------------------------------------
// NAME: Emiliqn Sokolov
// CLASS: XI b
// NUMBER: 15
// PROBLEM: ls
// FILE NAME: Emiliqn_Sokolov_15.c
// FILE PURPOSE:
// This file contains the program
//--------------------------------------------

char buf[FILENAME_MAX];
int ls_all_flag = 0;
int file_info_flag = 0;
int ls_recursive_flag = 0;
//--------------------------------------------
//FUNCTION: ls
//This function lists all files in the current directory     
//
//PARAMETERS: -
//----------------------------------------------
void ls() {
	struct dirent *e;
	struct stat st;
	char c;
	DIR *dir;
	if((dir = opendir(".")) == NULL) {
    		perror("Error");
    		exit(1);
    	}
	while((e = readdir(dir)) != NULL){
		if(stat(e->d_name,&st) == -1) {
        		perror("stat");
        		exit(1);
       		}
		c = *e->d_name;
		if(c != '.') {
			switch(st.st_mode & S_IFMT) {
	    			case S_IFBLK:  
	    				printf("b ");
	    				break;
	   			case S_IFCHR:  
	   				printf("c ");       
	   				break;
	    			case S_IFDIR:  
	    				printf("d ");               
	    				break;
	    			case S_IFIFO:  
	    				printf("p ");               
	    				break;
	    			case S_IFLNK:  
	    				printf("l ");                 
	    				break;
	   			case S_IFREG:  
	   			 	printf("- ");            
	   			 	break;
	    			case S_IFSOCK: 
	    				printf("s ");                  
	    				break;
	   			default:
					printf("Unknown type ");
					break;
			}
			printf("%s\n",e->d_name);
		}
	}
	closedir(dir);
}

//--------------------------------------------
//FUNCTION: lsAll
//This function lists all files in the current directory including
//the hidden files.     
//
//PARAMETERS: -
//----------------------------------------------
void lsAll() {
	struct dirent *e;
	struct stat st;
	DIR *dir;
	if((dir = opendir(".")) == NULL) {
    		perror("Error");
    		exit(1);
    	}

	while((e = readdir(dir)) != NULL){
		if(stat(e->d_name,&st) == -1) {
        		perror("stat");
        		exit(1);
        	}
		switch(st.st_mode & S_IFMT) {
			case S_IFBLK:  
				printf("b ");
				break;
			case S_IFCHR:  
				printf("c ");       
				break;
			case S_IFDIR:  
				printf("d ");               
				break;
			case S_IFIFO:  
				printf("p ");               
				break;
			case S_IFLNK:  
				printf("l ");                 
				break;
			case S_IFREG:  
		 		printf("- ");            
		 		break;
			case S_IFSOCK: 
				printf("s ");                  
				break;
			default:
				printf("Unknown type ");
				break;
		}
		printf("%s\n",e->d_name);
	}
	closedir(dir);
}

//--------------------------------------------
//FUNCTION: lsWithArg
//The same function as ls but with argument
//PARAMETERS: - arg
//arg contains the file/folder
//----------------------------------------------
void lsWithArg(char* arg) {
	struct stat st;
	char err[100];
	if(stat(arg,&st) == -1) {
    	strcpy(err, "ls: cannot access ");
		perror(strcat(err, arg));
		exit(1);
   	}
	switch(st.st_mode & S_IFMT) {
		case S_IFDIR:
		{
			printf("%s:\n",arg);
			if(chdir(arg) == -1) {
				perror("cant change current path");
				exit(1);
			}
			if(ls_all_flag == 1) {
				lsAll();
			}
			else {
				ls();
			}
			if(chdir(buf) == -1) {
				perror("cant change current path");
				exit(1);
			}
			printf("\n");               
			break;
		}
		case S_IFREG:
		{	  
	 		printf("- "); 
	 		printf("%s\n",arg);           
	 		break;
	 	}
	 	case S_IFSOCK: 
	 	{
			printf("s "); 
			printf("%s\n",arg);                    
			break;
		}
		case S_IFLNK:  
		{
			printf("l ");  
			printf("%s\n",arg);                        
			break;
		}
		case S_IFIFO:
		{  
			printf("p ");
			printf("%s\n",arg);                    
			break;
		}
		case S_IFBLK: 
		{ 
			printf("b ");
			printf("%s\n",arg);   
			break;
		}
		case S_IFCHR: 
		{ 
			printf("c ");
			printf("%s\n",arg);          
			break;
		}
		default:
		{
			printf("Unknown type %s\n",arg);
			break;
		}
	}	
}


//--------------------------------------------
//FUNCTION: ls_recursive
//This function lists all files including directiories and their subfiles 
//by recursive method.  
//PARAMETERS: -
//----------------------------------------------

int ls_recursive(char* arg) {
	int i;
	int dir_count = 0;
	struct dirent *e;
	struct stat st;
	struct stat st2;
	char *folders_arr[1024];
	char current_path[FILENAME_MAX];
	char c;
	char err[100];
	DIR *dir;
	
	if(stat(arg,&st) == -1) {
    	strcpy(err, "ls: cannot access ");
		perror(strcat(err, arg));
		exit(1);
   	}
	
	switch(st.st_mode & S_IFMT) {
		case S_IFDIR:
		{
			if(chdir(arg) == -1) {
				perror("cant change current path");
				exit(1);
			}
			if(getcwd(current_path, FILENAME_MAX) == NULL){
				perror("getcwd error");
				exit(1);
			}
			printf("%s:\n",current_path);
			if((dir = opendir(".")) == NULL) {
    				perror("Error");
    				exit(1);
    			}
			while((e = readdir(dir)) != NULL){
				if(stat(e->d_name,&st2) == -1) {
        				strcpy(err, "ls: cannot access ");
					perror(strcat(err, e->d_name));
					exit(1);
       				}
				c = *e->d_name;
				if(c != '.') {
					switch(st2.st_mode & S_IFMT) {
						case S_IFBLK:  
							printf("b ");
							break;
						case S_IFCHR:  
							printf("c ");       
							break;
						case S_IFDIR:
							folders_arr[dir_count] = e->d_name;
							dir_count++;
							printf("d ");             
							break;
						case S_IFIFO:  
							printf("p ");               
							break;
						case S_IFLNK:  
							printf("l ");                 
							break;
						case S_IFREG:  
					 		printf("- ");            
					 		break;
						case S_IFSOCK: 
							printf("s ");                  
							break;
						default:
							printf("Unknown type ");
							break;	
					}
					printf("%s\n",e->d_name);
				}
			}
			closedir(dir);

			for(i = 0; i < dir_count; i++) {	
				ls_recursive(folders_arr[i]);	
				if(chdir("..") == -1) {
					perror("cant change current path");
					exit(1);
				}
			}
			break;
		}
		
		default:
		{
			lsWithArg(arg);
			break;
		}
		
	}
}


//--------------------------------------------
//FUNCTION: file_info
//This function gets the info of the files which are in the specified folder. 
//PARAMETERS: -
//----------------------------------------------
void file_info(char* arg) {
	struct dirent *e;
	struct stat st;
	struct stat st2;
	struct passwd *name;
	struct passwd *group;
	DIR *dir;
	char c;
	char err[100];
	if(stat(arg,&st) == -1) {
    	strcpy(err, "ls: cannot access ");
		perror(strcat(err, arg));
		exit(1);
   	}
   	switch(st.st_mode & S_IFMT) {
		case S_IFDIR:
		{
			if(chdir(arg) == -1) {
				perror("cant change current path");
				exit(1);
			}
			
			if((dir = opendir(".")) == NULL) {
    				perror("Error");
    				exit(1);
   			}
			while((e = readdir(dir)) != NULL){
				if(stat(e->d_name,&st2) == -1) {
			    		strcpy(err, "ls: cannot access ");
					perror(strcat(err, e->d_name));
					exit(1);
		    		}
				name = getpwuid(st2.st_uid);
				group = getpwuid(st2.st_gid);
				c = *e->d_name;
				if(c != '.') {	
					//file permissions
					printf( (S_ISDIR(st2.st_mode)) ? "d" : "-");
		   			printf( (st2.st_mode & S_IRUSR) ? "r" : "-");
			    		printf( (st2.st_mode & S_IWUSR) ? "w" : "-");
			    		printf( (st2.st_mode & S_IXUSR) ? "x" : "-");
			    		printf( (st2.st_mode & S_IRGRP) ? "r" : "-");
			    		printf( (st2.st_mode & S_IWGRP) ? "w" : "-");
			    		printf( (st2.st_mode & S_IXGRP) ? "x" : "-");
			    		printf( (st2.st_mode & S_IROTH) ? "r" : "-");
			    		printf( (st2.st_mode & S_IWOTH) ? "w" : "-");
			    		printf( (st2.st_mode & S_IXOTH) ? "x" : "-");
			    		//number of links
			    		printf(" %ld",(long int)st2.st_nlink);
			    		//user name
			    		printf(" %s",name->pw_name);
			    		//group name
			    		printf(" %s",group->pw_name);
			    		//size
			    		printf(" %ld",(long int) st2.st_size);
			    		//time
			    		printf(" %s", ctime(&st2.st_mtime));
			    		//file name
			    		printf(" %s",e->d_name);
			    		printf("\n");
				}
			}
			closedir(dir);    
			if(chdir(buf) == -1) {
				perror("cant change current path");
				exit(1);
			}
			
			break;
		}
		default:
		{	  
			name = getpwuid(st.st_uid);
			group = getpwuid(st.st_gid);
			//file permissions
			printf( (S_ISDIR(st.st_mode)) ? "d" : "-");
   			printf( (st.st_mode & S_IRUSR) ? "r" : "-");
	    		printf( (st.st_mode & S_IWUSR) ? "w" : "-");
	    		printf( (st.st_mode & S_IXUSR) ? "x" : "-");
	    		printf( (st.st_mode & S_IRGRP) ? "r" : "-");
	    		printf( (st.st_mode & S_IWGRP) ? "w" : "-");
	    		printf( (st.st_mode & S_IXGRP) ? "x" : "-");
	    		printf( (st.st_mode & S_IROTH) ? "r" : "-");
	    		printf( (st.st_mode & S_IWOTH) ? "w" : "-");
	    		printf( (st.st_mode & S_IXOTH) ? "x" : "-");
	    		//number of links
	    		printf(" %ld",(long int)st.st_nlink);
	    		//user name
	    		printf(" %s",name->pw_name);
	    		//group name
	    		printf(" %s",group->pw_name);
	    		//size
	    		printf(" %ld",(long int) st.st_size);
	    		//time
	    		printf(" %s", ctime(&st.st_mtime));
	    		//file name
	    		printf(" %s",arg);
	    		printf("\n");	  
	    		        
	 		break;
	 	}
	}
}

int main(int argc, char *argv[]) {
	int opt;
	int i;
	int flag;
	if(getcwd(buf, FILENAME_MAX) == NULL){
		perror("getcwd error");
		exit(1);
	}

	if(argc > 1){
		while((opt = getopt(argc, argv, "aRl")) != -1) {			
			switch(opt) {
				case 'a':
				{
					ls_all_flag = 1;			
					break;
				}
				case 'l':
				{
					file_info_flag = 1;
					break;
				}
				case 'R':
				{
					ls_recursive_flag = 1;
					break;
				}
			}
		}
		if(ls_all_flag == 0 && file_info_flag == 0 && ls_recursive_flag == 0) {
			for(i = 1; i < argc; i++) {
				lsWithArg(argv[i]);
			}
		}
		else {
			for(i = 1; i < argc; i++) {
				if(*argv[i] != '-') {
					flag = 1;
					if(ls_all_flag == 1) {
						lsWithArg(argv[i]);
					}
		
					if(file_info_flag == 1) {
						file_info(argv[i]);
					}
		
					if(ls_recursive_flag == 1) {
						ls_recursive(argv[i]);
						if(chdir(buf) == -1) {
							perror("cant change current path");
							exit(1);
						}
					}
				}
			}
			if(flag == 0) {
				if(ls_all_flag == 1) {
					lsAll();
				}
	
				if(file_info_flag == 1) {
					file_info(buf);
				}
	
				if(ls_recursive_flag == 1) {
					ls_recursive(buf);
					if(chdir(buf) == -1) {
						perror("cant change current path");
						exit(1);
					}
				}
			}
		}
	}
	else {
		ls();
	}	

	return 0;
}
