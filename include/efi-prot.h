#ifndef EFI_ELF_PROT
#define EFI_ELF_PROT

#include "efi-api.h"
#include "efi-devpath.h"
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

	efi_status              (*reset_blocks) (
		input efi_block_io_protocol_t   *This,
		input bool                      extended_verification);

	efi_status              (*read_blocks) (
		input efi_block_io_protocol_t   *This,
		input uint32                    media_id,
		input efi_lba_t                 lba,
		input uint64                    buffer_size,
		output void                     *buffer);

	efi_status              (*write_blocks) (
		input efi_block_io_protocol_t   *This,
		input uint32                    media_id,
		input efi_lba_t                 lba,
		input uint64                    buffer_size,
		input void                      *buffer);

	efi_status              (*flush_blocks) (
		input efi_block_io_protocol_t   *This);

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

	efi_status              (*reset_blocks_ex) (
		input _efi_block_io2_protocol_t         *This,
		input bool                              ExtendedVerification);

	efi_status              (*read_blocks_ex) (
		input _efi_block_io2_protocol_t         *This,
		input uint32                            media_id,
		input efi_lba_t                         lba,
		input output efi_block_io2_token_t      *token,
		input uint64                            buffer_size,
		output void                             *buffer);

	efi_status              (*write_blocks_ex) (
		input _efi_block_io2_protocol_t         *This,
		input uint32                            media_id,
		input efi_lba_t                         lba,
		input output efi_block_io2_token_t      *token,
		input uint64                            buffer_size,
		input void                              *buffer);

	efi_status              (*flush_blocks_ex) (
		input _efi_block_io2_protocol_t         *This,
		input output efi_block_io2_token_t      *token
		);
} efi_block_io2_protocol_t;

// Disk Block IO protocol

#define EFI_DISK_IO_PROTOCOL_GUID \
    { 0xce345171, 0xba0b, 0x11d2,  {0x8e, 0x4f, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b} }
#define DISK_IO_PROTOCOL EFI_DISK_IO_PROTOCOL_GUID

#define EFI_DISK_IO_PROTOCOL_REVISION  0x00010000
#define EFI_DISK_IO_INTERFACE_REVISION EFI_DISK_IO_PROTOCOL_REVISION

typedef struct efi_disk_io_protocol_t {
	uint64              revision;

	efi_status          (*read_disk) (
		input efi_disk_io_protocol_t    *This,
		input uint32                    media_id,
		input uint64                    offset,
		input uint64                    buffer_size,
		output void                     *buffer
		);

	efi_status          (*write_disk) (
		input efi_disk_io_protocol_t    *This,
		input uint32                    media_id,
		input uint64                    offset,
		input uint64                    buffer_size,
		input void                      *buffer);
} efi_disk_io_protocol_t;

typedef efi_disk_io_protocol_t efi_disk_io_t;

#define EFI_DISK_IO2_PROTOCOL_GUID \
    { 0x151c8eae, 0x7f2c, 0x472c,  {0x9e, 0x54, 0x98, 0x28, 0x19, 0x4f, 0x6a, 0x88} }

#define EFI_DISK_IO2_PROTOCOL_REVISION  0x00020000

typedef struct {
    efi_event_t         Event;
    efi_status          TransactionStatus;
} efi_disk_io2_token_t;

typedef struct efi_disk_io2_protocol_t {
	uint64                            revision;

	efi_status                        (*cancel) (
		input struct efi_disk_io2_protocol_t    *This);

	efi_status                        (*read_disk_ex) (
		input struct efi_disk_io2_protocol_t    *This,
		input uint32                            media_id,
		input uint64                            offset,
		input output efi_disk_io2_token_t       *token,
		input uint64                            buffer_size,
		output void                             *buffer);

	efi_status                        (*write_disk_ex) (
		input struct efi_disk_io2_protocol_t    *This,
		input uint32                            media_id,
		input uint64                            offset,
		input output efi_disk_io2_token_t       *token,
		input uint64                            buffer_size,
		input void                              *buffer);

	efi_status                        (*Flush_disk_ex) (
		input struct efi_disk_io2_protocol_t    *This,
		input output efi_disk_io2_token_t       *token);

} efi_disk_io2_protocol_t;

// Simple file system protocol


#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID \
    { 0x964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b} }
#define SIMPLE_FILE_SYSTEM_PROTOCOL EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID

#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_REVISION  0x00010000
#define EFI_FILE_IO_INTERFACE_REVISION EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_REVISION

typedef struct efi_simple_file_system_protocol_t {
	uint64                  revision;

	efi_status              (*open_volume) (
		input struct efi_simple_file_system_protocol_t  *This,
		output struct _efi_file_handle_t                **root);

} efi_simple_file_system_protocol_t;

typedef efi_simple_file_system_protocol_t efi_file_io_interface_t;

// Open modes
#define EFI_FILE_MODE_READ      0x0000000000000001
#define EFI_FILE_MODE_WRITE     0x0000000000000002
#define EFI_FILE_MODE_CREATE    0x8000000000000000

// File attributes
#define EFI_FILE_READ_ONLY      0x0000000000000001
#define EFI_FILE_HIDDEN         0x0000000000000002
#define EFI_FILE_SYSTEM         0x0000000000000004
#define EFI_FILE_RESERVIED      0x0000000000000008
#define EFI_FILE_DIRECTORY      0x0000000000000010
#define EFI_FILE_ARCHIVE        0x0000000000000020
#define EFI_FILE_VALID_ATTR     0x0000000000000037


typedef struct {
    efi_event_t     event;
    efi_status      status;
    uint64          buffer_size;
    void            *buffer;
} efi_file_io_token_t;

#define EFI_FILE_PROTOCOL_REVISION         0x00010000
#define EFI_FILE_PROTOCOL_REVISION2        0x00020000
#define EFI_FILE_PROTOCOL_LATEST_REVISION  EFI_FILE_PROTOCOL_REVISION2
#define EFI_FILE_HANDLE_REVISION           EFI_FILE_PROTOCOL_REVISION

typedef struct _efi_file_handle_t {
	uint64                  revision;

	efi_status              (*open) (
		input  struct _efi_file_handle_t    *file,
		output struct _efi_file_handle_t    **new_handle,
		input wchar                         *file_name,
		input uint64                        open_mode,
		input uint64                        attributes);

	efi_status              (*close) (
		input struct _efi_file_handle_t     *file);

	efi_status              (*delete_file) (
		input struct _efi_file_handle_t     *file);

	efi_status              (*read) (
		input struct _efi_file_handle_t     *file,
		input output uint64                 *buffer_size,
		output void                         *buffer);

	efi_status              (*write) (
		input struct _efi_file_handle_t     *file,
		input output uint64                 *buffer_size,
		input void                          *buffer);

	efi_status              (*get_position) (
		input struct _efi_file_handle_t     *file,
		output uint64                       *position);

	efi_status              (*set_position) (
		input struct _efi_file_handle_t     *file,
		input uint64                        position);

	efi_status              (*get_info) (
		input struct _efi_file_handle_t     *File,
		input efi_guid_t                    *InformationType,
		input output uint64                 *buffer_size,
		output void                         *Buffer);

	efi_status              (*set_info) (
		input struct _efi_file_handle_t     *File,
		input efi_guid_t                    *information_type,
		input uint64                        buffer_size,
		input void                          *buffer);

	efi_status              (*Flush) (
		input struct _efi_file_handle_t     *file);

	efi_status              (*open_ex) (
		input  struct _efi_file_handle_t    *file,
		output struct _efi_file_handle_t    **new_handle,
		input wchar                         *file_name,
		input uint64                        open_mode,
		input uint64                        attributes,
		input output efi_file_io_token_t    *token);

	efi_status              (*read_ex) (
		input struct _efi_file_handle_t     *file,
		input output efi_file_io_token_t    *token);

	efi_status              (*write_ex) (
		input struct _efi_file_handle_t     *file,
		input output efi_file_io_token_t    *token);

	efi_status              (*flush_ex) (
		input struct _efi_file_handle_t     *file,
		input output efi_file_io_token_t    *token);

} efi_file_protocol_t, *efi_file_handle_t;

typedef efi_file_protocol_t efi_file_t;

// File information types


#define EFI_FILE_INFO_ID   \
    { 0x9576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b} }

typedef struct {
    uint64                  size;
    uint64                  file_size;
    uint64                  physical_size;
    efi_time_t              create_time;
    efi_time_t              last_access_time;
    efi_time_t              modification_time;
    uint64                  attribute;
    wchar                   file_name[1];
} efi_file_info_t;

#define SIZE_OF_EFI_FILE_INFO EFI_FIELD_OFFSET(EFI_FILE_INFO,FileName)

#define EFI_FILE_SYSTEM_INFO_ID    \
    { 0x9576e93, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b} }

typedef struct {
    uint64                  size;
    bool                    read_only;
    uint64                  volume_size;
    uint64                  free_space;
    uint32                  block_size;
    wchar                   volume_label[1];
} efi_file_system_info_t;


#define SIZE_OF_EFI_FILE_SYSTEM_INFO EFI_FIELD_OFFSET(EFI_FILE_SYSTEM_INFO,VolumeLabel)

#define EFI_FILE_SYSTEM_VOLUME_LABEL_ID    \
    { 0xDB47D7D3,0xFE81, 0x11d3, {0x9A, 0x35, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D} }

typedef struct {
    wchar                  volume_label[1];
} efi_file_system_volume_label_t;

#define SIZE_OF_EFI_FILE_SYSTEM_VOLUME_LABEL_INFO EFI_FIELD_OFFSET(EFI_FILE_SYSTEM_VOLUME_LABEL,VolumeLabel)

// For compatibility with older versions of gnu-efi
#define EFI_FILE_SYSTEM_VOLUME_LABEL_INFO_ID EFI_FILE_SYSTEM_VOLUME_LABEL_ID
#define EFI_FILE_SYSTEM_VOLUME_LABEL_INFO    EFI_FILE_SYSTEM_VOLUME_LABEL

// Load file protocol

#define EFI_LOAD_FILE_PROTOCOL_GUID \
    { 0x56EC3091, 0x954C, 0x11d2, {0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B} }
#define LOAD_FILE_PROTOCOL EFI_LOAD_FILE_PROTOCOL_GUID


typedef struct _efi_load_file_protocol_t {
    efi_status                      (*LoadFile) (
		input struct _efi_load_file_protocol_t  *This,
		input efi_device_path_t                 *file_path,
		input bool                              boot_policy,
		input output uint64                     *buffer_size,
		input void                              *buffer optional);
} efi_load_file_protocol_t;

typedef efi_load_file_protocol_t efi_load_file_interface_t;


// Device IO protocol
//

#define EFI_DEVICE_IO_PROTOCOL_GUID \
    { 0xaf6ac311, 0x84c3, 0x11d2, {0x8e, 0x3c, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b} }
#define DEVICE_IO_PROTOCOL EFI_DEVICE_IO_PROTOCOL_GUID

struct _EFI_DEVICE_IO_PROTOCOL;

typedef enum {
    IO_uint8,
    IO_uint16,
    IO_uint32,
    IO_uint64,
//
// Specification Change: Copy from MMIO to MMIO vs. MMIO to buffer, buffer to MMIO
//
    MMIO_COPY_uint8,
    MMIO_COPY_uint16,
    MMIO_COPY_uint32,
    MMIO_COPY_uint64
} efi_io_width_t;

#define EFI_PCI_ADDRESS(_bus,_dev,_func) \
    ( (uint64) ( (((uint32)_bus) << 24) + (((uint64)_dev) << 16) + (((uint64)_func) << 8) ) )


typedef struct {
    efi_status                      (*read) (
		input struct efi_device_io_protocol_t   *This,
		input efi_io_width_t                    width,
		input uint64                            address,
		input uint64                            count,
		input output void                       *buffer);

    efi_status                      (*write) (
		input struct efi_device_io_protocol_t   *This,
		input efi_io_width_t                    width,
		input uint64                            address,
		input uint64                            count,
		input output void                       *buffer);
} efi_io_access_t;


typedef enum {
    EFI_BUS_MASTER_READ,
    EFI_BUS_MASTER_WRITE,
    EFI_BUS_Master_COMMON_BUFFER
} efi_io_operation_type_t;

typedef struct efi_device_io_protocol_t {
    efi_io_access_t                     mem;
    efi_io_access_t                     io;
    efi_io_access_t                     pci;

    efi_status                          (*map) (
		input struct efi_device_io_protocol_t   *This,
		input efi_io_operation_type_t           operation,
		input efi_physical_addr_t               *host_address,
		input output uint64                     *size,
		output efi_physical_addr_t              *device_address,
		output void                             **mapping);

    efi_status                          (*pci_device_path) (
		input struct efi_device_io_protocol_t    *This,
		input uint64                            address,
		input output efi_device_path_t          **pci_device_path);

    efi_status                          (*unmap) (
		input struct efi_device_io_protocol_t   *This,
		input void                              *mapping);

    efi_status                          (*allocate_buffer) (
		input struct efi_device_io_protocol_t   *This,
		input efi_allocate_type_t               Type,
		input efi_memory_type_t                 memory_type,
		input uint64                            pages,
		input output efi_physical_addr_t        *host_address);

    efi_status                          (*flush) (
		input struct efi_device_io_protocol_t   *This);

    efi_status                          (*free_buffer) (
		input struct efi_device_io_protocol_t   *This,
		input uint64                            pages,
		input efi_physical_addr_t               host_address);
} efi_device_io_protocol_t;

typedef efi_device_io_protocol_t efi_device_io_interface_t;

// Unicode Collation protocol
//

#define EFI_UNICODE_COLLATION_PROTOCOL_GUID \
    { 0x1d85cd7f, 0xf43d, 0x11d2, {0x9a, 0xc,  0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d} }
#define UNICODE_COLLATION_PROTOCOL EFI_UNICODE_COLLATION_PROTOCOL_GUID

#define EFI_UNICODE_BYTE_ORDER_MARK       (wchar)(0xfeff)
#define UNICODE_BYTE_ORDER_MARK EFI_UNICODE_BYTE_ORDER_MARK


// Hash Protocol
//
#define EFI_HASH_PROTOCOL_GUID \
  { 0xC5184932, 0xDBA5, 0x46DB, { 0xA5, 0xBA, 0xCC, 0x0B, 0xDA, 0x9C, 0x14, 0x35 } }
#define HASH_PROTOCOL EFI_HASH_PROTOCOL_GUID

#define EFI_HASH_ALGORITHM_SHA1_GUID \
  { 0x2AE9D80F, 0x3FB2, 0x4095, { 0xB7, 0xB1, 0xE9, 0x31, 0x57, 0xB9, 0x46, 0xB6 } } // Deprecated
#define EFI_HASH_ALGORITHM_SHA1 EFI_HASH_ALGORITHM_SHA1_GUID

#define EFI_HASH_ALGORITHM_SHA224_GUID \
  { 0x8DF01A06, 0x9BD5, 0x4BF7, { 0xB0, 0x21, 0xDB, 0x4F, 0xD9, 0xCC, 0xF4, 0x5B } } // Deprecated
#define EFI_HASH_ALGORITHM_SHA224 EFI_HASH_ALGORITHM_SHA224_GUID

#define EFI_HASH_ALGORITHM_SHA256_GUID \
  { 0x51AA59DE, 0xFDF2, 0x4EA3, { 0xBC, 0x63, 0x87, 0x5F, 0xB7, 0x84, 0x2E, 0xE9 } } // Deprecated
#define EFI_HASH_ALGORITHM_SHA256 EFI_HASH_ALGORITHM_SHA256_GUID

#define EFI_HASH_ALGORITHM_SHA384_GUID \
  { 0xEFA96432, 0xDE33, 0x4DD2, { 0xAE, 0xE6, 0x32, 0x8C, 0x33, 0xDF, 0x77, 0x7A } } // Deprecated
#define EFI_HASH_ALGORITHM_SHA384 EFI_HASH_ALGORITHM_SHA384_GUID

#define EFI_HASH_ALGORITHM_SHA512_GUID \
  { 0xCAA4381E, 0x750C, 0x4770, { 0xB8, 0x70, 0x7A, 0x23, 0xB4, 0xE4, 0x21, 0x30 } } // Deprecated
#define EFI_HASH_ALGORITHM_SHA512 EFI_HASH_ALGORITHM_SHA512_GUID

#define EFI_HASH_ALGORITHM_MD5_GUID \
  { 0x0AF7C79C, 0x65B5, 0x4319, { 0xB0, 0xAE, 0x44, 0xEC, 0x48, 0x4E, 0x4A, 0xD7 } } // Deprecated
#define EFI_HASH_ALGORITHM_MD5 EFI_HASH_ALGORITHM_MD5_GUID

#define EFI_HASH_ALGORITHM_SHA1_NOPAD_GUID \
  { 0x24C5DC2F, 0x53E2, 0x40CA, { 0x9E, 0xD6, 0xA5, 0xD9, 0xA4, 0x9F, 0x46, 0x3B } }
#define EFI_HASH_ALGORITHM_SHA1_NOPAD EFI_HASH_ALGORITHM_SHA1_NOPAD_GUID

#define EFI_HASH_ALGORITHM_SHA256_NOPAD_GUID \
  { 0x8628752A, 0x6CB7, 0x4814, { 0x96, 0xFC, 0x24, 0xA8, 0x15, 0xAC, 0x22, 0x26 } }
#define EFI_HASH_ALGORITHM_SHA256_NOPAD EFI_HASH_ALGORITHM_SHA256_NOPAD_GUID


typedef uint8 Efi_MD5_hash_t[16];
typedef uint8 Efi_SHA1_hash_t[20];
typedef uint8 Efi_SHA224_hash_t[28];
typedef uint8 Efi_SHA256_hash_t[32];
typedef uint8 Efi_SHA384_hash_t[48];
typedef uint8 Efi_SHA512_hash_t[64];

typedef union {
  Efi_MD5_hash_t                    *Md5Hash;
  Efi_SHA1_hash_t                   *Sha1Hash;
  Efi_SHA224_hash_t                 *Sha224Hash;
  Efi_SHA256_hash_t                 *Sha256Hash;
  Efi_SHA384_hash_t                 *Sha384Hash;
  Efi_SHA512_hash_t                 *Sha512Hash;
} efi_hash_output_t;


typedef struct efi_hash_protocol_t {
	efi_status                  (*get_hash_size) (
		input const struct efi_hash_protocol_t    *This,
		input const efi_guid_t                    *HashAlgorithm,
		output uint64                             *HashSize);

	efi_status                  (*hash) (
		input const struct efi_hash_protocol_t    *This,
		input const efi_guid_t                    *hash_algorithm,
		input bool                                extend,
		input const uint8                         *message,
		input uint64                              message_size,
		input output efi_hash_output_t            *hash);
} efi_hash_protocol_t;

typedef efi_hash_protocol_t efi_hash_t;

typedef struct efi_unicode_collation_protocol_t efi_unicode_collation_protocol_t;

typedef struct efi_unicode_collation_protocol_t {
    // general
    int64                   (*stri_coll) (
		input efi_unicode_collation_protocol_t  *This,
		input wchar                             *s1,
		input wchar                             *s2);

    bool                    (*metai_match) (
		input efi_unicode_collation_protocol_t  *This,
		input wchar                             *string,
		input wchar                             *pattern);

    bool                    (*str_lower) (
		input efi_unicode_collation_protocol_t  *This,
		input output wchar                      *str);

    bool                    (*str_upper) (
		input efi_unicode_collation_protocol_t  *This,
		input output wchar                      *str);

    // for supporting fat volumes
    void                    (*fat_to_str) (
		input efi_unicode_collation_protocol_t  *This,
		input uint64                            fat_size,
		input char                              *fat,
		output wchar                            *string);

    bool                    (*str_to_fat) (
		input efi_unicode_collation_protocol_t  *This,
		input wchar                             *string,
		input uint64                            fat_size,
		output char                             *fat);

    char                    *supported_languages;
} efi_unicode_collation_protocol_t;

typedef efi_unicode_collation_protocol_t efi_unicode_collation_interface_t;


/* Graphics output protocol */
#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID \
   { 0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a } }
typedef struct _EFI_GRAPHICS_OUTPUT_PROTOCOL EFI_GRAPHICS_OUTPUT_PROTOCOL;

typedef struct {
  uint32            red_mask;
  uint32            green_mask;
  uint32            blue_mask;
  uint32            reserved_mask;
} efi_pixel_bitmask_t;

typedef enum {
  PIXEL_RGB_RESERVED_8BIT_PER_COLOR,
  PIXEL_BGR_RESERVED_8BIT_PER_COLOR,
  PIXEL_BIT_MASK,
  PIXEL_BLT_ONLY,
  PIXEL_FORMAT_MAX
} efi_graphics_pixel_format_t;

typedef struct {
  uint32                        version;
  uint32                        horizontal_resolution;
  uint32                        vertical_resolution;
  efi_graphics_pixel_format_t   pixel_format;
  efi_pixel_bitmask_t           pixel_information;
  uint32                        pixels_per_scan_line;
} efi_graphics_output_mode_information_t;


typedef struct {
  uint8 blue;
  uint8 green;
  uint8 red;
  uint8 reserved;
} efi_graphics_output_blt_pixel_t;

typedef union {
	efi_graphics_output_blt_pixel_t pixel;
	uint32                          raw;
} efi_graphics_output_blt_pixel_union_t;

/*
  <B>Efi_BLT_VIDEO_FILL</B> - Write data from the  BltBuffer pixel (SourceX, SourceY)
  directly to every pixel of the video display rectangle
  (DestinationX, DestinationY) (DestinationX + Width, DestinationY + Height).
  Only one pixel will be used from the BltBuffer. Delta is NOT used.

  <B>Efi_BLT_VIDEO_TO_Blt_BUFFER</B> - Read data from the video display rectangle
  (SourceX, SourceY) (SourceX + Width, SourceY + Height) and place it in
  the BltBuffer rectangle (DestinationX, DestinationY )
  (DestinationX + Width, DestinationY + Height). If DestinationX or
  DestinationY is not zero then Delta must be set to the length in bytes
  of a row in the BltBuffer.

  <B>Efi_BLT_BUFFER_TO_VIDEO</B> - Write data from the  BltBuffer rectangle
  (SourceX, SourceY) (SourceX + Width, SourceY + Height) directly to the
  video display rectangle (DestinationX, DestinationY)
  (DestinationX + Width, DestinationY + Height). If SourceX or SourceY is
  not zero then Delta must be set to the length in bytes of a row in the
  BltBuffer.

  <B>Efi_BLT_VIDEO_TO_VIDEO</B> - Copy from the video display rectangle (SourceX, SourceY)
  (SourceX + Width, SourceY + Height) .to the video display rectangle
  (DestinationX, DestinationY) (DestinationX + Width, DestinationY + Height).
  The BltBuffer and Delta  are not used in this mode.
 */
typedef enum {
  Efi_BLT_VIDEO_FILL,
  Efi_BLT_VIDEO_TO_Blt_BUFFER,
  Efi_BLT_BUFFER_TO_VIDEO,
  Efi_BLT_VIDEO_TO_VIDEO,
  Efi_GRAPHICS_OUTPUT_BLT_OPERATION_MAX
} efi_graphics_output_blt_operation_t;


typedef struct {
	uint32                                  max_mode;
	uint32                                  mode;
	efi_graphics_output_mode_information_t  *info;
	uint64                                  size_of_info;
	efi_physical_addr_t                     frame_buffer_base;
	uint64                                  frame_buffer_size;
} efi_graphics_output_protocol_mode_t;

typedef struct efi_graphics_output_protocol_t {
/**
  Return the current video mode information.

  @param  This       Protocol instance pointer.
  @param  ModeNumber The mode number to return information on.
  @param  SizeOfInfo A pointer to the size, in bytes, of the Info buffer.
  @param  Info       A pointer to callee allocated buffer that returns information about ModeNumber.

  @retval EFI_SUCCESS           Mode information returned.
  @retval EFI_BUFFER_TOO_SMALL  The Info buffer was too small.
  @retval EFI_DEVICE_ERROR      A hardware error occurred trying to retrieve the video mode.
  @retval EFI_NOT_STARTED       Video display is not initialized. Call SetMode ()
  @retval EFI_INVALID_PARAMETER One of the input args was NULL.

**/
	efi_status                              (*query_mode) (
		input  efi_graphics_output_protocol_t           *This,
		input  uint32                                   mode_number,
		output uint64                                   *size_of_info,
		output efi_graphics_output_mode_information_t   **info);

/**
  Return the current video mode information.

  @param  This              Protocol instance pointer.
  @param  ModeNumber        The mode number to be set.

  @retval EFI_SUCCESS       Graphics mode was changed.
  @retval EFI_DEVICE_ERROR  The device had an error and could not complete the request.
  @retval EFI_UNSUPPORTED   ModeNumber is not supported by this device.

**/
	efi_status                              (*set_mode) (
		input  efi_graphics_output_protocol_t           *This,
		input  uint32                                   mode_number);

/**
  @param  This         Protocol instance pointer.
  @param  BltBuffer    Buffer containing data to blit into video buffer. This
                       buffer has a size of Width*Height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
  @param  BltOperation Operation to perform on BlitBuffer and video memory
  @param  SourceX      X coordinate of source for the BltBuffer.
  @param  SourceY      Y coordinate of source for the BltBuffer.
  @param  DestinationX X coordinate of destination for the BltBuffer.
  @param  DestinationY Y coordinate of destination for the BltBuffer.
  @param  Width        Width of rectangle in BltBuffer in pixels.
  @param  Height       Hight of rectangle in BltBuffer in pixels.
  @param  Delta        OPTIONAL

  @retval EFI_SUCCESS           The Blt operation completed.
  @retval EFI_INVALID_PARAMETER BltOperation is not valid.
  @retval EFI_DEVICE_ERROR      A hardware error occured writting to the video buffer.

**/
	efi_status                              (*blt) (
		input  efi_graphics_output_protocol_t           *This,
		input  efi_graphics_output_blt_pixel_t          *blt_buffer,   optional
		input  efi_graphics_output_blt_operation_t      blt_operation,
		input  uint64                                   sourceX,
		input  uint64                                   sourceY,
		input  uint64                                   destinationX,
		input  uint64                                   destinationY,
		input  uint64                                   width,
		input  uint64                                   height,
		input  uint64                                   delta         optional
		);
	efi_graphics_output_protocol_mode_t     *Mode;
} efi_graphics_output_protocol_t;

#endif // !EFI_ELF_PROT
