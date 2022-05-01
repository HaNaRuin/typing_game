#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include <mmsystem.h>             //wav ���� ���
#pragma comment(lib,"winmm.lib")

#define KEY_ESC 27
#define KEY_ENTER '\r'
#define KEY_BS '\b'

#define MAX_WORD_LENGTH 24
#define MAX_SCREEN_WORD_COUNT 19
#define WORD_COUNT 48                          // Ȯ�� ��

int g_word_count = 48;
char** g_words;
int g_fail_count = 0;                           // ������ ����
int g_score = 0;                                // ����
char g_input_word[MAX_WORD_LENGTH + 1];         // �Է� �ܾ� ����
int g_input_word_length = 0;                    // �Է� �ܾ� ����
int stage = 1;                                  // �������� �ܰ� ����

struct ScreenWord
{
	int index;
	int x;
	int y;
};
typedef struct ScreenWord ScreenWord;
ScreenWord g_screen_word[MAX_SCREEN_WORD_COUNT];// ȭ�鿡 ��Ÿ�� �ܾ��(g_words�� ���� index ����)
int g_screen_word_count = 0;                    // ȭ�鿡 ��Ÿ�� �ܾ� ����

clock_t g_start_time;                           // ���� �ð�
double g_falling_speed = 2.0;                   // �ܾ� ���� �ð�(�� ����)

void InitScreen(void);
void InitData(void);                            // ���� ���� ���� (Ȯ�� ����)
void Run(void);
void CompareWords(void);
void ProcessInput(int key);
void DrawWord(int i);                           // ȭ�鿡 ��Ÿ�� i��° �ܾ� ���
void EraseWord(int i);                          // ȭ�鿡 ��Ÿ�� i��° �ܾ� �����
void UpdateInputWord(void);
void UpdateScore(void);
void UpdateFailCount(void);
void InitWord(int i);
void NextStage(void);
void StageScreen(void);
void StartGame(void);
void pause(void);                               //�Ͻ����� ���

int main(void)
{
	SetCursorVisible(FALSE);
	g_start_time = clock();
	srand(time(NULL));
	StartGame();
	InitData();
	InitScreen();
	Run();
	return 0;
}

void InitScreen(void)
{
	int i;

	system("cls");

	printf("��");
	for (i = 0; i < 74; i++)
		printf("��");
	printf("��\n");
	for (i = 0; i < 19; i++)
		printf("��                                                                          ��\n");
	printf("��");
	for (i = 0; i < 74; i++)
		printf("��");
	printf("��\n");

	// Programmed by
	printf("����������������������������������������������\n");
	printf("��Programmed by �ڽ��� ��\n");
	printf("����������������������������������������������");

	// �Է�ĭ
	GotoXY(28, 21); printf("����������������������������");
	GotoXY(28, 22); printf("��            ��");
	GotoXY(28, 23); printf("����������������������������");

	//score ĭ
	GotoXY(56, 21); printf("������������");
	GotoXY(56, 22); printf("��   %d��", g_score);
	GotoXY(56, 23); printf("������������");

	//fail ĭ
	GotoXY(68, 21); printf("������������");
	GotoXY(68, 22); printf("��   %d��", g_fail_count);
	GotoXY(68, 23); printf("������������");
}
void UpdateScore(void)
{
	GotoXY(60, 22); printf("%d", g_score);
}
void UpdateFailCount(void)
{
	GotoXY(72, 22); printf("%d", g_fail_count);
}
void DrawWord(int i)
{
	GotoXY(g_screen_word[i].x, g_screen_word[i].y);
	printf("%s", g_words[g_screen_word[i].index]);
}
void EraseWord(int i)
{
	int count;
	if (g_screen_word[i].y > 0)
	{
		GotoXY(g_screen_word[i].x, g_screen_word[i].y);
		for (count = 0; count < strlen(g_words[g_screen_word[i].index]); count++)
			printf(" ");
	}
}
void InitData(void)
{
	int i;
	FILE* fp;
	fp = fopen("g_words.txt", "rt");

	g_words = (char**)malloc(sizeof(char*) * (WORD_COUNT + 1));

	for (i = 0; i < WORD_COUNT; i++)
	{
		g_words[i] = (char*)malloc(sizeof(char) * (MAX_WORD_LENGTH + 1));
		fgets(g_words[i], sizeof(char) * MAX_WORD_LENGTH, fp);
		g_words[i][strlen(g_words[i]) - 1] = '\0';
	}

	g_words[WORD_COUNT] = " ";

	fclose(fp);
}
void Run(void)
{
	int i, count, key;
	count = 1;

	for (i = 0; i < 19; i++)
	{
		InitWord(i);
	}
	while (1)
	{
		if (_kbhit())
		{
			key = _getch();
			ProcessInput(key);
			if (key == 27)                                       //ESCŰ �Է½� �Ͻ�����
			{
				system("cls");
				pause();
			}
		}
		if (GetDiffTime(g_start_time) > g_falling_speed)
		{
			for (i = 0; i < count; i++)
			{
				EraseWord(i);
				g_screen_word[i].y++;
				if (g_screen_word[i].y == 20)
				{
					if (g_screen_word[i].index != WORD_COUNT)
					{
						g_fail_count++;
						UpdateFailCount();
						NextStage();
					}
					InitWord(i);
					g_screen_word[i].y++;
					DrawWord(i);
				}
				else
					DrawWord(i);

			}
			if (count < MAX_SCREEN_WORD_COUNT)
				count++;
			g_start_time = clock();

		}
	}
}
void InitWord(int i)
{
	g_screen_word[i].index = rand() % (WORD_COUNT);
	g_screen_word[i].x = rand() % 73 + 2;
	g_screen_word[i].y = 0;

	if (g_screen_word[i].x + strlen(g_words[g_screen_word[i].index]) > 75)
	{
		while (1)
		{
			g_screen_word[i].x = rand() % 73 + 2;
			if (g_screen_word[i].x + strlen(g_words[g_screen_word[i].index]) < 75)
				break;
		}
	}

}
void ProcessInput(int key)
{
	if (key == KEY_BS)
	{
		if (g_input_word_length != 0)
		{
			GotoXY(30 + g_input_word_length - 1, 22);
			g_input_word[g_input_word_length] = NULL;
			printf(" ");
			g_input_word_length -= 2;
		}
	}
	if (key != KEY_ENTER)
	{
		if (g_input_word_length != 24)
		{
			GotoXY(30 + g_input_word_length, 22);
			printf("%c", key);
			g_input_word[g_input_word_length] = key;
			g_input_word_length++;
		}
	}
	if (key == KEY_ENTER)
	{
		int count;
		CompareWords();
		GotoXY(30, 22);
		for (count = 0; count < strlen(g_input_word); count++)
		{
			printf(" ");
		}
		for (count = 0; count < g_input_word_length; count++)
		{
			g_input_word[count] = NULL;
		}
		g_input_word_length = 0;
	}
	NextStage();

}
void CompareWords(void)
{
	int i;
	int	Delete = -1;

	for (i = 0; i < 19; i++)
	{
		if (strcmp(g_input_word, g_words[g_screen_word[i].index]) == 0)
		{
			if (Delete != -1)
			{
				if (g_screen_word[i].y > g_screen_word[Delete].y)
					Delete = i;
			}
			if (Delete == -1)
				Delete = i;
		}

	}
	if (Delete != -1)
	{
		EraseWord(Delete);
		g_screen_word[Delete].index = WORD_COUNT;
		g_score++;
		UpdateScore();
	}

}
void NextStage(void)
{
	if (g_fail_count == 5)
	{
		system("cls");

		GotoXY(28, 10); printf("����������������������������������������������������");
		GotoXY(28, 11); printf("��       fail.sorry       ��");
		GotoXY(28, 12); printf("����������������������������������������������������");
		GotoXY(23, 14); printf("�ƹ�Ű�� �ι������� ������ ����˴ϴ�.");
		system("pause > null");
		exit(0);
	}


	if (g_score == 8)
	{
		g_falling_speed -= 0.2;
		g_score = 0;
		g_fail_count = 0;
		UpdateScore();
		UpdateFailCount();
		StageScreen();
	}


}
void StageScreen(void)
{
	system("cls");

	if (stage < 5)
	{
		system("cls");
		PlaySound(TEXT("�ڼ��Ҹ�.wav"), NULL, SND_ASYNC | SND_NODEFAULT);
		GotoXY(28, 10); printf("������������������������������������������������������");
		GotoXY(28, 11); printf("��     %d Stage Clear!      ��", stage);
		GotoXY(28, 12); printf("��     Next Stage >> %d     ��", stage + 1);
		GotoXY(28, 13); printf("������������������������������������������������������");
		GotoXY(22, 15); printf("���� STAGE�� �����Ϸ��� �ƹ�Ű�� �����ּ���.");
		system("pause > null");
		stage += 1;
		InitScreen();
		Run();
	}
	else
	{
		GotoXY(28, 10); printf("����������������������������������������������������");
		GotoXY(28, 11); printf("��   Success. thank you   ��");
		GotoXY(28, 12); printf("����������������������������������������������������");
		Sleep(3000);
		exit(0);
	}
}
void StartGame(void)
{
	int i, key;

	printf("��");
	for (i = 0; i < 74; i++)
		printf("��");
	printf("��\n");
	for (i = 0; i < 22; i++)
		printf("��                                                                          ��\n");
	printf("��");
	for (i = 0; i < 74; i++)
		printf("��");
	printf("��\n");

	GotoXY(28, 10); printf("����������������������������������������������������");
	GotoXY(28, 11); printf("��   If You Want Start    ��");
	GotoXY(28, 12); printf("��      key : enter       ��");
	GotoXY(28, 13); printf("����������������������������������������������������");

	while (1)
	{
		if (_kbhit())
		{
			key = _getch();
			if (key == KEY_ENTER)
				break;
		}
	}

}
void pause(void)
{
	GotoXY(30, 10); printf("��� ������ ���߼̽��ϴ�");
	GotoXY(21, 12); printf("������ ����Ͻ÷��� �ƹ�Ű�� �����ּ���");
	_getch();
	system("cls");
	InitScreen();
}