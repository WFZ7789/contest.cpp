#include <ctime>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <random>

std::mt19937 rnd(time(nullptr));

unsigned mod;

const char Process[] = {'-', '\\', '|', '/'};

double TI = 1.15, Step = 0.5;

unsigned Hash(int key) {
	return key % mod;
}

void insert(std::vector<std::list<std::pair<int, int>>>& map, int n) {
	while(n--)
	{
		int k = rnd();
		map[Hash(k)].emplace_back(k, rnd()); 	
	}
}

int ac(std::vector<std::list<std::pair<int, int>>>& map, int key) {
	unsigned d = Hash(key);
	for(const auto& i : map[d])
		if(i.first == key)
			return i.second;
	return 0;
}

int test(std::vector<std::list<std::pair<int, int>>>& map, int t) {
	int ver = 0;
	double it = 0.;
	for(int i = 0; i < t; i++)
	{
		if(double(i) / t >= it)
		{
			putchar('>');
			it += 5e-2;
		}
		ver += ac(map, rnd());	
	}
	return ver;
}

int main() {
	using namespace std;
	cout << "请输入 N >>> ";
	unsigned n;
	cin >> n;
	vector<list<pair<int, int>>> map;
	double top = 0., bot = 0.; 
	double* time = new double[n * 2 + 1];
	for(int i = 1, cnt = 0; i <= 3 * n; i = ceil(i * TI), cnt++)
	{
		system("cls");
		cout << "测试中 " << Process[i % sizeof(Process)] << ' ';
		map.assign(i, list<pair<int, int>>());
		mod = i;
		clock_t start = clock();
		insert(map, n);
		test(map, n * 3);
		clock_t end = clock();
		time[i] = double(end - start) / CLOCKS_PER_SEC + 2e-4;
		top = max(top, log2(time[i]));
		bot = min(bot, log2(time[i]));
	} 
	cout << " 测试完成 \n";
	cout.flags(ios::fixed);
	cout.precision(3);
	int cnt = 0;
	for(double i = top + Step; i >= bot - Step; i -= Step, cnt++)
	{
		if(cnt & 1)
			cout << setw(6) << pow(2, i) << ' ';
		else
			cout << "       ";
		for(int j = 1; j <= 2 * n; j = ceil(j * TI))
		{
			if(log2(time[j]) >= i)
				cout << '#';
			else if(log2(time[j]) >= i - Step / 2.)
				cout << '_';
			else
				cout << ' ';
		}
		cout << endl;	
	}
	cout << "       ";
	int mdms = 0;
	for(int i = 0, j = 1; j <= 3 * n; j = ceil(j * TI), i++)
		if(i >= mdms)
		{
			cout << j << ' ';
			mdms += int(log10(j)) + 2;
		}
	delete[] time;
	return 0;
}
