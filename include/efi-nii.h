#ifndef EFI_ELF_NII
#define EFI_ELF_NII

#include "types.h"

#define EFI_NETWORK_INTERFACE_IDENTIFIER_PROTOCOL_GUID \
    { 0xE18541CD, 0xF755, 0x4f73, {0x92, 0x8D, 0x64, 0x3C, 0x8A, 0x79, 0xB2, 0x29} }

#define EFI_NETWORK_INTERFACE_IDENTIFIER_PROTOCOL_REVISION  0x00010000
#define EFI_NETWORK_INTERFACE_IDENTIFIER_INTERFACE_REVISION EFI_NETWORK_INTERFACE_IDENTIFIER_PROTOCOL_REVISION

typedef enum {
    EFI_NETWORK_INTERFACE_UNDI = 1
} efi_network_interface_type_t;

typedef struct {

    // Revision of the network interface identifier protocol interface.
    uint64 revision;

    // Address of the first byte of the identifying structure for this
    // network interface.  This is set to zero if there is no structure.
    //
    // For PXE/UNDI this is the first byte of the !PXE structure.
    uint64 id;

    // Address of the UNrelocated driver/ROM image.  This is set
    // to zero if there is no driver/ROM image.
    //
    // For 16-bit UNDI, this is the first byte of the option ROM in
    // upper memory.
    //
    // For 32/64-bit S/W UNDI, this is the first byte of the EFI ROM
    // image.
    //
    // For H/W UNDI, this is set to zero.
    uint64 image_addr;

    // Size of the UNrelocated driver/ROM image of this network interface.
    // This is set to zero if there is no driver/ROM image.
    uint32 image_size;

    // 4 char ASCII string to go in class identifier (option 60) in DHCP
    // and Boot Server discover packets.
    // For EfiNetworkInterfaceUndi this field is "UNDI".
    // For EfiNetworkInterfaceSnp this field is "SNPN".
    char string_id[4];

    // Information to be placed into the PXE DHCP and Discover packets.
    // This is the network interface type and version number that will
    // be placed into DHCP option 94 (client network interface identifier).
    uint8 type;
    uint8 major_ver;
    uint8 minor_ver;

    bool    ipv6_supported;
    uint8   if_num;	// interface number to be used with pxeid structure
} efi_network_interface_identifier_protocol_t, efi_network_interface_identifier_interface_t;

#endif //! EFI_ELF_NII
