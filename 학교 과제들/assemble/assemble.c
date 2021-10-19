#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h> 
#include <stdlib.h> 
#include <string.h> 
#define max 1024

typedef struct FormalArgumentTable { // 매크로 이름표
	int	index;
	char name[10];
}FormalArgumentTable;

typedef struct MacroDefineTable { //매크로 정의표
	int	index;
}MacroDefineTable;

//MNT 
typedef struct MacroNameTable { // 매크로 이름표
	int	index;
	char name[10];
	int	MDTindex;
}MacroNameTable;

MacroNameTable mnt[20];
MacroDefineTable mdt[20];
FormalArgumentTable fat[10];

int Put_MDT(char* s, FILE* p);		//MDT에 입력	
int Put_MNT(char* s, int x, FILE* p);		//MNT에 입력
int Put_FAT(char* s, FILE* p); // FAT 입력

void Eliminate(char* str, char ch); // 특정 문자열 제거
int findString(const char str2[], const char str1[]); // 특정 문자열 포함 여부 확인
char* replaceWord(const char* s, const char* oldW, const char* newW); //특정 문자열 교환

void Pass1() {

	FILE* fp;
	fp = fopen("macro2.asm", "r");			//읽기 전용으로 파일은 읽어드리기 위해 사용

	FILE* MNT;
	MNT = fopen("MNT.txt", "w");
	if (MNT == NULL) {			//못읽었을때 예외처리
		printf("MNT 파일읽기 실패\n");
		return 1;
	}

	FILE* MDT;
	MDT = fopen("MDT.txt", "w");
	if (MDT == NULL) {			//못읽었을때 예외처리
		printf("MDT 파일읽기 실패\n");
		return 1;
	}

	FILE* FAT;
	FAT = fopen("FAT.txt", "w");
	if (FAT == NULL) {			//못읽었을때 예외처리
		printf("FAT 파일읽기 실패\n");
		return 1;
	}

	char line[max];

	int token = 0;			//일종의 토큰으로 이 값이 1이면 macro가 선언된 부분

	int MNTC = 1;
	int MDTC = 1;

	int param_num = 0;  // 매크로 파라미터를 구분하기 위한 변수

	if (fp == NULL) {			//못읽었을때 예외처리
		printf("파일읽기 실패\n");
		return 1;
	}

	while (fgets(line, max, fp) != NULL)			//line을 한줄씩 읽어가며 끝날떄까지 반복시킴
	{

		if (token == 0)		//macro가 선언되있지않다면
		{
			if (strstr(line, "MACRO"))			//macro라는 단어를 가지고 있으면 
			{
				printf("MACRO CHECK ! %d\n",MDTC);
				token = 1;

				char line2[max];
				strcpy(line2, line);

				mnt[MNTC].MDTindex = MDTC; // MNT 기록
				mnt[MNTC].index = MNTC;

				char* ptr = strtok(line2, " ");      // " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환
				int line_count = 0;

				while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
				{
					switch (line_count)  // 라인 분할하여 형식인수표에 삽입
					{
					case 0: // 매크로 이름 부분
						strcpy(mnt[MNTC].name, ptr);
						break;
					case 1: // MACRO 부분
						break;
					default: // 매크로 파라미터 부분
						Eliminate(ptr, ',');
						fat[param_num].index = param_num;
						strcpy(fat[param_num].name, ptr);
						param_num++;
						break;
					}

					ptr = strtok(NULL, " ");// 다음 문자열을 잘라서 포인터를 반환
					line_count++;
				}

				char FAT_result[max] = "MNT NUMBER : ";  // FAT 입력
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

				char MNT_result[max];  // MNT 입력
				char mdtIndex[10];

				sprintf(MNT_result, "%d", mnt[MNTC].index);
				sprintf(mdtIndex, "%d", mnt[MNTC].MDTindex);

				strcat(MNT_result, " ");
				strcat(MNT_result, mnt[MNTC].name);
				strcat(MNT_result, " ");
				strcat(MNT_result, mdtIndex);
				strcat(MNT_result, "\n");

				Put_MNT(MNT_result, MNTC, MNT);

				char MDT_result[max]; // MDT MACRO 시작 입력

				sprintf(MDT_result, "%d", mnt[MNTC].MDTindex);
				strcat(MDT_result, "   ");
				strcat(MDT_result, line);

				Put_MDT(MDT_result, MDT);
				MNTC += 1;
			}
			MDTC++;
		}
		else {
			//macro가 선언되어있다면 계속 입력
			char line2[max];
			strcpy(line2, line);
			char* ptr = strtok(line2, " ");

			while (ptr != NULL)               // 형식 인수표와 같은지 검사
			{
				for (int i = 0; i < param_num; i++) {
					if (findString(ptr, fat[i].name) == 0) { // 문장 속 형식인수표와 동일한 인수 찾기

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


			if (strstr(line, "ENDM"))			//ENDM라는 단어를 가지고 있으면 
			{
				char MDT_result[max];

				sprintf(MDT_result, "\n%d", MDTC);
				strcat(MDT_result, "   ");
				strcat(MDT_result, line);

				printf("ENDM Check! \n");
				token = 0;

				Put_MDT(MDT_result, MDT);
			}
			else {                            // 매크로 중간 부분 넣기
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
	printf("종료");
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
int Put_MDT(char* s, FILE* p)		//mdt에 넣는 부분
{
	fputs(s, p);
	return 0;
}
int Put_MNT(char* s, int x, FILE* p)		//mdt에 넣는 부분
{
	fputs(s, p);
	return 0;
}
int Put_FAT(char* s, FILE* p)		//mdt에 넣는 부분
{
	fputs(s, p);
	return 0;
}

void Eliminate(char* str, char ch) // 문자 제거
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

int findString(const char str2[], const char str1[]) { // 특정 문자 포함 여부
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

char* replaceWord(const char* s, const char* oldW,  // 특정 문자열 대체하기
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