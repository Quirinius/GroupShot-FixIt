#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef struct { unsigned long offset; size_t size; char data[]; } patch;
size_t modify(FILE *f, const patch *p);

const char filename[] = "MSRGS.exe";
const int filesize = 1355776;

const patch p00001D97 = {0x00001D97, 1, {0xEB}};

const patch *modif[] = {&p00001D97};

int main(int  argc, char *argv[])
{
	FILE *f;
	int i;
	long size;

	printf("Patch is for MS Research GroupShot 1.03\n\n");
	

	f = fopen(filename, "rb+");
	if (!f )
	{
		fprintf(stderr, "Unable to open '%s' for writing!\n\n", filename);
		return EXIT_FAILURE;
	}

	fseek(f, 0L, SEEK_END);
	size = ftell(f);

	if (size != filesize)
	{
		fprintf(stderr, "Wrong file size.", filename);
		return EXIT_FAILURE;
	}


	for(i=0; i < (sizeof(modif)/sizeof(patch*)); i++)
	{
		printf("Patching...step %i...", i);
		if (modify(f, modif[i]) == modif[i]->size)
		{
			printf("ok!\n");
		}
		else
		{
			printf("error! (file may be corrupt)\n");
		}
	}
	fclose(f);

	return EXIT_SUCCESS;
}



size_t modify(FILE *f, const patch *p)
{
	fseek(f, p->offset, SEEK_SET);
	if (errno)
	{
		return 0;
	}
	else 
	{
		return fwrite(p->data, 1, p->size, f);
	}
}
