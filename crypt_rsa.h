//coding : utf-8
#pragma once
/*
* 作者:SupreME
* 日期:2021.12.2
* 版本:0.1
* 
RSA加密流程:
I.密钥生成
	1. 随机获取到两个非常大的质数p和q
	2. 计算n = pq
	3. 计算n的欧拉函数: phai = (p-1)(q-1)
	4. 随机选取1到phai之间的一个整数e,且e与phai互质,将它与模数n一起作为公钥
	5. 得到e模phai的逆元d,说人话就是 de=1(mod phai)
	6. 至此得到公钥：(e,n) , 与私钥: (d,n), p,q和phai的记录则被保密或者销毁
II.加密过程
	1.发送方得到了公钥(e,n),对于明文p，计算c=p^e (mod n).这里要求0<p<n
III.解密过程
	1.获得密文c后，解密方用只有之间知道的私钥(d,n)来计算: p = c^d (mod n)

对于RSA1024，要求明文不得大于128-11 = 117bytes，而因为填充之类的原因，实际长度上限可能更小.

*/
#include "biguint.h"
//rsa1024
#define RSA_UINT_SIZE 128   // MAX_BIGUINT_SIZE >=  RSA_UINT_SIZE

//default eky
#define DEFAULT_EKY_EXPONENT 0x10001



typedef union BIGUINT  rsa_uint;
class rsa_key {
public:
	uint32_t scale;
	rsa_uint modulus;
	rsa_uint exponent;
	rsa_key();
	rsa_key(uint64_t exponent, uint64_t modulus, uint32_t scale = RSA_UINT_SIZE);
	rsa_key(rsa_uint& exponent, rsa_uint &modulus, uint32_t scale = RSA_UINT_SIZE);


	rsa_uint get_modulus()const;
	rsa_uint get_exponent()const;

	static bool gen_keypair(rsa_uint p, rsa_uint q, rsa_key& eky, rsa_key& dky, uint32_t scale = RSA_UINT_SIZE);
	static bool get_dky(const rsa_uint& p, const rsa_uint& q, const rsa_key& eky, rsa_key& dky);
	static bool get_dky(const rsa_uint& phai, const rsa_key& eky, rsa_key& dky);

	//int cast 
	static bool get_dky(uint64_t p, uint64_t q, const rsa_key& eky, rsa_key& dky);
	static bool get_dky(uint64_t phai, uint64_t eky, rsa_key& dky);


	rsa_uint crypt(rsa_uint &base_text);
	friend rsa_uint crypt(rsa_uint& base_text, rsa_key& key);
};


