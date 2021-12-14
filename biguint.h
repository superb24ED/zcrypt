#ifndef __BIGINT_H_
#define __BIGINT_H_
/*
author:SupreME
*/
	
#define MAX_BIGUINT_SIZE 256	 //大整数的大小(字节),按照实际情况定义

//增加以下两个值可以增加素数判断可信度，但是可能会延长判断时间
#define BIGUINT_ISPRIME_ENUM_BITS_BORDER 0 //is_prime()的算法选择策略，过大将严重影响性能(更倾向于枚举，但是得到的是明确的判断)
#define BIGUINT_ISPRIME_MILLER_RABIN_ATTEMPS 10	//miller_rabbin算法尝试次数,建议不小于10
//大整数数据结构
#include <stdint.h>
#include<stdlib.h>


#include <random>
//随机数发生器
//int get_rand_int_privous();//使用rand()生成，不满足crypto标准，舍弃
//todo: 可以将这里完善打包为随机数生成模块
static std::random_device rd;
size_t gen_rand();



typedef union BIGUINT
{
	uint8_t byte_part[MAX_BIGUINT_SIZE];
	uint16_t lbyte_part[MAX_BIGUINT_SIZE >> 1];
	uint32_t dbyte_part[MAX_BIGUINT_SIZE >> 2];
	uint64_t qbyte_part[MAX_BIGUINT_SIZE >> 3];
	//构造
	BIGUINT();
	BIGUINT(uint64_t b);
	BIGUINT(const BIGUINT& b);
	BIGUINT(const uint8_t* buffer, uint32_t buf_len); //从一段缓存生成大质数(直接将它作为一个数).若不足以填满，则将尽量靠后填充(相当于前面补0)
	//RSAUINT(const uint8_t* buffer, uint32_t buf_len, uint32_t base=10); //从一段缓存生成大质数(输入数字，进行进制转换)
	BIGUINT& operator=(uint64_t b);
	BIGUINT& operator=(const BIGUINT& b);

	//比较
	//int cmp(const RSAUINT& b) const;
	//以下比较函数，实际上都是通过memcmp实现..
	//当一次涉及到众多比较运算的时候，实际上用memcmp + seitch.case更加高效
	bool operator>(const BIGUINT& b) const;
	bool operator>=(const BIGUINT& b) const;
	bool operator<(const BIGUINT& b) const;
	bool operator<=(const BIGUINT& b) const;
	bool operator==(const BIGUINT& b) const;
	bool operator!=(const BIGUINT& b) const;

	//大小端判断
	static bool is_little_edian();
	//大小端转换
	static uint16_t u16edian_trans(uint16_t t);
	static uint32_t u32edian_trans(uint32_t t);
	static uint64_t u64edian_trans(uint64_t t);

	//类型转换 功能与==有冲突，因此弃用
	//operator uint64_t();


	//移位，逻辑运算
	BIGUINT operator<<(uint32_t m)const;
	BIGUINT operator>>(uint32_t m)const;
	bool operator<<=(uint32_t m);
	bool operator>>=(uint32_t m);


	BIGUINT operator |(const BIGUINT& b)const;
	BIGUINT operator &(const BIGUINT& b)const;
	BIGUINT operator ^(const BIGUINT& b)const;
	BIGUINT operator~()const;

	bool operator!()const;


	//加法
	BIGUINT operator+(const BIGUINT& b) const;
	BIGUINT operator-(const BIGUINT& b) const;
	bool operator+=(const BIGUINT& b);
	bool operator-=(const BIGUINT& b);

	BIGUINT operator++();
	BIGUINT operator--();

	BIGUINT operator++(int);
	BIGUINT operator--(int);

	//乘法
	BIGUINT operator*(const BIGUINT& b) const;
	bool operator*=(const BIGUINT& b);


	//输出
	void show_hex()const;
	void show_dec()const;
	void show_oct()const;
	void show_binary()const;



	//prime
	bool miller_rabin_test(uint32_t attempts)const;
	bool prime_enum_test()const;

	bool is_prime()const;
	uint32_t get_binary_bits()const;
	//取模
	BIGUINT operator%(const BIGUINT& modulus)const;
	bool operator%=(const BIGUINT& modulus);
	//乘方取模
	BIGUINT modulus_pow(const BIGUINT& exponent, const BIGUINT& modulus)const;
	BIGUINT modulus_pow1(const BIGUINT& exponent, const BIGUINT& modulus)const; //递归 会出现栈溢出
	BIGUINT modulus_pow2(const BIGUINT& exponent, const BIGUINT& modulus)const; //动态优化
	//相乘取模
	BIGUINT modulus_mul(const BIGUINT& b, const BIGUINT& modulus)const;
	BIGUINT modulus_mul1(const BIGUINT& b, const BIGUINT& modulus)const;
	BIGUINT modulus_mul2(const BIGUINT& b, const BIGUINT& modulus)const;
	//相加取模
	BIGUINT modulus_add(const BIGUINT& b, const BIGUINT& modulus)const;

	//公因数
	//整数除法
	BIGUINT int_dev(const BIGUINT& b) const;
	BIGUINT gcd(const BIGUINT& b) const;
	static BIGUINT gcd(const BIGUINT& a, const BIGUINT& b);


	//尝试求模modulus的逆元(除法)
	BIGUINT modulus_inv(const BIGUINT& modulus)const;
	BIGUINT modulus_inv1(const BIGUINT& modulus) const;
	BIGUINT modulus_inv2(const BIGUINT& modulus)const;
	//指定大数生成
	static BIGUINT zero();			//0
	static BIGUINT max_biguint();  //0xfffffffff......
	static BIGUINT binary_ones(uint32_t bits);//bits个1组成的二进制数

	static BIGUINT gen_random_integer(uint32_t bits);
	static BIGUINT gen_random_odd(uint32_t bits);
	static BIGUINT gen_random_prime(uint32_t bits, uint32_t attempts);
} UNSIGNED_LARGE_INTENGER,*PBIGUINT,*PUNSIGNED_LARGE_INTENGER,&REFBIGUINT,&REFUNSIGNED_LARGE_INTENGER;


#endif
