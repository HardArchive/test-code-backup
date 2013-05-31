//==================================================
// ShowSEHFrames - Matt Pietrek 1997
// Microsoft Systems Journal, January 1997
// FILE: ShowSEHFrames.CPP
// To compile: CL ShowSehFrames.CPP
//==================================================
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#pragma hdrstop

//----------------------------------------------------------------------------
// !!! WARNING !!!  This program only works with Visual C++, as the data
// structures being shown are specific to Visual C++.
//----------------------------------------------------------------------------

#ifndef _MSC_VER
#error Visual C++ Required (Visual C++ specific information is displayed)
#endif

//----------------------------------------------------------------------------
// Structure Definitions
//----------------------------------------------------------------------------

// The basic, OS defined exception frame

struct EXCEPTION_REGISTRATION
{
    EXCEPTION_REGISTRATION* prev;
    FARPROC                 handler;
};

// Data structure(s) pointed to by Visual C++ extended exception frame

struct scopetable_entry
{
    DWORD       previousTryLevel;
    FARPROC     lpfnFilter;
    FARPROC     lpfnHandler;
};

// The extended exception frame used by Visual C++

struct VC_EXCEPTION_REGISTRATION : EXCEPTION_REGISTRATION
{
    scopetable_entry *  scopetable;
    int                 trylevel;
    int                 _ebp;
};

//----------------------------------------------------------------------------
// Prototypes
//----------------------------------------------------------------------------

// __except_handler3 is a Visual C++ RTL function.  We want to refer to
// it in order to print it's address.  However, we need to prototype it since
// it doesn't appear in any header file.

extern "C" int _except_handler3(PEXCEPTION_RECORD, EXCEPTION_REGISTRATION *,
                                PCONTEXT, PEXCEPTION_RECORD);


//----------------------------------------------------------------------------
// Code
//----------------------------------------------------------------------------

//
// Display the information in one exception frame, along with its scopetable
//

void ShowSEHFrame( VC_EXCEPTION_REGISTRATION * pVCExcRec )
{
    printf( "Frame: %08X  Handler: %08X  Prev: %08X  Scopetable: %08X\n",
            pVCExcRec, pVCExcRec->handler, pVCExcRec->prev,
            pVCExcRec->scopetable );

    scopetable_entry * pScopeTableEntry = pVCExcRec->scopetable;

    for ( unsigned i = 0; i <= pVCExcRec->trylevel; i++ )
    {
        printf( "    scopetable[%u] PrevTryLevel: %08X  "
                "filter: %08X  __except: %08X\n", i,
                pScopeTableEntry->previousTryLevel,
                pScopeTableEntry->lpfnFilter,
                pScopeTableEntry->lpfnHandler );

        pScopeTableEntry++;
    }

    printf( "\n" );
}   

//
// Walk the linked list of frames, displaying each in turn
//

void WalkSEHFrames( void )
{
    VC_EXCEPTION_REGISTRATION * pVCExcRec;

    // Print out the location of the __except_handler3 function
    printf( "_except_handler3 is at address: %08X\n", _except_handler3 );
    printf( "\n" );

    // Get a pointer to the head of the chain at FS:[0]
    __asm   mov eax, FS:[0]
    __asm   mov [pVCExcRec], EAX

    // Walk the linked list of frames.  0xFFFFFFFF indicates the end of list
    while (  0xFFFFFFFF != (unsigned)pVCExcRec )
    {
        ShowSEHFrame( pVCExcRec );
        pVCExcRec = (VC_EXCEPTION_REGISTRATION *)(pVCExcRec->prev);
    }       
}

void Function1( void )
{
    // Set up 3 nested _try levels (thereby forcing 3 scopetable entries)
    _try
    {
        _try
        {
            _try
            {
                WalkSEHFrames();    // Now show all the exception frames
            }
            _except( EXCEPTION_CONTINUE_SEARCH )
            {
            }
        }
        _except( EXCEPTION_CONTINUE_SEARCH )
        {
        }
    }
    _except( EXCEPTION_CONTINUE_SEARCH )
    {
    }
}

int main()
{
    int i;

    // Use two (non-nested) _try blocks.  This causes two scopetable entries
    // to be generated for the function.

    _try
    {
        i = 0x1234;     // Do nothing in particular
    }
    _except( EXCEPTION_CONTINUE_SEARCH )
    {
        i = 0x4321;     // Do nothing (in reverse)
    }

    _try
    {
        Function1();    // Call a function that sets up more exception frames
    }
    _except( EXCEPTION_EXECUTE_HANDLER )
    {
        // Should never get here, since we aren't expecting an exception
        printf( "Caught Exception in main\n" );
    }

    return 0;
}
