#include <string.h>
#include <stdlib.h>
#include <stdio.h> // extern FILE *stdin
#include <stdint.h>
#include <unistd.h>
#include <stdarg.h>
#include "md5.h"

int NUMBER_OF_ACCOUNTS = 10;

uint8_t *import_db()
{
	FILE* fp;
	if(!(fp = fopen("secure.data", "rb")))
	{
		printf("\nError: secure.data database file not found\n");
		exit(0);
	}
	fseek(fp, 0, SEEK_END);
	int sz = ftell(fp);
	rewind(fp);
	uint8_t *buff = (uint8_t*)calloc((sz + 1), sizeof(*buff));
	fread(buff, sizeof(uint8_t), sz, fp);
	fclose(fp);
	return buff;
}

char *my_decrypt(uint8_t *enc_db)
{
	uint32_t xor_k      = *(uint32_t *)(enc_db);
	uint32_t data_sz    = *(uint32_t *)(enc_db + 4);
	uint8_t *start_data = enc_db + 8;
	for (int i = 0; i < data_sz; ++i)
	{
		start_data[i] ^= *(((uint8_t *)&xor_k) + (i % 4));
	}
	return (char *)start_data;
}

void mem_cleanup(uint32_t n, ...)
{
	va_list vl;

	va_start(vl, n);
	for (int i = 0; i < n; ++i) {
  		free(va_arg(vl, void *));
	}
	va_end(vl);
}

int main()
{
	char *user = (char *)calloc(21, sizeof(*user)); //Calloc makes sure the 21st byte is already a zero terminator
	char *pass = (char *)calloc(21, sizeof(*pass));
	char *hash = (char *)calloc(33, sizeof(*hash));
	char *array[NUMBER_OF_ACCOUNTS];
	int  win = 0;

	printf("Please Login\n");
	printf("User: ");
	fgets(user, 20, stdin);							   // 1 is stdin
	printf("Pass: ");
	fgets(pass, 20, stdin);

	/* Since fgets includes the \n in the buffer, 
	     and we need it to only include the raw password chars
	     we need to set the position the newline char is at to 0 */
	pass[strcspn(pass, "\n")] = 0x00;
	user[strcspn(user, "\n")] = 0x00;
	my_md5(pass, strlen(pass), hash);

	uint8_t *in = import_db();
	char *db = my_decrypt(in);

	char* pos = strtok(db, ";");
	int i = 0;
	while (pos)
	{
		array[i] = (char*)calloc(strlen(pos) + 1, sizeof(char));
		strcpy(array[i++], pos);
		pos = strtok(NULL, ";");
	}
	for (int j = 0; j < i; ++j)
	{
		if (strcmp(user, strtok(array[j], ":")) != 0)
			continue;
		if (strcmp(hash, strtok(NULL, ":")) == 0)
			printf("Correct!\n");
			win = 1;
	}

	if (win == 0)
		printf("Wrong password or username.\n");
	
	
    // Cleanup
	for (int j = 0; j <= NUMBER_OF_ACCOUNTS; ++j)
	{
		if (array[j])
			free(array[j]);
	}

	mem_cleanup(3, user, pass, hash);
	return 0;
}