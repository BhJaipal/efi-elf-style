#ifndef EFI_ELF_SER_H
#define EFI_ELF_SER_H
#include "types.h"

// Serial protocol

#define EFI_SERIAL_IO_PROTOCOL_GUID \
    { 0xBB25CF6F, 0xF1D4, 0x11D2, {0x9A, 0x0C, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0xFD} }

typedef struct efi_serial_io_protocol_t efi_serial_io_protocol_t;

typedef enum {
    DEFAULT_PARITY,
    NO_PARITY,
    EVEN_PARITY,
    ODD_PARITY,
    MARK_PARITY,
    SPACE_PARITY
} efi_parity_type_t;

typedef enum {
    DEFAULT_STOP_BITS,
    ONE_STOP_BIT,         // 1 stop bit
    ONE_FIVE_STOP_BITS,    // 1.5 stop bits
    TWO_STOP_BITS         // 2 stop bits
} efi_stop_bits_type_t;

#define CLEAR_TO_SEND                   0x0010  // RO
#define DATA_SET_READY                  0x0020  // RO
#define RING_INDICATE                   0x0040  // RO
#define CARRIER_DETECT                  0x0080  // RO
#define REQUEST_TO_SEND                 0x0002  // WO
#define DATA_TERMINAL_READY             0x0001  // WO
#define INPUT_BUFFER_EMPTY              0x0100  // RO
#define OUTPUT_BUFFER_EMPTY             0x0200  // RO
#define HARDWARE_LOOPBACK_ENABLE        0x1000  // RW
#define SOFTWARE_LOOPBACK_ENABLE        0x2000  // RW
#define HARDWARE_FLOW_CONTROL_ENABLE    0x4000  // RW


typedef struct {
    uint32                  control_mask;

    // current Attributes
    uint32                  timeout;
    uint64                  baud_rate;
    uint32                  receive_fifo_depth;
    uint32                  data_bits;
    uint32                  parity;
    uint32                  stop_bits;
} efi_serial_io_mode_t;

#define SERIAL_IO_INTERFACE_REVISION    0x00010000

typedef struct efi_serial_io_protocol_t {
    uint32                       Revision;

	efi_status (*reset) (
		input efi_serial_io_protocol_t  *This);

	efi_status (*set_attributes) (
		input efi_serial_io_protocol_t  *This,
		input uint64                    baud_rate,
		input uint32                    receive_fifo_depth,
		input uint32                    timeout,
		input efi_parity_type_t         parity,
		input uint8                     data_bits,
		input efi_stop_bits_type_t      stop_bits
		);

	efi_status (*set_control_bits) (
		input efi_serial_io_protocol_t  *This,
		input uint32                    control
		);

	efi_status (*get_control_bits) (
		input efi_serial_io_protocol_t  *This,
		output uint32                   *control
		);

	efi_status (*write) (
		input efi_serial_io_protocol_t  *This,
		input output uint64             *buffer_size,
		input void                      *buffer
		);

	efi_status (*read) (
		input efi_serial_io_protocol_t  *This,
		input output uint64             *buffer_size,
		output void                     *buffer
		);

    efi_serial_io_mode_t               *Mode;
} efi_serial_io_protocol_t;

typedef efi_serial_io_protocol_t serial_io_interface_t;

#endif /* ifndef EFI_ELF_SER_H */
