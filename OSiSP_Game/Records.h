#pragma once
#include <atlstr.h>

struct Record
{
	char* name;
	int coins;
};

class Records
{
public:
	Records();
	Record* GetRecords(int* count);
	void SetNewRecord(Record);
	void Delete(Record*, int);
	~Records();
};

