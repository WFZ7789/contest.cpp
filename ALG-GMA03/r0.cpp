#include <ctime>
#include <iostream>
#include <random>
#include <string>

const char lib[12] = {'O', '|', 'z', 'E', 'F', 'S', 'b', '?', '&', 'q', '^', '8'};
const char Gr[12] = {'.', '\'', '*', '\"', '[', ']', '{', '}', '(', ')', '`', '~'};
const int N = 8, M = 16; 

int main() {
	using namespace std;
	mt19937_64 rnd(time(nullptr));
	uniform_int_distribution<> rt(0, 200);
	uniform_int_distribution<> dig(0, 11);
	int difficulty[2] = {10, 30};
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
	cout << "请输入验证码（数字+16进制AB） >>> ";
	cin >> in;
	if(ans == in)
		cout << 'V';
	else
	{	
		cout << 'X';
		difficulty[0] += 3, difficulty[1] += 17;
	}
	cout << endl << ans << endl;
	_sleep(1000);
	goto RETRY;
	
	return 0;
}
