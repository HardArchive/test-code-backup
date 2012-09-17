//
//	InjectThread
//
//
//	Inject into a running thread....
//	simulates CreateThread but without creating an extra thread!
//
//	Should work on ALL windows (9x/NT etc)
//
//	www.catch22.net
//
#include <windows.h>

// Function prototype (Same as callback function for CreateThread, but
// with no return-value)
typedef VOID (__stdcall * PINJCODE)(PVOID);

//
//	InjectThread
//
//	Arguments:
//		ppi	- pointer to PROCESS_INFORMATION structure (i.e. as returned
//			  by CreateProcess. Only need to know the proc/thread HANDLES, not IDs
//
//		pCode	 - pointer to routine to inject
//		nCodeLen - size (in bytes) of this function
//		pData	 - pointer to data to inject
//		nDataLen - size (in bytes) of data
//
//
//	[ ???? ???? ???? ???? ???? ]	  <--- Orig. ESP
//  [ EFlags				   ]
//  [ SegCs					   ]
//  [ Orig. EIP				   ]
//  [ Eax/Ebx/Ecx..			   ]
//  [ ...					   ]
//  [ POP ESP / POPAD / IRETD  ] <--\  <--- User func returns here (0xCF6158)
//                                  |
//  [						   ]    |
//  [		USER DATA          ]    |
//	[						   ]<-\ |
//								  | |
//	[	   -- padding --       ]  | |
//                                | |
//  [						   ]  | |
//  [		USER CODE          ]  | |
//	[						   ]  | |  <--- EIP points here
//  [ ESP (points to popad regs]  | |
//  [ Argument (ptr to data)   ] -/ |
//  [ Return address (cleanup) ]----/  <--- ESP points here
//
//
//	Once ESP and EIP have been properly setup, the thread
//  is resumed and execution begins at the start of the user-code
//  (on the thread's stack). Because the function is __stdcall,
//  when the function returns, the return address and function
//  argument are popped off the stack (using a RETN 4 instruction which
//  must be the last instruction in the user-supplied code) and
//  execution shifts to the first DWORD of the cleanup[] array, which contains
//  three (3) op-codes: POP ESP, POPAD, IRETD.
//
//  The first of these (POP ESP) takes the current DWORD stored at
//  the current location of the stack pointer (we carefully placed it there).
//  After the POP ESP executes, ESP now jumps up the stack to the second DWORD in
//  the cleanup[] array, which contains all the registers (in order) required
//  by the POPAD instruction (which executes next)
//
//  The last instruction to execute is IRETD - this pops it's return address
//  off the stack (the original value of EIP before we injected), and also
//  pops (and restores) the CS register and Flags register - once this is complete,
//  all registers are back in their initial state, ESP points back to the original
//  stack location, and execution resumes where it left off. Sweet :-)
//


BOOL InjectThread(PPROCESS_INFORMATION ppi, PINJCODE pCode, ULONG nCodeLen, PVOID pData, ULONG nDataLen)
{
	CONTEXT	context = { CONTEXT_FULL };

	ULONG	cleanup[12];
	ULONG	callenv[3];
	ULONG   stackptr;

	if(-1 == SuspendThread(ppi->hThread))
		return FALSE;

	GetThreadContext(ppi->hThread, &context);

	// construct cleanup frame
	cleanup[11] = context.EFlags;
	cleanup[10] = context.SegCs;
	cleanup[9]  = context.Eip;		// return address for IRETD
	cleanup[8]  = context.Eax;
	cleanup[7]  = context.Ecx;
	cleanup[6]  = context.Edx;
	cleanup[5]  = context.Ebx;
	cleanup[4]  = 0xffffffff;		// esp, ignored by POPAD
	cleanup[3]  = context.Ebp;
	cleanup[2]  = context.Esi;
	cleanup[1]  = context.Edi;
	cleanup[0]  = 0x00CF615C;		// POP ESP / POPAD / IRETD

	stackptr = context.Esp - sizeof(cleanup);
	WriteProcessMemory(ppi->hProcess, (PVOID)stackptr, cleanup, sizeof(cleanup), 0);

	// push user-supplied data
	stackptr -= nDataLen;
	WriteProcessMemory(ppi->hProcess, (PVOID)stackptr, pData, nDataLen, 0);

	// push user-supplied code
	stackptr= (stackptr - nCodeLen) & ~0x1f;
	WriteProcessMemory(ppi->hProcess, (PVOID)stackptr, pCode, nCodeLen, 0);

	// push code environment
	callenv[2] = context.Esp - sizeof(cleanup) + 4;			// ESP for executing cleanup
	callenv[1] = context.Esp - sizeof(cleanup) - nDataLen;	// Argument - pointer to user data
	callenv[0] = context.Esp - sizeof(cleanup);				// Return address - to cleanup code

	stackptr -= sizeof(callenv);
	WriteProcessMemory(ppi->hProcess, (PVOID)stackptr, callenv, sizeof(callenv), 0);

	// resume execution on thread's stack
	context.Esp = stackptr;
	context.Eip = stackptr + sizeof(callenv);

	FlushInstructionCache(ppi->hProcess, (PVOID)context.Eip, nCodeLen);
	SetThreadContext(ppi->hThread, &context);

	ResumeThread(ppi->hThread);

	return TRUE;
}