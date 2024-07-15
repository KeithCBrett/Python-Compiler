#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	char *mystring;
	int BUFFER = 100;
	mystring = malloc(sizeof(char) * BUFFER);
	scanf("%s", mystring);
	printf("you entered \n%s", mystring);

	return 0;
}
