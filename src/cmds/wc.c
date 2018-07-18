/**
 * @file
 * @brief Starts services
 *
 * @date 16.04.16
 * @author Alexander Kalmuk
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <stdbool.h>

static int max_string(char *name) {
	int   byte_file = 0;
	int   count = 0;

	int max_len = 0;
       
	FILE*    file_in = fopen(name,"rt");
     
	if (!file_in) {
		printf("Error with opening file\n");
		return 0;
	}
     
	while ((byte_file = fgetc(file_in)) != EOF) {      
		if (byte_file != '\n') {
			if (byte_file == '\t') {
				count = count + 8; 

			} else {
				count++;
			}

		} else {

		if (max_len < count) {
			max_len = count;
		}
		count = 0;
		}
	}

	fclose(file_in);
     
	return max_len;
}


static int number_of_bytes(char *name) {
	int   byte_file = 0;
	int   count = 0;
     
	FILE*    file_in = fopen(name,"rt");
     
	if (!file_in) {
		printf("Error with opening file\n");
		return 0;
	}
     
	while ((byte_file = fgetc(file_in)) != EOF) {      
		count++;
	}
     
	fclose(file_in);
     
	return count;
}

static int number_of_words(char *name) {
	int   byte_file = 0;
	int   count = 0;
     
	bool  prev_symbol = false;
	bool  next_symbol = false;
     
	FILE*    file_in = fopen(name,"rt");
     
	if (!file_in) {
		printf("Error with opening file\n");
		return 0;
	}
     
	while ((byte_file = fgetc(file_in)) != EOF) {      
		next_symbol = !isspace(byte_file)  ?  true  :  false;
     
		if (next_symbol) {
             		prev_symbol = true;
          	}
     
          	if (prev_symbol && !next_symbol){
                	bPrev = false;
    			count++;
          	}
	}
     
	fclose(file_in);
     
	return count;
}

static int number_of_strings(char *name) {
	int   byte_file = 0;
	int   prev_byte = 0;
	int   count = 0;
	
	FILE*    file_in = fopen(name,"rt");
 
	if (!file_in) {
		printf("Error with opening file\n");
		return 0;
	}

 	while ((byte_file = fgetc(file_in)) != EOF ) { 
		prev_byte = byte_file; 
 
        	if ( ch == '\n' ) { 
            		counter++; 
		}
	}

	if ( pre == EOF ) { // если файл пустой
        	return 0; 
	} else if ( pre != '\n'  ) { 
		counter++; 
	}
   
        return counter;
}




int main(int argc, char **argv) {
	char *file_name;
	int opt, i;
	i=0;

	static bool print_lines, print_words,print_bytes,print_chars;
	static bool print_linelength;
	
	print_lines = print_words = print_chars = print_bytes = false;
  	print_linelength = false;

	if (argc == 0) {
		return -EINVAL;
	}
	printf("%dit\n", i);
	while (-1 != (opt = getopt(argc, argv, "lLcmw"))) {
		switch(opt) {
			case 'l':
			print_lines = true;
			i++;
			break;

			case 'L':
			print_linelength = true;
			i++;
			break;
                        
			case 'c':
			print_bytes = true;
			i++;
			break;

			case 'm':
			print_chars = true;
			i++;
			break;

			case 'w':
			print_words = true;
			i++;
			break;
			
		}
	}

	if (! (print_lines || print_words || print_chars || print_bytes
         || print_linelength))
		print_lines = print_words = print_bytes = true;
	
	for (i = i; argc -1; i++) {
		file_name = argv[i];
		
		


	argv[argc] = NULL;
	
	printf("%d\n",str(file_name));
	words(file_name);
	bytes(file_name);
	len(file_name);
	
	return 0;
}
