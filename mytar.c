#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "mytar.h"
#include <unistd.h>

int main(int argc, char *argv[]){
	int i, file;
	static int c = 0, t = 0, x = 0, v = 0; f = 0, S = 0;
	archive arch;
	char path[PATH_MAX];

	/*read in options from argv[1]*/
	if (argv[1]){
		for(i = 0; i <strlen(argv[1]); i++){ 
			switch(argv[1][i]){
				case 'c':
					c = 1;
					break;
				case 't':
					t = 1;
					break;
				case 'x':
					x = 1;
					break;
				case 'v':
					v = 1;
					break;
				case 'f':
					f = 1;
					break;
				case 'S':
					S = 1;
					break;
				default:
					perror("Unsupported option");
					break;
			}
		}
	}
	else{
		perror("No options argument");
		exit(EXIT_FAILURE);
	}
	/*check options for next instruction*/
	if(f && argv[2] && c){
		file = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR, S_IWUSR);
	}
	else if(f && argv[2] && (t | x)){
		file = open(argv[2], O_RDONLY, S_IRUSR);
	}
	else{
		perror("No archive file");
		exit(EXIT_FAILURE);
	}
	if(S){
		/*do ustar stuff, only for t and x?*/
		/*if ustar is null terminated and version is '00': continue*/
		/*else exit?*/
	}
	if(c){
		if(argv[3]){
			arch = createArchive(argv[3]);
		}
		else{
	        perror("No path provided");	
		}
		/*write archive*/
		if(v){
			/*print to terminal as writing to archive*/
		}
	}
	else if(t){
		if(argv[3]){
			/*list contents of given archive*/
			createTable(argv[3], file);
		}
		else{
			/*list all files in archive*/
			if(getcwd(path, PATH_MAX) != NULL){
				createTable(path, file);
			}
			else{
				perror("path not found");
			}
		}
		/*print to terminal in createTable?*/
		if(v){
			/*print expanded info to terminal*/
		}
		else{
			/*print basic info to terminal*/
		}
	}
	else if(x){
		if(argv[3]){
			/*extract given path from archive*/
			if(v){
			/*print to terminal as extracting from archive*/
			}
		}
		else{
			/*extract all files from archive*/
			if(v){
			/*print to terminal as extracting from archive*/
			}
		}
	}
	close(file);
	return(0);
}
