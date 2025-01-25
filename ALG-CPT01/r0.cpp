#include <bits/stdc++.h>

std::pair<double, int> load(int n) {
	int fibo[3]{1, 1};
	auto start = clock();
	for(int i = 2; i < n; i++)
			fibo[i % 3] = fibo[(i - 1) % 3] + fibo[(i - 2) % 3];
	auto end = clock();
	return {double(clock() - start) / CLOCKS_PER_SEC, fibo[n % 3]};
}

int main() {
	using namespace std;
	unsigned long nowN = 134217728;
	unsigned long totN = 0;
	double nowtime = 0.0;
	printf("    N     |\t Time   |\t   Hash  \n");
	while(nowtime < 0.998 || nowtime > 1.005)
	{
		while(nowtime <= 1.0)
		{
			nowtime += load(nowN).first;
			totN += nowN;
		}
		totN -= nowN;
		std::pair<double, int> res = load(totN);
		nowtime = res.first;
		printf("%10lu|\t%6lf|\t%d\n", totN, nowtime, res.second);
		nowN *= 0.6;
	}
	cout << "\n\nResult = " << totN << endl; 
	return 0;
}
