#ifndef EFI_ELF_CON
#define EFI_ELF_CON

#include "types.h"


#define EFI_SIMPLE_TEXT_output_PROTOCOL_GUID \
    { 0x387477c2, 0x69c7, 0x11d2, {0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b} }
#define SIMPLE_TEXT_output_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID

struct simple_text_output_interface_t;

typedef
efi_status
(*efi_text_reset_t) (
    input struct simple_text_output_interface_t *This,
    input bool                                  extended_verification
    );

typedef
efi_status
(*efi_text_output_string_t) (
    input struct simple_text_output_interface_t *This,
    input int16                                 *string
    );

typedef
efi_status
(*efi_text_test_string_t) (
    input struct simple_text_output_interface_t *This,
    input int16                                 *string
    );

typedef
efi_status
(*efi_text_query_mode_t) (
    input struct simple_text_output_interface_t *This,
    input uint64                                mode_number,
    output uint64                               *columns,
    output uint64                               *rows
    );

typedef
efi_status
(*efi_text_set_mode_t) (
    input struct simple_text_output_interface_t *This,
    input uint64                                mode_number
    );

typedef
efi_status
(*efi_text_set_attribute_t) (
    input struct simple_text_output_interface_t *This,
    input uint64                                attribute
    );

#define EFI_BLACK   0x00
#define EFI_BLUE    0x01
#define EFI_GREEN   0x02
#define EFI_CYAN            (EFI_BLUE | EFI_GREEN)
#define EFI_RED     0x04
#define EFI_MAGENTA         (EFI_BLUE | EFI_RED)
#define EFI_BROWN           (EFI_GREEN | EFI_RED)
#define EFI_LIGHTGRAY       (EFI_BLUE | EFI_GREEN | EFI_RED)
#define EFI_BRIGHT  0x08
#define EFI_DARKGRAY        (EFI_BRIGHT)
#define EFI_LIGHTBLUE       (EFI_BLUE | EFI_BRIGHT)
#define EFI_LIGHTGREEN      (EFI_GREEN | EFI_BRIGHT)
#define EFI_LIGHTCYAN       (EFI_CYAN | EFI_BRIGHT)
#define EFI_LIGHTRED        (EFI_RED | EFI_BRIGHT)
#define EFI_LIGHTMAGENTA    (EFI_MAGENTA | EFI_BRIGHT)
#define EFI_YELLOW          (EFI_BROWN | EFI_BRIGHT)
#define EFI_WHITE           (EFI_BLUE | EFI_GREEN | EFI_RED | EFI_BRIGHT)

#define EFI_TEXT_ATTR(f,b)  ((f) | ((b) << 4))

#define EFI_BACKGROUND_BLACK        0x00
#define EFI_BACKGROUND_BLUE         0x10
#define EFI_BACKGROUND_GREEN        0x20
#define EFI_BACKGROUND_CYAN         (EFI_BACKGROUND_BLUE | EFI_BACKGROUND_GREEN)
#define EFI_BACKGROUND_RED          0x40
#define EFI_BACKGROUND_MAGENTA      (EFI_BACKGROUND_BLUE | EFI_BACKGROUND_RED)
#define EFI_BACKGROUND_BROWN        (EFI_BACKGROUND_GREEN | EFI_BACKGROUND_RED)
#define EFI_BACKGROUND_LIGHTGRAY    (EFI_BACKGROUND_BLUE | EFI_BACKGROUND_GREEN | EFI_BACKGROUND_RED)


typedef
efi_status
(*efi_text_clear_screen_t) (
    input struct simple_text_output_interface_t     *This
    );

typedef
efi_status
(*efi_text_set_cursor_position_t) (
    input struct simple_text_output_interface_t     *This,
    input uint64                                    column,
    input uint64                                    row
    );

typedef
efi_status
(*efi_text_enable_cursor_t) (
    input struct simple_text_output_interface_t     *This,
    input bool                                      enable
    );

typedef struct {
    int32   max_mode;
    // current settings
    int32   mode;
    int32   attribute;
    int32   cursor_column;
    int32   cursor_row;
    bool    cursor_visible;
} simple_text_output_mode_t;

typedef struct simple_text_output_interface_t {
    efi_text_reset_t            reset;

    efi_text_output_string_t    output_string;
    efi_text_test_string_t      test_string;

    efi_text_query_mode_t       query_mode;
    efi_text_set_mode_t         set_mode;
    efi_text_set_attribute_t    set_attribute;

    efi_text_clear_screen_t           clear_screen;
    efi_text_set_cursor_position_t    set_cursor_position;
    efi_text_enable_cursor_t          enable_cursor;

    // Current mode
    simple_text_output_mode_t         *mode;
} simple_text_output_interface_t;

typedef simple_text_output_interface_t efi_simple_text_out_protocol_t;

//
// Define's for required EFI Unicode Box Draw character
//

#define BOXDRAW_HORIZONTAL                  0x2500
#define BOXDRAW_VERTICAL                    0x2502
#define BOXDRAW_DOWN_RIGHT                  0x250c
#define BOXDRAW_DOWN_LEFT                   0x2510
#define BOXDRAW_UP_RIGHT                    0x2514
#define BOXDRAW_UP_LEFT                     0x2518
#define BOXDRAW_VERTICAL_RIGHT              0x251c
#define BOXDRAW_VERTICAL_LEFT               0x2524
#define BOXDRAW_DOWN_HORIZONTAL             0x252c
#define BOXDRAW_UP_HORIZONTAL               0x2534
#define BOXDRAW_VERTICAL_HORIZONTAL         0x253c

#define BOXDRAW_DOUBLE_HORIZONTAL           0x2550
#define BOXDRAW_DOUBLE_VERTICAL             0x2551
#define BOXDRAW_DOWN_RIGHT_DOUBLE           0x2552
#define BOXDRAW_DOWN_DOUBLE_RIGHT           0x2553
#define BOXDRAW_DOUBLE_DOWN_RIGHT           0x2554

#define BOXDRAW_DOWN_LEFT_DOUBLE            0x2555
#define BOXDRAW_DOWN_DOUBLE_LEFT            0x2556
#define BOXDRAW_DOUBLE_DOWN_LEFT            0x2557

#define BOXDRAW_UP_RIGHT_DOUBLE             0x2558
#define BOXDRAW_UP_DOUBLE_RIGHT             0x2559
#define BOXDRAW_DOUBLE_UP_RIGHT             0x255a

#define BOXDRAW_UP_LEFT_DOUBLE              0x255b
#define BOXDRAW_UP_DOUBLE_LEFT              0x255c
#define BOXDRAW_DOUBLE_UP_LEFT              0x255d

#define BOXDRAW_VERTICAL_RIGHT_DOUBLE       0x255e
#define BOXDRAW_VERTICAL_DOUBLE_RIGHT       0x255f
#define BOXDRAW_DOUBLE_VERTICAL_RIGHT       0x2560

#define BOXDRAW_VERTICAL_LEFT_DOUBLE        0x2561
#define BOXDRAW_VERTICAL_DOUBLE_LEFT        0x2562
#define BOXDRAW_DOUBLE_VERTICAL_LEFT        0x2563

#define BOXDRAW_DOWN_HORIZONTAL_DOUBLE      0x2564
#define BOXDRAW_DOWN_DOUBLE_HORIZONTAL      0x2565
#define BOXDRAW_DOUBLE_DOWN_HORIZONTAL      0x2566

#define BOXDRAW_UP_HORIZONTAL_DOUBLE        0x2567
#define BOXDRAW_UP_DOUBLE_HORIZONTAL        0x2568
#define BOXDRAW_DOUBLE_UP_HORIZONTAL        0x2569

#define BOXDRAW_VERTICAL_HORIZONTAL_DOUBLE  0x256a
#define BOXDRAW_VERTICAL_DOUBLE_HORIZONTAL  0x256b
#define BOXDRAW_DOUBLE_VERTICAL_HORIZONTAL  0x256c

//
// EFI Required Block Elements Code Chart
//

#define BLOCKELEMENT_FULL_BLOCK             0x2588
#define BLOCKELEMENT_LIGHT_SHADE            0x2591
//
// EFI Required Geometric Shapes Code Chart
//

#define GEOMETRICSHAPE_UP_TRIANGLE           0x25b2
#define GEOMETRICSHAPE_RIGHT_TRIANGLE        0x25ba
#define GEOMETRICSHAPE_DOWN_TRIANGLE         0x25bc
#define GEOMETRICSHAPE_LEFT_TRIANGLE         0x25c4

//
// EFI Required Arrow shapes
//

#define ARROW_UP                            0x2191
#define ARROW_DOWN                          0x2193

//
// Text input protocol
//

#define EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID \
    { 0x387477c1, 0x69c7, 0x11d2, {0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b} }
#define SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID

struct _SIMPLE_INPUT_intERFACE;

typedef struct {
    uint16                              scan_code;
    int16                              unicode_char;
} efi_input_key_t;

//
// Baseline unicode control chars
//

#define CHAR_NULL                       0x0000
#define CHAR_BACKSPACE                  0x0008
#define CHAR_TAB                        0x0009
#define CHAR_LINEFEED                   0x000A
#define CHAR_CARRIAGE_RETURN            0x000D

//
// Scan codes for base line keys
//

#define SCAN_NULL                       0x0000
#define SCAN_UP                         0x0001
#define SCAN_DOWN                       0x0002
#define SCAN_RIGHT                      0x0003
#define SCAN_LEFT                       0x0004
#define SCAN_HOME                       0x0005
#define SCAN_END                        0x0006
#define SCAN_INSERT                     0x0007
#define SCAN_DELETE                     0x0008
#define SCAN_PAGE_UP                    0x0009
#define SCAN_PAGE_DOWN                  0x000A
#define SCAN_F1                         0x000B
#define SCAN_F2                         0x000C
#define SCAN_F3                         0x000D
#define SCAN_F4                         0x000E
#define SCAN_F5                         0x000F
#define SCAN_F6                         0x0010
#define SCAN_F7                         0x0011
#define SCAN_F8                         0x0012
#define SCAN_F9                         0x0013
#define SCAN_F10                        0x0014
#define SCAN_F11                        0x0015
#define SCAN_F12                        0x0016
#define SCAN_ESC                        0x0017

struct simple_input_interface_t;

typedef
efi_status
(*efi_input_reset_t) (
    input struct simple_input_interface_t   *This,
    input bool                          extended_verification
    );

typedef
efi_status
(*efi_input_read_key_t) (
    input struct simple_input_interface_t   *This,
    output efi_input_key_t                   *key
    );

typedef struct simple_input_interface_t {
    efi_input_reset_t       reset;
    efi_input_read_key_t    read_key_stroke;
    efi_event_t             wait_for_key;
} simple_input_interface_t;

typedef simple_input_interface_t efi_simple_text_in_protocol_t;

#endif // !EFI_ELF_CON
