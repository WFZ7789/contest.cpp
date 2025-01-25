#include <ctime>
#include <cmath>
#include <iostream>
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
	unsigned h, w;
	cout << "请输入操场宽度和高度 >>> \n";
	cin >> h >> w;
	mt19937 rnd(time(nullptr));
	uniform_int_distribution<> num1(0, min(h, w));
	uniform_int_distribution<> num2(1, w - 1);
	double rating = 1.;
	uniform_int_distribution<> ran(0, h * w - 1);
	while (true)
	{
		cin.get();
		clear_scr();
		unsigned dst = num1(rnd) * num1(rnd) + num2(rnd), cnt = 0;
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
		SLEEP(1000);
		clear_scr();
		cout << "请输入你刚才看到了多少个 # >>> ";
		unsigned ans;
		cin >> ans;
		if (ans == cnt)
		{
			cout << "## Accepted ##\n";
			rating = (rating + log2(h * w) * pow(cnt, 1.5)) / 2;
		}
		else
		{
			cout << "Wrong Answer\n";
			rating = (rating * 1.2 + log(rating)) / 2;
		}
		cout << "当前 Rating: " << rating << endl;
		SLEEP(3000);
	}
	return 0;
}
