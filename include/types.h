#ifndef EFI_ELF_TYPES
#define EFI_ELF_TYPES

#ifdef WIN32
# error "Why? Just Why? Remove this if you wanna do it anyways"
#endif // WIN32

typedef unsigned long       uint64;
typedef long                int64;
typedef unsigned int        uint32;
typedef int                 int32;
typedef unsigned short      uint16;
typedef short               int16;
typedef unsigned char       uint8;
typedef char                int8;

#define STRUCT(NAME, MEMBERS)   \
	typedef _##NAME {           \
	MEMBERS;                    \
} NAME;


typedef void* efi_handle_t;
typedef void* efi_event_t;
// stdcall result
typedef uint64 efi_status;
// EFI Logical Block Addressing
typedef uint64 efi_lba_t;
typedef uint64 efi_tpl_t;

// Argument's value passed to function
#define input
// Argument takes out value from function
#define output
// Argument is optional
#define optional

#ifndef __cplusplus
typedef char bool;
#define true  (uint8) 1
#define false (uint8) 0
#endif // !__cplusplus

#define NULL (void *) 0


/**
 * Full description: https://uefi.org/specs/UEFI/2.10/Apx_A_GUID_and_Time_Formats.html
 * 
 *         Mnemonic        |  Byte Offset  |  Byte Length  |           Description
 *         TimeLow         |      0        |       4       | The low field of the timestamp.
 *         TimeMid         |      4        |       2       | The middle field of the timestamp.
 *   TimeHighAndVersion    |      6        |       2       | The high field of the timestamp multiplexed with the version number.
 * ClockSeqHighAndReserved |      8        |       1       | The high field of the clock sequence multiplexed with the variant.
 *      ClockSeqLow        |      9        |       1       | The low field of the clock sequence.
 *          Node           |     10        |       6       | The spatially unique node identifier
 *
 */
typedef struct {
	// TimeLow
    uint32  data1;
	// TimeMid
    uint16  data2;
	// TimeHighAndVersion
    uint16  data3;
	// ClockSeqHighAndReserved, ClockSeqLow, Node
	//
	//         0                    1        2..7
    uint8   data4[8];
} efi_guid_t;

typedef struct {
	uint16 year;
	uint8  month;
	uint8  day;
	uint8  hour;
	uint8  minute;
	uint8  second;
	uint8  pad1;
	// 0 - 999,999,999
	uint32 nanosecond;
	// -1440 to 1440 or 2047
	uint16 timezone;
	uint8  daylight;
	uint8  pad2;
} efi_time_t;

#define EFI_TIME_DAYLIGHT_ADJUST    0x01
#define EFI_TIME_DAYLIGHT_IN        0x02

#define EFI_UNSPECIFIED_TIMEZONE    0x07FF

// Format
// Byte.Byte.Byte.Byte
typedef struct {
	uint8 addr[4];
} efi_ipv4_addr_t;

// Short:Short:Short:Short:Short:Short:Short:Short
typedef struct {
	uint8 addr[16];
} efi_ipv6_addr_t;

typedef struct {
	uint8 addr[32];
} efi_mac_addr_t;


typedef struct {
    uint32 received_queue_timeout_value;
    uint32 transmit_queue_timeout_value;
    uint16 protocol_type_filter;
    bool enable_unicast_receive;
    bool enable_multicast_receive;
    bool enable_broadcast_receive;
    bool enable_promiscuous_receive;
    bool flush_queues_on_reset;
    bool enable_receive_timestamps;
    bool disable_background_polling;
} efi_managed_network_config_data_t;


typedef uint64 efi_physical_addr_t;
typedef uint64 efi_virtual_addr_t;

typedef enum {
    ALLOCATE_ANY_PAGES,
    ALLOCATE_MAX_ADDRESS,
    ALLOCATE_ADDRESS,
    MAX_ALLOCATE_TYPE
} efi_allocate_type_t;

typedef enum {
	EFI_RESERVED_MEMORY_TYPE,
	EFI_LOADER_CODE,
	EFI_LOADER_DATA,
	EFI_BOOT_SERVICES_CODE,
	EFI_BOOT_SERVICES_DATA,
	EFI_RUNTIME_SERVICES_CODE,
	EFI_RUNTIME_SERVICES_DATA,
	EFI_CONVENTIONAL_MEMORY,
	EFI_UNUSABLE_MEMORY,
	EFI_ACPI_RECLAIM_MEMORY,
	EFI_ACPI_MEMORY_NVS,
	EFI_MEMORY_MAPPED_IO,
	EFI_MEMORY_MAPPED_IO_PORT_SPACE,
	EFI_PAL_CODE,
	EFI_MAX_MEMORY_TYPE
} efi_memory_type_t;

// possible caching types for the memory range
#define EFI_MEMORY_UC           0x0000000000000001
#define EFI_MEMORY_WC           0x0000000000000002
#define EFI_MEMORY_WT           0x0000000000000004
#define EFI_MEMORY_WB           0x0000000000000008
#define EFI_MEMORY_UCE          0x0000000000000010  

// physical memory protection on range 
#define EFI_MEMORY_WP           0x0000000000001000
#define EFI_MEMORY_RP           0x0000000000002000
#define EFI_MEMORY_XP           0x0000000000004000

// range requires a runtime mapping
#define EFI_MEMORY_RUNTIME      0x8000000000000000
//
// International Language
//
typedef struct {
    uint32              type;           // Field size is 32 bits followed by 32 bit pad
    uint32              pad;
    efi_physical_addr_t physicalStart;  // Field size is 64 bits
    efi_virtual_addr_t  virtualStart;   // Field size is 64 bits
    uint64              numberOfPages;  // Field size is 64 bits
    uint64              attribute;      // Field size is 64 bits
} efi_memory_descriptor_t;

typedef uint8   ISO_639_2;
#define ISO_639_2_ENTRY_SIZE    3

//
//
//

#define EFI_PAGE_SIZE   4096
#define EFI_PAGE_MASK   0xFFF
#define EFI_PAGE_SHIFT  12

#define EFI_SIZE_TO_PAGES(a)  \
    ( ((a) >> EFI_PAGE_SHIFT) + ((a) & EFI_PAGE_MASK ? 1 : 0) )

#define EFI_OS_INDICATIONS_BOOT_TO_FW_UI        0x0000000000000001
#define EFI_OS_INDICATIONS_TIMESTAMP_REVOCATION 0x0000000000000002
#define EFI_OS_INDICATIONS_FILE_CAPSULE_DELIVERY_SUPPORTED \
                                                0x0000000000000004
#define EFI_OS_INDICATIONS_FMP_CAPSULE_SUPPORTED \
                                                0x0000000000000008
#define EFI_OS_INDICATIONS_CAPSULE_RESULT_VAR_SUPPORTED \
                                                0x0000000000000010

#endif // !EFI_ELF_TYPES
