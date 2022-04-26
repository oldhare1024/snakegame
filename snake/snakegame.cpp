//制作贪吃蛇游戏
#include<stdio.h>
#include<windows.h>
#include<graphics.h>
#include<conio.h>
#include<stdlib.h>
#include<mmsystem.h>
#include<tchar.h>
#pragma comment(lib,"winmm.lib")
#define SNAKE_NUM 500//蛇的最大节数
HWND hwnd = NULL;
int i=0,point=0;
enum DIR//定义方向
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};
struct Snake//初始化蛇
{
	int size;//蛇的节数
	int dir;//蛇的方向
	int speed;//蛇的移动速度
	POINT coor[SNAKE_NUM];//坐标(x,y)
}snake;
struct Food//定义食物
{
	int x, y, r;//食物随机生成的坐标以及固定半径
	bool flag;//是否被吃
	DWORD color;//食物的颜色
}food;
void GameInit()	//初始化
{
	//设置随机种子，值为系统开机时间（毫秒为单位）
	srand(GetTickCount64());
	snake.size = 10;
	snake.speed = 10;
	snake.dir;
	for (i = 0; i < snake.size; i++)
	{
		snake.coor[i].x = 400-10*i;
		snake.coor[i].y = 300;
	}
	//随机生成食物位置，颜色，固定半径大小
	food.x = rand()%800+1;
	food.y = rand()%600+1;
	food.color = RGB(rand()%256, rand() % 256, rand() % 256);
	food.r = 7;
	food.flag = true;
}
void GameDraw()//绘制蛇
{
	BeginBatchDraw();
	//设置窗口背景RGB颜色
	setbkcolor(RGB(28,115,119));
	//填充整个窗口
	cleardevice();
	//绘制蛇身颜色
	setfillcolor(GREEN);
	solidcircle(snake.coor[0].x, snake.coor[0].y, 6);//设置蛇头圆心与半径
	for (i = 1; i < snake.size; i++)
	{
		setfillcolor(RGB(255,255,224));
		solidcircle(snake.coor[i].x, snake.coor[i].y, 5);//设置蛇身半径
		FlushBatchDraw();
	}
	if (food.flag)
		solidcircle(food.x,food.y,food.r);
	EndBatchDraw();
}
//移动蛇
void snakeMove()
{
	//移动是什么发生改变
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
void stop()//暂停函数
{
	if (_getch() == ' ')//暂停键设为空格
	{
		while (1)
		{
			if (_getch() != ' ')
				break;
		}
	}
}
void keyControl()//通过键盘WASD改变移动方向
{
	//判断是否按键
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
void Eatfood()//吃食物机制
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
	/*计分制度
	if (food.flag == true)
	{	
		point++;
	}
	*/
}
int gameover()//自撞检测游戏结束
{
	for (int i = 1; i < snake.size; i++) {
		if (snake.coor[0].x == snake.coor[i].x && snake.coor[0].y == snake.coor[i].y) {
			MessageBox(hwnd,_T("你撞了自己!"), _T( "GAME OVER!"), MB_OK);
			return 1;
		}
	}
	return 0;
}
int main()
{
	//初始化graph图像窗口分辨率大小
	initgraph(800, 600);
	//替换背景图片
	IMAGE background;//定义一个图片名.
	loadimage(&background, L"c:\\background.jpg", 800, 600, 1);//从图片文件获取图像
	putimage(0, 0, &background);//绘制图像到屏幕，图片左上角坐标为(0,0)
	_getch();//防止程序关闭
	//播放背景音乐
	mciSendString(_T("open c:\\crystaloverture.mp3 alias BGM"), NULL, 0, NULL);
	mciSendString(_T("play BGM repeat"), NULL, 0, NULL);
	GameInit();
	while (!gameover())//调用功能函数
	{
		GameDraw();
		snakeMove();
		keyControl();
		Eatfood();
		Sleep(80);
	 }
	closegraph();
}
