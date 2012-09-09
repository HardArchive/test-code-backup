#include <ntddk.h>

#define IOCTL_LISTDRV_BUFFERED_IO			 \
        CTL_CODE(FILE_DEVICE_UNKNOWN,        \
                 0x802,                      \
                 METHOD_BUFFERED,            \
                 FILE_READ_DATA | FILE_WRITE_DATA)
#define  MAX_PATH 260

#define DEVICE_NAME_NT L"\\Device\\ListFileDrv"
#define DEVICE_NAME_DOS L"\\DosDevices\\ListFileDrv"

NTSYSAPI NTSTATUS NTAPI ObQueryNameString(/*POBJECT*/PVOID, PUNICODE_STRING, ULONG, PULONG);

void UnLoadDriver(PDRIVER_OBJECT  pDriverObject)
{    
    
    UNICODE_STRING usDosDeviceName;
    RtlInitUnicodeString(&usDosDeviceName, DEVICE_NAME_DOS );
    IoDeleteSymbolicLink(&usDosDeviceName);
    IoDeleteDevice(pDriverObject->DeviceObject);
}

typedef struct _FILE_INFO
{	
	USHORT tcDeviceName[260];
	USHORT tcFilleName[260];
    ULONG uType;
}FILE_INFO;

typedef struct _ADDRESS_INFO
{
	PVOID pAddress;
}ADDRESS_INFO;

NTSTATUS DoCopyOperation( PIO_STACK_LOCATION pStack, PIRP pIrp, unsigned int* uDataWritten )
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	try
	{
		PCHAR pInputBuffer = 0;
		PCHAR pOutputBuffer = 0;	
		ADDRESS_INFO *pstAddress = (ADDRESS_INFO*)pIrp->AssociatedIrp.SystemBuffer;
		PFILE_OBJECT pFileObject = pstAddress->pAddress;
		FILE_INFO* pHadnleInfo = (FILE_INFO*)pIrp->AssociatedIrp.SystemBuffer;
		ULONG uNeeded = 0;
		PUNICODE_STRING pstrDeviceName = 0;

           
		if(  pStack->Parameters.DeviceIoControl.InputBufferLength != sizeof(ADDRESS_INFO) ||
			 pStack->Parameters.DeviceIoControl.OutputBufferLength != sizeof(FILE_INFO))
		{
			DbgPrint("Buffer not correct");
			return STATUS_INVALID_PARAMETER;
		}	
		
		if(pFileObject->Type != IO_TYPE_FILE )
        {
            DbgPrint("File type not supported");
		    return STATUS_INVALID_PARAMETER;
        }
		/*if( pFileObject->DeviceObject->DeviceType != FILE_DEVICE_DISK && 
            pFileObject->DeviceObject->DeviceType != FILE_DEVICE_CD_ROM )*/

        if( pFileObject->DeviceObject->Type != IO_TYPE_DEVICE )
		{
                        
            /*DbgPrint( "Device type not supported" );*/
			return STATUS_INVALID_PARAMETER;
		}
        pHadnleInfo->uType = pFileObject->DeviceObject->DeviceType;
		RtlZeroMemory( pHadnleInfo->tcFilleName, MAX_PATH * 2 );
		RtlZeroMemory( pHadnleInfo->tcDeviceName, MAX_PATH * 2 );
		RtlCopyMemory( pHadnleInfo->tcFilleName, pFileObject->FileName.Buffer, pFileObject->FileName.Length );
		
    	pstrDeviceName = ExAllocatePool( PagedPool, ( MAX_PATH * 2 ) + ( 2 * sizeof( ULONG )));
		pstrDeviceName->MaximumLength = MAX_PATH * 2;
		pstrDeviceName->Length = MAX_PATH;
		if(NT_SUCCESS(ObQueryNameString(pFileObject->DeviceObject, pstrDeviceName, MAX_PATH * 2, &uNeeded )))
		{
            RtlCopyMemory( pHadnleInfo->tcDeviceName, pstrDeviceName->Buffer, pstrDeviceName->Length );
		}
        else
        {
            DbgPrint( "ObQueryNameString failed" );
        }
		ExFreePool( pstrDeviceName );
		*uDataWritten = sizeof( FILE_INFO );
	}
	except(EXCEPTION_EXECUTE_HANDLER)
	{
		ntStatus = STATUS_ACCESS_VIOLATION;
	}

	return ntStatus;
}

NTSTATUS ProcessIORequest( PDEVICE_OBJECT pDeviceObject, PIRP pIrp )
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	unsigned int uDataWritten = 0;
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	
	switch( pStack->MajorFunction )
	{
		case IRP_MJ_CREATE:
        case IRP_MJ_CLOSE:
			// nothing to do in this cases
			break;
		case IRP_MJ_DEVICE_CONTROL:
			{
				if( IOCTL_LISTDRV_BUFFERED_IO == pStack->Parameters.DeviceIoControl.IoControlCode )
				{
					ntStatus = DoCopyOperation( pStack, pIrp, &uDataWritten );
				}
				else
				{
					ntStatus = STATUS_NOT_IMPLEMENTED;
				}
				
			}
			break;
		default:
			ntStatus = STATUS_NOT_IMPLEMENTED;
	}
	pIrp->IoStatus.Status = ntStatus;
	pIrp->IoStatus.Information = uDataWritten;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return ntStatus;
}

NTSTATUS DriverEntry( PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath )
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	PDEVICE_OBJECT pDeviceObject = NULL;
	UNICODE_STRING usDriverName;
    UNICODE_STRING usDosDeviceName;
	RtlInitUnicodeString(&usDriverName, DEVICE_NAME_NT );
    RtlInitUnicodeString(&usDosDeviceName, DEVICE_NAME_DOS );
	ntStatus = IoCreateDevice( pDriverObject, 0, &usDriverName, FILE_DEVICE_UNKNOWN,
							   0, FALSE, &pDeviceObject );
	if (!NT_SUCCESS(ntStatus))
        return ntStatus;
	ntStatus = IoCreateSymbolicLink( &usDosDeviceName, &usDriverName );

	if (!NT_SUCCESS(ntStatus))
	{
		IoDeleteDevice(pDeviceObject);
        return ntStatus;
	}
	pDriverObject->MajorFunction[IRP_MJ_CREATE]          = ProcessIORequest;
    pDriverObject->MajorFunction[IRP_MJ_CLOSE]           = ProcessIORequest;
    pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]  = ProcessIORequest;
    pDriverObject->DriverUnload                          = UnLoadDriver;	

	return STATUS_SUCCESS;
}

