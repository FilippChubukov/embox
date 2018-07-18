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

int symbol(char *name)
    {
       int   iChr = 0;
       int   iCnt = 0;
     
      
     
       FILE*    pIn = fopen(name,"rt");
     
       if (!pIn)
       {
          // Error !
          return 0;
       }
     
       while ((iChr = fgetc(pIn)) != EOF)
       {      
          ++iCnt;
       }
     
       fclose(pIn);
     
       printf("s total: %d\n",iCnt);
     
       return 0;
    }

    int words(char *name)
    {
       int   iChr = 0;
       int   iCnt = 0;
     
       bool  bPrev = false;
       bool  bNext = false;
     
       FILE*    pIn = fopen(name,"rt");
     
       if (!pIn)
       {
          // Error !
          return 0;
       }
     
       while ((iChr = fgetc(pIn)) != EOF)
       {      
          bNext = !isspace(iChr)  ?  true  :  false;
     
          if (bNext)
          {
             bPrev = true;
          }
     
          if (bPrev && !bNext)
          {
             bPrev = false;
     
             ++iCnt;
          }
       }
     
       fclose(pIn);
     
       printf("Words total: %d\n",iCnt);
     
       return 0;
    }

static int str( char *name ) {
    FILE* in = fopen( name, "r" );
 
    if ( in == NULL ) {
        perror( "Ошибка при открытии исходного файла" );
        return 0;
    }
 
    size_t counter = 0; 
    int ch, pre = EOF; 
 
 
    
    while ( ( ch = fgetc( in ) ) != EOF ) { 
        pre = ch; 
 
        if ( ch == '\n' ) 
            ++counter; 
 
    }
 
    
    // последняя строка файла может заканчиваться не \n, а строку посчитать надо - вот для этого и нужна переменная pre
 
    if ( pre == EOF ) // если файл пустой
        return 0; // выводим информацию об этом
    else if ( pre != '\n'  ) { // если pre содержит в себе не перевод строки, и файл не пустой
        ++counter; // то увеличиваем счетчик строк
        puts( "" );
    }
 
    puts( "" );
    // выводим на экран информацию о количестве строк
   
    return counter;
}




int main(int argc, char **argv) {
	char *command;

	if (argc == 0) {
		return -EINVAL;
	}

	
	command = argv[1];
	argv[argc] = NULL;
	printf("Starting service: %s\n", command);
	printf("%d\n",str(command));
	words(command);
	symbol(command);
	
	return 0;
}
