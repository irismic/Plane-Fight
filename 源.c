#include<stdio.h>
#include <windows.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#define width 61													//�趨���ڿ��
#define high 40													//���ڸ߶�
#define MAX_enemy  10										//�����������

int enemynum = 0;												//�����ִ�����

int score = 0;														//�÷�

typedef struct _Point {											//�����������
	int x, y;
} point;

typedef struct _Plane {						//����ɻ��ṹ
	char type;											//�ɻ�����
	point xy;											//�ɻ�����
	char Shape[5][5];								//�ɻ���״
	int speed;											//�����ٶ�
	int Shapewidth, Shapehigh;			//�ɻ�ռ�ݿռ�
	char flag;											//�ɻ�״̬
	char hp;											//�ɻ�Ѫ��
} plane;

typedef struct _Bullet {										//�ӵ��ṹ
	point xy;
	char flag;
} _bullet;

void Initialization(plane*, plane*, _bullet*,int*,int*);					//��ʼ��
void Menu();																				//���˵�
void mainloop(plane*,plane*,_bullet*,int*,int*);							//��ѭ��
void Instructions();																		//��Ϸ˵��
void Introduce();
void pause();																				//��Ϸ����ͣ
void print(_bullet*, plane*, plane*);												//��ͼ����
void shooting(_bullet*);																//�����ӵ�����
void move(plane*, _bullet*,int*);													//��������ƶ�����
void enemy(plane*,int*);																//���������ƶ�
char bingo(plane*,plane*,_bullet*,int*,int*);								//�ж���Ϸ�е����е�
void Gotoxy(int x, int y);																//����ƶ�
void HideCursor();																		//�������
char die(plane*, plane*, _bullet*, int*, int*);								//gameover����

int main() {
	system("mode con cols=61 lines=40");
	plane me;
	plane enemyplane[MAX_enemy];
	_bullet bullet[high];
	int shootnum;
	int speed[MAX_enemy];
	while (1) {
		Menu();
		Initialization(&me, enemyplane, bullet, speed, &shootnum);
		mainloop(&me, enemyplane, bullet, speed, &shootnum);
	}
	return 0;
}		

void shooting(_bullet* bullet) {	//�����ӵ�������
	_bullet* pbullet = bullet;
	for (int i = 0; i < high; i++, bullet++) {									//ָ��������ݶ�ȡ��i����ѭ���������������ƽṹһ����Ч��
		if (bullet->xy.y > 1) {															//ȱ������Ҫ��ʼ��¼��ͷָ�룬��Ȼ�ٴ������ָ��ָ�����������
			if (bullet->flag != 0) {														//��Ϊָ���ѽ���ָ�������ͷ�����������ƽṹ
				bullet->xy.y--;																
			}
		}
		else {
			bullet->flag = 0;
		}
	}
	bullet = pbullet;
}

void move(plane* pme, _bullet* bullet,int *shootnum)																			//���Ƽ����ɻ��ƶ�
{
	char input;
	if (_kbhit()) {
		input = _getch();
		switch (input) {
		case 'w':
		case'W':
			if (pme->xy.y > 0) {
				pme->xy.y--;
			}
			break;
		case's':
		case'S':
			if (pme->xy.y < high - pme->Shapehigh-5) {
				pme->xy.y++;
			}
			break;
		case 'a':
		case'A':
			if (pme->xy.x > 0) {
				pme->xy.x--;
			}
			break;
		case'd':
		case'D':
			if (pme->xy.x < width - pme->Shapewidth) {
				pme->xy.x++;
			}
			break;
		case' ':
			if (*shootnum == high - 1) {											//�����õ���shootnum,���ǵ��ӵ�����������ʱ�ͻ���ʧ���ӵ��������Ҳ���Ǹ߶�high
			*shootnum = 0;																//��shoot==high-1ʱ������һ���ӵ�������bullet��ͷ�����������ж���ӵ�����
			}
			bullet[*shootnum].flag = 1;
			bullet[*shootnum].xy = pme->xy;
			bullet[*shootnum].xy.x += pme->Shapewidth / 2;
			(*shootnum)++;
			break;
		case 27:
			pause();
			break;
		}
	}
}

void enemy(plane* enemyplane,int* speed) {																//���Ƶз��ɻ������䣬�Լ��������
	for (int i = 0; i < (score < 10 ? MAX_enemy / 5 : (score < 20 ? MAX_enemy / 2 : MAX_enemy)); i++, enemyplane++,speed++) {
		if (enemyplane->flag == 0) {
			enemyplane->flag = 1;
			enemyplane->xy.x = rand() % (width - enemyplane->Shapewidth-1) + enemyplane->Shapewidth / 2;
			enemyplane->xy.y = 0;
			enemynum++;
			switch (enemyplane->type) {																			//��������type���õз��ɻ�Ѫ��
			case 0:
				enemyplane->hp = 3;
				break;
			case 1:
				enemyplane->hp = 2;
				break;
			case 2:
				enemyplane->hp = 1;
				break;
			}
		};
		if (*speed == enemyplane->speed) {		//�����õ���speed���飬�������Ƶ��˵��ٶȣ�ԭ����ÿ�����˶���speed����һ������
			enemyplane->xy.y++;							//��������ݴﵽenemyplane�е�speedʱ��*speed == enemyplane->speed����������y�������ӣ�Ҳ�����½���
			*speed = 0;											//��Ȼ���ö�Ӧspeed++
		}
		else {
			(*speed)++;
		}
	}
}

void print(_bullet* bullet, plane* me, plane* enemyplane) {	//�ڴ�����ʾ������
	for (int t = 0; t < high; t++, bullet++) {								//ԭ������Gotoxy�����ù���ƶ���ÿ���ӵ����߷ɻ��Ŀ�ͷ
		if (bullet->flag != 0) {														//Ȼ����printf���forѭ���������
			Gotoxy(bullet->xy.x, bullet->xy.y);
			printf("%c", '|');
		}
	}
	plane* penemyplane = enemyplane;
	for (int s = 0; s <MAX_enemy; s++, enemyplane++) {
		if (enemyplane->flag != 0) {
			for (int a = enemyplane->xy.x; a < enemyplane->xy.x + enemyplane->Shapewidth; a++) {
				for (int b = enemyplane->xy.y; b < enemyplane->xy.y + enemyplane->Shapehigh; b++) {
					Gotoxy(a, b);
					printf("%c", enemyplane->Shape[b - enemyplane->xy.y ][a - enemyplane->xy.x]);
				}
			}
		}
	}
	enemyplane = penemyplane;
	for (int a = me->xy.x; a < me->xy.x + me->Shapewidth; a++) {
		for (int b = me->xy.y; b < me->xy.y + me->Shapehigh; b++) {
			Gotoxy(a, b);
			printf("%c", me->Shape[b - me->xy.y][a - me->xy.x]);
		}
	}
	Gotoxy(0, 35);
	printf("#############################################################");
	Gotoxy(0, 38);
	printf("    �÷֣�%d\t\t\t\t\tѪ����%d\n    ESC����ͣ", score,me->hp);
}

char bingo(plane* enemyplane,plane*me, _bullet* bullet,int*speed,int*shootnum) {		//�ж��ӵ����У��Լ���Ϸ����
		plane* penemyplane = enemyplane;																			//���ӵ�����������˵��������򵥵��ж�
		char a = 0;
		for (int t = 0; t < enemynum; t++, enemyplane++) {
			_bullet* pbullet = bullet;
			for (int u = 0; u < high; u++, bullet++) {
				if (bullet->flag != 0) {
					if (bullet->xy.x >= enemyplane->xy.x && bullet->xy.x <= (enemyplane->xy.x + enemyplane->Shapewidth) && bullet->xy.y >= enemyplane->xy.y && bullet->xy.y <= (enemyplane->xy.y + enemyplane->Shapewidth)) {
						score++;
						(enemyplane->hp)--;
						bullet->flag = 0;
					}
				}
			}
			bullet = pbullet;
			if (enemyplane->hp == 0) {
				enemyplane->flag = 0;
				enemynum--;
			}
			if ((enemyplane->xy.y + enemyplane->Shapehigh) > high - 5) {
				enemyplane->hp = 0;
				enemyplane->flag = 0;
				enemynum--;
				me->hp--;
			}
			if (me->hp == 0) {
				me->flag = 0;
				a=die(me,penemyplane,bullet,speed,shootnum);
				break;
			}
		}
		enemyplane = penemyplane;
		return a;
}

char die(plane* me, plane* enemyplane, _bullet* bullet, int* speed, int* shootnum) {
	system("cls");
	printf("\n\n\n\n\n\t\tyou die");
	char a[] = { "�Ƿ������Ϸ��" };
	char* pa = a;
	char b[3] = { 0,0,'\0' };
	Sleep(1000);
	printf("\n\t\t");
	do {
		memcpy(b, pa, 2);
		printf("%s", b);
		pa += 2;
		Sleep(100);
	} while ((b[0]) != '\0');
	printf("\n\n\t\t\t(1)yes       (2)no");
	char ch = 0;
	ch = _getch();
	while (ch < '1' || ch>'2') {
		printf("\n\n\n\t\t\t��������");
		ch = _getch();
	}
	if (ch == '1') {
		return 1;
	}
	else {
		exit(0);
	}
}

void mainloop(plane* me, plane* enemyplane, _bullet* bullet, int* speed, int* shootnum) {	//����Ϸѭ��
	while (1) {
		char a = bingo(enemyplane, me, bullet, speed, shootnum);
		if (a == 1)
			break;
		shooting(bullet);
		move(me, bullet, shootnum);
		enemy(enemyplane, speed);
		print(bullet, me, enemyplane);
		Sleep(20);
		system("cls");
	}
}

void Initialization(plane* me, plane* enemyplane, _bullet* bullet, int* speed, int* shootnum) {			//�����������Լ��ṹ��ʼ��
	HideCursor();
	char myshape[5][5] = {
		{' ', ' ', '*', ' ', ' ', },
		{ ' ','*',' ','*',' ', },
		{ '*',' ','*',' ','*', },
		{ '*','*','*','*','*', },
		{ ' ','*',' ','*',' ', },
	};
	memcpy(me->Shape, myshape, sizeof(myshape));
	me->Shapewidth = sizeof(me->Shape[0]) / sizeof(me->Shape[0][0]);
	me->Shapehigh = sizeof(me->Shape) / sizeof(me->Shape[0]);
	me->xy.x = (width - sizeof(me->Shape[0]) / sizeof(me->Shape[0][0])) / 2;
	me->xy.y = high / 2;
	me->speed = 0;
	me->flag = 1;
	me->hp = 10;
	char shape[3][5][5] = { {
	{' ','*',' ','*',' ',},
	{'*','*','*','*','*',},
	{'*',' ','*',' ','*',},
	{' ','*','*','*',' ',},
	{' ',' ','*',' ',' ',},},
	{
	{' ','*',' ','*',' ',},
	{'*','*','*','*','*',},
	{' ','*',' ','*',},
	{' ',' ','*',},},
	{
	{'*','*','*',},
	{' ','*',},},
	};
	for (int i = 0; i < MAX_enemy; i++) {
		enemyplane[i].type = rand() % 3;
		enemyplane[i].Shapewidth = sizeof(enemyplane[0].Shape[0]) / sizeof(enemyplane[0].Shape[0][0]);
		enemyplane[i].Shapehigh = sizeof(enemyplane[0].Shape) / sizeof(enemyplane[0].Shape[0]);
		enemyplane[i].flag = 0;
		switch (enemyplane[i].type)
		{
		case 0:
			enemyplane[i].speed = 40;
			memcpy(enemyplane[i].Shape, shape[0], sizeof(shape[0]));
			break;
		case 1:
			enemyplane[i].speed = 20;
			memcpy(enemyplane[i].Shape, shape[1], sizeof(shape[1]));
			break;
		case 2:
			enemyplane[i].speed = 10;
			memcpy(enemyplane[i].Shape, shape[2], sizeof(shape[2]));
			break;
		}
	}
	for (int i = 0; i < high; i++) {
		bullet[i].flag = 0;
		bullet[i].xy.x = 0;
		bullet[i].xy.y = 0;
	}
	*shootnum = 0;
	for (int t = 0; t < MAX_enemy; t++, speed++) {
		*speed = 0;
	}
	enemynum = 0;
	score = 0;
}

void pause() {																								//��Ϸ����ͣ
	while (1) {
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		Gotoxy(21, 10);
		printf("*****��Ϸ��ͣ*****");
		printf("\n\t\t   (1)��������");
		printf("\n\t\t   (2)������Ϸ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		char ch = 0;
		ch = _getch();
		while (ch < '1' || ch>'2') {
			if (ch == 27)
				break;
			printf("\n\t\t  ��������");
			ch = _getch();
		}
		if (ch == '1') {
			Instructions();
		}
		else if (ch == '2') {
			int i = MessageBox(NULL, L"\nȷ��Ҫ�˳���Ϸ��", L"�˳���ʾ", MB_YESNO);
			if (i == IDYES)
				exit(0);
		}
		else if (ch == 27) {
			break;
		}
	}
}

void Gotoxy(int x, int y) {																									//���ƹ���ƶ�
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void HideCursor(){																											//���ع��
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = sizeof(cursor);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cursor);
}

void Menu() {//���˵�
	while (1) {
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		printf("\n\n\n\n\n\t\t    ��       ��       ��\n\n");
		printf("\n\n\t\t\t  �ɻ���ս\n\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		printf("\t     ------------------------------------\n\n");
		printf("\t\t       (1):��ʼ��Ϸ\n\n");
		printf("\t\t       (2):��������\n\n");
		printf("\t\t       (3):��Ϸ����\n\n");
		printf("\t\t       (4):�˳���Ϸ\n\n");
		printf("\t    ------------------------------------\n\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		printf("   ****************��һ�����ȹۿ���������****************\n\n");
		printf("\t\t         ��������š�\n");
		int s;
		s = _getch();
		while (s < '1' || s>'4'){
			printf("\t\t�����������������롣\n");
			s = _getch();
		}
		if (s == '1') 
			break;
		else if (s == '2') 
			Instructions();
		else if(s=='3')
			Introduce();
		else{
			int i = MessageBox(NULL, L"\nȷ��Ҫ�˳���Ϸ��", L"�˳���ʾ", MB_YESNO);
			if (i == IDYES)
				exit(0);
		}
	}
}

void Instructions() {
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	Gotoxy(26, 8);
	printf("��Ϸ����");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	Gotoxy(13, 11);
	printf("������Ϸǰ��ؽ����뷨����ΪӢ�ģ�����");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	Gotoxy(19, 14);
	printf("W,S,A,D�ֱ������������");
	Gotoxy(15, 16);
	printf("�ո����ӵ�����ֹ���˿����ײ�");
	Gotoxy(25, 20);
	printf(" ��Ϸ��죡");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	Gotoxy(25, 26);
	printf(" ��ESC����");
	char ch = 0;
	do {
		ch = _getch();
	} while (ch != 27);
}

void Introduce() {
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY |FOREGROUND_GREEN);
	Gotoxy(18,10);
	printf("�����Ǵ�һ�����רҵ�ڶ�");
	Gotoxy(17, 12);
	printf("���ź���ʱ����ѧ��һЩC����");
	Gotoxy(13, 14);
	printf("ǰ��Ŀİ��д�����(ʵ������Щ��)");
	Gotoxy(21, 16);
	printf("�������Ҳ�ǽ���β");
	Gotoxy(13, 18);
	printf("������ڴ˳����з�����bug���ຣ��");
	Gotoxy(15, 20);
	printf("������github�ϻظ����һἰʱ�ظ�");
	Gotoxy(23, 22);
	printf("��л����֧�֣�");
	Gotoxy(27, 24);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("ESC����");
	char ch = 0;
	do {
		ch = _getch();
	} while (ch != 27);
}
