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
	mt19937 rnd(time(nullptr));
	double rating = 1.;
	uniform_int_distribution<> dst_gen(1, h * w * 0.7);
	uniform_int_distribution<> ran(0, h * w - 1);
	auto ct = new unsigned[h * w + 1]();
	auto tot = new unsigned[h * w + 1]();
	double a = 0, b = 0;
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
		cout << "请输入你刚才看到了多少个 # >>> ";
		unsigned ans;
		cin >> ans;
		++tot[cnt];
		cin.get();
		++b;
		if (ans == cnt)
		{
			cout << "## Accepted ##\n";
			rating = rating + log2(h * w) * pow(cnt, 1.5) * 1e3 / tim;
			++ct[cnt];
			++a;
		}
		else
			cout << "Wrong Answer\n正确答案 " << cnt << endl;
		cout << "当前 Rating: " << rating << endl;
		
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
