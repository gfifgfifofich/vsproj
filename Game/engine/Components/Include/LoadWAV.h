#pragma once
#include "AL/alhelpers.h"


int ReadFrom4(std::ifstream* ifs)// reads 4 bytes from ifstring as integer
{
	char* data = new char[4];
	ifs->read(data, 4);

	unsigned int aboba = 0;
	memcpy(&aboba, data, 4);

	delete[] data;
	return aboba;
}
short ReadFrom2(std::ifstream* ifs)// reads 2 bytes from ifstring as short
{
	char* data = new char[2];
	ifs->read(data, 2);

	unsigned short aboba = 0;
	memcpy(&aboba, data, 2);
	delete[] data;
	return aboba;
}
