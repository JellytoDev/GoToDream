#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#define max 512

int make_instruct(char* s, FILE* fpts);		//���þ�ǥ �߰�
int set_instruct();				//���Ǵ� ���þ�ǥ �־��ֱ�, ���⼭ ����κ� �߰��Ҽ�������
void put_numeric(char* b, int k, int rep, FILE* fpnt);
void Eliminate(char* str, char ch);
int do_function(char* s, int LC, FILE* fpt);
int find(char* s, FILE* fpt, int LC);
int pass1(void);
void put_struct_numeric(char* b, int k, int rep);
int put_struct_code(int LC, char* s);
int print_Code_with_LC();
int print_Numeric();
int do_func_Commend(int x);
char* replaceWord(const char* s, const char* oldW, const char* newW);
int put_Commend(char* s);
int print_Commend();
int write_Commend(FILE* fpt2);
int change_hex_dec(char* s);
int define_of_INT(int x);
int def_seg_reg(char* s);
int define_of_MOV(int x);
int define_of_ADD(int x);
int def_seg_ch(char* s, int x);
int def_reg_ch(char* s, int x);
int print_Code_with_LC_mc();
int is_numeric_in(char* s);
int put_DW(char* s, int x);
int print_elcode_to_16();

typedef struct Code_with_LC {
	char line[100];
	int LC;
}Code_with_LC;

typedef struct Numeric {
	char name[100];
	int LC;
	int Rep;
}Numeric;
typedef struct Source_code {
	char original[100];
	char value1[100];
	char value2[100];
	char value3[100];
	char result[100];
	int LC;
}Source_code;

Code_with_LC slt[max];
Numeric st[max];
Source_code sct[max];

int Numeric_count = 0;
int Code_with_LC_count = 0;
int Sourc_code_count = 0;
int main(void)
{
	int sel = 0;
	pass1();
	printf("\npass1 ���� �Ϸ�\n");
	while (sel != 3)
	{
		printf("\n���� ��� ����\n1.LC���� ���Ե� �ڵ� ����\n2. ��ȣǥ Ȯ��\n3. pass 2 ����\n");
		scanf("%d", &sel);
		switch (sel)
		{
		case 1:
			Sleep(100);
			system("cls");
			print_Code_with_LC();
			break;
		case 2:
			Sleep(100);
			system("cls");
			print_Numeric();
			break;
		case 3:
			printf("pass 2 ����\n");
			break;
		default:
			break;
		}
	}
	Sleep(100);
	system("cls");
	pass2();
	printf("\npass2 ���� �Ϸ�\n");
	while (sel != 4)
	{
		printf("\n���� ��� ����\n1.LC���� ���Ե� �ڵ� ����\n2. ���� �ڵ� Ȯ��\n3. 16���� �ڵ� Ȯ��\n4. ����\n");
		scanf("%d", &sel);
		switch (sel)
		{
		case 1:
			Sleep(100);
			system("cls");
			printf("�ڵ� ���");
			print_Code_with_LC();
			printf("\n");
			break;
		case 2:
			Sleep(100);
			system("cls");
			printf("�ڵ� ���");
			print_Code_with_LC_mc();
			printf("\n");
			break;
		case 3:
			Sleep(100);
			system("cls");
			printf("�ڵ� ���");
			print_elcode_to_16();
			printf("\n");
			break;
		default:
			break;
		}
	}
	
}
int pass2(void)
{
	printf("\n");
	FILE* fp;
	char line[max];
	fp = fopen("ex.asm", "r");					//asmǥ �б�
	if (fp == NULL) {							//���о����� ����ó��
		printf("�����б� ����\n");
		return 1;
	}
	int ct = 0;
	while (fgets(line, max, fp) != NULL)		//���پ� �о��
	{
		strcpy(sct[ct].original, line);
		int x = is_Commend(line);
		if (x == 1)
		{
			//printf("%s %d\n",line,x);
			printf(line);
			do_func_Commend(ct);

		}
		else if (is_numeric_in(line))
		{
			if (strstr(sct[ct].value2, "DW"))
			{
				put_DW(sct[ct].value3, ct);
			}
		}
		ct += 1;
	}
	//print_Commend();
	fclose(fp);
}
int put_DW(char* s, int x)
{
	char temp[max] = "";
	strcpy(temp, s);
	if (!strstr(temp, "H"))
	{
		int num = atoi(temp);
		char tmp2[max] = "";
		char ch[max];
		char tmp2_r[max] = "";
		while (num != 0)
		{
			sprintf(ch, "%d", num % 2);
			strcat(tmp2, ch);
			if (num == 1)
			{
				num = 0;
			}
			else
			{
				num = num / 2;
			}
		}
		for (int i = 0; i < strlen(tmp2); i++)
		{
			tmp2_r[i] = tmp2[strlen(tmp2) - 1 - i];
		}
		int k = 16 - strlen(tmp2_r);
		if (k != 0)
		{
			strcpy(sct[x].result, "0");
			k -= 1;
		}
		while (k != 0)
		{
			strcat(sct[x].result, "0");
			k -= 1;
		}
		
		strcat(sct[x].result, tmp2_r);
	}
}
int is_numeric_in(char* s)
{
	if (strstr(s, "DW") || strstr(s, "EQU") || strstr(s, "END"))
	{
		return 1;
	}
	else
		return 0;
}
int print_elcode_to_16()
{
	for (int i = 0; i < Sourc_code_count; i++)
	{
		if (strlen(sct[i].result))
		{
			if (slt[i].LC >= 16)
			{
				printf("00");
			}
			else
			{
				printf("000");
			}
			printf("%X\t", slt[i].LC);
			int count = 0;
			int tmp = 0;
			for (int j = 0; j < strlen(sct[i].result); j++)
			{
				int temp = sct[i].result[j] - 48;
				if (count == 0)
				{
					tmp += temp * 8;
				}
				else if (count == 1)
				{
					tmp += temp * 4;
				}
				else if (count == 2)
				{
					tmp += temp * 2;
				}
				else if (count == 3)
				{
					tmp += temp;
				}
				count += 1;
				if (count == 4)
				{
					if (tmp <= 9)
					{
						printf("%d", tmp);
					}
					else
					{
						printf("%c", tmp + 55);
					}
					count = 0;
					tmp = 0;
				}
			}
			printf("    \t%s",sct[i].original);
		}
		else
		{
			printf("%s", sct[i].original);
		}
	}
}
int do_func_Commend(int x)
{
	if (strstr(sct[x].value1, "MOV"))
	{
		define_of_MOV(x);
		printf("%s\n", sct[x].result);
	}
	if (strstr(sct[x].value1, "INT"))
	{
		define_of_INT(x);
		printf("%s\n", sct[x].result);
	}
	if (strstr(sct[x].value1, "ADD"))
	{
		define_of_ADD(x);
		printf("%s\n", sct[x].result);
	}
}
int define_of_ADD(int x)
{
	int sec = def_seg_reg(sct[x].value2);
	int thd = def_seg_reg(sct[x].value3);
	int token = 0;
	if (sec == 1 || thd == 1)		//numeric�� �ش��ϴ°� �ִٸ�
	{
		char result[max] = "000000";
		if (sec == 2)
		{
			strcat(result, "1");
			if (strstr(sct[x].value2, "AX"))		//dw�� 1 db�� 0
			{
				strcat(result, "1");
				token = 1;
			}
			else
			{
				strcat(result, "0");
			}
			strcpy(sct[x].result, result);
			strcat(sct[x].result, "00");		//numeric�� ������ �����ּ������̹Ƿ� mod =00
			def_reg_ch(sct[x].value2, x);
			strcat(sct[x].result, "110");		//�����ּ������� r/m�� 110
			strcat(sct[x].result, "00000000");		//�ּҺκ��� ���� �����缭 ���⼭ ����
			int i = 0;
			for (i = 0; i < Numeric_count; i++)
			{
				if (strstr(st[i].name, sct[x].value3))
				{
					int tmp = st[i].LC;
					char temp_put[max] = "";
					char temp_put_r[max] = "";
					char temp[max] = "";
					while (tmp != 0)
					{
						sprintf(temp, "%d", tmp % 2);
						strcat(temp_put, temp);
						if (tmp == 1)
						{
							tmp = 0;
						}
						else
						{
							tmp = tmp / 2;
						}
					}
					int inval = strlen(temp_put) % 4;
					if (inval > 0)
					{
						int inval_2 = 4 - inval;
						while (inval_2 > 0)
						{
							strcat(sct[x].result, "0");
							inval_2 -= 1;
						}
					}
					for (int j = 0; j < strlen(temp_put); j++)
					{
						temp_put_r[j] = temp_put[strlen(temp_put) - 1 - j];
					}

					strcat(sct[x].result, temp_put_r);
					break;
				}
			}

		}
		else
		{
			strcat(result, "0");
			if (strstr(sct[x].value3, "AX"))		//dw�� 1 db�� 0
			{
				strcat(result, "1");
				token = 1;
			}
			else
			{
				strcat(result, "0");
			}
			strcpy(sct[x].result, result);
			strcat(sct[x].result, "00");		//numeric�� ������ �����ּ������̹Ƿ� mod =00
			def_reg_ch(sct[x].value3, x);
			strcat(sct[x].result, "110");
			strcat(sct[x].result, "00000000");		//�ּҺκ��� ���� �����缭 ���⼭ ����
			int i = 0;
			for (i = 0; i < Numeric_count; i++)
			{
				if (strstr(st[i].name, sct[x].value2))
				{
					int tmp = st[i].LC;
					char temp_put[max] = "";
					char temp_put_r[max] = "";
					char temp[max] = "";
					while (tmp != 0)
					{
						sprintf(temp, "%d", tmp % 2);
						strcat(temp_put, temp);
						if (tmp == 1)
						{
							tmp = 0;
						}
						else
						{
							tmp = tmp / 2;
						}
					}
					int inval = strlen(temp_put) % 4;
					if (inval > 0)
					{
						int inval_2 = 4 - inval;
						while (inval_2 > 0)
						{
							strcat(sct[x].result, "0");
							inval_2 -= 1;
						}
					}
					for (int j = 0; j < strlen(temp_put); j++)
					{
						temp_put_r[j] = temp_put[strlen(temp_put) - 1 - j];
					}

					strcat(sct[x].result, temp_put_r);
					break;
				}
			}
			
		}

		


	}
}
int define_of_MOV(int x)
{
	int sec = def_seg_reg(sct[x].value2);
	int thd = def_seg_reg(sct[x].value3);
	if (sec == 3 && thd == 2)
	{
		char result[max] = "10001110110";			//reg to seg
		strcpy(sct[x].result, result);
		def_seg_ch(sct[x].value2, x);
		def_reg_ch(sct[x].value3, x);
	}
	else if (sec == 2 && thd == 3)
	{
		char result[max] = "10001100110";			//seg to reg
		strcpy(sct[x].result, result);
		def_seg_ch(sct[x].value3, x);
		def_reg_ch(sct[x].value2, x);
	}
	else if (sec == 2 && thd == 1)
	{
		if (strstr(sct[x].value2, "AX") || strstr(sct[x].value2, "AH") || strstr(sct[x].value2, "AL"))
		{
			char result[max] = "1010000";			//mem to acc
			strcpy(sct[x].result, result);
			if (strstr(sct[x].value2, "AX"))
				strcat(sct[x].result, "1");
			else
				strcat(sct[x].result, "0");
		}
		int i = 0;
		for (i = 0; i < Numeric_count; i++)
		{
			if (strstr(st[i].name, sct[x].value3))
			{
				int tmp = st[i].LC;
				char temp_put[max] = "";
				char temp_put_r[max] = "";
				char temp[max] = "";
				while (tmp != 0)
				{
					sprintf(temp, "%d", tmp % 2);
					strcat(temp_put, temp);
					if (tmp == 1)
					{
						tmp = 0;
					}
					else
					{
						tmp = tmp / 2;
					}
				}
				strcat(sct[x].result, "00000000");
				
				int inval = strlen(temp_put) % 4;
				if (inval > 0)
				{
					int inval_2 = 4 - inval;
					while (inval_2 > 0)
					{
						strcat(sct[x].result, "0");
						inval_2 -= 1;
					}
				}
				for (int j = 0; j < strlen(temp_put); j++)
				{
					temp_put_r[j] = temp_put[strlen(temp_put) - 1 - j];
				}

				strcat(sct[x].result, temp_put_r);
				break;
			}
		}
	}

	else if (sec == 1 && thd == 2)
	{
		if (strstr(sct[x].value3, "AX") || strstr(sct[x].value3, "AH") || strstr(sct[x].value3, "AL"))
		{
			char result[max] = "1010001";			//mem to acc
			strcpy(sct[x].result, result);
			if (strstr(sct[x].value3, "AX"))
				strcat(sct[x].result, "1");
			else
				strcat(sct[x].result, "0");

		}
		int i = 0;
		for (i = 0; i < Numeric_count; i++)
		{
			if (strstr(st[i].name, sct[x].value2))
			{
				int tmp = st[i].LC;
				char temp_put[max] = "";
				char temp_put_r[max] = "";
				char temp[max] = "";
				while (tmp != 0)
				{
					sprintf(temp, "%d", tmp % 2);
					strcat(temp_put, temp);
					if (tmp == 1)
					{
						tmp = 0;
					}
					else
					{
						tmp = tmp / 2;
					}
				}
				strcat(sct[x].result, "00000000");
				int inval = strlen(temp_put) % 4;
				if (inval > 0)
				{
					int inval_2 = 4 - inval;
					while (inval_2 > 0)
					{
						strcat(sct[x].result, "0");
						inval_2 -= 1;
					}
				}
				for (int j = 0; j < strlen(temp_put); j++)
				{
					temp_put_r[j] = temp_put[strlen(temp_put) - 1 - j];
				}

				strcat(sct[x].result, temp_put_r);
				break;
			}
		}
	}
	else if (sec == 0 || thd == 0)
	{
		char result[max] = "1011";
		if (strstr(sct[x].value3, "AX") || strstr(sct[x].value3, "BX") || strstr(sct[x].value3, "CX") || strstr(sct[x].value3, "DX"))
		{
			strcat(result, "1");
		}
		else
		{
			strcat(result, "0");
		}
		char temp[max] = "";
		strcpy(temp, def_reg_ch(sct[x].value2, x));
		strcat(result, temp);
		strcpy(sct[x].result, result);
		strcpy(temp, sct[x].value3);
		char res[max] = "";
		int k = 0;
		if (strstr(temp, "H"))
		{
			strncpy(res, temp, strlen(temp) - 1);
		}
		else {
			strcpy(res, sct[x].value3);
		}
		k = change_hex_dec(res);
		char dec_2[max] = "";
		char final[max] = "";
		while (k != 0)
		{
			sprintf(dec_2, "%d", k % 2);
			strcat(final, dec_2);
			if (k == 1)
			{
				k = 0;
			}
			else
			{
				k = k / 2;
			}
		}
		int inval = strlen(final) % 4;
		if (inval > 0)
		{
			int inval_2 = 4 - inval;
			while (inval_2 > 0)
			{
				strcat(sct[x].result, "0");
				inval_2 -= 1;
			}
		}
		char final_r[max] = "";
		for (int j = 0; j < strlen(final); j++)
		{
			final_r[j] = final[strlen(final) - 1 - j];
		}

		strcat(sct[x].result, final_r);
	}

 }

int def_reg_ch(char* s, int x)
{
	if (strstr(s, "AL") || strstr(s, "AX"))
	{
		strcat(sct[x].result, "000");
	}
	else if (strstr(s, "CL") || strstr(s, "CX"))
	{
		strcat(sct[x].result, "001");
	}
	else if (strstr(s, "DL") || strstr(s, "DX"))
	{
		strcat(sct[x].result, "010");
	}
	else if (strstr(s, "BL") || strstr(s, "BX"))
	{
		strcat(sct[x].result, "011");
	}
	else if (strstr(s, "AH") || strstr(s, "SP"))
	{
		strcat(sct[x].result, "100");
	}
	else if (strstr(s, "CH") || strstr(s, "BP"))
	{
		strcat(sct[x].result, "101");
	}
	else if (strstr(s, "DH") || strstr(s, "SI"))
	{
		strcat(sct[x].result, "110");
	}
	else if (strstr(s, "BH") || strstr(s, "DI"))
	{
		strcat(sct[x].result, "111");
	}
}
int def_seg_ch(char* s, int x)
{
	if (strstr(s, "ES"))
	{
		strcat(sct[x].result, "00");
	}
	else if (strstr(s, "CS"))
	{
		strcat(sct[x].result, "01");
	}
	else if (strstr(s, "SS"))
	{
		strcat(sct[x].result, "10");
	}
	else if (strstr(s, "DS"))
	{
		strcat(sct[x].result, "11");
	}
}
int def_seg_reg(char* s)
{
	if (strstr(s, "ES") || strstr(s, "CS") || strstr(s, "SS") || strstr(s, "DS"))
	{
		if (strlen(s) == 2)
			return 3;
	}		//3�� ������ ���׸�Ʈ
	if (strstr(s, "AX") || strstr(s, "BX") || strstr(s, "CX") || strstr(s, "DX")
		|| strstr(s, "AL") || strstr(s, "BL") || strstr(s, "CL") || strstr(s, "DL")
		|| strstr(s, "AH") || strstr(s, "BH") || strstr(s, "CH") || strstr(s, "DH")
		|| strstr(s, "SP") || strstr(s, "BP") || strstr(s, "SI") || strstr(s, "DI"))
	{
		if (strlen(s) == 2)
			return 2;				//2�� ������ ��������
	}
	for (int i = 0; i < Numeric_count; i++)
	{
		if (strstr(s, st[i].name))
			return 1;			//1�� ������ ��ȣǥ�� �ִ¾�
	}
	return 0;
}
int define_of_INT(int x)
{
	char result[max] = "11001101";
	char temp[max] = "";
	char res[max] = "";
	char tmp2[max] = "";
	int tem;
	strcpy(temp, sct[x].value2);
	if (strstr(temp, "H"))
	{
		strncpy(res, temp, strlen(temp) - 1);
		tem = change_hex_dec(res);

		while (tem != 0)
		{
			sprintf(temp, "%d", tem % 2);
			strcat(tmp2, temp);
			if (tem == 1)
			{
				tem = 0;
			}
			else
			{
				tem = tem / 2;
			}
		}
		char tmp2_r[max] = "";
		for (int j = 0; j < strlen(tmp2); j++)
		{
			tmp2_r[j] = tmp2[strlen(tmp2) - 1 - j];
		}
		int inval = strlen(tmp2) % 4;
		if (inval > 0)
		{
			int inval_2 = 4 - inval;
			while (inval_2 > 0)
			{
				strcat(result, "0");
				inval_2 -= 1;
			}
		}
		strcat(result, tmp2_r);

	}
	strcpy(sct[x].result, result);
}
int print_Commend()
{
	printf("\n");
	for (int i = 0; i < Sourc_code_count; i++)
	{
		char tem1[max] = "MOV";
		char tem2[max] = "ADD";
		if (strstr(sct[i].value1, tem1) || strstr(sct[i].value1, tem2))
		{
			printf("/%s/ /%s/ /%s/\n", sct[i].value1, sct[i].value2, sct[i].value3);
		}
		else
		{
			printf("/%s/ /%s/\n", sct[i].value1, sct[i].value2);
			define_of_INT(i);
			printf("\n%s\n", sct[i].result);
		}

	}
}
int change_hex_dec(char* s)
{
	char hexadecimal[20] = "";    // 16������ �� ���ڿ�
	strcpy(hexadecimal, s);
	int decimal = 0;                  // 10������ ������ ����

	int position = 0;
	for (int i = strlen(hexadecimal) - 1; i >= 0; i--)    // ���ڿ��� �������� �ݺ�
	{
		char ch = hexadecimal[i];         // �� �ڸ����� �ش��ϴ� ���ڸ� ����

		if (ch >= 48 && ch <= 57)         // ���ڰ� 0~9�̸�(ASCII �ڵ� 48~57)
		{
			// ���ڿ��� 0�� �ش��ϴ� ASCII �ڵ� ���� ����
			// 16�� �ڸ����� �ŵ������� ���� ����
			decimal += (ch - 48) * pow(16, position);
		}
		else if (ch >= 65 && ch <= 70)    // ���ڰ� A~F�̸�(ASCII �ڵ� 65~70)
		{                                 // �빮�ڷ� �� 16������ ó��
			// ���ڿ��� (A�� �ش��ϴ� ASCII �ڵ� �� - 10)�� ����
			// 16�� �ڸ����� �ŵ������� ���� ����
			decimal += (ch - (65 - 10)) * pow(16, position);
		}
		else if (ch >= 97 && ch <= 102)   // ���ڰ� a~f�̸�(ASCII �ڵ� 97~102)
		{                                 // �ҹ��ڷ� �� 16������ ó��
			// ���ڿ��� (a�� �ش��ϴ� ASCII �ڵ� �� - 10)�� ����
			// 16�� �ڸ����� �ŵ������� ���� ����
			decimal += (ch - (97 - 10)) * pow(16, position);
		}

		position++;
	}
	return decimal;
}
int put_Commend(char* s)
{
	char temp[max] = "";
	strcpy(temp, s);
	char ch[] = ",";
	char ch1[] = "";
	char* temps = strstr(temp, ch);
	if (temps != NULL && strstr(temps, ch))
	{
		temps = replaceWord(temp, ch, ch1);
		strcpy(temp, temps);
	}
	temps = strtok(temp, " ");
	strcpy(sct[Sourc_code_count].value1, temps);
	temps = strtok(NULL, " ");
	if (temps != NULL)
	{
		Eliminate(temps, 10);
		strcpy(sct[Sourc_code_count].value2, temps);
		temps = strtok(NULL, " ");
		if (temps != NULL)
		{
			Eliminate(temps, 10);
			strcpy(sct[Sourc_code_count].value3, temps);
		}
	}
	Sourc_code_count += 1;
}
int is_Commend(char* s)
{
	FILE* fpt;
	char Commends_line[max];
	int token = 0;
	fpt = fopen("Commend.txt", "r");
	if (fpt == NULL) {			//���о����� ����ó��
		printf("�����б� ����\n");
		return 1;
	}
	while (fgets(Commends_line, max, fpt) != NULL)		//���پ� �о��
	{
		char ch = '\n';
		char temp[max] = "";
		strcpy(temp, Commends_line);
		Eliminate(temp, ch);
		if (strstr(s, temp))
		{
			token = 1;
		}

	}
	fclose(fpt);
	return token;
}
int pass1(void)
{
	FILE* fp;
	char line[max];

	fp = fopen("ex.asm", "r");					//asmǥ �б�
	if (fp == NULL) {							//���о����� ����ó��
		printf("�����б� ����\n");
		return 1;
	}
	FILE* fpt;
	fpt = fopen("numeric.txt", "w");
	if (fpt == NULL) {			//���о����� ����ó��
		printf("�����б� ����\n");
		return 1;
	}
	FILE* fpt2;						//��ɾ�ǥ
	fpt2 = fopen("Commend.txt", "w");
	if (fpt2 == NULL) {			//���о����� ����ó��
		printf("�����б� ����\n");
		return 1;
	}
	write_Commend(fpt2);
	int LC = 0;
	set_instruct();			//���þ�ǥ ����
	while (fgets(line, max, fp) != NULL)		//���پ� �о��
	{
		put_Commend(line);
		put_struct_code(LC, line);
		int t = find(line, fpt, LC);
		if (t == 2)
			LC += t;
		else
		{
			if (strstr(line, "MOV"))
			{
				int sec = def_seg_reg(sct[Sourc_code_count - 1].value2);
				int thd = def_seg_reg(sct[Sourc_code_count - 1].value3);
				if (sec == 3 || thd == 3)
				{
					LC += 2;
				}
				else if (strstr(line, "AL") || strstr(line, "BL") || strstr(line, "CL") || strstr(line, "DL")||
					strstr(line, "AH") || strstr(line, "BH") || strstr(line, "CH") || strstr(line, "DH"))
				{
					LC += 2;
				}
				else
				{
					LC += 3;
				}

			}
			else if (strstr(line, "ADD"))
			{
				if (strstr(line, "AX") || strstr(line, "BX") || strstr(line, "CX") || strstr(line, "DX"))
					LC += 4;
			}
			else if (strstr(line, "INT"))
			{
				LC += 2;
			}
		}
		printf(line);
	}
	//print_Code_with_LC();
	// print_Numeric();
	fclose(fpt2);
	fclose(fpt);
	fclose(fp);
	return 0;
}
int write_Commend(FILE* fpt2) {
	fputs("MOV", fpt2);
	fputs("\n", fpt2);
	fputs("ADD", fpt2);
	fputs("\n", fpt2);
	fputs("INT", fpt2);
	fputs("\n", fpt2);
	return 0;
}
void Eliminate(char* str, char ch)
{
	int len = strlen(str) + 1;
	for (; *str != '\0'; str++, len--)//���� ���ڸ� ���� ������ �ݺ�
	{
		if (*str == ch)//ch�� ���� ������ ��
		{
			strcpy_s(str, len, str + 1);
			str--;
		}
	}
}
int find(char* s, FILE* fpt, int LC)
{
	FILE* fpt1;
	fpt1 = fopen("ins.txt", "r");
	if (fpt1 == NULL) {			//���о����� ����ó��
		printf("�����б� ����\n");
		return 1;
	}
	char instruct[max];								//���þ�ǥ�� �о ������ ����
	char temp[max];
	strcpy(temp, s);
	int k = 0;
	for (int i = 0; i < strlen(temp); i++)
	{
		if (temp[i + k] == 10)				//���� �κ��� null�� ���� 
		{
			temp[i + k] = ' ';
			temp[i + 1 + k] = NULL;
			break;
		}
	}
	char ch = ',';
	Eliminate(temp, ch);
	int x = -2;
	while (fgets(instruct, max, fpt1) != NULL)		//���þ�ǥ�� �а� ������ do_function ȣ��
	{
		char temp_ins[max] = "";
		for (int i = 0; i < strlen(instruct); i++)
		{
			if (instruct[i] == 10)				//���� �κ��� null�� ���� 
			{
				temp_ins[i] = NULL;
				break;
			}
			else if (instruct[i] == 59)
			{
				temp_ins[i] = NULL;
				break;
			}
			else
			{
				temp_ins[i] = instruct[i];
			}
		}

		//------------------------------------------temp_ins�� ����� ���þ ���忡 �ִ��� Ȯ���ϰ� ������ do_functionȣ��
		char* ptr = strstr(temp, temp_ins);
		if (ptr)
		{
			x = do_function(temp, LC, fpt);
		}
	}

	fclose(fpt1);
	return x;

}
int do_function(char* s, int LC, FILE* fpt)		//s�� ���þ� �κ�, k�� LC, fpt�� Ȥ�ø��� ��ȣǥ �ֱ����� ������
{		//���������� ���þ�ǥ�� �ִ°͵鿡 ���� ��ġ�ϴ� �κ�
	char* temp_end;
	if (strstr(s, "DW"))
	{
		char temp_name[max] = "";
		strcpy(temp_name, strtok(s, " "));
		strtok(NULL, " ");
		char temp_value[max] = "";
		strcpy(temp_value, strtok(NULL, " "));
		put_numeric(temp_name, LC, 1, fpt);	//��������, LC , ���ġ����=1
		return 2;				//ũ�⸸ŭ �����༭ k�� ������Ű����
	}
	else if (strstr(s, "EQU"))
	{
		char temp_name[max] = "";
		strcpy(temp_name, strtok(s, " "));
		strtok(NULL, " ");
		char temp_value[max] = "";
		strcpy(temp_value, strtok(NULL, " "));
		int k = atoi(temp_value);
		put_numeric(temp_value, k, 0, fpt);	//��������, �� , ���ġ����=0
		return 0;				//EQU�� ���� �ȴþ�Ƿ�
	}
	else if (temp_end = strstr(s, "END"))
	{

		return -1;				//-1�� �Ǹ� ����Ǵ°ɷ�

	}
	else
	{
		printf("��ϵ��� ���� ���þ� �� �ֽ��ϴ�.%d\n", strlen(s));
		return -1;					//���� ������ ���þ��� ���⿡ ���°� �ִٴ¶�
	}

}
int print_Code_with_LC() {
	printf("\n");
	int i = 0;
	for (i = 0; i < Code_with_LC_count; i++)
	{
		if(strstr(slt[i].line, "MOV") || strstr(slt[i].line, "ADD") || strstr(slt[i].line, "INT") ||strstr(slt[i].line,"DW") || strstr(slt[i].line, "EQU"))
			printf("%d %s", slt[i].LC, slt[i].line);
		else if (strstr(slt[i].line, "END"))
		{
			printf("%d %s", slt[i].LC, slt[i].line);
			break;
		}
		else
		{
			printf("%s", slt[i].line);
		}
	}
	printf("%s\n", slt[i+1].line);
	return 0;
}
int print_Code_with_LC_mc() {
	printf("\n");
	int i = 0;
	int k = 0;
	for (i = 0; i < Sourc_code_count; i++)
	{
		if (strstr(slt[i].line, "MOV") || strstr(slt[i].line, "ADD") || strstr(slt[i].line, "INT") || strstr(slt[i].line, "DW") || strstr(slt[i].line, "EQU"))
		{
			printf("%d %s%s\n", slt[i].LC, slt[i].line, sct[i].result);
		}
		else if (strstr(slt[i].line, "END"))
		{
			printf("%d %s%s\n", slt[i].LC, slt[i].line, sct[i].result);
			break;
		}
		else
		{
			printf("%s", slt[i].line);
		}
	}
	printf("%s\n", slt[i + 1].line);
	return 0;
}
int print_Numeric() {
	for (int i = 0; i < Numeric_count; i++)
	{
		printf("%d %s %d\n", st[i].LC, st[i].name, st[i].Rep);
	}
	return 0;
}
void put_numeric(char* b, int k, int rep, FILE* fpnt)
{
	char str_temp[max] = "";
	sprintf(str_temp, "%s %d %d\n", b, k, rep);
	put_struct_numeric(b, k, rep);
	fputs(str_temp, fpnt);

}
void put_struct_numeric(char* b, int k, int rep) {
	strcpy(st[Numeric_count].name, b);
	st[Numeric_count].LC = k;
	st[Numeric_count].Rep = rep;
	Numeric_count += 1;
}
int make_instruct(char* s, FILE* fpts)
{
	fputs(s, fpts);
	fputs("\n", fpts);
}
int put_struct_code(int LC, char* s)
{
	strcpy(slt[Code_with_LC_count].line, s);
	slt[Code_with_LC_count].LC = LC;
	Code_with_LC_count += 1;
	return 0;
}
int set_instruct()		//���Ǵ� ���þ ���� ����
{
	FILE* fpts;
	fpts = fopen("ins.txt", "w");
	if (fpts == NULL) {			//���о����� ����ó��
		printf("�����б� ����\n");
		return 1;
	}
	make_instruct("DW", fpts);
	make_instruct("END", fpts);
	make_instruct("EQU", fpts);
	fclose(fpts);
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