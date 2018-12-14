#include "Records.h"



Records::Records()
{
}

Record* Records::GetRecords(int* count)
{
	Record* records;
	FILE *file;
	file = fopen("records.txt", "r");
	fscanf(file, "%d\n", count);
	records = (Record*)malloc(sizeof(Record)*(*count)+1);
	for (int i = 0; i < (*count); i++)
	{
		int nameLength;
		fscanf(file, "%d\n", &nameLength);
		records[i].name = (char*)malloc(sizeof(char) * nameLength + 1);
		fscanf(file, "%s\n", records[i].name);
		fscanf(file, "%d\n", &records[i].coins);
	}
	fclose(file);
	return records;
}

void Records::SetNewRecord(Record newRecord)
{ 
	int count;
	Record* records = GetRecords(&count);
	for (int i = 0; i <= count; i++)
	{
		if ((i == count)||(records[i].coins < newRecord.coins))
		{
			for (int j = count;i < j; j--)
			{
				records[j] = records[j - 1];
			}
			records[i] = newRecord;
			break;
		}
	}
	count++;
	FILE *file;
	file = fopen("records.txt", "w");
	fprintf(file, "%d\n", count);
	for (int i = 0; i < count; i++)
	{
		fprintf(file, "%d\n", strlen(records[i].name));
		fprintf(file, "%s\n", records[i].name);
		fprintf(file, "%d\n", records[i].coins);
	}
	fclose(file);
}

void Records::Delete(Record* records, int count)
{
	for (int i = 0; i < count; i++)
	{
		delete records[i].name;
	}
	delete records;
}

Records::~Records()
{
}
