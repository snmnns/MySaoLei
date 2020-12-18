#include <bits/stdc++.h>
using namespace std;
int P_k = 0;//地雷存储个数计数器
int mp_m[105][105];//地雷计数器，0~8表示周围8格地雷的个数
char mp_c[105][105];//地雷显示,*为未打开,^为标记地雷,?为怀疑的雷,#为已打开
int mp_o[105][105];//地雷输出控制数组
int mp_t[105][105];//地雷Test模式输出控制数组
int n_x[10005];//存储随机X值
int n_y[10005];//存储随机Y值
//int n_x_cpy[105];//存储排序后的随机X值(曾经)
//int n_y_cpy[105];//存储排序后的随机Y值(曾经)
bool sthwr = 0, game_over = 1;
int pt_len = 1;
int pt_x[10005];//标点的x值
int pt_y[10005];//标点的y值
int mp_len = 1;//想挑战的格子n*n
bool test = 0;//Debug模式
bool re = 0;//用于判断是否重来
int x, y;//输入的x,y
bool Re_re = true;//重来用的
bool D_ok_b = false;//判断输入值是否合法的bool
char D_ok[20] = { '\0', 'B', 'F', 'G', 'N', 'M', 'P', 'R', 'S', 'T', 'H' };//能输入的所有字符,用于输入比较
double TStart = clock();//开始时间
double TEnd;//结束时间
double BMode[105];//管理历史游戏模式最佳时间
int BTTime[105];//管理历史游戏尝试次数
void rezero() {
	memset(mp_m, 0, sizeof(mp_m));
	memset(mp_c, 42, sizeof(mp_c));
	memset(mp_o, 0, sizeof(mp_o));
	memset(mp_t, 0, sizeof(mp_t));
	memset(n_x, 0, sizeof(n_x));
	memset(n_y, 0, sizeof(n_y));
	memset(pt_x, 0, sizeof(pt_x));
	memset(pt_y, 0, sizeof(pt_y));
	P_k = 0;
	sthwr = 0;
	game_over = 1;
	pt_len = 1;
	test = 0;
	re = 0;
	x = 1;
	y = 1;
	Re_re = 1;
	return;
}
void re_run()
{
	char M;
	while (Re_re)
	{
		printf("重来吗?\nY即为是,N即为否\n");
		while (M = cin.get()) {
			if (M == '\n') {
				continue;
			}
			else {
				break;
			}
		}
		if (M == 'Y')
		{
			re = 1;
			game_over = 0;
			Re_re = 0;
		}
		else if (M == 'N')
		{
			game_over = 0;
			Re_re = 0;
		}
		else
		{
			printf("团长,你在输入什么啊团长?\n请验证是否有不合法输入值\n\n");
		}
		M = '\0';
	}
	Re_re = 1;
}
void me()
{
	double LTime;
	int LMode;
	FILE* fpt = fopen("HistoryTime.txt", "r");
	FILE* fpm = fopen("HistoryMode.txt", "r");
	fscanf(fpm, "\n%d", &LMode);
	fscanf(fpt, "\n%lf", &LTime);
	fclose(fpt);
	fclose(fpm);
	printf("T开启Debug模式,S关闭Debug模式,B退出游戏,R重来.\n");
	printf("完成游戏请将地雷标记并提交,答案正确即通关.\n");
	printf("地雷与格子总数之比为5:1\n");
	printf("以数组的坐标为准,反智设计\n");
	printf("此程序Bug极多,祝游玩愉快!\n");
	printf("上次游戏模式: %d * %d\n", LMode, LMode);
	printf("用时%f秒\n", LTime);
	printf("*---------------------------------------------------*\n");
	return;
}
void out(int n)//游戏模式输出
{
	for (int i = 0; i <= n; i++)
	{
		if (i == 0)
		{
			printf("     ");
		}
		if (i >= 1 && i <= 9)
		{
			printf("%d行: ", i);
		}
		else if (i >= 10)
		{
			printf("%d行:", i);
		}
		for (int k = 1; k <= n; k++)
		{
			if (i >= 1 && k >= 1)
			{
				if (mp_o[i][k] == 1)
				{
					if (mp_m[i][k] == 10)
					{
						printf("0  ");
					}
					else
					{
						if (k >= 1 && k <= 9)
							printf("%d  ", mp_m[i][k]);
						else if (k >= 10)
							printf("%d  ", mp_m[i][k]);
					}
				}
				else  if (mp_o[i][k] == 0)
				{
					printf("%c  ", mp_c[i][k]);
				}
			}
			else if (k <= 9 && k >= 0)
			{
				printf("%d  ", k);
			}
			else if (k >= 10)
			{
				printf("%d ", k);
			}
		}
		printf("\n");
	}
	return;
}
void over_out(int n)//用于游戏结束后的输出
{
	for (int i = 0; i <= n; i++)
	{
		if (i == 0)
		{
			printf("     ");
		}
		if (i >= 1 && i <= 9)
		{
			printf("%d行: ", i);
		}
		else if (i >= 10)
		{
			printf("%d行:", i);
		}
		for (int k = 1; k <= n; k++)
		{
			if (i >= 1 && k >= 1)
			{
				if (mp_m[i][k] == 10)
				{
					printf("0  ");
				}
				else
				{
					if (mp_m[i][k] != 9)
					{
						printf("%d  ", mp_m[i][k]);
					}
					else
					{
						printf("$  ");
					}
				}
			}
			else if (k <= 9 && k >= 0)
			{
				printf("%d  ", k);
			}
			else if (k >= 10)
			{
				printf("%d ", k);
			}
		}
		printf("\n");
	}
	return;
}
void out_t(int n)//测试模式输出
{
	printf("输出时,数字数组的&代表被连锁去除的0.\n");
	for (int i = 1; i <= n; i++)//两个数组的输出
	{
		for (int k = 1; k <= n; k++)
		{
			if (mp_t[i][k] == 1)
			{
				printf("& ");
			}
			else if (mp_t[i][k] == 0)
			{
				printf("%d ", mp_m[i][k]);
			}
		}
		printf("                 ");
		for (int t = 1; t <= n; t++)
		{
			printf("%c ", mp_c[i][t]);
		}
		printf("\n");
	}
	return;
}
/*
int rest(int n)//4个数组的初始化(已转用memset实现)
{
	for (int i = 1; i <= n; i++)//将两个数组初始化，char型存入*,int型存入0
	{
		for (int k = 1; k <= n; k++)
		{
			mp_o[i][k] = 0;
			mp_m[i][k] = 0;
			mp_c[i][k] = '*';
			mp_t[i][k] = 0;
		}
	}
	out(n);
	if (test)
	{
		out_t(n);
	}
	return 0;
}*/
bool sc(int x, int y, int n)//对输入数值的扫描，检测是否与地雷重合,1时重合,0时未重合
{
	if (mp_m[x][y] == 9)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void tg(int x, int y, int num)//递归函数,对连锁去0的操作
{
	if (mp_m[x][y] != 0)
	{
		mp_o[x][y] = 1;
	}
	else if (mp_m[x][y] == 0)
	{
		mp_c[x][y] = '#';
		mp_m[x][y] = 10;
		mp_t[x][y] = 1;
		for (int i = x - 1; i <= x + 1; i++)
		{
			for (int k = y - 1; k <= y + 1; k++)
			{
				if (!(i != x && k != y) && k != 0 && i != 0 && i <= mp_len && k <= mp_len)
				{
					tg(i, k, num + 1);
				}
			}
		}
	}
	return;
}
bool check_same_pn(int a, int b, int s)//输入的N值与P值对比，有重复即重新输入
{
	for (int i = 1; i <= s; i++)
	{
		if (pt_x[i] == a && pt_y[i] == b)
		{
			return 1;
		}
	}
	return 0;
}
void gt(int x, int y, int n, char D)//对于输入数值的操作,即图像化
{
	if (D == 'N')//继续下一轮
	{
		if (sc(x, y, n))//重合的情况
		{
			//	cout << "Game Over!" << endl << "Boom" << endl;
			TEnd = clock() - TStart;
			printf("Game Over!\nBoom\n");
			mp_c[x][y] = '&';
			//game_over = 0;
			over_out(n);
			re_run();
			return;
		}
		else//未重合
		{
			mp_c[x][y] = '#';
			tg(x, y, 0);
			out(mp_len);
			return;
		}
	}
	else if (D == 'P')//标记地雷(正式)
	{
		int x_rl, y_rl;//希望永远都不会用的去标点重复的变量
		P_k++;
		bool OK_p_xy = 1, Y_ptxy = 0;
		while (OK_p_xy)
		{
			Y_ptxy = 0;
			for (int i = 1; i <= P_k; i++)
			{
				if (x == pt_x[i] && y == pt_y[i])
				{
					Y_ptxy = 1;
				}
			}
			if (Y_ptxy)
			{
				printf("重复标点,由于会出错(其实是作者太懒),请重标:");
				scanf("%d%d", &x_rl, &y_rl);
				while (! (x_rl >= 1 && x_rl <= mp_len) || ! (y_rl >= 1 && x_rl <= mp_len) || Y_ptxy){
					printf("团长,你在输入什么啊团长?\n请验证是否有不合法输入值.\n\n");
					scanf("%d%d", &x_rl, &y_rl);
					Y_ptxy = 0;
					for (int i = 1; i <= P_k; i++)
					{
						if (x_rl == pt_x[i] && y_rl == pt_y[i])
						{
							Y_ptxy = 1;
						}
					}
				}
				x = x_rl;
				y = y_rl;
			}
			else
			{
				OK_p_xy = 0;
			}
		}
		mp_c[x][y] = '^';
		pt_x[pt_len] = x;
		pt_y[pt_len] = y;
		pt_len++;
	}
	else if (D == 'G')//标记地雷(非正式,别想存进后台耗内存,就是做给你自己看的)
	{
		mp_c[x][y] = '?';
	}
	else if (D == 'M')
	{
		printf("P类orG类?\n");
		D = cin.get();
		while (D == '\n') {
			D = cin.get();
		}
		printf("接下来操作对象的x,y轴位置.\n");
		//		cin >> x >> y;
		scanf("%d%d", &x, &y);
		if (D == 'P')
		{
			mp_c[x][y] = '*';
			for (int i = 1; i <= pt_len; i++)
			{
				if (x == pt_x[i] && y == pt_y[i])
				{
					printf("fd\n");
					pt_x[i] = 0;
					pt_y[i] = 0;
					break;
				}
			}
		}
		else if (D == 'G')
		{
			mp_m[x][y] = '*';
		}
		else
		{
			printf("团长,你在输入什么啊团长?\n请验证是否有不合法输入值.\n\n");
		}
	}
	else
	{
		printf("团长,你在输入什么啊团长?\n请验证是否有不合法输入值.\n\n");
		return;
	}
	out(n);
	if (test)
	{
		out_t(n);
	}
	return;
}
void gen(int s, int n)//地雷数组的生成(包括周围数字个数的生成)
{
	unsigned seed;//此句为随机种子初始化
	seed = (int)time(0);//赋予随机种子时间的值
	srand((int)seed);//将int型随机种子加载进srand函数
	for (int i = 1; i <= s; i++)//随机生成n个X,Y值
	{
		n_x[i] = rand() % n + 1;//限定X值在1~n间
		n_y[i] = rand() % n + 1;//限定Y值在1~n间

/*
		while (n_x[i - 1] == n_x[i] && n_y[i - 1] == n_y[i])//硬核去重(废除,这说明投偷懒是不可取的)
		{
			n_x[i] = rand() % n + 1;//限定X值在1~n间
			n_y[i] = rand() % n + 1;//限定Y值在1~n间
		}
		if (test == 1)
		{
			cout << n_x[i] << " " << n_y[i] << endl;//地雷位置输出
		}
*/
	}
	for (int i = 1; i <= s; i++)
	{
		for (int j = 1; j <= s; j++)//真正意义上的绝对无重复的硬核去重
		{
			if (i == j)
			{
				break;
			}
			while (n_x[i] == n_x[j] && n_y[i] == n_y[j])
			{
				//				cout << "%%%" << endl;
				n_x[i] = rand() % n + 1;//限定X值在1~n间
				n_y[i] = rand() % n + 1;//限定Y值在1~n间
			}
		}
	}
	for (int i = 1; i <= s; i++)//真正对地雷数组的操作
	{
		//		n_x_cpy[i] = n_x[i];//偷懒的遗留产物
		//		n_y_cpy[i] = n_y[i];//偷懒的遗留产物
		mp_m[n_x[i]][n_y[i]] = 9;
		for (int k = n_x[i] - 1; k <= n_x[i] + 1; k++)//将地雷四周8格累加1
		{
			for (int m = n_y[i] - 1; m <= n_y[i] + 1; m++)
			{
				if (mp_m[k][m] != 9)
				{
					mp_m[k][m]++;
				}
			}
		}
	}
	return;
}
bool yn_F(int s)//提交是否正确??
{
	int sm_xy = 0;
	/*	sort(pt_x + 1, pt_x + 1 + s);
		sort(pt_y + 1, pt_y + 1 + s);
		sort(n_x_cpy + 1, n_x_cpy + 1 + s);
		sort(n_y_cpy + 1, n_y_cpy + 1 + s);//将输入标记与地雷位置排序*/
	for (int i = 1; i < 10005; i++)
	{
		if (mp_m[pt_x[i]][pt_y[i]] == 9)
		{
			cout << n_x[i] << " " << n_y[i] << endl;
			sm_xy++;
		}
	}
	//	cout << "s:" << s << " " << "sm_xy:" << sm_xy << endl;
	if (sm_xy == s)
	{
		printf("You Win!\n");
		game_over = 0;
		TEnd = clock() - TStart;
		TEnd /= 1000;
		if (TEnd < BMode[mp_len]) {
			BMode[mp_len] = TEnd;
		}
		FILE* fpt = fopen("HistoryTime.txt", "w");
		FILE* fpm = fopen("HistoryMode.txt", "w");
		FILE* fpbm = fopen("HistoryBMode.txt", "w");
		for (int i = 1; i <= 100; i++) {
			fprintf(fpbm, " %lf", BMode[i]);
		}
		fprintf(fpm, "\n%d", mp_len);
		fprintf(fpt, "\n%lf", TEnd);
		fclose(fpbm);
		fclose(fpt);
		fclose(fpm);
		return true;
	}
	else
	{
		printf("Game Over!\nWrong Points.\n");
		TEnd = clock() - TStart;
		TEnd /= 1000;
		game_over = 0;
		return false;
	}
}
int move(int n, char D)//对玩家提交动作的处理
{
	if (D == 'F')
	{
		if (yn_F(n))
		{
			return 1;
		}
		else
		{
			return 1;
		}
	}
	else if (D == 'B')
	{
		game_over = 0;
		re = 0;
		return 3;
	}
	return 0;
}
void bug_fix(int n)//对于地图周围一圈的清零
{
	for (int i = 0; i <= n + 1; i++)
	{
		for (int k = 0; k <= n + 1; k++)
		{
			if (i == 0 || k == 0 || i == n + 1 || k == n + 1)
			{
				mp_m[i][k] = 0;
			}
		}
	}
	return;
}
int input_int(int input)// 带容错功能的控制台整数输入函数
{
	// 处理输入字符时的死循环异常
	while (!(cin >> input) || cin.peek() != '\n')
	{
		cin.clear();// 恢复状态标志
		cin.ignore(numeric_limits<streamsize>::max(), '\n');// 略过缓存
		printf("团长,你在输入什么啊团长?\n请验证是否有不合法输入值.\n\n");
	}
	return input;
}
void input_int_2()// 带容错功能的控制台整数输入函数(x,y)
{
	// 处理输入字符时的死循环异常
	while (!(cin >> x >> y) || cin.peek() != '\n')
	{
		cin.clear();// 恢复状态标志
		cin.ignore(numeric_limits<streamsize>::max(), '\n');// 略过缓存
		printf("团长,你在输入什么啊团长?\n请验证是否有不合法输入值.\n\n");
	}
	return;
}
int play_main(int n) {//游戏主体运行部分,返回1(2为练习模式不结束)表示继续游戏,0表示结束
	bool backs = 0;
	char D;//此时的操作
	D_ok_b = 0;
	printf("\n接下来的操作，N为继续开区,P为给地雷标点,G为猜测标点,F为提交当前的地雷标记,M可以消除标记.\n");
	D = cin.get();
	while (D == '\n') {
		D = cin.get();
	}
	for (int i = 1; i <= 20; i++)
	{
		if (D_ok[i] == D)
		{
			D_ok_b = 1;
		}
	}
	if (!D_ok_b)
	{
		printf("团长,你在输入什么啊团长?\n请验证是否有不合法输入值.\n\n");
		backs = 1;
		return backs;
	}
	if (move(n, D) == 1)
	{
		re_run();
		return backs;
	}
	else if (move(n, D) == 3)
	{
		return backs;
	}
	else if (D == 'T')
	{
		test = 1;
		for (int i = 1; i <= n; i++)
		{
			printf("%d %d\n", n_x[i], n_y[i]);//地雷位置输出
		}
		backs = 1;
		return backs;
	}
	else if (D == 'S')
	{
		test = 0;
		backs = 1;
		return backs;
	}
	else if (D == 'R')
	{
		game_over = 0;
		re = 1;
		return backs;
	}
	else if (D == 'M')
	{
		gt(1, 1, mp_len, D);
		backs = 1;
		return backs;
	}
	printf("接下来操作对象的x,y轴位置.\n");
	input_int_2();
	if (x < 1 || x > mp_len || y < 0 || y > mp_len)
	{
		printf("团长,你在输入什么啊团长?\n请验证是否有不合法输入值.\n\n");
		backs = 1;
		return backs;
	}
	if (D == 'N' && check_same_pn(x, y, mp_len))//若输入的N与P重合重新输入
	{
		printf("与之前标过的P点重合,请重新输入.\n");
		backs = 1;
		return backs;
	}
	gt(x, y, mp_len, D);//对于输入的操作
	system("pause");
	return 1;
}
int main()//主函数不多说
{
again:
	rezero();
	me();
	system("pause");
	int in_times = 1;
	printf("数值必须为5的倍数.\n");
	mp_len = 1;
	while (mp_len % 5 != 0)
	{
		in_times++;//计数器+1，用于控制报错信息的输出
		mp_len=input_int(mp_len);
		if (mp_len <= 0 ||(in_times != 0 && mp_len % 5 != 0))
		{
			printf("数值必须为5的倍数,请重试.\n");
			mp_len = 1;
			mp_len=input_int(mp_len);
		}
		if (mp_len > 100)
		{
			printf("必须小于100哦,不然不保证你玩得过!\n");
			mp_len = 1;
			continue;
		}
		system("pause");
	}
	int SMT = 0;
//	FILE* fpgt = fopen("HistoryBTime.txt", "r");
	FILE* fpgm = fopen("HistoryBMode.txt", "r");
	FILE* fptt = fopen("HistoryBTTime.txt", "r");
	while (!feof(fpgm)) {
		fscanf(fpgm, " %lf", &BMode[++SMT]);
	}
	SMT = 0;
	while (!feof(fptt)) {
		fscanf(fptt, " %d", &BTTime[++SMT]);
	}
	fclose(fpgm);
	fclose(fptt);
	printf("此模式尝试次数:%d次\n", BTTime[mp_len]);
	if (BMode[mp_len] == 10000000000.000000) {
		printf("此模式最佳成绩:%lf秒\n", 0);
	}
	else {
		printf("此模式最佳成绩:%lf秒\n", BMode[mp_len]);
	}
	out(mp_len);
	if (test) {
		out_t(mp_len);
	}
//	rest(mp_len);
	int n;//地雷个数
	n = mp_len * mp_len / 5;
	gen(n, mp_len);
	bug_fix(mp_len);
	TStart = clock();
	test = 0;
	game_over = 1;
	while (game_over) {
		if (play_main(n) == 1) {
			TEnd = clock() - TStart;
			TEnd /= 1000;
			printf("%f秒过去了\n", TEnd);
			continue;
		}
		else {
			break;
		}
	}
	printf("此次记录:%f秒\n", TEnd);
	BTTime[mp_len]++;
	FILE* fptts = fopen("HistoryBTTime.txt", "w");
	for (int i = 1; i <= 100; i++) {
		fprintf(fptts, " %d", BTTime[i]);
	}
	fclose(fptts);
	system("pause");
	if (re)
	{
		re = 0;
		goto again;
	}
	return 0;
}
