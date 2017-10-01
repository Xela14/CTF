#include <stdio.h>
#include <stdint.h>

char data[] = "Miamoto:3fc7dd234f89784404859cd365015732
				;D4rkE13ment:7ac3b3e9a6ca17ad69ba39eac0b623e2
				;0xACEDBEEF:aef2830b46fb23d87d1f55951aaa089b
				;dan:6f01133adcef43373a8d611073ee5a8b
				;admin:5ae9b7f211e23aac3df5f2b8f3b8eada
				;rugr4t:436a93f8906ff8c9e7a3e11ec95d2daf
				;kuroneko-ruri:cb0187a36a08fdb9d9ba0a20ec15c90b
				;";


uint32_t xork = 0x3790ACE1;
uint32_t sz   = sizeof(data);

int main()
{

	char enc[sizeof(data) + 1];
	for (int i = 0; i <= sizeof(data); ++i)
	{
		printf("enc[%d] = %02x ^ %02x\n", i, data[i], (*(((uint8_t *)&xork) + (i % 4))));
		enc[i] = data[i] ^ (*(((uint8_t *)&xork) + (i % 4)));
	}

	FILE *f = fopen("secure.data", "ab");
	fwrite(&xork ,4 , 1, f);				// Write the xorkey to the first 4 bytes
	fwrite(&sz ,4 , 1, f);					// Write the length of the actual data to the second 4 bytes
	fwrite(enc, 1, sizeof(enc), f);			// Write the encrypted data 
	fclose(f);	
	return 0;
}