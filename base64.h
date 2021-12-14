#pragma once
/*
author:SupreME
*/
class base64 {
	static const char b64_alphabet_table[];
	static unsigned char b64_alpha_to_data(const char c, bool return_when_error = false);
	static bool checkb64(const char* b64string, int b64strlen);
public:
	//decode a base64 string
	//[in]  b64string: a base64 string, with null terminated
	//[in]  b64strlen:length of this base64 string
	//[out] praw_data: a user-specified pace to receive the decoded raw data 
	//[in,out] prawdatalen: specify the size of praw_data , and  receive the size of decoded raw data
	static unsigned char* b64decode(
		const char* b64string,	//[in]  b64string: a base64 string,with null terminated
		int b64strlen,			//[in]  b64strlen:length of this base64 string
		unsigned char* praw_data, //[out] praw_data: a user-specified pace to receive the decoded raw data 
		unsigned int* prawdatalen);		//[in,out] prawdatalen: specify the size of praw_data , and  receive the size of decoded raw data


	//encode a raw data string to  base64 string
	//[in]  raw_data: a raw data string
	//[in]  rawdatalen:length of this base64 string
	//[in,out] pb64strlen: specify the size of pb64string , and  receive the strlen of encoded str (strlen of null-terminate string')
	static char* b64encode(
		const unsigned char* raw_data,//[in]  raw_data: a raw data string
		int rawdatalen,				  //[in]  rawdatalen:length of this base64 string
		char* pb64string,             //[out] pb64string:a ptr to receive the encoded base64 string
		unsigned int* pb64strlen);		//[in,out] pb64strlen: specify the size of pb64string , and  receive the size of encoded str (strlen of null-terminate string)

};


