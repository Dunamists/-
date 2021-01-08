#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
using namespace std;

#define high 20
#define width 50    //游戏画面尺寸
#define EnemyNum 5      //敌机数目


//全局变量
int canvas[high][width] = { 0 };    //二维数组记录游戏画面中对应元素
									  //0输出空格，1输出飞机,2输出子弹,3输出敌机
int position_x, position_y;  //飞机位置
int enemy_x[EnemyNum], enemy_y[EnemyNum];  //敌机位置
int score;              //得分
int BulletWidth;          //子弹宽度
int EnemyMoveSpeed;         //敌机移动速度
char username[20];
int flag;		//结束标志
string player[20];	//玩家记录
int get_score[20];	//分数记录
int list = 0;	//排行榜成员个数
int res;   //重开标志


void gotoxy(int x, int y)  //光标移动到（x,y）位置
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);

}

void welcome()
{
	for (int i = 0; i <= 10; i++) {
		cout << endl;
	}
	cout << "\t空战游戏" << endl;
	cout << "╭ ︿︿︿╮ " << endl;
	cout << "{/ o  o /}  " << endl;
	cout << "(  (oo)  )   " << endl;
	cout << " ︶︶︶︶" << endl;
	cout << "╭ ︿︿︿╮" << endl;
	cout << "{/ $  $ /}" << endl;
	cout << "(  (oo)  ) " << endl;
	cout << " ︶︶︶︶" << endl;
	cout << "╭ ︿︿︿╮" << endl;
	cout << "{/-◎◎-/}" << endl;
	cout << "(  (oo)  ) " << endl;
	cout << " ︶︶︶︶" ;
	cout << "        用户名：";
	cin>>username;
	cout << endl;
}
void player_list()
{
	player[list] = username;
	get_score[list] = score;
	int count_list = 0;
	string a;
	int b;
	cout << "       排行榜" << endl;
	cout << "排名\t用户名\t得分"<<endl;
	for (int i = 0; i < list; i++) {
		for (int j = i; j < list; j++) {
			if (get_score[j] < get_score[j + 1]) {
				a = player[j];
				b = get_score[j];
				get_score[j] = get_score[j + 1];
				player[j] = player[j + 1];
				get_score[j + 1] = b;
				player[j + 1] = a;
			}
		}
	}
	while (count_list <= list) {
		cout << count_list + 1 << "\t" << player[count_list] << "\t" << get_score[count_list] << endl;
		count_list++;
	}
	cout << endl;
	system("pause");
	list++;
}

int restart() {
	char ch;
	cout << "是否重新开一局？[Y/N]" << endl;
	while (1) {
		cin >> ch;
		if (ch == 'Y') { 
			res = 1;
			break;
		}
		else if (ch == 'N') { 
			res = 0;
			break;
		}
		else { 
			cout << "输入错误！请重新输入：" << endl; 
		}
	}
	return res;
}




void startup() //数据初始化
{

	position_x = high / 2;
	position_y = width / 2;
	canvas[position_x][position_y] = 1;
	flag = 0;


	int k;
	for (k = 0; k < EnemyNum; k++)
	{
		enemy_x[k] = rand() % 2;
		enemy_y[k] = rand() % width;
		canvas[enemy_x[k]][enemy_y[k]] = 3;
	}

	BulletWidth = 0;
	EnemyMoveSpeed = 20;
	score = 0;

}
void show()   //显示画面
{
	int i, j;
	gotoxy(0, 0);   //光标移动到原点，以下重画清屏

	for (i = 0; i < high + 1; i++)
	{
		for (j = 0; j < width + 1; j++)
		{
			if ((i == high) && (j < width))
				printf("-");
			else if ((j == width) && (i < high))
				printf("|");

			else if (canvas[i][j] == 1)
				printf("*");   //输出飞机
			else if (canvas[i][j] == 2)
				printf("|");   //输出子弹
			else if (canvas[i][j] == 3)
				printf("@");   //输出敌机
			else
				printf(" ");    //输出空格
		}
		printf("\n");
	}

	cout << "用户名：" << username << endl << "得分：" << score << endl;
}
void updateWithoutInput()    //与用户输入无关的更新
{
	int i, j, k;
	for (i = 0; i < high - 1; i++)
	{
		for (j = 0; j < width - 1; j++)
		{
			if (canvas[i][j] == 2)
			{
				for (k = 0; k < EnemyNum; k++)
				{
					if (i == enemy_x[k] && j == enemy_y[k])   //击中敌机
					{
						score++;      //分数加1
						if (score % 5 == 0 && EnemyMoveSpeed > 3)
							EnemyMoveSpeed--;
						if (score % 5 == 0)
							BulletWidth++;        //子弹宽度增加


						canvas[enemy_x[k]][enemy_y[k]] = 0;
						enemy_x[k] = rand() % 2;     //产生新的敌机
						enemy_y[k] = rand() % width;
						canvas[enemy_x[k]][enemy_y[k]] = 3;
						canvas[i][j] = 0;   //子弹消失
						score++;
					}
				}


				//使子弹自动向上运动
				canvas[i][j] = 0;
				if (i > 0)
					canvas[i - 1][j] = 2;
			}
		}
	}

	static int speed = 0;
	if (speed < EnemyMoveSpeed)
		speed++;

	for (k = 0; k < EnemyNum; k++)
	{
		if ((position_x == enemy_x[k]) && (position_y == enemy_y[k]))    //相撞
		{
			flag = 1;
			printf("失败!\n");
			system("pause");
			printf("得分： %d\n", score);
		}
		if (enemy_x[k] > high)       //敌机跑出边界
		{
			canvas[enemy_x[k]][enemy_y[k]] = 0;
			enemy_x[k] = rand() % 2;       //产生新飞机
			enemy_y[k] = rand() % width;
			canvas[enemy_x[k]][enemy_y[k]] = 3;
			score--;     //减分
		}
	}


	if (speed == EnemyMoveSpeed)
	{
		//敌机自动下落 


		for (k = 0; k < EnemyNum; k++)
		{
			canvas[enemy_x[k]][enemy_y[k]] = 0;
			enemy_x[k]++;
			canvas[enemy_x[k]][enemy_y[k]] = 3;

			speed = 0;
		}

	}
}
void updateWithInput()  //与用户输入有关的更新
{
	char input;
	if (_kbhit())   //当按键时执行
	{
		input = _getch();   //不输入回车而根据输入移动

		if (input == 'a' && position_y > 0)
		{
			canvas[position_x][position_y] = 0;
			position_y--;
			canvas[position_x][position_y] = 1;

		}
		if (input == 'd' && position_y < width - 1)
		{
			canvas[position_x][position_y] = 0;
			position_y++;
			canvas[position_x][position_y] = 1;

		}
		if (input == 'w' && position_x > 0)
		{
			canvas[position_x][position_y] = 0;
			position_x--;
			canvas[position_x][position_y] = 1;

		}
		if (input == 's' && position_x < high - 1)
		{
			canvas[position_x][position_y] = 0;
			position_x++;
			canvas[position_x][position_y] = 1;

		}
		if (input == ' ')
		{
			int left, right;
			left = position_y - BulletWidth;
			if (left < 0)
				left = 0;
			right = position_y + BulletWidth;
			if (right > width - 1)
				right = width - 1;
			int x;
			for (x = left; x <= right; x++)   //发射子弹

				canvas[position_x - 1][x] = 2;    //子弹初始位置在飞机正上方
		}
	}
}
void hideCursor(int code) //隐藏光标
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = code;
	SetConsoleCursorInfo(hOut, &cci);
}

int main()
{
	int re = 1;
	hideCursor(0);
	while (re) {
	welcome();
	system("cls");
	startup();  //数据初始化

	while (!flag)   //游戏循环执行
	{
		show();  //显示画面
		updateWithoutInput();  //与用户输入无关的更新
		updateWithInput();     //与用户输入有关的更新
	}
	system("cls");
	player_list();
	system("cls");
	re = restart();
}
	system("cls");
	cout << endl << endl << endl << "GAME OVER!" << endl << endl << endl;
	return 0;
}
