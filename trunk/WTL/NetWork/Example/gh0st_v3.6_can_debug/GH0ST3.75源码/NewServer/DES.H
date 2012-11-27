enum  {ENCRYPT,DECRYPT};

class DES  
{
public:
	bool Des_Go(char *Out, char *In, long datalen, const char *Key, int keylen, bool Type);
	DES();
	virtual ~DES();
	
};