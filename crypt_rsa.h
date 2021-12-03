//coding : utf-8
#pragma once
/*
* ����:SupreME
* ����:2021.12.2
* �汾:0.1
* 
RSA��������:
I.��Կ����
	1. �����ȡ�������ǳ��������p��q
	2. ����n = pq
	3. ����n��ŷ������: phai = (p-1)(q-1)
	4. ���ѡȡ1��phai֮���һ������e,��e��phai����,������ģ��nһ����Ϊ��Կ
	5. �õ�eģphai����Ԫd,˵�˻����� de=1(mod phai)
	6. ���˵õ���Կ��(e,n) , ��˽Կ: (d,n), p,q��phai�ļ�¼�򱻱��ܻ�������
II.���ܹ���
	1.���ͷ��õ��˹�Կ(e,n),��������p������c=p^e (mod n).����Ҫ��0<p<n
III.���ܹ���
	1.�������c�󣬽��ܷ���ֻ��֮��֪����˽Կ(d,n)������: p = c^d (mod n)

����RSA1024��Ҫ�����Ĳ��ô���128-11 = 117bytes������Ϊ���֮���ԭ��ʵ�ʳ������޿��ܸ�С.

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


