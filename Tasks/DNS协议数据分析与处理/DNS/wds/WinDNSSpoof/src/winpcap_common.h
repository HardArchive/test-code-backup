#ifndef _WINPCAP_COMMON_H_
#define _WINPCAP_COMMON_H_


/* Adapter parameters */
#define ADAPTER_NAMES_SIZE 4096
#define MAX_ADAPTER_NUM 10

/* Prototype */
int  open_adapter(LPADAPTER *);
void close_adapter(LPADAPTER);


#endif /* _WINPCAP_COMMON_H_ */