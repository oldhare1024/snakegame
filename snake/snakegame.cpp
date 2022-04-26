//����̰������Ϸ
#include<stdio.h>
#include<windows.h>
#include<graphics.h>
#include<conio.h>
#include<stdlib.h>
#include<mmsystem.h>
#include<tchar.h>
#pragma comment(lib,"winmm.lib")
#define SNAKE_NUM 500//�ߵ�������
HWND hwnd = NULL;
int i=0,point=0;
enum DIR//���巽��
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};
struct Snake//��ʼ����
{
	int size;//�ߵĽ���
	int dir;//�ߵķ���
	int speed;//�ߵ��ƶ��ٶ�
	POINT coor[SNAKE_NUM];//����(x,y)
}snake;
struct Food//����ʳ��
{
	int x, y, r;//ʳ��������ɵ������Լ��̶��뾶
	bool flag;//�Ƿ񱻳�
	DWORD color;//ʳ�����ɫ
}food;
void GameInit()	//��ʼ��
{
	//����������ӣ�ֵΪϵͳ����ʱ�䣨����Ϊ��λ��
	srand(GetTickCount64());
	snake.size = 10;
	snake.speed = 10;
	snake.dir;
	for (i = 0; i < snake.size; i++)
	{
		snake.coor[i].x = 400-10*i;
		snake.coor[i].y = 300;
	}
	//�������ʳ��λ�ã���ɫ���̶��뾶��С
	food.x = rand()%800+1;
	food.y = rand()%600+1;
	food.color = RGB(rand()%256, rand() % 256, rand() % 256);
	food.r = 7;
	food.flag = true;
}
void GameDraw()//������
{
	BeginBatchDraw();
	//���ô��ڱ���RGB��ɫ
	setbkcolor(RGB(28,115,119));
	//�����������
	cleardevice();
	//����������ɫ
	setfillcolor(GREEN);
	solidcircle(snake.coor[0].x, snake.coor[0].y, 6);//������ͷԲ����뾶
	for (i = 1; i < snake.size; i++)
	{
		setfillcolor(RGB(255,255,224));
		solidcircle(snake.coor[i].x, snake.coor[i].y, 5);//��������뾶
		FlushBatchDraw();
	}
	if (food.flag)
		solidcircle(food.x,food.y,food.r);
	EndBatchDraw();
}
//�ƶ���
void snakeMove()
{
	//�ƶ���ʲô�����ı�
	for (i = snake.size-1;i>0;i--)
	{
		snake.coor[i] = snake.coor[i - 1];
	}
		switch (snake.dir)
		{
		case UP:
			snake.coor[i].y-=snake.speed;
			if (snake.coor[0].y + 10 <= 0)
				snake.coor[0].y = 600;
			break;
		case DOWN:
			snake.coor[i].y+=snake.speed;
			if (snake.coor[0].y + 10 >=600 )
				snake.coor[0].y = 0;
			break;
		case LEFT:
			snake.coor[i].x-= snake.speed;
			if (snake.coor[0].x + 10 <= 0)
				snake.coor[0].x = 800;
			break;
		case RIGHT:
			snake.coor[i].x+= snake.speed;
			if (snake.coor[0].x + 10 >= 800)
				snake.coor[0].x = 0;
			break;
		}
}
void stop()//��ͣ����
{
	if (_getch() == ' ')//��ͣ����Ϊ�ո�
	{
		while (1)
		{
			if (_getch() != ' ')
				break;
		}
	}
}
void keyControl()//ͨ������WASD�ı��ƶ�����
{
	//�ж��Ƿ񰴼�
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'w':
		case 'W':
		case 72:
		if(snake.dir !=DOWN)
			snake.dir = UP;
				break;
		case 's':
		case 'S':
		case 80:
			if (snake.dir != UP)
				snake.dir = DOWN;
			break;
		case 'a':
		case 'A':
		case 75:
			if (snake.dir != RIGHT)
				snake.dir = LEFT;
			break;
		case 'd':
		case 'D':
		case 77:
			if (snake.dir != LEFT)
				snake.dir = RIGHT;
			break;
		case ' ':
			while (1)
			{
				if (_getch() == ' ')
					stop();
			}
			break;
		}
	}
}
void Eatfood()//��ʳ�����
{
	if (food.flag && snake.coor[0].x >= food.x - food.r && snake.coor[0].x <= food.x + food.r && snake.coor[0].y >= food.y - food.r && snake.coor[0].y <= food.y + food.r)
	{
		food.flag = false;
		snake.size+=3;
	}
	if (!food.flag)
	{
		food.x = rand() % 800 + 1;
		food.y = rand() % 600 + 1;
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
		food.r = 7;
		food.flag = true;
	}
	/*�Ʒ��ƶ�
	if (food.flag == true)
	{	
		point++;
	}
	*/
}
int gameover()//��ײ�����Ϸ����
{
	for (int i = 1; i < snake.size; i++) {
		if (snake.coor[0].x == snake.coor[i].x && snake.coor[0].y == snake.coor[i].y) {
			MessageBox(hwnd,_T("��ײ���Լ�!"), _T( "GAME OVER!"), MB_OK);
			return 1;
		}
	}
	return 0;
}
int main()
{
	//��ʼ��graphͼ�񴰿ڷֱ��ʴ�С
	initgraph(800, 600);
	//�滻����ͼƬ
	IMAGE background;//����һ��ͼƬ��.
	loadimage(&background, L"c:\\background.jpg", 800, 600, 1);//��ͼƬ�ļ���ȡͼ��
	putimage(0, 0, &background);//����ͼ����Ļ��ͼƬ���Ͻ�����Ϊ(0,0)
	_getch();//��ֹ����ر�
	//���ű�������
	mciSendString(_T("open c:\\crystaloverture.mp3 alias BGM"), NULL, 0, NULL);
	mciSendString(_T("play BGM repeat"), NULL, 0, NULL);
	GameInit();
	while (!gameover())//���ù��ܺ���
	{
		GameDraw();
		snakeMove();
		keyControl();
		Eatfood();
		Sleep(80);
	 }
	closegraph();
}
