#include "crypt_rsa.h"


rsa_key::rsa_key()
{
	scale = RSA_UINT_SIZE;
	modulus = 0;
	exponent = 0;
}

rsa_key::rsa_key(uint64_t exponent, uint64_t modulus, uint32_t scale)
{
	this->scale = scale;
	this->modulus = modulus;
	this->exponent = exponent;
}

rsa_key::rsa_key(rsa_uint& exponent, rsa_uint &modulus, uint32_t scale)
{
	this->scale = scale;
	this->modulus = modulus;
	this->exponent = exponent;
}


rsa_uint rsa_key::get_modulus() const
{
	return this->modulus;
}

rsa_uint rsa_key::get_exponent() const
{
	return this->exponent;
}

bool rsa_key::gen_keypair(rsa_uint p, rsa_uint q, rsa_key& eky, rsa_key& dky, uint32_t scale)
{
		//uint32_t p_bits = rand() & 0x3ff;

	uint32_t p_bits = 0;
	do {
		p_bits = gen_rand() % (scale << 3);
	} while ((p_bits > 16) &&
		(p_bits < 1008) &&
		(scale << 3) - (p_bits << 1) < 0xff);
	uint32_t q_bits = (scale << 3) - p_bits;
	if (p == 0 ) {
		p = rsa_uint::gen_random_prime(p_bits, 200);//0.4ln(2^600) = 166.3
		if (p == 0) {
			p = rsa_uint::gen_random_prime(p_bits, 200);
			if (p == 0)
				return false;
		}
	}
	else {
		if (!p.is_prime()) {
			return false;
		}
	}
	if (q == 0) {
		q= rsa_uint::gen_random_prime(q_bits, 200);//0.4ln(2^600) = 166.3
		if (p == 0) {
			//_sleep(0x500u);
			p = rsa_uint::gen_random_prime(q_bits, 200);
			if (p == 0)
				return false;
		}
	}
	else {
		if (!q.is_prime())
			return false;
	}


	//计算

	rsa_uint modulus = p * q, phai = (p - 1) * (q - 1);
	if (rsa_uint::gcd(eky.exponent, phai) != 1)//预设的eky非法
		return false;
	eky.modulus = modulus;
	eky.scale = scale;
	if (eky.exponent == 0 ) {//尝试生成满足加密要求的情况下，尽量小的素数,失败则直接使用 DEFAULT_EKY_EXPONENT
		uint32_t exponent = 0;
		int attempts = 15;//尝试15次
		while ((exponent < 0x100) && attempts) {//8到64位二进制的整数
			exponent = gen_rand() & 0xffffff;
			attempts--;
		}
		if(attempts)
			eky.exponent = exponent;
		else
			eky.exponent = DEFAULT_EKY_EXPONENT;
	}

	dky.modulus = modulus;
	dky.scale = scale;

	//核心：私钥dky.expo
	dky.exponent = eky.exponent.modulus_inv(modulus);
	return true;
}

bool rsa_key::get_dky(const rsa_uint& p, const rsa_uint& q, const rsa_key& eky, rsa_key& dky)
{
	if (!p || !q) return false;
	return get_dky( (p-1)*(q-1),eky,dky );
}

bool rsa_key::get_dky(const rsa_uint& phai, const rsa_key& eky, rsa_key& dky)
{
	if (!phai || !eky.modulus || !eky.exponent)
		return false;
	else {
		dky.scale = eky.scale;
		dky.modulus = eky.modulus;
		dky.exponent =  eky.exponent.modulus_inv(phai);
		return (dky.exponent != 0);
	}
}

bool rsa_key::get_dky(uint64_t p, uint64_t q, const rsa_key& eky, rsa_key& dky)
{
	return get_dky(p * q, eky, dky);
}

bool rsa_key::get_dky(uint64_t phai, uint64_t eky, rsa_key& dky)
{
	return get_dky(phai, eky, dky);
}


rsa_uint rsa_key::crypt(rsa_uint& base_text)
{
	return base_text.modulus_pow(exponent, modulus);
}
rsa_uint crypt(rsa_uint& base_text, rsa_key& key)
{
	return base_text.modulus_pow(key.exponent, key.modulus);
}