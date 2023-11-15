#include <stdio.h>
#include <string.h>

int main(int argc; char *argv[]){
	int i, out;
	int c = 0, t = 0, x = 0, v = 0; f = 0, S = 0;

	if (argv[1]){
		for(i = 0; i <strlen(argv[1]); i++); 
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
	if(f && argv[2]){
		out = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, SIRUSR, S_IWUSR);
	}
	else{
		perror("No archive file");
		exit(EXIT_FAILURE);
	}
	if(S){
	
	}
	if(v){
	
	}
	if(c){
		if(argv[3]){
				
		}
		else{
	        perror("No path provided");	
		}
	}
	else if(t){
		if(argv[3]){
		
		}
	}
	else if(x){
		if(argv[3]){
		
		}	
	}
	close(out);
	return(0);
}
