#ifndef EFI_ELF_ELF

#define Elf_Ident_Magic "\177ELF"
char a[] = Elf_Ident_Magic;

#include "types.h"

typedef enum : uint8 {
    ELFCLASSNONE = 0x00,
    ELFCLASS32   = 0x01,
    ELFCLASS64   = 0x02,
} ei_class_t;

typedef enum : uint8 {
    ELFDATANONE = 0x00,  /* Unknown */
    ELFDATA2LSB = 0x01,  /* 2's complement little-endian */
    ELFDATA2MSB = 0x02,  /* 2's complement big-endian */
} ei_data_t;

typedef enum : uint8 {
    NONE    = 0x00,
    CURRENT = 0x01,
} ei_version_t;

typedef enum : uint8 {
    EI_OSABI_SYSV           = 0x00,
    EI_OSABI_HPUX           = 0x01,
    EI_OSABI_NetBSD         = 0x02,
    EI_OSABI_Linux          = 0x03,
    EI_OSABI_GNUHurd        = 0x04,
    EI_OSABI_Solaris        = 0x06,
    EI_OSABI_AIX            = 0x07,
    EI_OSABI_IRIX           = 0x08,
    EI_OSABI_FreeBSD        = 0x09,
    EI_OSABI_Tru64          = 0x0A,
    EI_OSABI_NovellModesto  = 0x0B,
    EI_OSABI_OpenBSD        = 0x0C,
    EI_OSABI_OpenVMS        = 0x0D,
    EI_OSABI_NonStopKernel  = 0x0E,
    EI_OSABI_AROS           = 0x0F,
    EI_OSABI_FenixOS        = 0x10,
    EI_OSABI_CloudABI       = 0x11,
    EI_OSABI_OpenVOS        = 0x12,
    EI_OSABI_ARM_EABI       = 0x40,
    EI_OSABI_STANDALONE     = 0xFF,
} ei_osabi_t;

/* Elf Ident magic length */
#define EI_NIDENT 4

typedef struct e_ident_t {
	uchar           ei_magic[EI_NIDENT];
	ei_class_t      ei_class;       /* CPU architecture, bit size */
	ei_data_t       ei_data;        /* data encoding of the processor-specific data, invalid, little endian, big endian */
	ei_version_t    ei_version;     /* ELF version */
	ei_osabi_t      ei_osabi;       /* operating system and ABI */
	uint8           ei_abiversion;  /* ABI version */
	uint8           padding[7];
} e_ident_t;

typedef enum : uint16 {
    ET_NONE = 0x0000,  /* An unknown type */
    ET_REL  = 0x0001,  /* A relocatable file. */
    ET_EXEC = 0x0002,  /* An executable file. */
    ET_DYN  = 0x0003,  /* A shared object. */
    ET_CORE = 0x0004,  /* A core file. */
} e_type_t;

typedef enum : uint16 {
    EM_NONE          = 0x0000,
    EM_M32           = 0x0001,
    EM_SPARC         = 0x0002,
    EM_386           = 0x0003,
    EM_68K           = 0x0004,
    EM_88K           = 0x0005,
    EM_IAMCU         = 0x0006,
    EM_860           = 0x0007,
    EM_MIPS          = 0x0008,
    EM_S370          = 0x0009,
    EM_MIPS_RS4_BE   = 0x000a,
    EM_PARISC        = 0x000f,
    EM_VPP500        = 0x0011,
    EM_SPARC32PLUS   = 0x0012,
    EM_960           = 0x0013,
    EM_PPC           = 0x0014,
    EM_PPC64         = 0x0015,
    EM_S390          = 0x0016,
    EM_SPU           = 0x0017,
    EM_V800          = 0x0024,
    EM_FR20          = 0x0025,
    EM_RH32          = 0x0026,
    EM_RCE           = 0x0027,
    EM_ARM           = 0x0028,
    EM_ALPHA         = 0x0029,
    EM_SH            = 0x002A,
    EM_SPARCV9       = 0x002B,
    EM_TRICORE       = 0x002C,
    EM_ARC           = 0x002D,
    EM_H8_300        = 0x002E,
    EM_H8_300H       = 0x002F,
    EM_H8S           = 0x0030,
    EM_H8_500        = 0x0031,
    EM_IA_64         = 0x0032,
    EM_MIPS_X        = 0x0033,
    EM_COLDFIRE      = 0x0034,
    EM_68HC12        = 0x0035,
    EM_MMA           = 0x0036,
    EM_PCP           = 0x0037,
    EM_NCPU          = 0x0038,
    EM_NDR1          = 0x0039,
    EM_STARCORE      = 0x003A,
    EM_ME16          = 0x003B,
    EM_ST100         = 0x003C,
    EM_TINYJ         = 0x003D,
    EM_X86_64        = 0x003E,
    EM_PDSP          = 0x003F,
    EM_PDP10         = 0x0040,
    EM_PDP11         = 0x0041,
    EM_FX66          = 0x0042,
    EM_ST9PLUS       = 0x0043,
    EM_ST7           = 0x0044,
    EM_68HC16        = 0x0045,
    EM_68HC11        = 0x0046,
    EM_68HC08        = 0x0047,
    EM_68HC05        = 0x0048,
    EM_SVX           = 0x0049,
    EM_ST19          = 0x004A,
    EM_VAX           = 0x004B,
    EM_CRIS          = 0x004C,
    EM_JAVELIN       = 0x004D,
    EM_FIREPATH      = 0x004E,
    EM_ZSP           = 0x004F,
    EM_MMIX          = 0x0050,
    EM_HUANY         = 0x0051,
    EM_PRISM         = 0x0052,
    EM_AVR           = 0x0053,
    EM_FR30          = 0x0054,
    EM_D10V          = 0x0055,
    EM_D30V          = 0x0056,
    EM_V850          = 0x0057,
    EM_M32R          = 0x0058,
    EM_MN10300       = 0x0059,
    EM_MN10200       = 0x005A,
    EM_PJ            = 0x005B,
    EM_OPENRISC      = 0x005C,
    EM_ARC_COMPACT   = 0x005D,
    EM_XTENSA        = 0x005E,
    EM_VIDEOCORE     = 0x005F,
    EM_TMM_GPP       = 0x0060,
    EM_NS32K         = 0x0061,
    EM_TPC           = 0x0062,
    EM_SNP1K         = 0x0063,
    EM_ST200         = 0x0064,
    EM_IP2K          = 0x0065,
    EM_MAX           = 0x0066,
    EM_CR            = 0x0067,
    EM_F2MC16        = 0x0068,
    EM_MSP430        = 0x0069,
    EM_BLACKFIN      = 0x006A,
    EM_SE_C33        = 0x006B,
    EM_SEP           = 0x006C,
    EM_ARCA          = 0x006D,
    EM_UNICORE       = 0x006E,
    EM_EXCESS        = 0x006F,
    EM_DXP           = 0x0070,
    EM_ALTERA_NIOS2  = 0x0071,
    EM_CRX           = 0x0072,
    EM_XGATE         = 0x0073,
    EM_C166          = 0x0074,
    EM_M16C          = 0x0075,
    EM_DSPIC30F      = 0x0076,
    EM_CE            = 0x0077,
    EM_M32C          = 0x0078,
    EM_TSK3000       = 0x0083,
    EM_RS08          = 0x0084,
    EM_SHARC         = 0x0085,
    EM_ECOG2         = 0x0086,
    EM_SCORE7        = 0x0087,
    EM_DSP24         = 0x0088,
    EM_VIDEOCORE3    = 0x0089,
    EM_LATTICEMICO32 = 0x008A,
    EM_SE_C17        = 0x008B,
    EM_TI_C6000      = 0x008C,
    EM_TI_C2000      = 0x008D,
    EM_TI_C5500      = 0x008E,
    EM_TI_ARP32      = 0x008F,
    EM_TI_PRU        = 0x0090,
    EM_MMDSP_PLUS    = 0x00A0,
    EM_CYPRESS_M8C   = 0x00A1,
    EM_R32C          = 0x00A2,
    EM_TRIMEDIA      = 0x00A3,
    EM_QDSP6         = 0x00A4,
    EM_8051          = 0x00A5,
    EM_STXP7X        = 0x00A6,
    EM_NDS32         = 0x00A7,
    EM_ECOG1         = 0x00A8,
    EM_ECOG1X        = 0x00A8,
    EM_MAXQ30        = 0x00A9,
    EM_XIMO16        = 0x00AA,
    EM_MANIK         = 0x00AB,
    EM_CRAYNV2       = 0x00AC,
    EM_RX            = 0x00AD,
    EM_METAG         = 0x00AE,
    EM_MCST_ELBRUS   = 0x00AF,
    EM_ECOG16        = 0x00B0,
    EM_CR16          = 0x00B1,
    EM_ETPU          = 0x00B2,
    EM_SLE9X         = 0x00B3,
    EM_L10M          = 0x00B4,
    EM_K10M          = 0x00B5,
    EM_AARCH64       = 0x00B7,
    EM_AVR32         = 0x00B9,
    EM_STM8          = 0x00BA,
    EM_TILE64        = 0x00BB,
    EM_TILEPRO       = 0x00BC,
    EM_MICROBLAZE    = 0x00BD,
    EM_CUDA          = 0x00BE,
    EM_TILEGX        = 0x00BF,
    EM_CLOUDSHIELD   = 0x00C0,
    EM_COREA_1ST     = 0x00C1,
    EM_COREA_2ND     = 0x00C2,
    EM_ARC_COMPACT2  = 0x00C3,
    EM_OPEN8         = 0x00C4,
    EM_RL78          = 0x00C5,
    EM_VIDEOCORE5    = 0x00C6,
    EM_78KOR         = 0x00C7,
    EM_56800EX       = 0x00C8,
    EM_BA1           = 0x00C9,
    EM_BA2           = 0x00CA,
    EM_XCORE         = 0x00CB,
    EM_MCHP_PIC      = 0x00CC,
    EM_INTEL205      = 0x00CD,
    EM_INTEL206      = 0x00CE,
    EM_INTEL207      = 0x00CF,
    EM_INTEL208      = 0x00D0,
    EM_INTEL209      = 0x00D1,
    EM_KM32          = 0x00D2,
    EM_KMX32         = 0x00D3,
    EM_KMX16         = 0x00D4,
    EM_KMX8          = 0x00D5,
    EM_KVARC         = 0x00D6,
    EM_CDP           = 0x00D7,
    EM_COGE          = 0x00D8,
    EM_COOL          = 0x00D9,
    EM_NORC          = 0x00DA,
    EM_CSR_KALIMBA   = 0x00DB,
    EM_Z80           = 0x00DC,
    EM_VISIUM        = 0x00DD,
    EM_FT32          = 0x00DE,
    EM_MOXIE         = 0x00DF,
    EM_AMDGPU        = 0x00E0,
    EM_RISCV         = 0x00F3,
} e_machine_t;

/* Values for Elf64_Ehdr.e_flags.  */
typedef enum {
	EF_SPARCV9_MM     = 3,
	EF_SPARCV9_TSO    = 0,
	EF_SPARCV9_PSO    = 1,
	EF_SPARCV9_RMO    = 2,
	EF_SPARC_LEDATA   = 0x800000, /* little endian data */
	EF_SPARC_EXT_MASK = 0xFFFF00,
	EF_SPARC_32PLUS   = 0x000100, /* generic V8+ features */
	EF_SPARC_SUN_US1  = 0x000200, /* Sun UltraSPARC1 extensions */
	EF_SPARC_HAL_R1   = 0x000400, /* HAL R1 extensions */
	EF_SPARC_SUN_US3  = 0x000800, /* Sun UltraSPARCIII extensions */
} e_flags_t;

typedef struct {
	e_ident_t       e_ident;
	e_type_t        e_type;          /* object file type */
	e_machine_t     e_machine;       /* Architechture */
	uint32          e_version;
#ifdef __x86_64
	uint64          e_entry;
#else
	uint32          e_entry;
#endif /* ifdef __x86_64 */

#ifdef __x86_64
	uint64          e_phoff;        /* Program header location/ starting point */
	uint64          e_shoff;        /* Section header location/ starting point */
#else
	uint32          e_phoff;
	uint32          e_shoff;
#endif /* ifdef __x86_64 */

	e_flags_t       e_flags;         /* Processor flags */
	uint16          e_ehsize;        /* ELF header size */
	uint16          e_phentsize;     /* Program header size */
	uint16          e_phnum;         /* Program header count */
	uint16          e_shentsize;     /* Section header size */
	uint16          e_shnum;         /* Section header count */
	uint16          e_shstrndx;      /* Section header last index */
} Elf64_hdr_t;

typedef enum : uint32 {
	PT_NULL         = 0x00,
    PT_LOAD         = 0x01,
    PT_DYNAMIC      = 0x02,
    PT_INTERP       = 0x03,
    PT_NOTE         = 0x04,
    PT_SHLIB        = 0x05,
    PT_PHDR         = 0x06,
    PT_TLS          = 0x07,
    PT_LOOS         = 0x60000000,
    PT_HIOS         = 0x6FFFFFFF,
    PT_GNU_EH_FRAME = PT_LOOS + 0x474E550,
    PT_GNU_STACK    = PT_LOOS + 0x474E551,
    PT_GNU_RELRO    = PT_LOOS + 0x474E552,
    PT_GNU_PROPERTY = PT_LOOS + 0x474E553,
    PT_SUNWBSS      = 0x6FFFFFFA,
    PT_SUNWSTACK    = 0x6FFFFFFB,
    PT_ARM_ARCHEXT  = 0x70000000,
    PT_ARM_UNWIND   = 0x70000001,
} p_type_t;

/* Legal values for p_flags (segment flags).  */

typedef enum : uint32 {
	PF_X        = (1 << 0),     /* Segment is executable */
	PF_W        = (1 << 1),     /* Segment is writable */
	PF_R        = (1 << 2),     /* Segment is readable */
	PF_MASKOS   = 0x0ff00000,   /* OS-specific */
	PF_MASKPROC = 0xf0000000,   /* Processor-specific */
} p_flags_t;

typedef struct {
	p_type_t    p_type;
	uint32      p_flags;
	uint64      p_offset;

	uint64      p_phy_addr;
	uint64      p_vir_addr;

	uint64      p_filesz;
	uint64      p_memsz;
	uint64      p_align;
} Elf64_phdr_t;

typedef enum : uint64 {
	SHF_WRITE            = (1 << 0),    /* Writable */
	SHF_ALLOC            = (1 << 1),    /* Occupies memory during execution */
	SHF_EXECINSTR        = (1 << 2),    /* Executable */
	SHF_MERGE            = (1 << 4),    /* Might be merged */
	SHF_STRINGS          = (1 << 5),    /* Contains nul-terminated strings */
	SHF_INFO_LINK        = (1 << 6),    /* `sh_info' contains SHT index */
	SHF_LINK_ORDER       = (1 << 7),    /* Preserve order after combining */
	SHF_OS_NONCONFORMING = (1 << 8),    /* Non-standard OS specific handling required */
	SHF_GROUP            = (1 << 9),    /* Section is member of a group.  */
	SHF_TLS              = (1 << 10),   /* Section hold thread-local data.  */
	SHF_COMPRESSED       = (1 << 11),   /* Section with compressed data. */
	SHF_MASKOS           = 0x0ff00000,  /* OS-specific.  */
	SHF_MASKPROC         = 0xf0000000,  /* Processor-specific */
	SHF_GNU_RETAIN       = (1 << 21),   /* Not to be GCed by linker.  */
	SHF_ORDERED          = (1 << 30),   /* Special ordering requirement (Solaris).  */
	SHF_EXCLUDE          = (1U << 31),  /* Section is excluded unless referenced or allocated (Solaris).*/
} sh_flags_t;

typedef enum : uint32 {
    SH_NULL                   = 0x00,
    SH_PROGBITS               = 0x01,
    SH_SYMTAB                 = 0x02,
    SH_STRTAB                 = 0x03,
    SH_RELA                   = 0x04,
    SH_HASH                   = 0x05,
    SH_DYNAMIC                = 0x06,
    SH_NOTE                   = 0x07,
    SH_NOBITS                 = 0x08,
    SH_REL                    = 0x09,
    SH_SHLIB                  = 0x0A,
    SH_DYNSYM                 = 0x0B,
    SH_UNKNOWN12              = 0x0C,
    SH_UNKNOWN13              = 0x0D,
    SH_INIT_ARRAY             = 0x0E,
    SH_FINI_ARRAY             = 0x0F,
    SH_PREINIT_ARRAY          = 0x10,
    SH_GROUP                  = 0x11,
    SH_SYMTAB_SHNDX           = 0x12,
    SH_GNU_INCREMENTAL_INPUTS = 0x6FFF4700,
    SH_GNU_ATTRIBUTES         = 0x6FFFFFF5,
    SH_GNU_HASH               = 0x6FFFFFF6,
    SH_GNU_LIBLIST            = 0x6FFFFFF7,
    SH_CHECKSUM               = 0x6FFFFFF8,
    SH_SUNW_move              = 0x6FFFFFFA,
    SH_SUNW_COMDAT            = 0x6FFFFFFB,
    SH_SUNW_syminfo           = 0x6FFFFFFC,
    SH_GNU_verdef             = 0x6FFFFFFD,
    SH_GNU_verneed            = 0x6FFFFFFE,
    SH_GNU_versym             = 0x6FFFFFFF,
    SH_ARM_EXIDX              = 0x70000001,
    SH_ARM_PREEMPTMAP         = 0x70000002,
    SH_ARM_ATTRIBUTES         = 0x70000003,
    SH_ARM_DEBUGOVERLAY       = 0x70000004,
    SH_ARM_OVERLAYSECTION     = 0x70000005,
} sh_type_t;

typedef struct {
	uint32        sh_name;
	sh_type_t     sh_type;
	sh_flags_t    sh_flags;

	uint64        sh_addr;
	uint64        sh_offset;

	uint64        sh_size;
	uint32        sh_link;
	uint32        sh_info;

	uint64        sh_addralign;
	uint64        sh_entrysize;
} Elf64_shdr_t;

typedef enum : uint8 {
    DEFAULT   = 0x00,
    INTERNAL  = 0x01,
    HIDDEN    = 0x02,
    PROTECTED = 0x03,
} stv_t;

typedef enum : uint8 {
    STT_NOTYPE     = 0,  /* Symbol type is unspecified */
    STT_OBJECT     = 1,  /* Symbol is a data object */
    STT_FUNC       = 2,  /* Symbol is a code object */
    STT_SECTION    = 3,  /* Symbol associated with a section */
    STT_FILE       = 4,  /* Symbol's name is file name */
    STT_COMMON     = 5,  /* Symbol is a common data object */
    STT_TLS        = 6,  /* Symbol is thread-local data object */
    STT_NUM        = 7,  /* Number of defined types. */
    STT_LOOS       = 10, /* Start of OS-specific */
    STT_GNU_IFUNC  = 10, /* Symbol is indirect code object */
    STT_HIOS       = 12, /* End of OS-specific */
    STT_LOPROC     = 13, /* Start of processor-specific */
    STT_HIPROC     = 15, /* End of processor-specific */
} st_type_t;

typedef enum : uint8 {
    STB_LOCAL       = 0,  /* Local symbol */
    STB_GLOBAL      = 1,  /* Global symbol */
    STB_WEAK        = 2,  /* Weak symbol */
    STB_NUM         = 3,  /* Number of defined types. */
    STB_LOOS        = 10, /* Start of OS-specific */
    STB_GNU_UNIQUE  = 10, /* Unique symbol. */
    STB_HIOS        = 12, /* End of OS-specific */
    STB_LOPROC      = 13, /* Start of processor-specific */
    STB_HIPROC      = 15, /* End of processor-specific */
} st_bind_t;

typedef struct {
	st_bind_t st_bind : 4;
    st_type_t st_type : 4;
} st_info_t;

typedef struct {
	uint32      st_name;
    st_info_t   st_info;
    stv_t       st_other;
    uint16      st_shndx;
    uint64      st_value;
    uint64      st_size;
} Elf64_sym_t;

typedef struct {
	Elf64_phdr_t *phdr;
} Elf64_body_t;

typedef struct {
	Elf64_hdr_t  head;
	Elf64_body_t body;
} Elf64;

#endif /* ifndef EFI_ELF_ELF */
