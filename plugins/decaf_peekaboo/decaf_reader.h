#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define MAX_NUM_OPERANDS 30 // FNSAVE has a memory operand of 108 bytes
#define MAX_NUM_MEMREGS 5 
#define MAX_NUM_TAINTBYTE_RECORDS 3
#define MAX_STRING_LEN 32
#define MAX_OPERAND_LEN 8
#define MAX_INSN_BYTES 15

/*** Define Registers ***/

/* segment registers */
#define es_reg 100
#define cs_reg 101
#define ss_reg 102
#define ds_reg 103
#define fs_reg 104
#define gs_reg 105

/* address-modifier dependent registers */
#define eAX_reg 108
#define eCX_reg 109
#define eDX_reg 110
#define eBX_reg 111
#define eSP_reg 112
#define eBP_reg 113
#define eSI_reg 114
#define eDI_reg 115

/* 8-bit registers */
#define al_reg 116
#define cl_reg 117
#define dl_reg 118
#define bl_reg 119
#define ah_reg 120
#define ch_reg 121
#define dh_reg 122
#define bh_reg 123

/* 16-bit registers */
#define ax_reg 124
#define cx_reg 125
#define dx_reg 126
#define bx_reg 127
#define sp_reg 128
#define bp_reg 129
#define si_reg 130
#define di_reg 131

/* 32-bit registers */
#define eax_reg 132
#define ecx_reg 133
#define edx_reg 134
#define ebx_reg 135
#define esp_reg 136
#define ebp_reg 137
#define esi_reg 138
#define edi_reg 139

#define eax_idx 0
#define ecx_idx 1
#define edx_idx 2
#define ebx_idx 3
#define esp_idx 4
#define ebp_idx 5
#define esi_idx 6
#define edi_idx 7

#define indir_dx_reg 150

enum OpType { TNone = 0, TRegister, TMemLoc, TImmediate, TJump, TFloatRegister, TMemAddress, TMMXRegister, TXMMRegister, TFloatControlRegister, TDisplacement };

enum OpUsage { unknown = 0, esp, counter, membase, memindex, memsegment,
memsegent0, memsegent1 };

typedef struct _trace_header {
  int magicnumber;
  int version;
  int n_procs;
  uint32_t gdt_base;
  uint32_t idt_base;
} TraceHeader;

typedef struct _proc_record {
	char name[MAX_STRING_LEN];
	uint32_t pid;
	int n_mods;
	uint32_t ldt_base;
} ProcRecord;

typedef struct _module_record {
	char name[MAX_STRING_LEN];
	uint32_t base;
	uint32_t size;
} ModuleRecord;

typedef struct _taint_record {
	uint8_t numRecords; // How many TaintByteRecord currently used
	//TaintByteRecord taintBytes[MAX_NUM_TAINTBYTE_RECORDS];
} taint_record_t;

typedef struct _operand_val {
uint8_t access; /* xed_operand_action_enum_t */
uint8_t length;
uint64_t tainted_begin;//bitwise taint status at insn begin
uint64_t tainted_end;//bitwise taint status at insn end
uint32_t addr;
uint32_t value;
enum OpType type;
enum OpUsage usage;
//taint_record_t records[MAX_OPERAND_LEN];
} OperandVal;


typedef struct _entry_header {
	uint32_t address;	//4
	uint32_t tid;		//4
	uint16_t inst_size;	//2
	uint8_t num_operands;//1
	uint8_t tp;			//1
	uint32_t eflags;	//4
	uint32_t cc_op;		//4
	uint32_t df;		//4
	//unsigned char rawbytes[MAX_INSN_BYTES];//15
} EntryHeader;	//39 not 40 do not align

typedef struct _extend_entry_header {
	uint32_t address;
	uint32_t tid;
	uint16_t inst_size;
	uint8_t num_operands;
	uint8_t tp;
	uint32_t eflags;
	uint32_t cc_op;
	uint32_t df;
	unsigned char rawbytes[MAX_INSN_BYTES];	//TODO:
	OperandVal operand[MAX_NUM_OPERANDS];
	OperandVal memregs[MAX_NUM_OPERANDS][MAX_NUM_MEMREGS];
} ExtendEntryHeader;

int print_ProcRecord(ProcRecord *pr);
void print_trace_header(TraceHeader *th);
void print_ModuleRecord(ModuleRecord *mr);
void print_OperandVal(OperandVal *ov);
void read_OperandVal(FILE *trace);
void print_EntryHeader(EntryHeader *eh, FILE *trace, int idx);
