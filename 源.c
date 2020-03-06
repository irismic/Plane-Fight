#include<stdio.h>
#include <windows.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#define width 61													//设定窗口宽度
#define high 40													//窗口高度
#define MAX_enemy  10										//敌人最大数量
int enemynum = 0;
int score = 0;
typedef struct _Point {
	int x, y;
} point;
typedef struct _Plane {						//定义飞机结构
	char type;											//飞机类型
	point xy;											//飞机坐标
	char Shape[5][5];								//飞机形状
	int speed;											//飞行速度
	int Shapewidth, Shapehigh;			//飞机占据空间
	char flag;											//飞机状态
	char hp;											//飞机血量
} plane;
typedef struct _Bullet {
	point xy;
	char flag;
} _bullet;
void Menu(void);
void Instructions();
void Introduce();
void pause();
void print(_bullet*, plane, plane*);
void shooting(_bullet*);
void move(plane*, _bullet*,int*);
void enemy(plane*,int*);
void bingo(plane*,plane*,_bullet*);
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
		.flag = 1,
		.hp = 10,
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
		{' ','*',' ','*',},
		{' ',' ','*',},},
		{
		{'*','*','*',},
		{' ','*',},},
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
	_bullet bullet[high];
	for (int i = 0; i < high; i++) {
		bullet[i].flag = 0;
		bullet[i].xy.x = 0;
		bullet[i].xy.y = 0;
	}
	int shootnum = 0;
	int speed[10] = { 0 };
	HideCursor();
	/*———————以上是初始化———————*/
	Menu();
	while (1) {
		shooting(bullet);
		move(&me,bullet,&shootnum);
		enemy(enemyplane,speed);
		print(bullet,me,enemyplane);
		bingo(enemyplane, &me,bullet);
		system("cls");
	}
	return 0;
}								
void shooting(_bullet* bullet) {	//控制子弹的上升
	_bullet* pbullet = bullet;
	for (int i = 0; i < high; i++, bullet++) {
		if (bullet->xy.y > 1) {
			if (bullet->flag != 0) {
				bullet->xy.y--;
			}
		}
		else {
			bullet->flag = 0;
		}
	}
	bullet = pbullet;
}
void move(plane* pme, _bullet* bullet,int *shootnum)																			//控制己方飞机移动
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
			if (pme->xy.y < high - pme->Shapehigh) {
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
			if (*shootnum == high - 1) {
			*shootnum = 0;
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
void enemy(plane* enemyplane,int* speed) {																//控制敌方飞机的下落，以及随机生成
	for (int i = 0; i < (score < 10 ? MAX_enemy / 5 : (score < 20 ? MAX_enemy / 2 : MAX_enemy)); i++, enemyplane++,speed++) {
		if (enemyplane->flag == 0) {
			enemyplane->flag = 1;
			enemyplane->xy.x = rand() % (width - enemyplane->Shapewidth) + enemyplane->Shapewidth / 2;
			enemyplane->xy.y = 0;
			enemynum++;
			switch (enemyplane->flag) {																			//重置敌方飞机血量
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
		if (*speed == enemyplane->speed) {
			enemyplane->xy.y++;
			*speed = 0;
		}
		else {
			(*speed)++;
		}
	}
}
void print(_bullet* bullet, plane me, plane* enemyplane) {	//在窗口显示出界面
	for (int t = 0; t < high; t++, bullet++) {
		if (bullet->flag != 0) {
			Gotoxy(bullet->xy.x, bullet->xy.y);
			printf("%c", '|');
		}
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
void bingo(plane* enemyplane,plane*me, _bullet* bullet) {															//判断子弹命中，以及游戏结束
		plane* penemyplane = enemyplane;
		for (int t = 0; t < enemynum; t++, enemyplane++) {
			_bullet* pbullet = bullet;
			for (int u = 0; u < high; u++, bullet++) {
				if (bullet->flag != 0) {
					if (bullet->xy.x >= enemyplane->xy.x && bullet->xy.x <= (enemyplane->xy.x + enemyplane->Shapewidth) && bullet->xy.y >= enemyplane->xy.y && bullet->xy.y <= (enemyplane->xy.y + enemyplane->Shapewidth)) {
						score++;
						enemyplane->hp--;
						bullet->flag = 0;
					}
				}
			}
			bullet = pbullet;
			if (enemyplane->hp == 0) {
				enemyplane->flag = 0;
				enemynum--;
			}
			if (enemyplane->xy.y > width - 10) {																			
				enemyplane->flag = 0;
				enemynum--;
				me->hp--;
			}
			if (me->hp == 0) {
				system("cls");
				printf("\n\n\n\n\n\t\tyou die");
				char ch = 0;
				ch = _getch();
				while (ch < '1' || ch>'2' || ch != 27) {
					printf("\n输入有误");
					ch = _getch();
				}
			}
		}
		enemyplane = penemyplane;
}
void Gotoxy(int x, int y) {																									//控制光标移动
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void HideCursor(){																											//隐藏光标
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = sizeof(cursor);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cursor);
}
void Menu() {//主菜单
	while (1) {
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		printf("\n\n\n\n\n\t\t    主       菜       单\n\n");
		printf("\n\n\t\t\t  飞机大战\n\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		printf("\t     ------------------------------------\n\n");
		printf("\t\t       (1):开始游戏\n\n");
		printf("\t\t       (2):操作介绍\n\n");
		printf("\t\t       (3):游戏介绍\n\n");
		printf("\t\t       (4):退出游戏\n\n");
		printf("\t    ------------------------------------\n\n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		printf("   ****************第一次请先观看操作介绍****************\n\n");
		printf("\t\t         请输入序号。\n");
		int s;
		s = _getch();
		while (s < '1' || s>'4'){
			printf("\t\t输入有误，请重新输入。\n");
			s = _getch();
		}
		if (s == '1') 
			break;
		else if (s == '2') 
			Instructions();
		else if(s=='3')
			Introduce();
		else{
			int i = MessageBox(NULL, L"\n确定要退出游戏吗？", L"退出提示", MB_YESNO);
			if (i == IDYES)
			exit(0);
		}
	}
}
void Instructions() {
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	Gotoxy(26, 8);
	printf("游戏操作");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	Gotoxy(13, 11);
	printf("进行游戏前务必将输入法设置为英文！！！");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	Gotoxy(19, 14);
	printf("W,S,A,D分别控制上下左右");
	Gotoxy(24, 16);
	printf("空格发射子弹");
	Gotoxy(25, 20);
	printf(" 游戏愉快！");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	Gotoxy(25, 26);
	printf(" 按ESC返回");
	char ch = 0;
	do {
		ch = _getch();
	} while (ch != 27);
}
void Introduce() {
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY |FOREGROUND_GREEN);
	Gotoxy(18,10);
	printf("作者是大一计算机专业在读");
	Gotoxy(17, 12);
	printf("趁着寒假时间自学了一些C语言");
	Gotoxy(13, 14);
	printf("前后磕磕绊绊写半个月(实在是有些慢)");
	Gotoxy(21, 16);
	printf("不过最后也是结了尾");
	Gotoxy(13, 18);
	printf("如果您在此程序中发现有bug请多多海涵");
	Gotoxy(15, 20);
	printf("或者在github上回复，我会及时回复");
	Gotoxy(23, 22);
	printf("感谢您的支持！");
	Gotoxy(27, 24);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	printf("ESC返回");
	char ch = 0;
	do {
		ch = _getch();
	} while (ch != 27);
}
void pause() {
	while (1) {
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		Gotoxy(21, 10);
		printf("*****游戏暂停*****");
		printf("\n\t\t   (1)操作介绍");
		printf("\n\t\t   (2)结束游戏");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		char ch = 0;
		ch = _getch();
		while (ch < '1' || ch>'2' ){
			if (ch == 27)
				break;
			printf("\n输入有误");
			ch = _getch();
		}
		if (ch == '1') {
			Instructions();
		}else if(ch == '2') {
			int i = MessageBox(NULL, L"\n确定要退出游戏吗？", L"退出提示", MB_YESNO);
			if (i == IDYES)
				exit(0);
		}else if (ch == 27) {
			break;
		}	
	}
}