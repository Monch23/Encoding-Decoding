#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>


char* encode(char *str) {
	int length = strlen(str);
	char *encoded = (char*)malloc(length);
	int k = 0;

    for (int i = 0; str[i]; ++i) {
    	if (str[i] > 64 && str[i] < 91) {
        	encoded[k++] = str[i] - 23;
        } else {
            encoded[k++] = str[i] + 4;
        }
    }

    return encoded;
}

char* decode (char *str) {
	int length = strlen(str);
	char *decoded = (char*)malloc(length);
	int k = 0;

    for (int i = 0; str[i]; ++i) {
    	if (str[i] > 41 && str[i] < 91) {
            decoded[k++] = str[i] + 23;
        } else {
			decoded[k++] = str[i] - 4;
        }
    }
        
	return decoded;
}


int main(int argc, const char *argv[]) {
	int fd = open(argv[1], O_CREAT | O_RDWR , S_IRUSR | S_IWUSR);
	char *codedMsg = encode("Hello From Afganistan !");
	int length = strlen(codedMsg);
	pid_t pid = fork();

	if (pid > 0) { // parent
		write(fd, codedMsg, length);

		fsync(fd);
		usleep(30000);
		close(fd);
		kill(pid, SIGCONT);
		wait(NULL);
	} else if (!pid) { // child
		int fd2 = open(argv[1], O_RDONLY);
		char tmpBuff[100];

		int rd = read(fd2, tmpBuff, 100);
		tmpBuff[rd] = '\0';
		char *decodedMsg = decode(tmpBuff);
		printf("%s \n", decodedMsg);
		close(fd2);
	}
	
	return 0;
}
