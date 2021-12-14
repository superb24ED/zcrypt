#include "base64.h"
#include <stdio.h>	//fprintf
#include <stdlib.h> //abort()
#include <string.h>  //strchr


const char base64::b64_alphabet_table[] = {
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',	//0-25
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',	//26-51
	'0','1','2','3','4','5','6','7','8','9',	//52-61
	'+','='		//62,63
};

unsigned char base64::b64_alpha_to_data(const char c,bool return_when_error)
{
	if (c >= 'A' && c <= 'Z')//A->Z:0-25
		return c - 'A';
	else if (c >= 'a' && c <= 'z')//a->z:26-51
		return c - 'a' + 26;
	else if (c >= '0' && c <= '9')//0-9: 52-61
		return c - '0' + 52;
	else if (c == '+')
		return 62;
	else if (c == '/')
		return 63;
	else {
		if (return_when_error)
			return 64;
		fprintf(stderr, "转换错误,出现了非法的base64字符\n");
		exit(-1);
	}
}

bool base64::checkb64(const char* b64string, int b64strlen)
{
	if (strlen(b64string) != b64strlen) {
		fprintf(stderr, "错误: base64字符长度，与传入的长度不匹配，请检查参数\n");
		return false;
	}
	const char* pb64end = strchr(b64string, '=');
	if (!pb64end)
		pb64end = b64string + b64strlen;
	int padding_count = b64string + b64strlen - pb64end;
	if (b64strlen % 4) {
		fprintf(stderr, "错误: base64字符长度不合法\n");
		return false;
	}
	switch (padding_count)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	default:	
	{
		fprintf(stderr, "错误: 等号个数不应多于2\n");
		return false;
	}
	}
	const char* pb64cur = b64string;
	while (pb64cur < pb64end) {
		if (b64_alpha_to_data(*pb64cur, true) == 64) {
			fprintf(stderr, "错误: base64字符串中出现了非法字符\n");
			return false;
		}
		pb64cur++;
	}
	return true;
}


unsigned char* base64::b64decode(const char* b64string, int b64strlen, unsigned char* praw_data, unsigned int* prawdatalen)
{
	if (checkb64(b64string, b64strlen)) {
		const char* b64end = strchr(b64string, '=');
		if (!b64end || b64end > (b64string + b64strlen))
			b64end = b64string + b64strlen;
		const char* pcur = b64string;
		int padding = b64strlen + b64string - b64end;
		unsigned int rawdatalen = ((b64strlen - padding) / 4) * 3 ;
		if (padding == 2)//
			(rawdatalen)++;
		else if (padding == 1)
			(rawdatalen) += 2;
		if (*prawdatalen < rawdatalen)
			exit(-1);//error: insufficient space
		//praw_data = (unsigned char*)malloc(rawdatalen + 1);
		if (praw_data) {
			memset(praw_data, 0, rawdatalen);
			for (int i = 0; i < (b64strlen - padding) / 4; i++) {
				memset(praw_data + 3 * i, (b64_alpha_to_data(b64string[4 * i]) << 2) | (b64_alpha_to_data(b64string[4 * i + 1]) >> 4), 1);
				memset(praw_data + 3 * i + 1, (b64_alpha_to_data(b64string[4 * i + 1]) << 4) + (b64_alpha_to_data(b64string[4 * i + 2]) >> 2), 1);
				memset(praw_data + 3 * i + 2, (b64_alpha_to_data(b64string[4 * i + 2]) << 6) + (b64_alpha_to_data(b64string[4 * i + 3])), 1);
			}
			if (padding == 2) {
				memset(praw_data + rawdatalen - 1, (b64_alpha_to_data(*(b64end - 2)) << 2) | (b64_alpha_to_data(*(b64end - 1)) >> 4), 1);
			}
			else if (padding == 1) {
				memset(praw_data + rawdatalen - 2, (b64_alpha_to_data(*(b64end - 3)) << 2) | (b64_alpha_to_data(*(b64end - 2)) >> 4), 1);
				memset(praw_data + rawdatalen - 1, (b64_alpha_to_data(*(b64end - 2)) << 4) | (b64_alpha_to_data(*(b64end - 1)) >> 2), 1);
			}

			*prawdatalen = rawdatalen;
			return praw_data;
		}
	}
	else {
		exit(-1);
	}
}

char* base64::b64encode(const unsigned char* raw_data, int rawdatalen, char* pb64string, unsigned int* pb64strlen)
{
	int padding = rawdatalen % 3;
	unsigned int b64strlen = (rawdatalen / 3) * 4;
	if (padding)
		(b64strlen) += 4;
	if (*pb64strlen < b64strlen + 1)
		exit(-1);//error: insufficient space
	//pb64string = (char*)malloc(b64strlen + 1);
	if (pb64string) {
		memset(pb64string, 0, b64strlen + 1);
		for (int i = 0; i < (rawdatalen / 3); i++) {
			memset(pb64string + 4 * i, b64_alphabet_table[raw_data[3 * i] >> 2], 1);
			memset(pb64string + 4 * i + 1, b64_alphabet_table[(raw_data[3 * i] << 4 | raw_data[3 * i + 1] >> 4) & 0x3f], 1);
			memset(pb64string + 4 * i + 2, b64_alphabet_table[(raw_data[3 * i + 1] << 2 | raw_data[3 * i + 2] >> 6) & 0x3f], 1);
			memset(pb64string + 4 * i + 3, b64_alphabet_table[raw_data[3 * i + 2] & 0x3f], 1);
		}
		if (padding == 1) {
			memset(pb64string + b64strlen - 4, b64_alphabet_table[raw_data[rawdatalen - 1] >> 2], 1);
			memset(pb64string + b64strlen - 3, b64_alphabet_table[(raw_data[rawdatalen - 1] << 4) & 0x3f], 1);
			memset(pb64string + b64strlen - 2, '=', 1);
			memset(pb64string + b64strlen - 1, '=', 1);

		}
		else if (padding == 2) {
			memset(pb64string + b64strlen - 4, b64_alphabet_table[raw_data[rawdatalen - 2] >> 2], 1);
			memset(pb64string + b64strlen - 3, b64_alphabet_table[(raw_data[rawdatalen - 2] << 4 | raw_data[rawdatalen - 1] >> 4) & 0x3f], 1);
			memset(pb64string + b64strlen - 2, b64_alphabet_table[(raw_data[rawdatalen - 1] << 2) & 0x3f], 1);
			memset(pb64string + b64strlen - 1, '=', 1);
		}
		memset(pb64string + b64strlen, 0, 1);
		*pb64strlen = b64strlen;
		return pb64string;
	}
}
