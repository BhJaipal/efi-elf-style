#ifndef EFI_ELF_PROT
#define EFI_ELF_PROT

#include "efi-api.h"
#include "types.h"
#define EFI_FPSWA_PROTOCOL_GUID \
    { 0xc41b6531, 0x97b9, 0x11d3, {0x9a, 0x29, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d} }
#define FPSWA_PROTOCOL EFI_FPSWA_PROTOCOL_GUID

// Device Path protocol

#define EFI_DEVICE_PATH_PROTOCOL_GUID \
    { 0x9576e91, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b} }
#define DEVICE_PATH_PROTOCOL EFI_DEVICE_PATH_PROTOCOL_GUID

// Block IO protocol

#define EFI_BLOCK_IO_PROTOCOL_GUID \
    { 0x964e5b21, 0x6459, 0x11d2, {0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b} }
#define BLOCK_IO_PROTOCOL EFI_BLOCK_IO_PROTOCOL_GUID

#define EFI_BLOCK_IO_PROTOCOL_REVISION    0x00010000
#define EFI_BLOCK_IO_PROTOCOL_REVISION2   0x00020001
#define EFI_BLOCK_IO_PROTOCOL_REVISION3   ((2<<16) | 31)
#define EFI_BLOCK_IO_INTERFACE_REVISION   EFI_BLOCK_IO_PROTOCOL_REVISION
#define EFI_BLOCK_IO_INTERFACE_REVISION2  EFI_BLOCK_IO_PROTOCOL_REVISION2
#define EFI_BLOCK_IO_INTERFACE_REVISION3  EFI_BLOCK_IO_PROTOCOL_REVISION3


typedef struct {
    uint32              media_id;
    bool                removable_media;
    bool                media_present;

    bool                logical_partition;
    bool                read_Only;
    bool                write_caching;

    uint32              block_size;
    uint32              io_align;

    efi_lba_t           last_block;

    /* revision 2 */
    efi_lba_t           lowest_aligned_lba;
    uint32              logical_blocks_per_physical_block;
    /* revision 3 */
    uint32              optimal_transfer_length_granularity;
} efi_block_io_media_t;

typedef struct efi_block_io_protocol_t {
    uint64                  revision;

    efi_block_io_media_t    *media;

    efi_status (*reset_blocks) (
		input efi_block_io_protocol_t   *This,
		input bool                      extended_verification
		);

    efi_status (*read_blocks) (
		input efi_block_io_protocol_t   *This,
		input uint32                    media_id,
		input efi_lba_t                 lba,
		input uint64                    buffer_size,
		output void                     *buffer
	);
		efi_status (*write_blocks) (
		input efi_block_io_protocol_t   *This,
		input uint32                    media_id,
		input efi_lba_t                 lba,
		input uint64                    buffer_size,
		input void                      *buffer
		);
    efi_status (*flush_blocks) (
		input efi_block_io_protocol_t   *This
		);

} efi_block_io_protocol_t;

typedef efi_block_io_protocol_t efi_block_io_t;

#define EFI_BLOCK_IO2_PROTOCOL_GUID \
    { 0xa77b2472, 0xe282, 0x4e9f, {0xa2, 0x45, 0xc2, 0xc0, 0xe2, 0x7b, 0xbc, 0xc1} }

typedef struct {
    efi_event_t         event;
    efi_status          transaction_status;
} efi_block_io2_token_t;


typedef struct _efi_block_io2_protocol_t {
    efi_block_io_media_t    *media;
    efi_status (*reset_blocks_ex) (
		input _efi_block_io2_protocol_t         *This,
		input bool                              ExtendedVerification
		);
    efi_status (*read_blocks_ex) (
		input _efi_block_io2_protocol_t         *This,
		input uint32                            media_id,
		input efi_lba_t                         lba,
		input output efi_block_io2_token_t      *token,
		input uint64                            buffer_size,
		output void                             *buffer
		);
    efi_status (*write_blocks_ex) (
		input _efi_block_io2_protocol_t         *This,
		input uint32                            media_id,
		input efi_lba_t                         lba,
		input output efi_block_io2_token_t      *token,
		input uint64                            buffer_size,
		input void                              *buffer
		);
    efi_status (*flush_blocks_ex) (
		input _efi_block_io2_protocol_t         *This,
		input output efi_block_io2_token_t      *token
		);
} efi_block_io2_protocol_t;

//
// Disk Block IO protocol
//

#define EFI_DISK_IO_PROTOCOL_GUID \
    { 0xce345171, 0xba0b, 0x11d2,  {0x8e, 0x4f, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b} }
#define DISK_IO_PROTOCOL EFI_DISK_IO_PROTOCOL_GUID

#define EFI_DISK_IO_PROTOCOL_REVISION  0x00010000
#define EFI_DISK_IO_INTERFACE_REVISION EFI_DISK_IO_PROTOCOL_REVISION

#endif // !EFI_ELF_PROT
