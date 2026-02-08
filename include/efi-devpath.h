#ifndef EFI_ELF_DEVPATH
#define EFI_ELF_DEVPATH


typedef struct _efi_device_path_protocol_t {
        unsigned char   type;
        unsigned char   sub_type;
        unsigned char   length[2];
} efi_device_path_protocol_t;

typedef struct _efi_device_path_protocol_t _efi_device_path_t;
typedef efi_device_path_protocol_t efi_device_path_t;

#endif // !EFI_ELF_DEVPATH
