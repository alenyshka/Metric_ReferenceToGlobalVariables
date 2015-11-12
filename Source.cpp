#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include < stdio.h > 
#include < string.h > 
#include <conio.h>
#define LENGHT_OF_STRING 100
#define MAX_COUNT_OF_BLOCKS 50
#define POS 2

int Count_Appear(char *string, FILE *file, int Aup, int  array_position[MAX_COUNT_OF_BLOCKS][POS], int count_block);
int Appear_the_Global_Variable(char *string, FILE *file, int Aup, int  array_position[MAX_COUNT_OF_BLOCKS][POS], int count_block,
								int count_Glvariable);

int main()
{
	FILE *source_java;
	char string[LENGHT_OF_STRING];
	int array_position[MAX_COUNT_OF_BLOCKS][POS];

	int count_gloval_variable = 0, count_bracket = 0, i, lenght_string = 0, count_string = 0, start_position, 
		finish_position, count_block = 0;
	int Aup = 0, Pup = 0;
	float Rup = 0;
	bool flag_bracket = false, flag_block = true, flag_variable;
	if (!(source_java = fopen("source_java.java", "r")))
	{
		printf("The file don't found.\n");
		_getch();
		return 0;
	}
	count_string = 0;
	while (!feof(source_java))
	{

		while (flag_block)
		{
			flag_variable = false;
			fgets(string, LENGHT_OF_STRING, source_java);
			count_string++;

			lenght_string = strlen(string);
			for (i = 0; i < lenght_string; i++)
			{
				if ((string[i - 1] == ')') && (string[i] == '\n'))
				{
					flag_block = true;
					flag_bracket = true;
					start_position = count_string;
					array_position[count_block][0] = start_position;
				}
				else
					flag_block = 0;
			}
			while ((flag_bracket == true) && !feof(source_java))
			{
				fgets(string, LENGHT_OF_STRING, source_java);
				count_string++;
				lenght_string = strlen(string);
				for (i = 0; i < lenght_string; i++)
				{
					if (*(string + i) == '{')
						count_bracket++;
					if (*(string + i) == '}')
						count_bracket--;
				}
				if (count_bracket == 0)
				{
					flag_bracket = false;
					finish_position = count_string;
					array_position[count_block][1] = finish_position;
					count_block++;
					flag_variable = true;
				}

			}
			if (feof(source_java))
				flag_block = false;
			else
				flag_block = true;
		}
	}

	char search_string[] = "public static";
	const int end_string = 2;
	fseek(source_java, 0, SEEK_SET);
	while (!feof(source_java))
	{
		fgets(string, LENGHT_OF_STRING, source_java);
		lenght_string = strlen(string);
		if (strstr(string, search_string) != NULL)
		{
			if (*(string + lenght_string - end_string) == ';')
				count_gloval_variable++;
		}
	}

	Aup = Appear_the_Global_Variable(string, source_java, Aup, array_position, count_block, count_gloval_variable);

	Pup = count_gloval_variable * count_block;
	Rup = (float)Aup / (float)Pup;
	printf("\tThe count global variable = %d\n", count_gloval_variable);
	printf("----------------------------------------------------\n");
	printf("The value Aup = %d\n", Aup);
	printf("The value Pup = %d\n", Pup);
	printf("The  value Rup = Aup/Pup = %0.3f\n", Rup);
	_getch();
	return 0;
}
int Appear_the_Global_Variable(char *string, FILE *source_java, int Aup, int  array_position[MAX_COUNT_OF_BLOCKS][POS], int count_block, int count_Glvariable)
{
	int lenght_string = 0, count = 0, count_str = 0;
	const int end_string = 2;
	bool flag = true;
	char search_string[] = "public static";
	fseek(source_java, 0, SEEK_SET);
	while (!feof(source_java) && (count_Glvariable > count))
	{
		if (flag == true)
		{
			fseek(source_java, 0, SEEK_SET);
			for (int i = 0; i < count_str; i++)
				fgets(string, LENGHT_OF_STRING, source_java);
		}
		flag = false;
		fgets(string, LENGHT_OF_STRING, source_java);
		count_str++;
		lenght_string = strlen(string);
		if (strstr(string, search_string) != NULL)
		{
			if (*(string + lenght_string - end_string) == ';')
			{
				count++;
				Aup += Count_Appear(string, source_java, Aup, array_position, count_block);
				flag = true;
				//puts(name_variable);
			}
		}
	}
	return Aup;
}
int Count_Appear(char *string, FILE *source_java, int Aup, int  array_position[MAX_COUNT_OF_BLOCKS][POS], int count_block)
{
	int lenght_of_string, i, start_position, finish_position, j = 0, k = 0;
	char *name;
	const int end_string = 2;
	bool flag = true;
	Aup = 0;
	lenght_of_string = strlen(string);
	for (i = lenght_of_string; i > 0; i--)
	{
		if (string[i] == '=')
		{
			finish_position = i - end_string;
			break;
		}
	}
	for (i = finish_position; i > 0; i--)
	{
		if (string[i] == ' ')
		{
			start_position = i + 1;
			break;
		}

	}
	const int lenght_name = finish_position - start_position + end_string;
	name = (char*)calloc(lenght_name, sizeof(char));
	for (i = 0; i < lenght_name; i++)
	{
		if (string[start_position + i] == ' ')
			break;
		name[i] = string[start_position + i];

	}
	while (j < count_block)
	{
		flag = true;
		fseek(source_java, 0, SEEK_SET);
		for (i = 0; i < array_position[j][0]; i++)
			fgets(string, LENGHT_OF_STRING, source_java);
		while (!feof(source_java) && (i < array_position[j][1]) && (flag == true))
		{
			if (strstr(string, name) != NULL)
			{
				Aup++;
				flag = false;
			}
			i++;
			fgets(string, LENGHT_OF_STRING, source_java);
		}
		j++;
	}
	return Aup;
}