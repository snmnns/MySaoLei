#include <bits/stdc++.h>
using namespace std;
int P_k = 0;//地雷存储个数计数器
int mp_m[105][105];//地雷计数器，0~8表示周围8格地雷的个数
char mp_c[105][105];//地雷显示,*为未打开,^为标记地雷,?为怀疑的雷,#为已打开
int mp_o[105][105];//地雷输出控制数组
int mp_t[105][105];//地雷Test模式输出控制数组
int n_x[105];//存储随机X值
int n_y[105];//存储随机Y值
//int n_x_cpy[105];//存储排序后的随机X值(曾经)
//int n_y_cpy[105];//存储排序后的随机Y值(曾经)
bool sthwr = 0,game_over = 1;
int pt_len = 1;
int pt_x[105];//标点的x值
int pt_y[105];//标点的y值
int mp_len = 1;//想挑战的格子n*n
bool test = 0;//Debug模式
bool re = 0;//用于判断是否重来
int x, y;//输入的x,y
bool Re_re = true;
void re_run()
{
	char M;
    while (Re_re)
	{
		cout << "重来吗?" << endl;
		cout << "Y即为是,N即为否" << endl;
		cin >> M;
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
			cout << "团长,你在输入什么啊团长?" << endl << "请验证是否有不合法输入值" << endl << endl;
		}
		M = '\0';
	}
	Re_re = 1;
}
int me()
{
	cout << "T开启Debug模式,S关闭Debug模式,B退出游戏,R重来." << endl;
	cout << "完成游戏请将地雷标记并提交,答案正确即通关." << endl;
	cout << "地雷与格子总数之比为5:1" << endl;
	cout << "以数组的坐标为准,反智设计" << endl;
	cout << "此程序Bug极多,祝游玩愉快!" << endl;
	cout << "输入R可重来" << endl;
	cout << "*---------------------------------------------------*" << endl;
	return 0;
}
int out(int n)//游戏模式输出
{
	for (int i = 0; i <= n; i++)
	{
		if (i == 0)
		{
			cout << "     ";
		}
		if (i >= 1 && i<=9)
		{
			cout << i << "行: ";
		}
		else if (i >= 10)
		{
			cout << i << "行:";
		}
		for (int k = 1; k <= n; k++)
		{
			if (i >= 1 && k >= 1)
			{
				if (mp_o[i][k] == 1)
				{
					if (mp_m[i][k] == 10)
					{
						cout << 0 << "  ";
					}
					else
					{
						if (k >= 1 && k <= 9)
							cout << mp_m[i][k] << "  ";
						else if (k >= 10)
							cout << mp_m[i][k] << "  ";
					}
				}
				else  if (mp_o[i][k] == 0)
				{
					if (k >= 1 && k <= 9)
						cout << mp_c[i][k] << "  ";
					else if (k >= 10)
						cout << mp_c[i][k] << "  ";
				}
			}
			else if (k <=9 && k>=0)
			{
				cout << k << "  ";
			}
			else if (k >= 10)
			{
				cout << k << " ";
			}
		}
		cout << endl;
	}
	return 0;
}
int over_out(int n)//用于游戏结束后的输出
{
	for (int i = 0; i <= n; i++)
	{
		if (i == 0)
		{
			cout << "     ";
		}
		if (i >= 1 && i <= 9)
		{
			cout << i << "行: ";
		}
		else if (i >= 10)
		{
			cout << i << "行:";
		}
		for (int k = 1; k <= n; k++)
		{
			if (i >= 1 && k >= 1)
			{
				if (mp_m[i][k] == 10)
				{
					cout << "0" << "  ";
				}
				else
				{
					if (k >= 1 && k <= 9 && mp_m[i][k] != 9)
					{
						cout << mp_m[i][k] << "  ";
					}
					else if (k >= 10 && mp_m[i][k] != 9)
					{
						cout << mp_m[i][k] << "  ";
					}
					else if (mp_m[i][k] == 9)
					{
						cout << "$" << "  ";
					}
				}
			}
			else if (k <= 9 && k >= 0)
			{
				cout << k << "  ";
			}
			else if (k >= 10)
			{
				cout << k << " ";
			}
		}
		cout << endl;
	}
	return 0;
}
int out_t(int n)//测试模式输出
{
	cout << "输出时,数字数组的&代表被连锁去除的0" << endl;
	for (int i = 1; i <= n; i++)//两个数组的输出
	{
		for (int k = 1; k <= n; k++)
		{
			if (mp_t[i][k] == 1)
			{
				cout << "&" << " ";
			}
			else if(mp_t[i][k] == 0)
			{
				cout << mp_m[i][k] << " ";
			}
		}
		cout << "                 ";
		for (int t = 1; t <= n; t++)
		{
			cout << mp_c[i][t] << " ";
		}
		cout << endl;
	}
	return 0;
}
int rest(int n)//4个数组的初始化
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
}
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
int tg(int x, int y, int num)//递归函数,对连锁去0的操作
{
	if (mp_m[x][y] != 0)
	{
		mp_o[x][y] = 1;
		return 0;
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
		return 1;
	}
	return 0;
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
int gt(int x, int y,int n,char D)//对于输入数值的操作,即图像化
{
	if (D == 'N')//继续下一轮
	{
		if (check_same_pn(x, y, n))
		{

		}
		else if (sc(x, y, n))//重合的情况
		{
			cout << "Game Over!" << endl << "Boom" << endl;
			mp_c[x][y] = '&';
//			game_over = 0;
			over_out(n);
			re_run();
		}
		else//未重合
		{
			mp_c[x][y] = '#';
			tg(x, y, 0);
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
				cout << "重复标点,由于会出错(其实是作者太懒),请重标:";
				cin >> x_rl >> y_rl;
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
		cout << "P类orG类?" << endl;
		cin >> D;
		cout << "接下来操作对象的x,y轴位置." << endl;
		cin >> x >> y;
		if (D == 'P')
		{
			mp_c[x][y] = '*';
			for (int i = 1; i <= pt_len;i++)
			{
				if (x == pt_x[i] && y == pt_y[i])
				{
					pt_x[i] = 0;
					pt_y[i] = 0;
					break;
				}
			}
		}
		else if(D == 'G')
		{
			mp_m[x][y] = '*';
		}
		else
		{
			cout << "团长,你在输入什么啊团长?" << endl << "请验证是否有不合法输入值" << endl << endl;
		}
	}
	else
	{
		cout << "团长,你在输入什么啊团长?" << endl << "请验证是否有不合法输入值" << endl << endl;
		return 0;
	}
	out(n);
	if (test)
	{
		out_t(n);
	}
	return 0;
}
int gen(int s,int n)//地雷数组的生成(包括周围数字个数的生成)
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
	return 0;
}
int yn_F(int s)//提交是否正确??
{
	int sm_xy = 0;
/*	sort(pt_x + 1, pt_x + 1 + s);
	sort(pt_y + 1, pt_y + 1 + s);
	sort(n_x_cpy + 1, n_x_cpy + 1 + s);
	sort(n_y_cpy + 1, n_y_cpy + 1 + s);//将输入标记与地雷位置排序*/
	for (int i = 1; i <= s; i++)
	{
		if (mp_m[n_x[i]][n_y[i]] == 9)
		{
			sm_xy ++;
		}
	}
//	cout << "s:" << s << " " << "sm_xy:" << sm_xy << endl;
	if (sm_xy == s)
	{
		cout << "You Win!" << endl;
		game_over = 0;
		return true;
	}
	else
	{
		cout << "Game Over!" << endl << "Wrong Points" << endl;
		game_over = 0;
		return false;
	}
}
int move(int n,char D)//对玩家提交动作的处理
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
int bug_fix(int n)//对于地图周围一圈的清零
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
	return 0;
}
int input_int(int input)// 带容错功能的控制台整数输入函数
{
	// 处理输入字符时的死循环异常
	while (!(cin >> input) || cin.peek() != '\n')
	{
		cin.clear();// 恢复状态标志
		cin.ignore(numeric_limits<streamsize>::max(), '\n');// 略过缓存
		cerr << "团长,你在输入什么啊团长?" << endl << "请验证是否有不合法输入值" << endl << endl;
	}
	return input;
}
int input_int_2()// 带容错功能的控制台整数输入函数(x,y)
{
	// 处理输入字符时的死循环异常
	while (!(cin >> x >> y) || cin.peek() != '\n')
	{
		cin.clear();// 恢复状态标志
		cin.ignore(numeric_limits<streamsize>::max(), '\n');// 略过缓存
		cerr << "团长,你在输入什么啊团长?" << endl << "请验证是否有不合法输入值" << endl << endl;
	}
	return 0;
}
int main()//主函数不多说
{
again:
	memset(pt_x, 0, 105);
	memset(pt_y, 0, 105);
	bool D_ok_b = false;//判断输入值是否合法的bool
	char D_ok[20] = { '\0', 'B', 'F', 'G', 'N', 'N', 'P', 'R', 'S', 'T' };//能输入的所有字符,用于输入比较
	game_over = 1;
	me();
	system("pause");
	int in_times = 1;
	char D;//此时的操作,P为标记地雷,N为开点,G为猜测点
	cout << "数值必须为5的倍数." << endl;
	mp_len = 1;
	while (mp_len % 5 != 0)
	{
		in_times++;//计数器+1，用于控制报错信息的输出
		mp_len=input_int(mp_len);
		if (mp_len <= 0 ||(in_times != 0 && mp_len % 5 != 0))
		{
			cout << "数值必须为5的倍数,请重试." << endl;
			mp_len = 1;
			mp_len=input_int(mp_len);
		}
		if (mp_len > 100)
		{
			cout << "必须小于100哦,不然不保证你玩得过!" << endl;
			mp_len = 1;
			continue;
		}
		system("pause");
	}
	rest(mp_len);
	int n = mp_len * mp_len / 5;
	gen(n, mp_len);
	bug_fix(mp_len);
	while (game_over)
	{
		D_ok_b = 0;
		cout << endl << "接下来的操作，N为继续开区,P为给地雷标点,G为猜测标点,F为提交当前的地雷标记,M可以消除标记." << endl;
		cin >> D;
		for (int i = 1; i <= 20; i++)
		{
			if (D_ok[i] == D)
			{
				D_ok_b = 1;
			}
		}
		if (move(n, D) == 1)
		{
			re_run();
			break;
		}
		else if (move(n, D) == 3)
		{
			break;
		}
		else if (D == 'T')
		{
			test = 1;
			for (int i = 1; i <= n; i++)
			{
				cout << n_x[i] << " " << n_y[i] << endl;//地雷位置输出
			}
			continue;
		}
		else if (D == 'S')
		{
			test = 0;
			continue;
		}
		else if (D == 'R')
		{
			game_over = 0;
			re = 1;
			break;
		}
		else if (D == 'M')
		{
			gt(1, 1, mp_len, D);
			continue;
		}
		else if (! D_ok_b)
		{
			cout << "团长,你在输入什么啊团长?" << endl << "请验证是否有不合法输入值" << endl << endl;
			continue;
		}
		cout << "接下来操作对象的x,y轴位置." << endl;
		input_int_2();
		if (x < 1 || x > mp_len || y < 0 || y > mp_len)
		{
			cout << "团长,你在输入什么啊团长?" << endl << "请验证是否有不合法输入值" << endl << endl;
			continue;
		}
		if (D == 'N' && check_same_pn(x, y, mp_len))//若输入的N与P重合重新输入
		{
			cout << "与之前标过的P点重合,请重新输入." << endl;
			continue;
		}
		gt(x, y, mp_len, D);//对于输入的操作
		system("pause");
	}
	system("pause");
	if (re)
	{
		re = 0;
		goto again;
	}
	return 0;
}
