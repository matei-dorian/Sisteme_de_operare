#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv){

	/// write(1, "Hello World!\n", 13*sizeof(char)) -- pentru a

	const int N = 4096;
 	int fds, fdd;	
	char *txt = (char*) malloc(N * sizeof(char)); 
 	int cnt = 0; /// counter of bytes read

	if(argc <= 1){
		write(1, "Apelare gresita\n", 100*sizeof(char));
		return -1;
	}

	///deschidem fisierul sursa	
	fds = open(argv[1], O_RDONLY);

	if(fds < 0){
		write(1, "fisierul nu exista\n", 100*sizeof(char));
		return -1;
	} 
	
	///deschidem fisierul destinatie -> daca nu exista il cream
	fdd = open(argv[2], O_WRONLY | O_TRUNC | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);

	/// citim cate N bytes din src
	do{	
		cnt = read(fds, txt, N); 
		write(fdd, txt, cnt * sizeof(char));
		
	} while(cnt > 0);
	
	free(txt);
	close(fds);
	close(fdd);
	return 0;
}
