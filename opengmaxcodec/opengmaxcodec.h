#if !defined(_GMAX_H_)
#define _GMAX_H_

#pragma warning(disable:4200)  // suppress nameless struct/union warning
#pragma warning(disable:4201)  // suppress nameless struct/union warning
#pragma warning(disable:4214)  // suppress bit field types other than int warning
#include <initguid.h>
#include <wdm.h>

#pragma warning(default:4200)
#pragma warning(default:4201)
#pragma warning(default:4214)
#include <wdf.h>

#include <portcls.h>

#include <acpiioct.h>
#include <ntstrsafe.h>

#include <stdint.h>

#include "spb.h"

#define JACKDESC_RGB(r, g, b) \
    ((COLORREF)((r << 16) | (g << 8) | (b)))

//
// String definitions
//

#define DRIVERNAME                 "ssm4567.sys: "

#define GMAX_POOL_TAG            (ULONG) 'B343'

#define true 1
#define false 0

#pragma pack(push,1)
typedef struct _IntcSSTArg
{
	int32_t chipModel;
	int32_t sstQuery;
	int32_t caller;
	int32_t querySize;

#ifdef __GNUC__
	char EndOfHeader[0];
#endif

	uint8_t deviceInD0;
#ifdef __GNUC__
	char EndOfPowerCfg[0];
#endif

	int32_t dword11;
	GUID guid;

#ifdef __GNUC__
	char EndOfGUID[0];
#endif
	uint8_t byte25;
	int32_t dword26;
	int32_t dword2A;
	int32_t dword2E;
	int32_t dword32;
	int32_t dword36;
	int32_t dword3A;
	int32_t dword3E;
	uint8_t byte42;
	uint8_t byte43;
	char padding[90]; //idk what this is for
}  IntcSSTArg, * PIntcSSTArg;
#pragma pack(pop)

typedef enum {
	CSAudioEndpointTypeDSP,
	CSAudioEndpointTypeSpeaker,
	CSAudioEndpointTypeHeadphone,
	CSAudioEndpointTypeMicArray,
	CSAudioEndpointTypeMicJack
} CSAudioEndpointType;

typedef enum {
	CSAudioEndpointRegister,
	CSAudioEndpointStart,
	CSAudioEndpointStop,
	CSAudioEndpointOverrideFormat
} CSAudioEndpointRequest;

typedef struct CSAUDIOFORMATOVERRIDE {
	UINT16 channels;
	UINT16 frequency;
	UINT16 bitsPerSample;
	UINT16 validBitsPerSample;
	BOOL force32BitOutputContainer;
} CsAudioFormatOverride;

typedef struct CSAUDIOARG {
	UINT32 argSz;
	CSAudioEndpointType endpointType;
	CSAudioEndpointRequest endpointRequest;
	union {
		CsAudioFormatOverride formatOverride;
	};
} CsAudioArg, * PCsAudioArg;

typedef struct _GMAX_CONTEXT
{

	WDFDEVICE FxDevice;

	WDFQUEUE ReportQueue;

	SPB_CONTEXT I2CContext;

	BOOLEAN SetUID;
	INT32 UID;

	UINT32 chipModel;

	BOOLEAN DevicePoweredOn;
	INT8 IntcSSTStatus;

	WDFWORKITEM IntcSSTWorkItem;
	PCALLBACK_OBJECT IntcSSTHwMultiCodecCallback;
	PVOID IntcSSTCallbackObj;

	PCALLBACK_OBJECT CSAudioAPICallback;
	PVOID CSAudioAPICallbackObj;

	BOOL CSAudioManaged;

	IntcSSTArg sstArgTemp;

} GMAX_CONTEXT, *PGMAX_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(GMAX_CONTEXT, GetDeviceContext)

//
// Function definitions
//

DRIVER_INITIALIZE DriverEntry;

EVT_WDF_DRIVER_UNLOAD GmaxDriverUnload;

EVT_WDF_DRIVER_DEVICE_ADD GmaxEvtDeviceAdd;

EVT_WDFDEVICE_WDM_IRP_PREPROCESS GmaxEvtWdmPreprocessMnQueryId;

EVT_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL GmaxEvtInternalDeviceControl;

//
// Helper macros
//

#define DEBUG_LEVEL_ERROR   1
#define DEBUG_LEVEL_INFO    2
#define DEBUG_LEVEL_VERBOSE 3

#define DBG_INIT  1
#define DBG_PNP   2
#define DBG_IOCTL 4

#if 1
#define GmaxPrint(dbglevel, dbgcatagory, fmt, ...) {          \
    if (GmaxDebugLevel >= dbglevel &&                         \
        (GmaxDebugCatagories && dbgcatagory))                 \
	    {                                                           \
        DbgPrint(DRIVERNAME);                                   \
        DbgPrint(fmt, __VA_ARGS__);                             \
	    }                                                           \
}
#else
#define GmaxPrint(dbglevel, fmt, ...) {                       \
}
#endif

#endif