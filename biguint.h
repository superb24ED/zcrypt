#ifndef __BIGINT_H_
#define __BIGINT_H_
/*
author:SupreME
*/
	
#define MAX_BIGUINT_SIZE 256	 //�������Ĵ�С(�ֽ�),����ʵ���������

//������������ֵ�������������жϿ��Ŷȣ����ǿ��ܻ��ӳ��ж�ʱ��
#define BIGUINT_ISPRIME_ENUM_BITS_BORDER 0 //is_prime()���㷨ѡ����ԣ���������Ӱ������(��������ö�٣����ǵõ�������ȷ���ж�)
#define BIGUINT_ISPRIME_MILLER_RABIN_ATTEMPS 10	//miller_rabbin�㷨���Դ���,���鲻С��10
//���������ݽṹ
#include <stdint.h>
#include<stdlib.h>


#include <random>
//�����������
//int get_rand_int_privous();//ʹ��rand()���ɣ�������crypto��׼������
//todo: ���Խ��������ƴ��Ϊ���������ģ��
static std::random_device rd;
size_t gen_rand();



typedef union BIGUINT
{
	uint8_t byte_part[MAX_BIGUINT_SIZE];
	uint16_t lbyte_part[MAX_BIGUINT_SIZE >> 1];
	uint32_t dbyte_part[MAX_BIGUINT_SIZE >> 2];
	uint64_t qbyte_part[MAX_BIGUINT_SIZE >> 3];
	//����
	BIGUINT();
	BIGUINT(uint64_t b);
	BIGUINT(const BIGUINT& b);
	BIGUINT(const uint8_t* buffer, uint32_t buf_len); //��һ�λ������ɴ�����(ֱ�ӽ�����Ϊһ����).���������������򽫾����������(�൱��ǰ�油0)
	//RSAUINT(const uint8_t* buffer, uint32_t buf_len, uint32_t base=10); //��һ�λ������ɴ�����(�������֣����н���ת��)
	BIGUINT& operator=(uint64_t b);
	BIGUINT& operator=(const BIGUINT& b);

	//�Ƚ�
	//int cmp(const RSAUINT& b) const;
	//���±ȽϺ�����ʵ���϶���ͨ��memcmpʵ��..
	//��һ���漰���ڶ�Ƚ������ʱ��ʵ������memcmp + seitch.case���Ӹ�Ч
	bool operator>(const BIGUINT& b) const;
	bool operator>=(const BIGUINT& b) const;
	bool operator<(const BIGUINT& b) const;
	bool operator<=(const BIGUINT& b) const;
	bool operator==(const BIGUINT& b) const;
	bool operator!=(const BIGUINT& b) const;

	//��С���ж�
	static bool is_little_edian();
	//��С��ת��
	static uint16_t u16edian_trans(uint16_t t);
	static uint32_t u32edian_trans(uint32_t t);
	static uint64_t u64edian_trans(uint64_t t);

	//����ת�� ������==�г�ͻ���������
	//operator uint64_t();


	//��λ���߼�����
	BIGUINT operator<<(uint32_t m)const;
	BIGUINT operator>>(uint32_t m)const;
	bool operator<<=(uint32_t m);
	bool operator>>=(uint32_t m);


	BIGUINT operator |(const BIGUINT& b)const;
	BIGUINT operator &(const BIGUINT& b)const;
	BIGUINT operator ^(const BIGUINT& b)const;
	BIGUINT operator~()const;

	bool operator!()const;


	//�ӷ�
	BIGUINT operator+(const BIGUINT& b) const;
	BIGUINT operator-(const BIGUINT& b) const;
	bool operator+=(const BIGUINT& b);
	bool operator-=(const BIGUINT& b);

	BIGUINT operator++();
	BIGUINT operator--();

	BIGUINT operator++(int);
	BIGUINT operator--(int);

	//�˷�
	BIGUINT operator*(const BIGUINT& b) const;
	bool operator*=(const BIGUINT& b);


	//���
	void show_hex()const;
	void show_dec()const;
	void show_oct()const;
	void show_binary()const;



	//prime
	bool miller_rabin_test(uint32_t attempts)const;
	bool prime_enum_test()const;

	bool is_prime()const;
	uint32_t get_binary_bits()const;
	//ȡģ
	BIGUINT operator%(const BIGUINT& modulus)const;
	bool operator%=(const BIGUINT& modulus);
	//�˷�ȡģ
	BIGUINT modulus_pow(const BIGUINT& exponent, const BIGUINT& modulus)const;
	BIGUINT modulus_pow1(const BIGUINT& exponent, const BIGUINT& modulus)const; //�ݹ� �����ջ���
	BIGUINT modulus_pow2(const BIGUINT& exponent, const BIGUINT& modulus)const; //��̬�Ż�
	//���ȡģ
	BIGUINT modulus_mul(const BIGUINT& b, const BIGUINT& modulus)const;
	BIGUINT modulus_mul1(const BIGUINT& b, const BIGUINT& modulus)const;
	BIGUINT modulus_mul2(const BIGUINT& b, const BIGUINT& modulus)const;
	//���ȡģ
	BIGUINT modulus_add(const BIGUINT& b, const BIGUINT& modulus)const;

	//������
	//��������
	BIGUINT int_dev(const BIGUINT& b) const;
	BIGUINT gcd(const BIGUINT& b) const;
	static BIGUINT gcd(const BIGUINT& a, const BIGUINT& b);


	//������ģmodulus����Ԫ(����)
	BIGUINT modulus_inv(const BIGUINT& modulus)const;
	BIGUINT modulus_inv1(const BIGUINT& modulus) const;
	BIGUINT modulus_inv2(const BIGUINT& modulus)const;
	//ָ����������
	static BIGUINT zero();			//0
	static BIGUINT max_biguint();  //0xfffffffff......
	static BIGUINT binary_ones(uint32_t bits);//bits��1��ɵĶ�������

	static BIGUINT gen_random_integer(uint32_t bits);
	static BIGUINT gen_random_odd(uint32_t bits);
	static BIGUINT gen_random_prime(uint32_t bits, uint32_t attempts);
} UNSIGNED_LARGE_INTENGER,*PBIGUINT,*PUNSIGNED_LARGE_INTENGER,&REFBIGUINT,&REFUNSIGNED_LARGE_INTENGER;


#endif
