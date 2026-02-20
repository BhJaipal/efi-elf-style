#ifndef EFI_ELF_GPT
#define EFI_ELF_GPT

#include "efi-api.h"
#include "types.h"
#define PRIMARY_PART_HEADER_LBA         1

typedef struct {
    efi_table_header_t  header;
    efi_lba_t           my_lba;
    efi_lba_t           alternate_lba;
    efi_lba_t           first_usable_lba;
    efi_lba_t           last_usable_lba;
    efi_guid_t          disk_guid;
    efi_lba_t           partition_entry_lba;
    uint32              number_Of_partition_entries_len;
    uint32              partition_entry_size;
    uint32              partition_entry_array_crc32;
} efi_partition_table_header_t;

#define EFI_PTAB_HEADER_ID  "EFI PART"

typedef struct {
    efi_guid_t  partition_type_guid;
    efi_guid_t  unique_partition_guid;
    efi_lba_t   starting_lba;
    efi_lba_t   ending_lba;
    uint64      attributes;
    wchar       partition_name[36];
} efi_partition_entry_t;

//
// EFI Partition Attributes
//
#define EFI_PART_USED_BY_EFI            0x0000000000000001
#define EFI_PART_REQUIRED_TO_FUNCTION   0x0000000000000002
#define EFI_PART_USED_BY_OS             0x0000000000000004
#define EFI_PART_REQUIRED_BY_OS         0x0000000000000008
#define EFI_PART_BACKUP_REQUIRED        0x0000000000000010
#define EFI_PART_USER_DATA              0x0000000000000020
#define EFI_PART_CRITICAL_USER_DATA     0x0000000000000040
#define EFI_PART_REDUNDANT_PARTITION    0x0000000000000080

#define EFI_PART_TYPE_UNUSED_GUID   \
    { 0x00000000, 0x0000, 0x0000, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} }
    
#define EFI_PART_TYPE_EFI_SYSTEM_PART_GUID  \
    { 0xc12a7328, 0xf81f, 0x11d2, {0xba, 0x4b, 0x00, 0xa0, 0xc9, 0x3e, 0xc9, 0x3b} }

#define EFI_PART_TYPE_LEGACY_MBR_GUID   \
    { 0x024dee41, 0x33e7, 0x11d3, {0x9d, 0x69, 0x00, 0x08, 0xc7, 0x81, 0xf3, 0x9f} }


#endif // !EFI_ELF_GPT
