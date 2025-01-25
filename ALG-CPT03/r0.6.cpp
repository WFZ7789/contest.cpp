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
    for (int i = 0; i < 64; ++i)
    {
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
    for (char c: input)
    {
        data[datalen++] = c;
        bitlen += 8;

        // 如果数据块满了（64字节），进行变换
        if (datalen == 64)
        {
            sha256_transform(state, data);
            datalen = 0;
        }
    }

    // 填充数据
    data[datalen++] = 0x80; // 添加一个1
    if (datalen > 56)
    {
        // 如果当前块不足以容纳填充和长度
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
    for (int i = 0; i < 8; ++i)
    {
        ss << std::hex << std::setw(8) << std::setfill('0') << state[i];
    }
    return ss.str();
}

//以下是AES-128加密

const string mi = "1145141919810114";

const uint8_t S_BOX[256] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

// 逆S盒（Inverse Substitution Box）
const uint8_t INV_S_BOX[256] = {
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};

// 轮常数（Round Constants）
const uint8_t R_CON[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};

// 列混合矩阵
const uint8_t MIX_MATRIX[4][4] = {
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}
};

// 逆列混合矩阵
const uint8_t INV_MIX_MATRIX[4][4] = {
    {0x0E, 0x0B, 0x0D, 0x09},
    {0x09, 0x0E, 0x0B, 0x0D},
    {0x0D, 0x09, 0x0E, 0x0B},
    {0x0B, 0x0D, 0x09, 0x0E}
};

// 有限域乘法
uint8_t gf_multiply(uint8_t a, uint8_t b) {
    uint8_t result = 0;
    for (int i = 0; i < 8; ++i)
    {
        if (b & 1)
        {
            result ^= a;
        }
        bool carry = a & 0x80;
        a <<= 1;
        if (carry)
        {
            a ^= 0x1B; // x^8 + x^4 + x^3 + x + 1
        }
        b >>= 1;
    }
    return result;
}

// 列混合
void mix_columns(uint8_t state[16]) {
    uint8_t temp[16];
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            temp[i * 4 + j] = 0;
            for (int k = 0; k < 4; ++k)
            {
                temp[i * 4 + j] ^= gf_multiply(MIX_MATRIX[j][k], state[i * 4 + k]);
            }
        }
    }
    for (int i = 0; i < 16; ++i)
    {
        state[i] = temp[i];
    }
}

// 逆列混合
void inv_mix_columns(uint8_t state[16]) {
    uint8_t temp[16];
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            temp[i * 4 + j] = 0;
            for (int k = 0; k < 4; ++k)
            {
                temp[i * 4 + j] ^= gf_multiply(INV_MIX_MATRIX[j][k], state[i * 4 + k]);
            }
        }
    }
    for (int i = 0; i < 16; ++i)
    {
        state[i] = temp[i];
    }
}

// 密钥扩展
void key_expansion(const uint8_t* key, uint8_t* round_keys) {
    // 第一轮密钥就是初始密钥
    for (int i = 0; i < 16; ++i)
    {
        round_keys[i] = key[i];
    }

    // 生成后续轮密钥
    for (int i = 1; i <= 10; ++i)
    {
        // 处理每轮的第一个字
        uint8_t temp[4];
        for (int j = 0; j < 4; ++j)
        {
            temp[j] = round_keys[(i - 1) * 16 + 12 + j];
        }
        uint8_t t = temp[0];
        temp[0] = S_BOX[temp[1]] ^ R_CON[i - 1];
        temp[1] = S_BOX[temp[2]];
        temp[2] = S_BOX[temp[3]];
        temp[3] = S_BOX[t];
        for (int j = 0; j < 4; ++j)
        {
            round_keys[i * 16 + j] = round_keys[(i - 1) * 16 + j] ^ temp[j];
        }

        // 处理每轮的其他三个字
        for (int j = 4; j < 16; ++j)
        {
            round_keys[i * 16 + j] = round_keys[i * 16 + j - 4] ^ round_keys[(i - 1) * 16 + j];
        }
    }
}

// 字节替换（SubBytes）
void sub_bytes(uint8_t state[16]) {
    for (int i = 0; i < 16; ++i)
    {
        state[i] = S_BOX[state[i]];
    }
}

// 逆行替换（InvSubBytes）
void inv_sub_bytes(uint8_t state[16]) {
    for (int i = 0; i < 16; ++i)
    {
        state[i] = INV_S_BOX[state[i]];
    }
}

// 行移位（ShiftRows）
void shift_rows(uint8_t state[16]) {
    uint8_t temp;

    // 第二行左移 1 字节
    temp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = temp;

    // 第三行左移 2 字节
    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;

    // 第四行左移 3 字节
    temp = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = state[3];
    state[3] = temp;
}

// 逆行移位（InvShiftRows）
void inv_shift_rows(uint8_t state[16]) {
    uint8_t temp;

    // 第二行右移 1 字节
    temp = state[13];
    state[13] = state[9];
    state[9] = state[5];
    state[5] = state[1];
    state[1] = temp;

    // 第三行右移 2 字节
    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;

    // 第四行右移 3 字节
    temp = state[3];
    state[3] = state[7];
    state[7] = state[11];
    state[11] = state[15];
    state[15] = temp;
}

// 轮密钥加（AddRoundKey）
void add_round_key(uint8_t state[16], const uint8_t* round_key) {
    for (int i = 0; i < 16; ++i)
    {
        state[i] ^= round_key[i];
    }
}

// AES 加密
void aes_encrypt(const uint8_t* plaintext, const uint8_t* key, uint8_t* ciphertext) {
    uint8_t state[16];
    uint8_t round_keys[176]; // 11 轮密钥，每轮 16 字节

    // 初始化状态
    for (int i = 0; i < 16; ++i)
    {
        state[i] = plaintext[i];
    }

    // 密钥扩展
    key_expansion(key, round_keys);

    // 初始轮密钥加
    add_round_key(state, round_keys);

    // 主要轮操作
    for (int round = 1; round <= 9; ++round)
    {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, round_keys + round * 16);
    }

    // 最终轮操作
    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, round_keys + 10 * 16);

    // 输出密文
    for (int i = 0; i < 16; ++i)
    {
        ciphertext[i] = state[i];
    }
}

// AES 解密
void aes_decrypt(const uint8_t* ciphertext, const uint8_t* key, uint8_t* plaintext) {
    uint8_t state[16];
    uint8_t round_keys[176]; // 11 轮密钥，每轮 16 字节

    // 初始化状态
    for (int i = 0; i < 16; ++i)
    {
        state[i] = ciphertext[i];
    }

    // 密钥扩展
    key_expansion(key, round_keys);

    // 初始轮密钥加
    add_round_key(state, round_keys + 10 * 16);

    // 主要轮操作
    for (int round = 9; round >= 1; --round)
    {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, round_keys + round * 16);
        inv_mix_columns(state);
    }

    // 最终轮操作
    inv_shift_rows(state);
    inv_sub_bytes(state);
    add_round_key(state, round_keys);

    // 输出明文
    for (int i = 0; i < 16; ++i)
    {
        plaintext[i] = state[i];
    }
}

double bi = 0;
double bib = 0;

int main() {
    uint8_t plaintext[16];
    uint8_t key[16];
    uint8_t ciphertext[16];
    uint8_t decryptedtext[16];
    std::memcpy(key, mi.c_str(), 16);
    unsigned int seed;
    string id;
    cout << "请输入账号id（电子钱包文件名） >>> ";
    cin >> id;
    id += ".xt";
    ifstream oI(id);
    std::string input;
    oI >> input;
    if (!input.empty())
    {
        oI.close();
        for (int i = 0; i < 16; ++i)
        {
            std::string byteString = input.substr(2 * i, 2);
            ciphertext[i] = static_cast<uint8_t>(std::stoul(byteString, nullptr, 16));
        }
        aes_decrypt(ciphertext, key, decryptedtext);
        string k = "";
        for (int i = 0; i < 16; ++i)
            k.push_back(decryptedtext[i]);
        bib = bi = stod(k);
    }
    seed = time(nullptr);
    clock_t start;
    double past;
    mt19937 sui(seed);
    for (unsigned t = 1; ; t++)
    {
        if (t % 2500000 == 1)
        {
            start = clock();
            past = bi;
        }
        int n = sui() % 512 + 1;
        string str;
        for (int i = 1; i <= n; i++)
            str.push_back(sui() % 127 + 1);
        std::string output = sha256(str);
        int on = output.size();
        double r = 1;
        for (int i = 0; i < on; i += 6)
        {
            if (output.substr(i, 6) == "114514")
            {
                bi += r * r;
                r *= 1e4;
            }
            else if (output.substr(i, 4) == "1145")
                bi += 2e-3 * r * r;
            else if (output.substr(i, 3) == "114")
                bi += 5e-6 * r * r;
            else
                break;
        }
        if (bi - bib > 1.9e-3)
        {
            printf("%.3lf ", bi);
            std::cout << output << std::endl;
            ofstream oF(id);
            string h = to_string(bi);
            int hai = 16 - h.size();
            for (int i = 1; i <= hai; i++)
                h = "0" + h;
            std::memcpy(plaintext, h.c_str(), 16);
            aes_encrypt(plaintext, key, ciphertext);
            for (int i = 0; i < 16; ++i)
                oF << hex << setw(2) << setfill('0') << (int) ciphertext[i];
            oF.close();
            if (bi - bib >= 0.9)
            {
                oF.open(id + "dat", ios::app);
                oF << "Xtraci " << bi - bib << '\0' << str << '\0' << output << '\0' << "\n\n";
                oF.close();
            }
            bib = bi;
        }
        if (bi != bib)
            bib = bi;
        if (t % 2500000 == 0)
            printf("[ SPEED = %.6lf it / s ]\n", (bib - past) / (double(clock() - start) / CLOCKS_PER_SEC));
    }
    return 0;
}
