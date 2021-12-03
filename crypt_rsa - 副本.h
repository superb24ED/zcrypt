//coding : utf-8
#pragma once
#include<stdint.h>
#include "bignum.h"
#include<memory>


/*
RSA��������:
I.��Կ����
	1. �����ȡ�������ǳ��������p��q
	2. ����n = pxq
	3. ����n��ŷ������: phai = (p-1)(q-1)
	4. ���ѡȡ1��phai֮���һ������e,��e��phai����(Ϊ��ʹ�ù�˽Կ��Ψһ),������Ϊ��Կ��һ����
	5. ��ģphai�������£��õ�e����Ԫd,˵�˻����� de=1(mod phai)
	6. ���˵õ���Կ��(e,n) , ��˽Կ: (d,n)
II.���ܹ���
	1.���ͷ��õ��˹�Կ(e,n),��������p������c=p^e (mod n).����Ҫ��0<p<n
III.���ܹ���
	1.�������c�󣬽��ܷ���ֻ��֮��֪����˽Կ(d,n)������: p = c^d (mod n)

����RSA1024��Ҫ�����Ĳ��ô���128bytes������Ϊ���֮���ԭ��ʵ�ʳ������޿��ܸ�С.
*/

//�궨��
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



//�ṹ����
class rsa_key {
public:
	uint32_t scale;		//��ģ����rsa1024,��ʱ����
	rsa_int key;		//��Կ
	rsa_int n;			//������
};




//��������

//����rsa�ӽ���
void rsa(
	const rsa_int* key,		//�����������Կ
	const rsa_int* pn,		//������n
	const rsa_int* pp1,		//��������ܵ�����/����
	rsa_int* pp2			//����/���ܺ�Ľ��

);
void rsa(
	const rsa_key* key_obj, //��Կ��Ԫ
	const rsa_int* pp1,	//����/����
	rsa_int* pp2			//����/���ܺ�Ľ��
);	

//����

//��ȡ������
void gen_prime(rsa_int* out);
//��ȡ˽Կ
void rsa_get_dky(const rsa_int* pp, const rsa_int* pq, const rsa_int* peky, rsa_int* pdky);

int gcd(int a, int b);

bool isPrime(int p);

