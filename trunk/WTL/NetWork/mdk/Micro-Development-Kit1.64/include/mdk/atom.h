#ifndef MDK_ATOM_H
#define MDK_ATOM_H

#include "FixLengthInt.h"

#ifdef WIN32
#include <windows.h>
#endif

namespace mdk
{

//����
inline uint32 AtomSelfAdd(void * var) 
{
#ifdef WIN32
  return InterlockedIncrement((long *)(var)); // NOLINT
#else
  return __sync_add_and_fetch((uint32 *)(var), 1); // NOLINT
#endif
}

//�Լ�
inline uint32 AtomSelfDec(void * var) 
{
#ifdef WIN32
  return InterlockedDecrement((long *)(var)); // NOLINT
#else
  return __sync_add_and_fetch((uint32 *)(var), -1); // NOLINT
#endif
}

//��һ��ֵ
inline uint32 AtomAdd(void * var, const uint32 value) 
{
#ifdef WIN32
  return InterlockedExchangeAdd((long *)(var), value); // NOLINT
#else
  return __sync_fetch_and_add((uint32 *)(var), value);  // NOLINT
#endif
}

//��һ��ֵ
inline uint32 AtomDec(void * var, int32 value) 
{
	value = value * -1;
#ifdef WIN32
	return InterlockedExchangeAdd((long *)(var), value); // NOLINT
#else
	return __sync_fetch_and_add((uint32 *)(var), value);  // NOLINT
#endif
}

//��ֵ,windows�·�����ֵ��linux�·��ؾ�ֵ
inline uint32 AtomSet(void * var, const uint32 value) 
{
#ifdef WIN32
	::InterlockedExchange((long *)(var), (long)(value)); // NOLINT
#else
	__sync_lock_test_and_set((uint32 *)(var), value);  // NOLINT
#endif
	return value;
}

//ȡֵ
inline uint32 AtomGet(void * var) 
{
#ifdef WIN32
  return InterlockedExchangeAdd((long *)(var), 0); // NOLINT
#else
  return __sync_fetch_and_add((uint32 *)(var), 0);  // NOLINT
#endif
}

} //namespace mdk

#endif //MDK_ATOM_H
