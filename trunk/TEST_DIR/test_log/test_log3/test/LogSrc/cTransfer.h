/*
 * $Id: cTransfer.h,v 1.1.1.1 2001/11/22 15:18:55 hucl Exp $
 *
 * $Log: cTransfer.h,v $
 * Revision 1.1.1.1  2001/11/22 15:18:55  hucl
 * An initial version, and a great start!
 *
 */

#ifndef _HEAD_CTRANSFER_H
#define _HEAD_CTRANSFER_H

class CFTransfer {
private:

public:
    CFTransfer() {};
    virtual ~CFTransfer() {};

    virtual int fRead(char *, int) { return(0); };
    virtual int fWrite(const char *, int) { return(0); };
};

#endif // #ifndef _HEAD_CTRANSFER_H



