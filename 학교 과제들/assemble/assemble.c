#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h> 
#include <stdlib.h> 
#include <string.h> 
#define max 1024

typedef struct FormalArgumentTable { // ��ũ�� �̸�ǥ
	int	index;
	char name[10];
}FormalArgumentTable;

typedef struct MacroDefineTable { //��ũ�� ����ǥ
	int	index;
}MacroDefineTable;

//MNT 
typedef struct MacroNameTable { // ��ũ�� �̸�ǥ
	int	index;
	char name[10];
	int	MDTindex;
}MacroNameTable;

MacroNameTable mnt[20];
MacroDefineTable mdt[20];
FormalArgumentTable fat[10];

int Put_MDT(char* s, FILE* p);		//MDT�� �Է�	
int Put_MNT(char* s, int x, FILE* p);		//MNT�� �Է�
int Put_FAT(char* s, FILE* p); // FAT �Է�

void Eliminate(char* str, char ch); // Ư�� ���ڿ� ����
int findString(const char str2[], const char str1[]); // Ư�� ���ڿ� ���� ���� Ȯ��
char* replaceWord(const char* s, const char* oldW, const char* newW); //Ư�� ���ڿ� ��ȯ

void Pass1() {

	FILE* fp;
	fp = fopen("macro2.asm", "r");			//�б� �������� ������ �о�帮�� ���� ���

	FILE* MNT;
	MNT = fopen("MNT.txt", "w");
	if (MNT == NULL) {			//���о����� ����ó��
		printf("MNT �����б� ����\n");
		return 1;
	}

	FILE* MDT;
	MDT = fopen("MDT.txt", "w");
	if (MDT == NULL) {			//���о����� ����ó��
		printf("MDT �����б� ����\n");
		return 1;
	}

	FILE* FAT;
	FAT = fopen("FAT.txt", "w");
	if (FAT == NULL) {			//���о����� ����ó��
		printf("FAT �����б� ����\n");
		return 1;
	}

	char line[max];

	int token = 0;			//������ ��ū���� �� ���� 1�̸� macro�� ����� �κ�

	int MNTC = 1;
	int MDTC = 1;

	int param_num = 0;  // ��ũ�� �Ķ���͸� �����ϱ� ���� ����

	if (fp == NULL) {			//���о����� ����ó��
		printf("�����б� ����\n");
		return 1;
	}

	while (fgets(line, max, fp) != NULL)			//line�� ���پ� �о�� ���������� �ݺ���Ŵ
	{

		if (token == 0)		//macro�� ����������ʴٸ�
		{
			if (strstr(line, "MACRO"))			//macro��� �ܾ ������ ������ 
			{
				printf("MACRO CHECK ! %d\n",MDTC);
				token = 1;

				char line2[max];
				strcpy(line2, line);

				mnt[MNTC].MDTindex = MDTC; // MNT ���
				mnt[MNTC].index = MNTC;

				char* ptr = strtok(line2, " ");      // " " ���� ���ڸ� �������� ���ڿ��� �ڸ�, ������ ��ȯ
				int line_count = 0;

				while (ptr != NULL)               // �ڸ� ���ڿ��� ������ ���� ������ �ݺ�
				{
					switch (line_count)  // ���� �����Ͽ� �����μ�ǥ�� ����
					{
					case 0: // ��ũ�� �̸� �κ�
						strcpy(mnt[MNTC].name, ptr);
						break;
					case 1: // MACRO �κ�
						break;
					default: // ��ũ�� �Ķ���� �κ�
						Eliminate(ptr, ',');
						fat[param_num].index = param_num;
						strcpy(fat[param_num].name, ptr);
						param_num++;
						break;
					}

					ptr = strtok(NULL, " ");// ���� ���ڿ��� �߶� �����͸� ��ȯ
					line_count++;
				}

				char FAT_result[max] = "MNT NUMBER : ";  // FAT �Է�
				char mntIndex[10];

				sprintf(mntIndex, "%d", MNTC);
				strcat(FAT_result, mntIndex);
				strcat(FAT_result, "\n");

				for (int i = 0; i < param_num; i++) {
					char fatContent[max];
					strcpy(fatContent, fat[i].name);
					char fatIndex[10];

					sprintf(fatIndex, "%d", fat[i].index);

					strcat(fatContent, " ");
					strcat(fatContent, fatIndex);
					strcat(fatContent, "\n");

					strcat(FAT_result, fatContent);
				}


				Put_FAT(FAT_result, FAT);

				char MNT_result[max];  // MNT �Է�
				char mdtIndex[10];

				sprintf(MNT_result, "%d", mnt[MNTC].index);
				sprintf(mdtIndex, "%d", mnt[MNTC].MDTindex);

				strcat(MNT_result, " ");
				strcat(MNT_result, mnt[MNTC].name);
				strcat(MNT_result, " ");
				strcat(MNT_result, mdtIndex);
				strcat(MNT_result, "\n");

				Put_MNT(MNT_result, MNTC, MNT);

				char MDT_result[max]; // MDT MACRO ���� �Է�

				sprintf(MDT_result, "%d", mnt[MNTC].MDTindex);
				strcat(MDT_result, "   ");
				strcat(MDT_result, line);

				Put_MDT(MDT_result, MDT);
				MNTC += 1;
			}
			MDTC++;
		}
		else {
			//macro�� ����Ǿ��ִٸ� ��� �Է�
			char line2[max];
			strcpy(line2, line);
			char* ptr = strtok(line2, " ");

			while (ptr != NULL)               // ���� �μ�ǥ�� ������ �˻�
			{
				for (int i = 0; i < param_num; i++) {
					if (findString(ptr, fat[i].name) == 0) { // ���� �� �����μ�ǥ�� ������ �μ� ã��

						char* result;

						char in_num[100] = "#";
						int test_num = fat[i].index;
						char fat_cnum[10];

						sprintf(fat_cnum, "%d", test_num);
						strcat(in_num, fat_cnum);

						result = replaceWord(line, fat[i].name, in_num);
						strcpy(line, result);

					}
				}

				ptr = strtok(NULL, " ");
			}


			if (strstr(line, "ENDM"))			//ENDM��� �ܾ ������ ������ 
			{
				char MDT_result[max];

				sprintf(MDT_result, "\n%d", MDTC);
				strcat(MDT_result, "   ");
				strcat(MDT_result, line);

				printf("ENDM Check! \n");
				token = 0;

				Put_MDT(MDT_result, MDT);
			}
			else {                            // ��ũ�� �߰� �κ� �ֱ�
				char MDT_result[max];

				sprintf(MDT_result, "%d", MDTC);
				strcat(MDT_result, "   ");
				strcat(MDT_result, line);

				printf("MDT_RESULT : %s", MDT_result);

				Put_MDT(MDT_result, MDT);
			}
			MDTC++;
		}
		//printf("%s", line);
	}
	printf("����");
	fclose(fp);
	fclose(MNT);
	fclose(MDT);
	fclose(FAT);
	return 0;
}

int main(void)
{
	Pass1();


}
int Put_MDT(char* s, FILE* p)		//mdt�� �ִ� �κ�
{
	fputs(s, p);
	return 0;
}
int Put_MNT(char* s, int x, FILE* p)		//mdt�� �ִ� �κ�
{
	fputs(s, p);
	return 0;
}
int Put_FAT(char* s, FILE* p)		//mdt�� �ִ� �κ�
{
	fputs(s, p);
	return 0;
}

void Eliminate(char* str, char ch) // ���� ����
{
	for (; *str != '\0'; str++)
	{
		if (*str == ch)
		{
			strcpy(str, str + 1);
			str--;
		}
	}
}

int findString(const char str2[], const char str1[]) { // Ư�� ���� ���� ����
	int i;
	bool ok = false;

	for (i = 0; str2[i] != '\0' && !ok; i++) {
		ok = true;
		for (int j = 0; str1[j] != '\0' && ok; j++) {
			if (str2[i + j] != str1[j]) ok = false;
		}
	}
	return ok ? i - 1 : -1;
}

char* replaceWord(const char* s, const char* oldW,  // Ư�� ���ڿ� ��ü�ϱ�
	const char* newW)
{
	char* result;
	int i, cnt = 0;
	int newWlen = strlen(newW);
	int oldWlen = strlen(oldW);

	for (i = 0; s[i] != '\0'; i++) {
		if (strstr(&s[i], oldW) == &s[i]) {
			cnt++;

			i += oldWlen - 1;
		}
	}

	result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);

	i = 0;
	while (*s) {
		if (strstr(s, oldW) == s) {
			strcpy(&result[i], newW);
			i += newWlen;
			s += oldWlen;
		}
		else
			result[i++] = *s++;
	}

	result[i] = '\0';
	return result;
}