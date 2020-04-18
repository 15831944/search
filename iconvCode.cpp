#include "iconvCode.h"
#include "log.h"


CConvertStr::CConvertStr(conv_type type)
	:bFault(false)
{
	cd = (iconv_t)-1;
	cd = iconv_open(type_to(type), type_from(type));
	if (cd == (iconv_t)-1) {
		bFault = true;
	}

}

CConvertStr::~CConvertStr()
{
	iconv_close(cd);
}

const char* CConvertStr::type_from(const conv_type type)
{
	switch (type)
	{
	case conv_type::gbk_to_utf8: return "GBK";
	case conv_type::big5_to_utf8: return "BIG5";
	case conv_type::gb18030_to_utf8: return "GB18030";

	case conv_type::utf8_to_gbk:
	case conv_type::utf8_to_big5:
	case conv_type::utf8_to_gb18030: return "UTF-8";
	default: return "UNKOWN";
	}
}
const char* CConvertStr::type_to(const conv_type type)
{
	switch (type)
	{
	case conv_type::gbk_to_utf8:
	case conv_type::big5_to_utf8:
	case conv_type::gb18030_to_utf8: return "UTF-8//IGNORE";

	case conv_type::utf8_to_gbk: return "GBK//IGNORE";
	case conv_type::utf8_to_big5: return "BIG5//IGNORE";
	case conv_type::utf8_to_gb18030: return "GB18030//IGNORE";
	default: return "UNKOWN";
	}
}


bool CConvertStr::codeConvert(std::string & srcStr)
{	
	if (bFault) {
		return false;
	}
	string destStr;
	size_t size = static_cast<size_t>(srcStr.size());
	int out_lenght = size * 8;
	string strTempSrc = srcStr;	
	string strTempDest;
	strTempDest.resize(out_lenght);

	char* outbuf = reinterpret_cast<char*>(&strTempDest[0]);
	char* inbuf = reinterpret_cast<char*>(&strTempSrc[0]);
	char* inptr = inbuf;
	char* outptr = outbuf;
	size_t inlen = size;
	size_t outlen = out_lenght;
	size_t len = 0;
	while (inlen > 0) {
		size_t ret = iconv(cd, &inptr, &inlen, &outptr, &outlen);
		if (ret ==(size_t)-1) {
			return false;
		}
		len = out_lenght - outlen;
		destStr.append(outbuf, len);
		outbuf = outptr;
		out_lenght = outlen;		
	}
	srcStr = destStr;
	return true;
}


/*
iconv_t iconv_open(const char *tocode, const char *fromcode);
�˺���˵����Ҫ���������ֱ����ת��,tocode��Ŀ�����,fromcode��ԭ����,�ú�������һ��ת�����,��������������ʹ�á�
size_t iconv(iconv_t cd,char **inbuf,size_t *inbytesleft,char **outbuf,size_t *outbytesleft);
�˺�����inbuf�ж�ȡ�ַ�,ת���������outbuf��,inbytesleft���Լ�¼��δת�����ַ���,outbytesleft���Լ�¼��������ʣ��ռ�
int iconv_close(iconv_t cd);
*/



