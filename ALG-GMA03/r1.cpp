#include <ctime>
#include <iostream>
#include <random>
#include <string>

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
			_sleep(1000);
			return;	
		}
		else{
			t_avg = (t_avg + T) / 2;
			++cnt;
			cout<<"X\n您被识别为机器人，请帮助我们对抗机器人" << endl;
			_sleep(1000); 
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
		system("cls");
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
			_sleep(1000); 
		}
		else
		{	
			cnt += 2; 
			cout << "请重试 | ";
			cout << endl << ans << endl;
			_sleep(1000);
			difficulty[0] += 3, difficulty[1] += 17;
			goto RETRY;
		}
	}
}; 

int main() {
	using namespace std;
	Rendji a;
	while(1)
	a.onceclick() ;
	return 0;
}
