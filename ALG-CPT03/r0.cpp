#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <bits/stdc++.h>
using namespace std;
// SHA-256 常量
const uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// 循环右移
#define ROTRIGHT(word, bits) (((word) >> (bits)) | ((word) << (32 - (bits))))

// SHA-256 函数
#define EP0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22))
#define EP1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25))
#define SIG0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))

// SHA-256 核心变换函数
void sha256_transform(uint32_t state[8], const uint8_t data[64]) {
    uint32_t a, b, c, d, e, f, g, h, t1, t2, m[64];

    // 将数据分块为16个32位字
    for (int i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);

    // 扩展消息块
    for (int i = 16; i < 64; ++i)
        m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

    // 初始化工作变量
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    f = state[5];
    g = state[6];
    h = state[7];

    // 主循环
    for (int i = 0; i < 64; ++i) {
        t1 = h + EP1(e) + ((e & f) ^ (~e & g)) + K[i] + m[i];
        t2 = EP0(a) + ((a & b) ^ (a & c) ^ (b & c));
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    // 更新状态
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}
// SHA-256 主函数
std::string sha256(const std::string& input) {
    uint8_t data[64];
    uint32_t state[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    uint64_t bitlen = 0;
    uint32_t datalen = 0;

    // 处理输入数据
    for (char c : input) {
        data[datalen++] = c;
        bitlen += 8;

        // 如果数据块满了（64字节），进行变换
        if (datalen == 64) {
            sha256_transform(state, data);
            datalen = 0;
        }
    }

    // 填充数据
    data[datalen++] = 0x80; // 添加一个1
    if (datalen > 56) { // 如果当前块不足以容纳填充和长度
        while (datalen < 64)
            data[datalen++] = 0;
        sha256_transform(state, data);
        datalen = 0;
    }

    // 填充0直到剩余56字节
    while (datalen < 56)
        data[datalen++] = 0;

    // 添加原始数据的长度（以位为单位）
    bitlen = input.size() * 8;
    data[63] = bitlen;
    data[62] = bitlen >> 8;
    data[61] = bitlen >> 16;
    data[60] = bitlen >> 24;
    data[59] = bitlen >> 32;
    data[58] = bitlen >> 40;
    data[57] = bitlen >> 48;
    data[56] = bitlen >> 56;

    // 处理最后一个块
    sha256_transform(state, data);

    // 将结果转换为十六进制字符串
    std::stringstream ss;
    for (int i = 0; i < 8; ++i) {
        ss << std::hex << std::setw(8) << std::setfill('0') << state[i];
    }
    return ss.str();
}
double bi = 0;
double bib = 0;
int main() {
   	unsigned int seed;
   	cout << "请输入种子 >>> ";
   	cin >> seed;
   	mt19937 sui(seed);
   	while(1)
	{
   		int n = sui() % 512 + 1;
   		string str;
	    for(int i = 1; i <= n; i++)
	    	str.push_back(sui() % 128);
	    std::string output = sha256(str);
	    int on = output.size();
	    double r = 1;
	    for(int i = 0; i < on; i += 6)
		{
	    	if(output.substr(i, 6) == "114514")
	    	{
	    		bi += r;
				r *= 1e4;
			}
			else if(output.substr(i, 4) == "1145")
				bi += 2e-3 * r;
			else if(output.substr(i, 3) == "114")
				bi += 5e-6 * r;
			else
				break;
		}
	    if(bi - bib > 5e-3)
		{
			printf("%.3lf ", bi);
	    	std::cout << output << std::endl;  
	    	bib = bi;
		}		
	}
    return 0;
}
