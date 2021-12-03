//coding : utf-8
#pragma once
#include<stdint.h>
#include "bignum.h"
#include<memory>


/*
RSA加密流程:
I.密钥生成
	1. 随机获取到两个非常大的质数p和q
	2. 计算n = pxq
	3. 计算n的欧拉函数: phai = (p-1)(q-1)
	4. 随机选取1到phai之间的一个整数e,且e与phai互质(为了使得公私钥对唯一),它将作为公钥的一部分
	5. 在模phai有限域下，得到e的逆元d,说人话就是 de=1(mod phai)
	6. 至此得到公钥：(e,n) , 与私钥: (d,n)
II.加密过程
	1.发送方得到了公钥(e,n),对于明文p，计算c=p^e (mod n).这里要求0<p<n
III.解密过程
	1.获得密文c后，解密方用只有之间知道的私钥(d,n)来计算: p = c^d (mod n)

对于RSA1024，要求明文不得大于128bytes，而因为填充之类的原因，实际长度上限可能更小.
*/

//宏定义
#define DEAULT_RSA_SCALE 1024
typedef uint64_t rsa_int;

struct rsa_int2{
	union BIGNUM {
		uint8_t bytes[DEAULT_RSA_SCALE];
		uint16_t words[DEAULT_RSA_SCALE>>1];
		uint32_t dwords[DEAULT_RSA_SCALE>>2];
		uint64_t qwords[DEAULT_RSA_SCALE>>3];
	} value;
	rsa_int2(uint64_t n=0) {
		memset(&value, 0, sizeof(value));
		int i = 1;
		while (n) {
			value.bytes[DEAULT_RSA_SCALE - i++] = n & 0xFF;
			n >>= 8;
		}
	}
	rsa_int2(uint8_t* pData, uint32_t size) {
		if (size <= sizeof(value)) {
		uint32_t padding = sizeof(value) - size;
		memset(&value, 0, padding);
		memcpy_s(&value.bytes[padding], size, pData, size);
		}
	}

};



//结构定义
class rsa_key {
public:
	uint32_t scale;		//规模，如rsa1024,暂时弃用
	rsa_int key;		//密钥
	rsa_int n;			//大整数
};




//函数定义

//进行rsa加解密
void rsa(
	const rsa_int* key,		//加密与解密密钥
	const rsa_int* pn,		//大整数n
	const rsa_int* pp1,		//加密与解密的明文/密文
	rsa_int* pp2			//加密/解密后的结果

);
void rsa(
	const rsa_key* key_obj, //密钥单元
	const rsa_int* pp1,	//明文/密文
	rsa_int* pp2			//加密/解密后的结果
);	

//生成

//获取大素数
void gen_prime(rsa_int* out);
//获取私钥
void rsa_get_dky(const rsa_int* pp, const rsa_int* pq, const rsa_int* peky, rsa_int* pdky);

int gcd(int a, int b);

bool isPrime(int p);

