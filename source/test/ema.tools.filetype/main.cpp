
#include "windows.h"








int main(int argc, char** argv)
{

	if(argc!=2)
		return -1;


	HANDLE file = CreateFileA(argv[1], GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if(file == INVALID_HANDLE_VALUE)
		return -2;

	const DWORD bufferSize=1024;
	DWORD reallyReaded = 0;
	DWORD reallyWrited = 0;
	char buffer[bufferSize];


	if(!ReadFile(file, buffer, bufferSize, &reallyReaded, NULL))
		return -3;


	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	if(!output)
		return -4;


	if(!WriteFile(output, buffer, reallyReaded, &reallyWrited, NULL))
		return -5;


	return 0;
}