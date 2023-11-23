#include "mytar.h"

int main(int argc, char *argv[]){
	int i, j, file;
	int c = 0, t = 0, x = 0, f = 0, v = 0, S = 0;
	char *pathNames[PATH_MAX];
	char *emptyList[1] = { NULL };

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
	if(c){
		if(argv[3]){
			if (strlen(argv[3]) > PATH_MAX){
				perror("Path is too long");
				exit(EXIT_FAILURE);
			}
			createArchive(argv[3], file, v, S);
		}
		else{
	        perror("No path provided");	
		}
	}
	else if(t){
		if(argv[3]){
			/*list contents of given paths*/
			for (i = 3; i < argc; i++){
				if (strlen(argv[i]) > PATH_MAX){
					perror("path is too long");
				}
				pathNames[i - 3] = (char *)malloc(PATH_MAX + 1);
				if (pathNames[i - 3] == NULL) {
					perror("Null pointer");
					for (j = 0; j < i - 3; j++) {
						free(pathNames[j]);
					}
					exit(EXIT_FAILURE);
				}
        		strcpy(pathNames[i - 3], argv[i]);
			}
			createTable(pathNames, file, argc - 3, v, S);
		}
		else{
			/*list all files in archive*/
			createTable(emptyList, file, argc - 3, v, S);
		}
	}
	else if(x){
		if(argv[3]){
			/*extract given paths from archive*/
			for (i = 3; i < argc; i++){
				if (strlen(argv[i]) > PATH_MAX){
					perror("path is too long");
				}
				pathNames[i - 3] = (char *)malloc(PATH_MAX + 1);
				if (pathNames[i - 3] == NULL) {
					perror("Null pointer");
					for (j = 0; j < i - 3; j++) {
						free(pathNames[j]);
					}
					exit(EXIT_FAILURE);
				}
        		strcpy(pathNames[i - 3], argv[i]);
			}
			extractArchive(pathNames, file, argc - 3, v, S);
		}
		else{
			/*extract all files from archive*/
			extractArchive(emptyList, file, 0, v, S);
		}
	}
    for (i = 0; i < argc - 3 && i < PATH_MAX; i++) {
        free(pathNames[i]);
    }
	close(file);
	return(0);
}
