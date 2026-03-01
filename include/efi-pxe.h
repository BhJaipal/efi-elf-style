#ifndef EFI_ELF_PXE
#define EFI_ELF_PXE

#define PXE_INTEL_ORDER         1   // Intel order

#define PXE_uint64_SUPPORT          1   // uint64 supported

#define PXE_BUSTYPE(a,b,c,d)            \
((((pxe_uint32)(d) & 0xFF) << 24) | \
(((pxe_uint32)(c) & 0xFF) << 16) |  \
(((pxe_uint32)(b) & 0xFF) << 8) |       \
((pxe_uint32)(a) & 0xFF))

// UNDI ROM ID and devive ID signature
#define PXE_BUSTYPE_PXE         PXE_BUSTYPE('!', 'P', 'X', 'E')

// BUS ROM ID signatures
#define PXE_BUSTYPE_PCI         PXE_BUSTYPE('P', 'C', 'I', 'R')
#define PXE_BUSTYPE_PC_CARD     PXE_BUSTYPE('P', 'C', 'C', 'R')
#define PXE_BUSTYPE_USB         PXE_BUSTYPE('U', 'S', 'B', 'R')
#define PXE_BUSTYPE_1394        PXE_BUSTYPE('1', '3', '9', '4')

#define PXE_SWAP_uint16(n)          \
((((pxe_uint16)(n) & 0x00FF) << 8) |    \
(((pxe_uint16)(n) & 0xFF00) >> 8))

#define PXE_SWAP_uint32(n)              \
((((pxe_uint32)(n) & 0x000000FF) << 24) |   \
(((pxe_uint32)(n) & 0x0000FF00) << 8) |     \
(((pxe_uint32)(n) & 0x00FF0000) >> 8) |     \
(((pxe_uint32)(n) & 0xFF000000) >> 24))

#define PXE_SWAP_uint64(n)                  \
((((pxe_uint64)(n) & 0x00000000000000FF) << 56) |   \
(((pxe_uint64)(n) & 0x000000000000FF00) << 40) |    \
(((pxe_uint64)(n) & 0x0000000000FF0000) << 24) |    \
(((pxe_uint64)(n) & 0x00000000FF000000) << 8) | \
(((pxe_uint64)(n) & 0x000000FF00000000) >> 8) | \
(((pxe_uint64)(n) & 0x0000FF0000000000) >> 24) |    \
(((pxe_uint64)(n) & 0x00FF000000000000) >> 40) |    \
(((pxe_uint64)(n) & 0xFF00000000000000) >> 56))


#define PXE_DELAY_MILLISECOND                   1000
#define PXE_DELAY_SECOND                    1000000
#define PXE_IO_READ                     0
#define PXE_IO_WRITE                        1
#define PXE_MEM_READ                        2
#define PXE_MEM_WRITE                       4

typedef long unsigned  pxe_uint64;
typedef long unsigned  uint64;
typedef unsigned       pxe_uint32;
typedef unsigned short pxe_uint16;
typedef unsigned char  pxe_uint8;
typedef unsigned       pxe_uint32;
typedef pxe_uint16 pxe_opcode_t;
typedef pxe_uint16 pxe_opflags_t;
typedef pxe_uint16 pxe_statcode_t;
typedef pxe_uint16 pxe_statflags_t;
typedef pxe_uint16 pxe_control_t;
typedef pxe_uint16 pxe_statflags_t;

typedef pxe_uint32 pxe_ipv4_t;

typedef pxe_uint32 PXE_IPV6[4];
#define PXE_MAC_LENGTH 32
typedef pxe_uint8 pxe_mac_addr_t[PXE_MAC_LENGTH];

typedef pxe_uint8 pxe_iftype_t;
typedef pxe_uint16 pxe_media_protocol_t;
typedef pxe_uint8 pxe_frame_type_t;

typedef struct s_pxe_hw_undi {
	pxe_uint32 Signature;       // PXE_ROMID_SIGNATURE
	pxe_uint8 Len;          // sizeof(PXE_HW_UNDI)
	pxe_uint8 Fudge;            // makes 8-bit cksum equal zero
	pxe_uint8 Rev;          // PXE_ROMID_REV
	pxe_uint8 IFcnt;            // physical connector count
	pxe_uint8 MajorVer;         // PXE_ROMID_MAJORVER
	pxe_uint8 MinorVer;         // PXE_ROMID_MINORVER
	pxe_uint16 reserved;        // zero, not used
	pxe_uint32 Implementation;      // implementation flags
	// reserved             // vendor use
	// pxe_uint32 Status;       // status port
	// pxe_uint32 Command;      // command port
	// pxe_uint64 CDBaddr;      // CDB address port
} pxe_hw_undi_t;


typedef union pxe_device {
	// PCI and PC Card NICs are both identified using bus, device
	// and function numbers.  For PC Card, this may require PC
	// Card services to be loaded in the BIOS or preboot
	// environment.
	struct {
		// See S/W UNDI ROMID structure definition for PCI and
		// PCC BusType definitions.
		pxe_uint32 BusType;

		// Bus, device & function numbers that locate this device.
		pxe_uint16 Bus;
		pxe_uint8 Device;
		pxe_uint8 Function;
	} PCI, PCC;

	// %%TBD - More information is needed about enumerating
	// USB and 1394 devices.
	struct {
		pxe_uint32 BusType;
		pxe_uint32 tdb;
	} USB, _1394;
} pxe_device;

// cpb and db definitions

#define MAX_PCI_CONFIG_LEN 64   // # of dwords
#define MAX_EEPROM_LEN 128       // #of dwords
#define MAX_XMIT_BUFFERS    32  // recycling Q length for xmit_done
#define MAX_MCAST_ADDRESS_CNT 8

typedef struct {
    // pxe_VOID Delay(PXE_uint64 microseconds);
    //
    // UNDI will never request a delay smaller than 10 microseconds
    // and will always request delays in increments of 10 microseconds.
    // The Delay() CallBack routine must delay between n and n + 10 
    // microseconds before returning control to the UNDI.
    //
    // This field cannot be set to zero.
    pxe_uint64 Delay;

    // pxe_VOID Block(PXE_uint32 enable);
    //
    // UNDI may need to block multi-threaded/multi-processor access to
    // critical code sections when programming or accessing the network
    // device.  To this end, a blocking service is needed by the UNDI.
    // When UNDI needs a block, it will call Block() passing a non-zero
    // value.  When UNDI no longer needs a block, it will call Block()
    // with a zero value.  When called, if the Block() is already enabled,
    // do not return control to the UNDI until the previous Block() is
    // disabled.
    //
    // This field cannot be set to zero.
    pxe_uint64 Block;

    // pxe_VOID Virt2Phys(PXE_uint64 virtual, PXE_uint64 physical_ptr);
    //
    // UNDI will pass the virtual address of a buffer and the virtual
    // address of a 64-bit physical buffer.  Convert the virtual address
    // to a physical address and write the result to the physical address
    // buffer.  If virtual and physical addresses are the same, just
    // copy the virtual address to the physical address buffer.
    //
    // This field can be set to zero if virtual and physical addresses 
    // are equal.
    pxe_uint64 Virt2Phys;
    // pxe_VOID Mem_IO(PXE_uint8 read_write, PXE_uint8 len, PXE_uint64 port, 
    //              pxe_uint64 buf_addr);
    //
    // UNDI will read or write the device io space using this call back 
    // function. It passes the number of bytes as the len parameter and it 
    // will be either 1,2,4 or 8.
    //
    // This field can not be set to zero.
    pxe_uint64 Mem_IO;
} pxe_cpb_start_t;

typedef struct {
    // Minimum length of locked memory buffer that must be given to
    // the Initialize command. Giving UNDI more memory will generally
    // give better performance.
    //
    // If MemoryRequired is zero, the UNDI does not need and will not
    // use system memory to receive and transmit packets.
    pxe_uint32 MemoryRequired;

    // Maximum frame data length for Tx/Rx excluding the media header.
    pxe_uint32 FrameDataLen;

    // Supported link speeds are in units of mega bits.  Common ethernet
    // values are 10, 100 and 1000.  Unused LinkSpeeds[] entries are zero
    // filled.
    pxe_uint32 LinkSpeeds[4];

    // Number of non-volatile storage items.
    pxe_uint32 NvCount;

    // Width of non-volatile storage item in bytes.  0, 1, 2 or 4
    pxe_uint16 NvWidth;

    // Media header length.  This is the typical media header length for
    // this UNDI.  This information is needed when allocating receive
    // and transmit buffers.
    pxe_uint16 MediaHeaderLen;

    // Number of bytes in the NIC hardware (MAC) address.
    pxe_uint16 HWaddrLen;

    // Maximum number of multicast MAC addresses in the multicast
    // MAC address filter list.
    pxe_uint16 MCastFilterCnt;

    // Default number and size of transmit and receive buffers that will 
    // be allocated by the UNDI.  If MemoryRequired is non-zero, this 
    // allocation will come out of the memory buffer given to the Initialize 
    // command.  If MemoryRequired is zero, this allocation will come out of 
    // memory on the NIC.
    pxe_uint16 TxBufCnt;
    pxe_uint16 TxBufSize;
    pxe_uint16 RxBufCnt;
    pxe_uint16 RxBufSize;

    // Hardware interface types defined in the Assigned Numbers RFC
    // and used in DHCP and ARP packets.
    // See the pxe_iftype_t typedef and PXE_IFTYPE_xxx macros.
    pxe_uint8 IFtype;

    // Supported duplex.  See pxe_DUPLEX_xxxxx #defines below.
    pxe_uint8 Duplex;

    // Supported loopback options.  See pxe_LOOPBACK_xxxxx #defines below.
    pxe_uint8 LoopBack;
} pxe_db_get_init_info_t;

#define PXE_MAX_TXRX_UNIT_ETHER             1500

#define PXE_HWADDR_LEN_ETHER                    0x0006
#define PXE_MAC_HEADER_LEN_ETHER                0x000E

#define PXE_DUPLEX_ENABLE_FULL_SUPPORTED            1
#define PXE_DUPLEX_FORCE_FULL_SUPPORTED         2

#define PXE_LOOPBACK_INTERNAL_SUPPORTED         1
#define PXE_LOOPBACK_EXTERNAL_SUPPORTED         2


typedef struct {
    //
    // This is the flag field for the pxe_DB_GET_CONFIG_INFO union.
    // For PCI bus devices, this field is set to pxe_BUSTYPE_PCI.
    //
    pxe_uint32 BusType;

    //
    // This identifies the PCI network device that this UNDI interface
    // is bound to.
    //
    pxe_uint16 Bus;
    pxe_uint8 Device;
    pxe_uint8 Function;

    //
    // This is a copy of the PCI configuration space for this 
    // network device.
    //
    union {
        pxe_uint8 Byte[256];
        pxe_uint16 Word[128];
        pxe_uint32 Dword[64];
    } Config;
} pxe_pci_config_info_t;


typedef struct {
    //
    // This is the flag field for the pxe_DB_GET_CONFIG_INFO union.
    // For PCC bus devices, this field is set to pxe_BUSTYPE_PCC.
    //
    pxe_uint32 BusType;
    
    //
    // This identifies the PCC network device that this UNDI interface
    // is bound to.
    //
    pxe_uint16 Bus;
    pxe_uint8 Device;
    pxe_uint8 Function;

    //
    // This is a copy of the PCC configuration space for this 
    // network device.
    //
    union {
        pxe_uint8 Byte[256];
        pxe_uint16 Word[128];
        pxe_uint32 Dword[64];
    } Config;
} pxe_pcc_config_info_t;


typedef struct {
    pxe_uint32 BusType;
    // %%TBD What should we return here...
} pxe_usb_config_info_t;


typedef struct {
    pxe_uint32 BusType;
    // %%TBD What should we return here...
} pxe_1394_config_info_t;


typedef union {
    pxe_pci_config_info_t pci;
    pxe_pcc_config_info_t pcc;
    pxe_usb_config_info_t usb;
    pxe_1394_config_info_t _1394;
} pxe_db_get_config_info_t;


typedef struct {
    // Address of first (lowest) byte of the memory buffer.  This buffer must
    // be in contiguous physical memory and cannot be swapped out.  The UNDI
    // will be using this for transmit and receive buffering.
    pxe_uint64 MemoryAddr;

    // MemoryLength must be greater than or equal to MemoryRequired
    // returned by the Get Init Info command.
    pxe_uint32 MemoryLength;

    // Desired link speed in Mbit/sec.  Common ethernet values are 10, 100
    // and 1000.  Setting a value of zero will auto-detect and/or use the
    // default link speed (operation depends on UNDI/NIC functionality).
    pxe_uint32 LinkSpeed;

    // Suggested number and size of receive and transmit buffers to
    // allocate.  If MemoryAddr and MemoryLength are non-zero, this
    // allocation comes out of the supplied memory buffer.  If MemoryAddr 
    // and MemoryLength are zero, this allocation comes out of memory
    // on the NIC.
    //
    // If these fields are set to zero, the UNDI will allocate buffer
    // counts and sizes as it sees fit.
    pxe_uint16 TxBufCnt;
    pxe_uint16 TxBufSize;
    pxe_uint16 RxBufCnt;
    pxe_uint16 RxBufSize;

    // The following configuration parameters are optional and must be zero 
    // to use the default values.
    pxe_uint8 Duplex; 

    pxe_uint8 LoopBack;
} pxe_cpb_initialize_t;


#define PXE_DUPLEX_DEFAULT                  0x00
#define PXE_FORCE_FULL_DUPLEX                   0x01
#define PXE_ENABLE_FULL_DUPLEX              0x02

#define LOOPBACK_NORMAL 0
#define LOOPBACK_INTERNAL 1
#define LOOPBACK_EXTERNAL 2


typedef struct {
	// Actual amount of memory used from the supplied memory buffer.  This
	// may be less that the amount of memory suppllied and may be zero if
	// the UNDI and network device do not use external memory buffers.
	//
	// Memory used by the UNDI and network device is allocated from the 
	// lowest memory buffer address.
	pxe_uint32 MemoryUsed;

	// Actual number and size of receive and transmit buffers that were
	// allocated.
	pxe_uint16 TxBufCnt;
	pxe_uint16 TxBufSize;
	pxe_uint16 RxBufCnt;
	pxe_uint16 RxBufSize;
} pxe_DB_initialize_t;



typedef struct {
	pxe_opcode_t OpCode;
	pxe_opflags_t OpFlags;
	pxe_uint16 CPBsize;
	pxe_uint16 DBsize;
	uint64 CPBaddr;
	uint64 DBaddr;
	pxe_statcode_t StatCode;
	pxe_statflags_t StatFlags;
	pxe_uint16 IFnum;
	pxe_control_t Control;
} pxe_cdb_t;


typedef union {
	PXE_IPV6 IPv6;
	pxe_ipv4_t IPv4;
} pxe_ip_addr_t;

typedef struct {
    // List of multicast MAC addresses.  This list, if present, will
    // replace the existing multicast MAC address filter list.
    pxe_mac_addr_t MCastList[MAX_MCAST_ADDRESS_CNT];
} pxe_cpb_receive_filters_t;


typedef struct {
    // Filtered multicast MAC address list.
    pxe_mac_addr_t MCastList[MAX_MCAST_ADDRESS_CNT];
} pxe_db_receive_filters_t;


typedef struct {
    // If supplied and supported, the current station MAC address
    // will be changed.
    pxe_mac_addr_t StationAddr;
} pxe_cpb_station_address_t;


typedef struct {
    // Current station MAC address.
    pxe_mac_addr_t StationAddr;

    // Station broadcast MAC address.
    pxe_mac_addr_t BroadcastAddr;

    // Permanent station MAC address.
    pxe_mac_addr_t PermanentAddr;
} pxe_db_station_address_t;


typedef struct {
    // Bit field identifying what statistic data is collected by the 
    // UNDI/NIC.
    // If bit 0x00 is set, Data[0x00] is collected.
    // If bit 0x01 is set, Data[0x01] is collected.
    // If bit 0x20 is set, Data[0x20] is collected.
    // If bit 0x21 is set, Data[0x21] is collected.
    // Etc.
    pxe_uint64 Supported;

    // Statistic data.
    pxe_uint64 Data[64];
} pxe_db_statistics_t;

typedef struct {
	pxe_uint32 Signature;       // PXE_ROMID_SIGNATURE
	pxe_uint8 Len;          // sizeof(PXE_SW_UNDI)
	pxe_uint8 Fudge;            // makes 8-bit cksum zero
	pxe_uint8 Rev;          // PXE_ROMID_REV
	pxe_uint8 IFcnt;            // physical connector count
	pxe_uint8 MajorVer;         // PXE_ROMID_MAJORVER
	pxe_uint8 MinorVer;         // PXE_ROMID_MINORVER
	pxe_uint16 reserved1;       // zero, not used
	pxe_uint32 Implementation;      // Implementation flags
	pxe_uint64 EntryPoint;      // API entry point
	pxe_uint8 reserved2[3];     // zero, not used
	pxe_uint8 BusCnt;           // number of bustypes supported
	pxe_uint32 BusType[1];      // list of supported bustypes
} pxe_sw_undi_t;

typedef union {
	pxe_hw_undi_t hw;
	pxe_sw_undi_t sw;
} pxe_undi_t;

typedef struct {
    // Multicast IP address to be converted to multicast MAC address.
    pxe_ip_addr_t IP;
} pxe_cpb_mcast_ip_to_mac_t;


typedef struct {
    // Multicast MAC address.
    pxe_mac_addr_t MAC;
} pxe_db_mcast_ip_to_mac_t;


typedef struct {
    // NvData item list.  Only items in this list will be updated.
    struct {
        //  Non-volatile storage address to be changed.
        pxe_uint32 Addr;

        // Data item to write into above storage address.
        union {
            pxe_uint8 Byte;
            pxe_uint16 Word;
            pxe_uint32 Dword;
        } Data;
    } Item[MAX_EEPROM_LEN];
} pxe_cpb_nvdata_sparse_t;


// When using bulk update, the size of the CPB structure must be
// the same size as the non-volatile NIC storage.
typedef union {
    // Array of byte-wide data items.
    pxe_uint8 Byte[MAX_EEPROM_LEN << 2];

    // Array of word-wide data items.
    pxe_uint16 Word[MAX_EEPROM_LEN << 1];

    // Array of dword-wide data items.
    pxe_uint32 Dword[MAX_EEPROM_LEN];
} pxe_cpb_nvdata_bulk_t;

typedef struct {

    // Arrays of data items from non-volatile storage.

    union {
        // Array of byte-wide data items.
        pxe_uint8 Byte[MAX_EEPROM_LEN << 2];

        // Array of word-wide data items.
        pxe_uint16 Word[MAX_EEPROM_LEN << 1];

        // Array of dword-wide data items.
        pxe_uint32 Dword[MAX_EEPROM_LEN];
    } Data;
} pxe_db_nvdata_t;


typedef struct {
    // Length of next receive frame (header + data).  If this is zero,
    // there is no next receive frame available.
    pxe_uint32 RxFrameLen;

    // Reserved, set to zero.
    pxe_uint32 reserved;

    //  Addresses of transmitted buffers that need to be recycled.
    pxe_uint64 TxBuffer[MAX_XMIT_BUFFERS];
} pxe_db_get_status_t;



typedef struct {
    // Source and destination MAC addresses.  These will be copied into
    // the media header without doing byte swapping.
    pxe_mac_addr_t SrcAddr;
    pxe_mac_addr_t DestAddr;

    // Address of first byte of media header.  The first byte of packet data
    // follows the last byte of the media header.
    pxe_uint64 MediaHeader;

    // Length of packet data in bytes (not including the media header).
    pxe_uint32 PacketLen;

    // Protocol type.  This will be copied into the media header without
    // doing byte swapping.  Protocol type numbers can be obtained from
    // the Assigned Numbers RFC 1700.
    pxe_uint16 Protocol;

    // Length of the media header in bytes.
    pxe_uint16 MediaHeaderLen;
} pxe_cpb_fill_header_t;


#define PXE_PROTOCOL_ETHERNET_IP                0x0800
#define PXE_PROTOCOL_ETHERNET_ARP               0x0806
#define MAX_XMIT_FRAGMENTS 16

typedef struct {
    // Source and destination MAC addresses.  These will be copied into
    // the media header without doing byte swapping.
    pxe_mac_addr_t SrcAddr;
    pxe_mac_addr_t DestAddr;

    // Length of packet data in bytes (not including the media header).
    pxe_uint32 PacketLen;

    // Protocol type.  This will be copied into the media header without
    // doing byte swapping.  Protocol type numbers can be obtained from
    // the Assigned Numbers RFC 1700.
    pxe_media_protocol_t Protocol;

    // Length of the media header in bytes.
    pxe_uint16 MediaHeaderLen;

    // Number of packet fragment descriptors.
    pxe_uint16 FragCnt;

    // Reserved, must be set to zero.
    pxe_uint16 reserved;

    // Array of packet fragment descriptors.  The first byte of the media
    // header is the first byte of the first fragment.
    struct {
        // Address of this packet fragment.
        pxe_uint64 FragAddr;

        // Length of this packet fragment.
        pxe_uint32 FragLen;

        // Reserved, must be set to zero.
        pxe_uint32 reserved;
    } FragDesc[MAX_XMIT_FRAGMENTS];
} pxe_cpb_fill_header_fragmented_t;



typedef struct {
    // Address of first byte of frame buffer.  This is also the first byte
    // of the media header.
    pxe_uint64 FrameAddr;

    // Length of the data portion of the frame buffer in bytes.  Do not
    // include the length of the media header.
    pxe_uint32 DataLen;

    // Length of the media header in bytes.
    pxe_uint16 MediaheaderLen;

    // Reserved, must be zero.
    pxe_uint16 reserved;
} pxe_cpb_transmit_t;



typedef struct {
    // Length of packet data in bytes (not including the media header).
    pxe_uint32 FrameLen;

    // Length of the media header in bytes.
    pxe_uint16 MediaheaderLen;

    // Number of packet fragment descriptors.
    pxe_uint16 FragCnt;

    // Array of frame fragment descriptors.  The first byte of the first
    // fragment is also the first byte of the media header.
    struct {
        // Address of this frame fragment.
        pxe_uint64 FragAddr;

        // Length of this frame fragment.
        pxe_uint32 FragLen;

        // Reserved, must be set to zero.
        pxe_uint32 reserved;
    } FragDesc[MAX_XMIT_FRAGMENTS];
} pxe_cpb_transmit_fragments_t;


typedef struct {
    // Address of first byte of receive buffer.  This is also the first byte
    // of the frame header.
    pxe_uint64 BufferAddr;

    // Length of receive buffer.  This must be large enough to hold the
    // received frame (media header + data).  If the length of smaller than
    // the received frame, data will be lost.
    pxe_uint32 BufferLen;

    // Reserved, must be set to zero.
    pxe_uint32 reserved;
} pxe_cpb_receive_t;


typedef struct {
    // Source and destination MAC addresses from media header.
    pxe_mac_addr_t src_addr;
    pxe_mac_addr_t dest_addr;

    // Length of received frame.  May be larger than receive buffer size.
    // The receive buffer will not be overwritten.  This is how to tell
    // if data was lost because the receive buffer was too small.
    pxe_uint32 frame_len;

    // Protocol type from media header.
    pxe_media_protocol_t protocol;

    // Length of media header in received frame.
    pxe_uint16 media_header_len;

    // Type of receive frame.
    pxe_frame_type_t type;

    // Reserved, must be zero.
    pxe_uint8 reserved[7];

} pxe_db_receive_t;

#endif /* ifndef EFI_ELF_PXE */
