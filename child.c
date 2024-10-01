#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main (){
	int a1, a2;
	int c, u;
	while(read(fileno(stdin), &c, sizeof(int)) != 0) {
		if ((c != 'a') && (c != 'e') && (c != 'i') && (c != 'u') && (c != 'y') && (c != 'o') && 
		(c != 'A') && (c != 'E') && (c != 'I') && (c != 'U') && (c != 'Y') && (c != 'O')) {
			u = (char) c;
			write(fileno(stdout), &c, sizeof(char));
		}
	}	
	close(fileno(stdin));
}