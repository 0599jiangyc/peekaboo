#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "decaf_reader.h"


int print_ProcRecord(ProcRecord *pr)
{
	printf("name: %s\n", pr->name);
	printf("pid: %d\n", pr->pid);
	printf("n_mods: %d\n", pr->n_mods);
	printf("ldt_base: %p\n", pr->ldt_base);
	return pr->n_mods;
}

void print_trace_header(TraceHeader *th)
{
	printf("magicnumber: %d\n", th->magicnumber);
	printf("version: %d\n", th->version);
	printf("gdt_base: %p\n", th->gdt_base);
	printf("idt_base: %p\n", th->idt_base);
	printf("n_procs: %d\n", th->n_procs);
}

void print_ModuleRecord(ModuleRecord *mr)
{
	printf("module_name: %s\n", mr->name);
	printf("module_base: %p\n", mr->base);
	printf("module_size: %d\n", mr->size);
}

void print_OperandVal(OperandVal *ov)
{
	printf("access: 0x%x\n", ov->access);
	printf("length: %d\n", ov->length);
	printf("tainted begin: %p\n", ov->tainted_begin);
	printf("tainteed end: %p\n", ov->tainted_end);
	printf("addr: %p\n", ov->addr);
	printf("value: 0x%x\n", ov->value);
	printf("type: 0x%x\n", ov->type);
}


void read_OperandVal(FILE *trace)
{
	uint8_t *access = malloc(1);
	uint8_t *length = malloc(1);
	uint64_t *tainted_begin = malloc(8);
	uint64_t *tainted_end = malloc(8);
	uint32_t *addr = malloc(4);
	uint32_t *value = malloc(4);
	uint16_t *type = malloc(2);
	fread(access, 1, 1, trace);
	fread(length, 1, 1, trace);
	fread(tainted_begin, 8, 1, trace);
	fread(tainted_end, 8, 1, trace);
	fread(addr, 4, 1 , trace);
	fread(value, 4, 1, trace);
	fread(type, 2, 1 , trace);
	printf("	access: 0x%x\n", *access);
	printf("	length: %d\n", *length);
	printf("	tainted_begin: %p\n", *tainted_begin);
	printf("	tainted_end: %p\n", *tainted_end);
	printf("	addr: %p\n", *addr);
	printf("	value: 0x%x\n", *value);
	printf("	type: 0x%x\n", *type);
    if(*type==0x01){
        printf("register info\n");
    }
    for(int i=0; i<*length; i++){
        if(*tainted_begin & (1 << i)){
            printf("gg\n");
        }
    }
}

void print_EntryHeader(EntryHeader *eh, FILE *trace, int idx)
{
    printf("\ninsn %d\n", idx);
	printf("address: %p\n", eh->address);
	printf("tid: %d\n", eh->tid);
	printf("inst_size: %d\n", eh->inst_size);
	uint8_t num_operands = eh->num_operands;
	printf("num_operands: %d\n", num_operands);
	printf("tp: %d\n", eh->tp);
	printf("eflags: 0x%x\n", eh->eflags);
	printf("cc_op: 0x%x\n", eh->cc_op);
	printf("df: 0x%x\n", eh->df);
	size_t inst_size = eh->inst_size;
	unsigned char *rawbytes = malloc(inst_size);
	fread(rawbytes, inst_size, 1, trace);
	printf("rawbytes: ");
	for(int i=0; i<inst_size; i++){
		printf("%02x ", rawbytes[i]);
	}
	printf("\n");
	for(int i=0; i<num_operands; i++){
		printf("operand %d:\n", i+1);
		read_OperandVal(trace);
	}
}
