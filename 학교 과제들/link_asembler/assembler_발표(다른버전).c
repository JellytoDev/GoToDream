#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h> 
#include <stdlib.h> 
#include <string.h> 
#define max 1024

int Put_SLT(char* s, FILE* p);		//MDT�� �Է�	
int Put_ST(char* s, FILE* p);		//MNT�� �Է�
int Put_FAT(char* s, FILE* p); // FAT �Է�

void Eliminate(char* str, char ch); // Ư�� ���ڿ� ����
int findString(const char str2[], const char str1[]); // Ư�� ���ڿ� ���� ���� Ȯ��
char* replaceWord(const char* s, const char* oldW, const char* newW); //Ư�� ���ڿ� ��ȯ
const char* Analyze(char* operand); // ��������, ���׸�Ʈ ��������, �޸� ����

void hextobin(char* hex,char* result) { // 16���� -> 2����
	printf("hex : %s\n", hex);

	int i, h, len = 2, buf[8];
	char bit[50] = { 0x00 };
	while (len--) {
		printf("%c: ", *hex);
		h = *hex > 96 ? *hex++ - 87 : *hex++ - 48;
		for (i = 7; i >= 0; i--) {
			buf[i] = h % 2;
			h = h / 2;
		}
		for (i = 4; i < 8; i++) {
			char b[20] = {0};
			sprintf(b, "%d", buf[i]);
			
			strcat(bit, b);
		}
		printf("bit : %s\n", bit);
	}

	strcat(result, bit);
}

typedef struct SignLineTable { //��ũ�� ����ǥ
	char line[100];
	int index;
}SignLineTable;

typedef struct SignTable { //��ũ�� ����ǥ
	char name[100];
	int index;
	char value[100];
	int size;
}SignTable;

typedef struct Opcode {
	char name[100][100];
	char asemble[100][100];
	char code[100][100];
	char equal[100][100];
	char w1[10];
	char w2[10];
}Opcode;

/*typedef struct Register{

}Register;*/

Opcode MOV;
Opcode ADD;

SignLineTable slt[max];
SignTable st[max];

char sign_word[100][100];
int sign_count = 0;
int sign_w_count = 0;
int sign_line_count = 0;

void SignWordCheck() {
	FILE* fp;
	fp = fopen("asemble.asm", "r");			//�б� �������� ������ �о�帮�� ���� ���

	if (fp == NULL) {			//���о����� ����ó��
		printf("�����б� ���� assemble \n");
	}

	char line[max];

	while (fgets(line, max, fp) != NULL)			//line�� ���پ� �о�� ���������� �ݺ���Ŵ
	{
		char line2[max];
		strcpy(line2, line);

		char* ptr = strtok(line2, " ");      // " " ���� ���ڸ� �������� ���ڿ��� �ڸ�, ������ ��ȯ
		int line_count = 0;

		char line_word[100][100];

		while (ptr != NULL)               // �ڸ� ���ڿ��� ������ ���� ������ �ݺ�
		{
			strcpy(line_word[line_count], ptr);

			if (strstr(ptr, "DW") != NULL || strstr(ptr, "EQU") != NULL) {
				printf("line2 : %s\n", line);

				int sign_location = line_count - 1;
				strcpy(sign_word[sign_w_count], line_word[sign_location]);

				printf("sign word : %s\n", sign_word[sign_w_count]);
				sign_w_count++;

			}
			line_count++;
			ptr = strtok(NULL, " ");// ���� ���ڿ��� �߶� �����͸� ��ȯ
		}

	}

	printf("sign Check ����\n");
	fclose(fp);
}

void Pass1() {

	int LC = 0;

	SignWordCheck();

	FILE* fp;
	fp = fopen("asemble.asm", "r");			//�б� �������� ������ �о�帮�� ���� ���

	FILE* SLT;
	SLT = fopen("SLT.txt", "w");
	if (SLT == NULL) {			//���о����� ����ó��
		printf("SLT �����б� ����\n");
		//return 1;
	}

	FILE* ST;
	ST = fopen("ST.txt", "w");
	if (ST == NULL) {			//���о����� ����ó��
		printf("ST �����б� ����\n");
		//return 1;
	}

	if (fp == NULL) {			//���о����� ����ó��
		printf("assemble �����б� ����\n");
		//return 1;
	}

	char line[max];


	while (fgets(line, max, fp) != NULL)			//line�� ���پ� �о�� ���������� �ݺ���Ŵ
	{
		char line2[max];
		strcpy(line2, line);

		int line_count = 0;
		char line_word[100][100];

		char* ptr = strtok(line2, " ");      // " " ���� ���ڸ� �������� ���ڿ��� �ڸ�, ������ ��ȯ

		int token = 0; // ��ȣ ���� �κ� �ν� ����

		while (ptr != NULL)               // �ڸ� ���ڿ��� ������ ���� ������ �ݺ�
		{
			strcpy(line_word[line_count], ptr);
			
			if (token == 1) {
				strcpy(st[sign_count].value, ptr);
				//printf("st %d : %s %s\n", sign_count, st[sign_count].name, st[sign_count].value);

				sign_count++;
			}

			if (strstr(ptr, "DW") != NULL || strstr(ptr, "EQU") != NULL || strstr(ptr, "DB") != NULL) {
				//printf("��ȣ���� %s\n", line_word[line_count - 1]);

				strcpy(st[sign_count].name, line_word[line_count - 1]);
				st[sign_count].index = LC;		
				
				
				if (strstr(ptr, "DW") != NULL) {
					st[sign_count].size = 2;
				}
				else if (strstr(ptr, "DB") != NULL) {
					st[sign_count].size = 1;
					LC--;
				}


				char ST_result[max] = "";  // ST �Է�
				char STindex[10];

				sprintf(STindex, "%d", LC);

				strcat(ST_result, st[sign_count].name);
				strcat(ST_result, " ");
				strcat(ST_result, STindex);
				strcat(ST_result, " ");
				strcat(ST_result, "1\n");

				printf("ST_RESULT : %s\n", ST_result);

				Put_ST(ST_result,ST);
				
				token = 1;
			}
			

			for (int i = 0; i < sign_w_count; i++) {
				if (strstr(ptr, sign_word[i]) != NULL && strstr(ptr, "ASSUME") == NULL) {
					//printf("sign line : %s\n", line);

					strcpy(slt[sign_line_count].line, line);
					slt[sign_line_count].index = LC;

					char SLT_result[max];  // SLT �Է�

					sprintf(SLT_result, "%d", LC);
					strcat(SLT_result, " ");
					strcat(SLT_result, line);

					Put_SLT(SLT_result,SLT);

					LC += 2;
					sign_line_count++;
					//printf("sing line count : %d\n", sign_line_count);
				}
			}
			line_count++;
			ptr = strtok(NULL, " ");// ���� ���ڿ��� �߶� �����͸� ��ȯ
		}
	}

	printf("����\n");
	fclose(fp);
	fclose(SLT);
	fclose(ST);

	//return 0;
}

void Pass2() {

	strcpy(MOV.name, "MOV");
	strcpy(MOV.asemble[0], "100010dw"); //r/m->r, r-> r/m ,d�κ� �⺻���� 1�� ä��
	strcpy(MOV.asemble[1], "1100011w"); //i->r/m
	strcpy(MOV.asemble[2], "1011w");  //i->r
	strcpy(MOV.asemble[3], "10001110"); //r/m -> s
	strcpy(MOV.asemble[4], "10001100"); //s->r/m
	strcpy(MOV.asemble[5], "1010000w"); //m -> ax w=1
	strcpy(MOV.asemble[6], "1010001w"); //ax -> m w=1

	strcpy(ADD.name, "ADD");
	strcpy(ADD.asemble[0], "0000000w"); //r/m->r witheither
	strcpy(ADD.asemble[1], "100000sw");  // i -> r/m
	strcpy(ADD.asemble[2], "0000010w"); //i -> accumulator strcpy(MOV.code[2], "8A");

	int LC = 0;

	FILE* fp;
	fp = fopen("asemble.asm", "r");

	char line[max];
	int line_count = 0;

	for (int i = 0; i < sign_count; i++) {
		if (strstr(slt[i].line, sign_word[i]) != NULL) {
			char* result;

			result = replaceWord(slt[i].line, sign_word[i], st[i].value);
			strcpy(slt[i].line, result);
			//printf("result : %s %s %s\n", sign_word[i], st[i].value,slt[i].line);
		}
	}
	
	while (fgets(line, max, fp) != NULL)			//line�� ���پ� �о�� ���������� �ݺ���Ŵ
	{
		char line2[max];
		strcpy(line2, line);

		int line_count = 0; // ���� ���� ī��Ʈ   
		int word_count = 0; // �ܾ� �ϳ��� ī��Ʈ

		char words[100][100];
		char* memoryKind[2];

		char opcode[20] = "";
		char code_line[100] = "";

		char* ptr = strtok(line2, " ");
		int token = 0;

		char source[10]; // memory ����
		char destination[10];
		
		if (strstr(line, "MOV") != NULL) {
			token = 1;
			strcpy(opcode, "MOV");
		}
		else if (strstr(line, "ADD") != NULL) {
			token = 1;
			strcpy(opcode, "ADD");
		}
		
		while (ptr != NULL)               // �ڸ� ���ڿ��� ������ ���� ������ �ݺ�
		{
			strcpy(words[word_count], ptr);
			ptr = strtok(NULL, " ");      // ���� ���ڿ��� �߶� �����͸� ��ȯ
			word_count++;
		}

		for (int i = 0; i < word_count; i++) { //�޸� ���� 
			if (token == 1) {

				if (strstr(opcode, "MOV") != NULL && i > 0) {
					//printf("words : %s\n", words[i]);

					if (i == 1)
						strcpy(destination, Analyze(words[i]));
					else strcpy(source, Analyze(words[i]));

				}
				else if (strstr(opcode, "ADD") != NULL) {
					if (i == 1)
						strcpy(destination, Analyze(words[i]));
					else strcpy(source, Analyze(words[i]));
				}

			}
			else {

			}
		}
		
		char w[5] = "";

		for (int i = 0; i < sign_count; i++) {
			if (strstr(words[2], st[i].name) != NULL) {
				int w_size = st[i].size;

				switch (w_size) {
					case 1:
						strcpy(w ,"0");
						break;
					default:
						strcpy(w, "1");
						break;
				}
			}
		}

		printf("w : %s\n", w);


		printf("line : %s\n", line);

		if (strstr(opcode, "MOV") != NULL) {
			printf("source, destionation : %s %s\n", source, destination);

			if ((strstr(source, "r") != NULL && (strstr(destination, "r") != NULL || strstr(destination, "m") != NULL))){
				printf("r-> r/m\n");
				strcpy(code_line, "1000100w");
			}if ((strstr(source, "r") != NULL || strstr(source, "m") != NULL) && strstr(destination, "r") != NULL) {
				printf("r/m-> r\n");
				strcpy(code_line, "1000101w");
			}
			else if (strstr(source, "i") != NULL && (strstr(destination, "r") != NULL || strstr(destination, "m") != NULL)) {
				printf("i -> r/m\n");
				strcpy(code_line, MOV.asemble[1]);
			}
			else if (strstr(source, "i") != NULL && strstr(destination, "r") != NULL) {
				printf("i -> r\n");
				strcpy(code_line, MOV.asemble[2]);
			}
			else if (((strstr(source, "r") != NULL || strstr(source, "m") != NULL) && strstr(destination, "s") != NULL)) {
				printf("r/m -> s\n");
				strcpy(code_line, MOV.asemble[3]);
			}
			else if (strstr(source, "s") != NULL && (strstr(destination, "r") != NULL || strstr(destination, "m") != NULL)) {
				printf("s -> r/m\n");
				strcpy(code_line, MOV.asemble[4]);
			}
			else if (strstr(source, "m") != NULL && strstr(destination, "a") != NULL) {
				printf("m -> a\n");
				strcpy(code_line, MOV.asemble[5]);
			}
			else if (strstr(source, "a") != NULL && strstr(destination, "m") != NULL) {
				printf("a -> m\n");
				strcpy(code_line, MOV.asemble[6]);
			}
			else {
				strcpy(code_line, "1011010001001100");
			}
		}
		else if (strstr(opcode, "ADD") != NULL) {
			printf("source, destionation : %s %s\n", source, destination);

			if (strstr(source, "r") != NULL || strstr(source, "m") != NULL) {
				if (strstr(source, "r") != NULL) {
					printf("r -> either\n");
					strcpy(code_line, "0000000w");
				}
				else if (strstr(source, "m") != NULL) {
					printf("m -> either\n");
					strcpy(code_line, "0000001w");
				}
			}
			else if (strstr(source, "i") != NULL && (strstr(destination, "r") != NULL || strstr(destination, "m") != NULL)) {
				printf("i-> r/m\n");
				strcpy(code_line, ADD.asemble[1]);
			}
			else {
				
			}
		}

		char code_result[max] = { 0x00 };

		strcpy(code_result ,replaceWord(code_line, "w", w));

		printf("prev code_result : %s\n", code_result);
		strcat(code_result, " ");
		
		// word index �� �ֱ�

		for (int i = 0; i < sign_count; i++) {
			if (strstr(words[2], st[i].name) != NULL) {
				int index = st[i].index;
				printf("index : %d\n", st[i].index);

				strcat(code_result, "0000");
				strcat(code_result, "0000");

				int d10_index = index / 10;
				int d_index = index % 10;
				char d10_index_s[max] = {0x00};
				char d_index_s[100] = { 0x00 };

				if (index / 10 != 0) {
					sprintf(d10_index_s, "%d", d10_index);
					sprintf(d_index_s, "%d", d_index);
					strcat(d10_index_s, d_index_s);
					printf("d10_index_s : %s\n", d10_index_s);
				}
				else {
					strcat(d10_index_s, "0");
					sprintf(d_index_s, "%d", d_index);
					strcat(d10_index_s, d_index_s);
				}

				char result[50] = {0x00};
				hextobin(d10_index_s, result);

				printf("hex result : %s\n", result);

				strcat(code_result, result);
			}
		}

		printf("code_result : %s\n", code_result);
			
		// 1. Word �� üũ �� LC�� ����
		// 2. Register reg, r/m �ۼ�
		// 3. mod Ȯ��
	}
}

int main() {

	Pass1();
	Pass2();

}

int Put_SLT(char* s, FILE* p)		//mdt�� �ִ� �κ�
{
	fputs(s, p);
	return 0;
}
int Put_ST(char* s, FILE* p)		//mdt�� �ִ� �κ�
{
	fputs(s, p);
	return 0;
}
int Put_FAT(char* s, FILE* p)		//mdt�� �ִ� �κ�
{
	fputs(s, p);
	return 0;
}

int Put_OUT(char* s, FILE* p)		//mdt�� �ִ� �κ�
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
	char result[max];
	int i, cnt = 0;
	int newWlen = strlen(newW);
	int oldWlen = strlen(oldW);

	for (i = 0; s[i] != '\0'; i++) {
		if (strstr(&s[i], oldW) == &s[i]) {
			cnt++;

			i += oldWlen - 1;
		}
	}

	//result = (char*)malloc(10);//i + cnt * (newWlen - oldWlen) + 1

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

const char* Analyze(char* operand) // ���翡 �����ִ� operand�� �����͸� �˻��ϴ� �Լ� *   ���������̸� r / �޸��̸� m / ����̸� i / ���׸�Ʈ �������� �̸� s ; 
{
	int i = 0;
	const char* regist[] = { "AX" ,"BX" ,"CX" ,"DX" };
	const char* segment_regist[] = { "CS" ,"DS" ,"SS" ,"ES" };
	//const char* regist_memory[] = { "AL","BL","CL","DL" };
	//printf("operand : %s\n", operand);

	Eliminate(operand,",");
	Eliminate(operand, " ");
	Eliminate(operand,"\n");

	for (int i = 0; i < 4; i++)
	{
		if (strstr(operand, regist[i]) != NULL) // ������������ �˻�
		{
			if (regist[i] == "AX") {
				return "a"; // accumulator to memory
			}
			else {
				return "r"; // register�� ���ϴ� r�� ������
			}
			
		}
		else if(strstr(operand, segment_regist[i]) != NULL){
			return "s";
		}
	}

	int token = 0;
	for (int j = 0; j < sign_count; j++) {
		//printf("operand : %s %s \n", operand, sign_word[j]);
		if (strstr(operand,sign_word[j]) != NULL) {
			token = 1;
			break;
		}
	}

	if (token == 0) {
		return "i"; // error ǥ��
	}
	else {
		return "m";
	}
}

void my_htob(char c) {
	int n, arr[4] = { 0, };
	n = (int)c - 48; // ���ڰ� �ƴҰ�� A�� 10���� �������
	if (n > 9) n -= 7; 
	for (int i = 0; i < 4; i++) 
	{ 
		arr[i] = (n % 2); n /= 2;
	} 
	for (int i = 3; i >= 0; i--) { 
		printf("%d", arr[i]); 
	} 
	printf(" "); 
}

