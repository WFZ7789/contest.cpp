#include <ctime>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <random>
// --- Unix ---
/*
#include <unistd.h>
#define SLEEP(t_ms) usleep(t_ms * 1000u)
*/
// --- **** ---
// --- Win ---

#define MSWIN
#include <windows.h>
#define SLEEP(t_ms) _sleep(t_ms)

// --- *** ---

void clear_scr() {
#ifdef MSWIN
	system("cls");
#else
	printf("\033c");
#endif
}

struct Rendji {
	int cnt = 0;
	double t_avg = 0;
	void onceclick() {
		using namespace std;
		++cnt;
		cout << "我不是机器人[ ]" << endl << "I'm not a robot";
		clock_t start = clock();
		char ch = cin.get();
		double T = double(clock() - start) / CLOCKS_PER_SEC >= 1;
		if(ch == '\n' && T > 0.3 && cnt < 6 && abs(T - t_avg) >= 0.1)
		{
			t_avg = (t_avg + T) / 2;
			cout << "\b\b√" << endl;
			SLEEP(1000);
			return;	
		}
		else{
			t_avg = (t_avg + T) / 2;
			++cnt;
			cout<<"X\n您被识别为机器人，请帮助我们对抗机器人" << endl;
			SLEEP(1000); 
			verify_dif();
			cin.get();
		}
	}
	const char lib[12] = {'O', '|', 'Z', 'E', 'F', 'S', 'b', '?', '8', 'q', 'A', '&'};
	const char Gr[12] = {'.', '\'', '*', '\"', '-', ' ', ' ', ' ', ' ', ' ', '`', '~'};
	const int N = 8, M = 16; 
	void verify_dif() {
		using namespace std;
		mt19937_64 rnd(time(nullptr));
		uniform_int_distribution<> rt(0, 200);
		uniform_int_distribution<> dig(0, 11);
		int difficulty[2] = {7, 10};
	RETRY:
		clear_scr(); 
		string ans, in;
		for(int i = 0; i < N; i++)
		{
			for(int j = 0; j < M; j++)
			{
				int d = dig(rnd);
				if(rt(rnd) < difficulty[0])
				{
					cout << lib[d];
					if(d < 10)
						ans.push_back(d ^ 48);
					else if(d == 10)
						ans.push_back('A');
					else
						ans.push_back('B');	
				}
				else if(rt(rnd) < difficulty[1])
					cout << Gr[d];
				else
					cout << ' ';
			}
			cout << endl; 
		}
		cout << "请输入验证码（包含数字和大写字母AB） >>> ";
		cin >> in;
		if(ans == in)
		{
			cnt = max(cnt - 2, 0);
			cout << "通过验证\n";
			SLEEP(1000); 
		}
		else
		{	
			cnt += 2; 
			cout << "请重试 | 正确答案 ";
			cout << ans << endl;
			SLEEP(1000);
			difficulty[0] += 3, difficulty[1] += 17;
			goto RETRY;
		}
	}
}; 


int main() {
	using namespace std;
	int tim; 
	unsigned h, w;
	cout << "请输入操场高度和宽度 >>> \n";
	cin >> h >> w;
	cout << "请输入间隔时间(单位:ms)默认250ms，输入0默认 >>> \n";
	cin >> tim;
	if (tim == 0)
		tim = 250;
	Rendji verify;
	mt19937 rnd(time(nullptr));
	double rating = 1.;
	uniform_int_distribution<> dst_gen(1, h * w * 0.7);
	uniform_int_distribution<> ran(0, h * w - 1);
	auto ct = new unsigned[h * w + 1]();
	auto tot = new unsigned[h * w + 1]();
	double a = 0, b = 0;
	int wrong_cnt = 0;
	while (true)
	{
		cout << "输入 q 退出，Enter 继续 >>> ";
		if(cin.get() == 'q')
			break;
		clear_scr();
		unsigned dst = dst_gen(rnd), cnt = 0;
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
				if (ran(rnd) < dst)
				{
					cout << '#';
					++cnt;
				}
				else
					cout << ' ';
			cout << '\n';
		}
		SLEEP(tim);
		clear_scr();
		auto start = clock();
		cout << "请输入你刚才看到了多少个 # >>> ";
		unsigned ans;
		cin >> ans;
		auto end = clock();
		++tot[cnt];
		cin.get();
		++b;
		if (ans == cnt)
		{
			cout << "## Accepted ##\n";
			rating = rating + log2(h * w) * pow(cnt, 1.5) * 1e3 / tim;
			++ct[cnt];
			++a;
			wrong_cnt = 0;
		}
		else
		{
			++wrong_cnt;
			cout << "Wrong Answer\n正确答案 " << cnt << endl;
		} 
		cout << "当前 Rating: " << rating << endl;
		if(double(end - start) / CLOCKS_PER_SEC < 0.5 || wrong_cnt > 2)
			verify.onceclick();
	}
	clear_scr();
	cout << "成绩报告单：" << endl;
	for(int i = 100; i > 0; i -= 10)
	{
		cout << setw(3) << i << "% ";
		for(int j = 0; j <= h * w; j++)
		{
			
			for(int k = 0; k <= max(log10(j), 0.); k++)
				if(tot[j] != 0 && (double)ct[j] / tot[j] >= i / 100.)
					cout << '#';
				else if(tot[j] != 0 && (double)ct[j] / tot[j] >= i / 100. - .05)	
					cout << '=';
				else
					cout << ' ';
			cout << ' ';
		}
		cout << endl;
	}
	cout << "     "; 
	for(int j = 0; j <= h * w; j++)
		cout << j << ' '; 
	cout << "\n总概率 = " << a * 100 / b << "%\n";
	delete[] ct;
	delete[] tot;
	return 0;
}
