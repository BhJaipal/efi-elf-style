#ifndef EFI_ELF_API
#define EFI_ELF_API
#include "efi-devpath.h"
#include "types.h"
#include "efi-con.h"

struct efi_system_table_t;

// EFI Memory
#undef NextMemoryDescriptor
#define NextMemoryDescriptor(Ptr,Size)  ((efi_memory_descriptor_t *) (((uint8 *) Ptr) + Size))


#define EFI_OPTIONAL_PTR            0x00000001
#define EFI_INTERNAL_FNC            0x00000002      // Pointer to internal runtime fnc
#define EFI_INTERNAL_PTR            0x00000004      // Pointer to internal runtime data


// EFI Events
#define EVT_TIMER                           0x80000000
#define EVT_RUNTIME                         0x40000000
#define EVT_RUNTIME_CONTEXT                 0x20000000

#define EVT_NOTIFY_WAIT                     0x00000100
#define EVT_NOTIFY_SIGNAL                   0x00000200

#define EVT_SIGNAL_EXIT_BOOT_SERVICES       0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE   0x60000202

#define EVT_EFI_SIGNAL_MASK                 0x000000FF
#define EVT_EFI_SIGNAL_MAX                  4

#define EFI_EVENT_TIMER                         EVT_TIMER
#define EFI_EVENT_RUNTIME                       EVT_RUNTIME
#define EFI_EVENT_RUNTIME_CONTEXT               EVT_RUNTIME_CONTEXT
#define EFI_EVENT_NOTIFY_WAIT                   EVT_NOTIFY_WAIT
#define EFI_EVENT_NOTIFY_SIGNAL                 EVT_NOTIFY_SIGNAL
#define EFI_EVENT_SIGNAL_EXIT_BOOT_SERVICES     EVT_SIGNAL_EXIT_BOOT_SERVICES
#define EFI_EVENT_SIGNAL_VIRTUAL_ADDRESS_CHANGE EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE
#define EFI_EVENT_EFI_SIGNAL_MASK               EVT_EFI_SIGNAL_MASK
#define EFI_EVENT_EFI_SIGNAL_MAX                EVT_EFI_SIGNAL_MAX


typedef
void
(*efi_event_notify_t) (
    input efi_event_t   event,
    input void          *context
    );


typedef enum {
    TIMER_CANCEL,
    TIMER_PERIODIC,
    TIMER_RELATIVE,
    TIMER_TYPE_MAX
} efi_timer_delay_t;

// Task priority level
#define TPL_APPLICATION       4
#define TPL_CALLBACK          8
#define TPL_NOTIFY           16
#define TPL_HIGH_LEVEL       31
#define EFI_TPL_APPLICATION  TPL_APPLICATION
#define EFI_TPL_CALLBACK     TPL_CALLBACK
#define EFI_TPL_NOTIFY       TPL_NOTIFY
#define EFI_TPL_HIGH_LEVEL   TPL_HIGH_LEVEL

// EFI platform varibles
#define EFI_GLOBAL_VARIABLE     \
    { 0x8BE4DF61, 0x93CA, 0x11d2, {0xAA, 0x0D, 0x00, 0xE0, 0x98, 0x03, 0x2B, 0x8C} }

// Variable attributes
#define EFI_VARIABLE_NON_VOLATILE                          0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS                    0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS                        0x00000004
#define EFI_VARIABLE_HARDWARE_ERROR_RECORD                 0x00000008
#define EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS            0x00000010
#define EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS 0x00000020
#define EFI_VARIABLE_APPEND_WRITE                          0x00000040

// Variable size limitation
#define EFI_MAXIMUM_VARIABLE_SIZE           1024


// EFI Time
typedef struct {
        uint32  resolution;     // 1e-6 parts per million
        uint32  accuracy;       // hertz
        bool    sets_to_zero;     // Set clears sub-second time
} efi_time_capabilities_t;


// Image functions

// PE32+ Subsystem type for EFI images

#if !defined(IMAGE_SUBSYSTEM_EFI_APPLICATION)
#define IMAGE_SUBSYSTEM_EFI_APPLICATION             10
#define IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER     11
#define IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER          12
#endif

// PE32+ Machine type for EFI images

#if !defined(EFI_IMAGE_MACHINE_IA32)
#define EFI_IMAGE_MACHINE_IA32      0x014c
#endif

#if !defined(EFI_IMAGE_MACHINE_IA64)
#define EFI_IMAGE_MACHINE_IA64      0x0200
#endif

#if !defined(EFI_IMAGE_MACHINE_EBC)
#define EFI_IMAGE_MACHINE_EBC      0x0EBC
#endif

#if !defined(EFI_IMAGE_MACHINE_X64)
#define EFI_IMAGE_MACHINE_X64       0x8664
#endif

#if !defined(EFI_IMAGE_MACHINE_ARMTHUMB_MIXED)
#define EFI_IMAGE_MACHINE_ARMTHUMB_MIXED 0x01C2
#endif

#if !defined(EFI_IMAGE_MACHINE_AARCH64)
#define EFI_IMAGE_MACHINE_AARCH64   0xAA64
#endif

#if !defined(EFI_IMAGE_MACHINE_RISCV32)
#define EFI_IMAGE_MACHINE_RISCV32   0x5032
#endif

#if !defined(EFI_IMAGE_MACHINE_RISCV64)
#define EFI_IMAGE_MACHINE_RISCV64   0x5064
#endif

#if !defined(EFI_IMAGE_MACHINE_RISCV128)
#define EFI_IMAGE_MACHINE_RISCV128  0x5128
#endif

// Image Entry prototype

typedef 
efi_status
(*efi_image_entry_point_t) (
    input efi_handle_t          image_handle,
    input struct efi_system_table_t    *system_table
    );


typedef 
efi_status
(*efi_image_load_t) (
    input bool              boot_policy,
    input efi_handle_t      parent_image_handle,
    input efi_device_path_t *file_path,
    input void              *source_buffer   optional,
    input uint64            source_size,
    output efi_handle_t     *image_handle
    );

typedef 
efi_status
(*efi_image_start_t) (
    input efi_handle_t  image_handle,
    output uint64       *exit_data_size,
    output int16        **exit_data optional
    );

typedef efi_status (*efi_image_unload_t) (input efi_handle_t  ImageHandle);

typedef
efi_status
(*efi_exit_t) (
    input efi_handle_t  image_handle,
    input efi_status    exit_status,
    input uint64        exit_data_size,
    input int16         *exit_data optional
    );

typedef
efi_status
(*efi_exit_boot_services_t) (
    input efi_handle_t  image_handle,
    input uint64        map_key
    );


// Misc
typedef efi_status (*efi_stall_t) (input uint64 microseconds);

typedef
efi_status
(*efi_set_watchdog_timer_t) (
    input uint64    timeout,
    input uint64    watchdog_code,
    input uint64    data_size,
    input int16     *watchdog_data optional
    );

typedef
efi_status
(*efi_connect_controller_t) (
    input efi_handle_t      controller_handle,
    input efi_handle_t      *driver_image_handle optional,
    input efi_device_path_t *remaining_device_path optional,
    input bool              recursive
    );

typedef
efi_status
(*efi_disconnect_controller_t) (
    input efi_handle_t  controller_handle,
    input efi_handle_t  driver_image_handle optional,
    input efi_handle_t  child_handle optional
    );

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL  0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL        0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL       0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER           0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE           0x00000020

typedef
efi_status
(*efi_open_protocol_t) (
    input efi_handle_t  handle,
    input efi_guid_t    *protocol,
    output void         **interface optional,
    input efi_handle_t  agent_handle,
    input efi_handle_t  controller_handle,
    input uint32        attributes
    );

typedef
efi_status
(*efi_close_protocol_t) (
    input efi_handle_t  handle,
    input efi_guid_t    *protocol,
    input efi_handle_t  agent_handle,
    input efi_handle_t  controller_handle
    );

typedef struct {
    efi_handle_t    agent_handle;
    efi_handle_t    controller_handle;
    uint32          attributes;
    uint32          open_count;
} efi_open_protocol_information_entry_t;

typedef
efi_status
(*efi_open_protocol_information_t) (
    input efi_handle_t      handle,
    input efi_guid_t        *protocol,
    output efi_open_protocol_information_entry_t **entry_buffer,
    output uint64           *entry_count
    );

typedef
efi_status
(*efi_protocols_per_handle_t) (
    input efi_handle_t  handle,
    output efi_guid_t   ***protocol_buffer,
    output uint64       *protocol_buffer_count
    );

typedef enum {
    ALL_HANDLES,
    BY_REGISTER_NOTIFY,
    BY_PROTOCOL
} efi_locate_search_type_t;

typedef
efi_status
(*efi_locate_handle_buffer_t) (
    input efi_locate_search_type_t  searchType,
    input efi_guid_t                *protocol   optional,
    input void                      *search_key optional,
    input output uint64             *no_handles,
    output efi_handle_t             **buffer
    );

typedef
efi_status
(*efi_locate_protocol_t) (
    input efi_guid_t    *protocol,
    input void          *registration optional,
    output void         **interface
    );

typedef efi_status (*efi_install_multiple_protocol_interfaces_t)   (input output efi_handle_t *handle, ...);
typedef efi_status (*efi_uninstall_multiple_protocol_interfaces_t) (input output efi_handle_t  handle, ...);

typedef
efi_status
(*efi_calculate_crc32_t) (
    input void      *data,
    input uint64    data_size,
    output uint32   *crc32
    );

typedef
void
(*efi_copy_mem_t) (
    input void      *destination,
    input void      *source,
    input uint64    length
    );

typedef
void
(*efi_set_mem_t) (
    input void      *buffer,
    input uint64    size,
    input uint8     value
    );


typedef
efi_status
(*efi_create_event_ex_t) (
    input uint32                type,
    input efi_tpl_t             notify_tpl,
    input efi_event_notify_t    notify_function optional,
    input const void            *notify_context optional,
    input const efi_guid_t      *event_group    optional,
    output efi_event_t          *event
    );

typedef enum {
    EFI_RESET_COLD,
    EFI_RESET_WARM,
    EFI_RESET_SHUTDOWN
} efi_reset_type_t;

typedef efi_status (*efi_get_next_monotonic_count_t) (output uint64 *count);

typedef struct {
    uint64                     length;
    union {
        efi_physical_addr_t    data_block;
		efi_physical_addr_t    continuation_pointer;
    } group;
} efi_capsule_block_descriptor_t;

typedef struct {
    efi_guid_t          capsule_guid;
    uint32              header_size;
    uint32              flags;
    uint32              capsule_image_size;
} efi_capsule_header_t;

#define CAPSULE_FLAGS_PERSIST_ACROSS_RESET    0x00010000
#define CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE   0x00020000
#define CAPSULE_FLAGS_INITIATE_RESET          0x00040000

// Protocol handler functions
typedef enum {
    EFI_NATIVE_INTERFACE,
    EFI_PCODE_INTERFACE
} efi_interface_type_t;

typedef
efi_status
(*efi_install_protocol_interface_t) (
    input output efi_handle_t       *handle,
    input efi_guid_t                *protocol,
    input efi_interface_type_t      interface_type,
    input void                      *interface
    );

typedef
efi_status
(*efi_reinstall_protocol_interface_t) (
    input efi_handle_t              handle,
    input efi_guid_t                *protocol,
    input void                      *old_interface,
    input void                      *new_interface
    );

typedef
efi_status
(*efi_uninstall_protocol_interface_t) (
    input efi_handle_t          handle,
    input efi_guid_t            *protocol,
    input void                  *interface
    );

typedef
efi_status
(*efi_handle_protocol_t) (
    input efi_handle_t          handle,
    input efi_guid_t            *protocol,
    output void                 **interface
    );

typedef
efi_status
(*efi_register_protocol_notify_t) (
    input efi_guid_t            *protocol,
    input efi_event_t           event,
    output void                 **registration
    );

typedef
efi_status
(*efi_locate_handle_t) (
    input efi_locate_search_type_t  search_type,
    input efi_guid_t                *protocol   optional,
    input void                      *search_key optional,
    input output uint64             *buffer_size,
    output efi_handle_t             *buffer
    );

typedef
efi_status
(*efi_locate_device_path_t) (
    input efi_guid_t                *protocol,
    input output efi_device_path_t  **device_path,
    output efi_handle_t             *device
    );

typedef
efi_status
(*efi_install_configuration_table_t) (
    input efi_guid_t                *guid,
    input void                      *table
    );

typedef efi_status (*efi_reserved_service_t) ();

// Standard EFI table header

typedef struct {
    uint64  signature;
    uint32  revision;
    uint32  header_size;
    uint32  CRC32;
    uint32  reserved;
} efi_table_header_t;


// EFI Runtime Serivces Table

#define EFI_RUNTIME_SERVICES_SIGNATURE  0x56524553544e5552
#define EFI_RUNTIME_SERVICES_REVISION   (EFI_SPECIFICATION_MAJOR_REVISION<<16) | (EFI_SPECIFICATION_MINOR_REVISION)

typedef struct efi_runtime_services_t {
	efi_table_header_t                header;

	// Time services
	struct {
		efi_status (*get_time) (
			output efi_time_t                    *time,
			output efi_time_capabilities_t       *capabilities optional
			);

		efi_status (*set_time) (
			input efi_time_t *time
			);

		efi_status (*get_wakeup_time) (
			output bool         *enabled,
			output bool         *pending,
			output efi_time_t   *time
			);

		efi_status (*set_wakeup_time) (
			input bool         enable,
			input efi_time_t   *time optional
			);
	} time;

	// Virtual memory services
	efi_status (*set_virtual_address_map) (
		input uint64                    memory_map_size,
		input uint64                    descriptor_size,
		input uint32                    descriptor_version,
		input efi_memory_descriptor_t   *virtual_map
		);

	efi_status (*convert_pointer) (
		input uint64        debug_disposition,
		input output void   **address
		);

	// Variable serviers
	struct {
		efi_status (*get_variable) (
			input int16         *variable_name,
			input efi_guid_t    *vendor_guid,
			output uint32       *attributes optional,
			input output uint64 *data_size,
			output void         *data
			);

		efi_status (*get_next_variable_name) (
			input output uint64       *variable_name_size,
			input output int16        *variable_name,
			input output efi_guid_t   *vendor_guid
			);

		efi_status (*set_variable) (
			input int16         *variable_name,
			input efi_guid_t    *vendor_guid,
			input uint32        attributes,
			input uint64        data_size,
			input void          *data
			);
	} variable;

    // Misc
    efi_status (*get_next_high_monotonic_count) (output uint32 *high_count);

    efi_status (*reset_system) (
		input efi_reset_type_t  reset_type,
		input efi_status        reset_status,
		input uint64            data_size,
		input int16             *reset_data optional
		);

    efi_status (*update_capsule) (
		input efi_capsule_header_t  **capsule_header_array,
		input uint64                capsule_count,
		input efi_physical_addr_t   scatter_gather_list optional
		);

    efi_status (*query_capsule_capabilities) (
		input  efi_capsule_header_t     **capsule_header_array,
		input  uint64                   capsule_count,
		output uint64                   *maximum_capsule_size,
		output efi_reset_type_t         *reset_type
		);

    efi_status (*query_variable_info) (
		input  uint32       mttributes,
		output uint64       *maximum_variable_storage_size,
		output uint64       *remaining_variable_storage_size,
		output uint64       *maximum_variable_size
		);
} efi_runtime_services_t;

// EFI Boot Services Table

#define EFI_BOOT_SERVICES_SIGNATURE     0x56524553544f4f42
#define EFI_BOOT_SERVICES_REVISION      (EFI_SPECIFICATION_MAJOR_REVISION<<16) | (EFI_SPECIFICATION_MINOR_REVISION)

typedef struct efi_boot_services_t {
    efi_table_header_t              header;

	// Task priority functions
    efi_tpl_t (*raise_tpl) (input efi_tpl_t  new_tpl);
    void    (*restore_tpl) (input efi_tpl_t  old_tpl);

	// Memory functions
	struct {
		efi_status (*allocate_pages) (
			input efi_allocate_type_t   type,
			input efi_memory_type_t     memory_type,
			input uint64                no_pages,
			output efi_physical_addr_t  *memory
			);

		efi_status (*free_pages) (
			input efi_physical_addr_t  memory,
			input uint64               no_pages
			);

		efi_status (*get_memory_map) (
			input output uint64                     *memory_map_size,
			input output efi_memory_descriptor_t    *memory_map,
			output uint64                           *map_key,
			output uint64                           *descriptor_size,
			output uint32                           *descriptor_version
			);

		efi_status (*allocate_pool) (
			input efi_memory_type_t pool_type,
			input uint64            size,
			output void             **buffer
			);

		efi_status (*free_pool) (
			input void  *buffer
			);
	} memory;

	// Event & timer functions
	struct {
		efi_status (*create_event) (
			input uint32                type,
			input efi_tpl_t             notify_tpl,
			input efi_event_notify_t    notify_function,
			input void                  *notify_context,
			output efi_event_t          *event
			);

		efi_status (*set_timer) (
			input efi_event_t       event,
			input efi_timer_delay_t type,
			input uint64            trigger_time
			);

		efi_status (*wait_for_event) (
			input uint64            number_of_events,
			input efi_event_t       *event,
			output uint64           *index
			);

		efi_status (*signal_event) (input efi_event_t event);
		efi_status (*close_event)  (input efi_event_t event);
		efi_status (*check_event)  (input efi_event_t event);
	} event;

    // Protocol handler functions
    efi_install_protocol_interface_t    install_protocol_interface;
    efi_reinstall_protocol_interface_t  reinstall_protocol_interface;
    efi_uninstall_protocol_interface_t  uninstall_protocol_interface;
    efi_handle_protocol_t               handle_protocol;
    efi_handle_protocol_t               pc_handle_protocol;
    efi_register_protocol_notify_t      register_protocol_notify;
    efi_locate_handle_t                 locate_handle;
    efi_locate_device_path_t            locate_device_path;
    efi_install_configuration_table_t   install_configuration_table;

    // Image functions
    efi_image_load_t                load_image;
    efi_image_start_t               start_image;
    efi_exit_t                      exit;
    efi_image_unload_t              unload_image;
    efi_exit_boot_services_t        exit_boot_services;

    // Misc functions
    efi_get_next_monotonic_count_t  get_next_monotonic_count;
    efi_stall_t                     stall;
    efi_set_watchdog_timer_t        set_watchdog_timer;

    // DriverSupport Services
    efi_connect_controller_t        connect_controller;
    efi_disconnect_controller_t     disconnect_controller;

    // Open and Close Protocol Services
    efi_open_protocol_t             open_protocol;
    efi_close_protocol_t            close_protocol;
    efi_open_protocol_information_t open_protocol_information;

    // Library Services
    efi_protocols_per_handle_t                      protocols_per_handle;
    efi_locate_handle_buffer_t                      locate_handle_buffer;
    efi_locate_protocol_t                           locate_protocol;
    efi_install_multiple_protocol_interfaces_t      install_multiple_protocol_interfaces;
    efi_uninstall_multiple_protocol_interfaces_t    uninstall_multiple_protocol_interfaces;

    // 32-bit CRC Services
    efi_calculate_crc32_t             calculate_crc32;

    // Misc Services
    efi_copy_mem_t                    copy_mem;
    efi_set_mem_t                     set_mem;
    efi_create_event_ex_t             create_event_ex;
} efi_boot_services_t;

// EFI Configuration Table and GUID definitions

#define MPS_TABLE_GUID    \
    { 0xeb9d2d2f, 0x2d88, 0x11d3, {0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d} }

#define ACPI_TABLE_GUID    \
    { 0xeb9d2d30, 0x2d88, 0x11d3, {0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d} }

#define ACPI_20_TABLE_GUID  \
    { 0x8868e871, 0xe4f1, 0x11d3, {0xbc, 0x22, 0x0, 0x80, 0xc7, 0x3c, 0x88, 0x81} }

#define SMBIOS_TABLE_GUID    \
    { 0xeb9d2d31, 0x2d88, 0x11d3, {0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d} }

#define SMBIOS3_TABLE_GUID    \
    { 0xf2fd1544, 0x9794, 0x4a2c, {0x99, 0x2e, 0xe5, 0xbb, 0xcf, 0x20, 0xe3, 0x94} }

#define SAL_SYSTEM_TABLE_GUID    \
    { 0xeb9d2d32, 0x2d88, 0x11d3, {0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d} }

#define EFI_DTB_TABLE_GUID \
    { 0xb1b621d5, 0xf19c, 0x41a5, {0x83, 0x0b, 0xd9, 0x15, 0x2c, 0x69, 0xaa, 0xe0} }

typedef struct {
    efi_guid_t  vendor_guid;
    void        *vendor_table;
} efi_configuration_table_t;


#define EFI_SYSTEM_TABLE_SIGNATURE      0x5453595320494249
#define EFI_SYSTEM_TABLE_REVISION      (EFI_SPECIFICATION_MAJOR_REVISION<<16) | (EFI_SPECIFICATION_MINOR_REVISION)

typedef struct efi_system_table_t {
    efi_table_header_t              header;

    int16                           *firmware_vendor;
    uint32                          firmware_revision;
	uint32                          __padding;

    efi_handle_t                    console_in_handle;
    simple_input_interface_t        *cin;

    efi_handle_t                    console_out_handle;
    simple_text_output_interface_t  *cout;

    efi_handle_t                    standard_error_handle;
    simple_text_output_interface_t  *stderr;

    efi_runtime_services_t          *runtime_services;
    efi_boot_services_t             *boot_services;

    uint64                          number_of_table_entries;
    efi_configuration_table_t       *configuration_table;

} efi_system_table_t;

#endif // !EFI_ELF_API
