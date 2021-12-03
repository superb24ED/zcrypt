#include "crypt_rsa.h"

void rsa(const rsa_int* key, const rsa_int* pn, const rsa_int* pp1, rsa_int* pp2)
{
	*pp2 = *pp1 ^ *key % *pn;
}


void rsa(const rsa_key* key_obj, const rsa_int* pp1,rsa_int* pp2)
{
	rsa(&key_obj->key, &key_obj->n, pp1,pp2);
}

void rsa_get_dky(const rsa_int* pp, const rsa_int* pq, const rsa_int* peky, rsa_int* pdky)
{
	auto n = *pp * *pq;
	auto phai = (*pp - 1) * (*pq - 1);
	*pdky = 0;
	for (rsa_int k = 0; k < n; k++) {
		if (!(k * phai + 1) % *peky)
		{
			*pdky = (k * phai + 1) / *peky;
			break;
		}
	}
}


int gcd(int a, int b)  //求最大公约数
{
	     int c = 0;
		 if (a < b) { c = b; b = a; a = c; }
	     c = b;
	     do
		     {
		         b = c;
		         c = a % b;
		         a = b;
		     }
	     while (c != 0);
	     return b;
}

bool isPrime(int p) {
	if (p <= 1)
		return 0;
	for (int i = 2; i < p; i++) {
		if (0 == p % i) {
			return 0;
		}
	}
	return 1;
}