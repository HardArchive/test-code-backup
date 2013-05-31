//==================================================
// MYSEH2 - Matt Pietrek 1997
// Microsoft Systems Journal, January 1997
// FILE: MYSEH2.CPP
// To compile: CL MYSEH2.CPP
//==================================================
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

EXCEPTION_DISPOSITION
__cdecl
_except_handler(
    struct _EXCEPTION_RECORD *ExceptionRecord,
    void * EstablisherFrame,
    struct _CONTEXT *ContextRecord,
    void * DispatcherContext )
{
    printf( "Home Grown handler: Exception Code: %08X Exception Flags %X",
             ExceptionRecord->ExceptionCode, ExceptionRecord->ExceptionFlags );

    if ( ExceptionRecord->ExceptionFlags & 1 )
        printf( " EH_NONCONTINUABLE" );
    if ( ExceptionRecord->ExceptionFlags & 2 )
        printf( " EH_UNWINDING" );
    if ( ExceptionRecord->ExceptionFlags & 4 )
        printf( " EH_EXIT_UNWIND" );
    if ( ExceptionRecord->ExceptionFlags & 8 )
        printf( " EH_STACK_INVALID" );
    if ( ExceptionRecord->ExceptionFlags & 0x10 )
        printf( " EH_NESTED_CALL" );

    printf( "\n" );

    // Punt... We don't want to handle this... Let somebody else handle it
    return ExceptionContinueSearch;
}

void HomeGrownFrame( void )
{
    DWORD handler = (DWORD)_except_handler;

    __asm
    {                           // Build EXCEPTION_REGISTRATION record:
        push    handler         // Address of handler function
        push    FS:[0]          // Address of previous handler
        mov     FS:[0],ESP      // Install new EXECEPTION_REGISTRATION
    }

    *(PDWORD)0 = 0;             // Write to address 0 to cause a fault

    printf( "I should never get here!\n" );

    __asm
    {                           // Remove our EXECEPTION_REGISTRATION record
        mov     eax,[ESP]       // Get pointer to previous record
        mov     FS:[0], EAX     // Install previous record
        add     esp, 8          // Clean our EXECEPTION_REGISTRATION off stack
    }
}

int main()
{
    _try
    {
        HomeGrownFrame(); 
    }
    _except( EXCEPTION_EXECUTE_HANDLER )
    {
        printf( "Caught the exception in main()\n" );
    }

    return 0;
}

