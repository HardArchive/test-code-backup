#include "stdafx.h"
#include "CVodStreamCache.h"

Cache::Cache()
{
	filename = NULL;
	totalLen = 0;
	recvLen = 0;
}

Cache::~Cache()
{
	;
}

void Cache::setTotalLen(long totalLen)
{
	this->totalLen = totalLen;
}

void Cache::setRecvLen(long recvLen)
{
	this->recvLen = recvLen;
}

long Cache::getTotalLen()
{
	return totalLen;
}

long Cache::getRecvLen()
{
	return recvLen;
}