#include <cmath>
#include <ctime>
#include <iostream>

int main() {
	using namespace std;
	double best = NAN;
	while (true)
	{
		cout << "\n\n+-- 欢迎来到掐 10 大赛 --+\n"
				"游戏规则：按下 Enter 键后，10秒后按下 Enter 结束，越接近 10s，得分越高\n"
				"最好成绩：" << best << " 分\n";
		cin.get();
		cout << ">>> 计时开始 <<<\n";
		auto start = clock();
		cin.get();
		auto end = clock();
		double time = double(end - start) / CLOCKS_PER_SEC;
		double score = log10(time) / abs(time - 10.);
		cout << "时间为：" << time << " 秒\n得分为：" << score << " 分\n";
		best = max(score, best);
	}
}
