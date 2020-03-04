#include<stdio.h>
#include <windows.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#define width 61													//�趨���ڿ��
#define high 40													//���ڸ߶�
#define MAX_enemy  10										//�����������
int shootflag = 1;
int enemynum = 0;
int score = 0;
typedef struct _Point {
	int x, y;
} point;
typedef struct _Plane {						//����ɻ��ṹ
	char type;											//�ɻ�����
	point xy;											//�ɻ�����
	char Shape[5][5];								//�ɻ���״
	int speed;											//�����ٶ�
	int Shapewidth, Shapehigh;			//�ɻ�ռ�ݿռ�
	char flag;											//�ɻ�״̬
} plane;
void Menu(void);
void Instructions();
void Introduce();
void print(point, plane, plane*);
void shooting(point*);
void move(plane*, point*);
void enemy(plane*,int*);
void bingo(plane*,point);
void Gotoxy(int x, int y);
void HideCursor();
int main() {
	system("mode con cols=61 lines=40");
	plane me = {
		.Shape = {
		{' ',' ','*',' ',' ',},
		{' ','*',' ','*',' ',},
		{'*',' ','*',' ','*',},
		{' ',' ','*',' ',' ',},
		{'*','*',' ','*','*',},
		},
		.Shapewidth = sizeof(me.Shape[0]) / sizeof(me.Shape[0][0]),.Shapehigh = sizeof(me.Shape) / sizeof(me.Shape[0]),
		.xy = {(width - sizeof(me.Shape[0]) / sizeof(me.Shape[0][0])) / 2,high / 2},
		.speed = 0,
		.flag = 1
	};
	srand((unsigned)time(NULL));
	char shape[3][5][5] = { {
		{' ','*',' ','*',' ',},
		{'*','*','*','*','*',},
		{'*',' ','*',' ','*',},
		{' ','*','*','*',' ',},
		{' ',' ','*',' ',' ',},},
		{
		{' ','*',' ','*',' ',},
		{'*','*','*','*','*',},
		{' ','*',' ','*',' ',},
		{' ',' ','*',' ',' ',},
		{' ',' ',' ',' ',' ',},},
		{
		{'*','*','*',' ',' ',},
		{' ','*',' ',' ',' ',},
		{' ',' ',' ',' ',' ',},
		{' ',' ',' ',' ',' ',},
		{' ',' ',' ',' ',' ',},},
		};
	plane enemyplane[MAX_enemy];
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
	point shoot = { 0,0 };
	int speed[10] = { 0 };
	HideCursor();
	/*�������������������ǳ�ʼ����������������*/
	Menu();
	while (1) {
		shooting(&shoot);
		move(&me,&shoot);
		enemy(enemyplane,speed);
		print(shoot,me,enemyplane);
		bingo(enemyplane, shoot);
		system("cls");
	}
	return 0;
}								
void shooting(point *shoot){																							//�����ӵ�������
	if (shoot->y > 1) {
		shoot->y--;
	}
	else {
		shootflag = 0;
	}
}
void move(plane* pme, point* shoot)																			//���Ƽ����ɻ��ƶ�
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
			if (pme->xy.y < high- pme->Shapehigh){
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
			if (pme->xy.x < width- pme->Shapewidth) {
				pme->xy.x++;
			}
			break;
		case' ':
			shootflag = 1;
			*shoot = pme->xy;
			shoot->x += pme->Shapewidth / 2;
		}
	}
}
void enemy(plane* enemyplane,int* speed) {																//���Ƶз��ɻ������䣬�Լ��������
	for (int i = 0; i < (score < 10 ? MAX_enemy / 5 : (score < 20 ? MAX_enemy / 2 : MAX_enemy)); i++, enemyplane++,speed++) {
		if (enemyplane->flag == 0) {
			enemyplane->flag = 1;
			enemyplane->xy.x = rand() % (width - enemyplane->Shapewidth) + enemyplane->Shapewidth / 2;
			enemyplane->xy.y = 0;
			enemynum++;
		};
		if (*speed == enemyplane->speed) {
			enemyplane->xy.y++;
			*speed = 0;
		}
		else {
			(*speed)++;
		}
	}
}
void print(point shoot, plane me, plane* enemyplane) {											//�ڴ�����ʾ������
	if (shootflag != 0) {
	Gotoxy(shoot.x, shoot.y);
	printf("%c", '|');
	}
	plane* penemyplane = enemyplane;
	for (int s = 0; s < enemynum; s++, enemyplane++) {
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
	for (int a = me.xy.x; a < me.xy.x + me.Shapewidth; a++) {
		for (int b = me.xy.y; b < me.xy.y + me.Shapehigh; b++) {
			Gotoxy(a, b);
				printf("%c", me.Shape[b - me.xy.y][a - me.xy.x]);
		}
	}	
}
void bingo(plane* enemyplane, point shoot) {															//�ж��ӵ��Ƿ�����
	if (shootflag == 1) {
		plane* penemyplane = enemyplane;
		for (int t = 0; t < enemynum; t++, enemyplane++) {
			if (shoot.x >= enemyplane->xy.x && shoot.x <= (enemyplane->xy.x + enemyplane->Shapewidth) && shoot.y >= enemyplane->xy.y && shoot.y <= (enemyplane->xy.y + enemyplane->Shapewidth)) {
				score++;
				enemyplane->flag = 0;
				enemynum--;
			}
		}
		enemyplane = penemyplane;
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
	while (1){
		system("cls");
		int s;
		printf("\n\n\n\n\n\t\t    ��       ��       ��\n\n");
		printf("\n\n\t\t\t  �ɻ���ս\n\n");
		printf("\t     ------------------------------------\n\n");
		printf("\t\t       (1):��ʼ��Ϸ\n\n");
		printf("\t\t       (2):��������\n\n");
		printf("\t\t       (3):��Ϸ����\n\n");
		printf("\t\t       (4):�˳���Ϸ\n\n");
		printf("\t    ------------------------------------\n\n");
		printf("   ****************��һ�����ȹۿ���������****************\n\n");
		printf("\t\t         ��������š�\n");
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
	
}
void Introduce() {

}