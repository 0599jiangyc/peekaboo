/*
 * write_trace_from_decaf.c
 * Copyright (C) 2020 jiangyc <0599jiangyc@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decaf_reader.h"

uint32_t reg32[] = {0, 0, 0, 0, 0, 0, 0, 0};

void print_reg32()
{
    //red();
    printf("registers:\n");
    //reset();
    printf("eax: 0x%08x\n", reg32[eax_idx]);
    printf("ecx: 0x%08x\n", reg32[ecx_idx]);
    printf("edx: 0x%08x\n", reg32[edx_idx]);
    printf("ebx: 0x%08x\n", reg32[ebx_idx]);
    printf("esp: 0x%08x\n", reg32[esp_idx]);
    printf("ebp: 0x%08x\n", reg32[ebp_idx]);
    printf("esi: 0x%08x\n", reg32[esi_idx]);
    printf("edi: 0x%08x\n", reg32[edi_idx]);
}

int read_operand(FILE *trace)
{
    uint8_t *access = malloc(1);
    uint8_t *length = malloc(1);
    uint64_t *tainted_begin = malloc(8);
    uint64_t *tainted_end = malloc(8);
    uint32_t *addr = malloc(4);
    uint32_t *value = malloc(4);
    uint8_t *type = malloc(1);
    uint8_t *usage = malloc(1);
    fread(access, 1, 1, trace);
    fread(length, 1, 1, trace);
    fread(tainted_begin, 8, 1, trace);
    fread(tainted_end, 8, 1, trace);
    fread(addr, 4, 1 , trace);
    fread(value, 4, 1, trace);
    fread(type, 1, 1 , trace);
    fread(usage, 1, 1, trace);
    switch(*type){
        case TRegister:  //register
            printf("usage: %x\n", *usage);
            switch(*addr){
                case eax_reg:
                    reg32[eax_idx] = *value;
                    break;
                case ecx_reg:
                    reg32[ecx_idx] = *value;
                    break;
                case edx_reg:
                    reg32[edx_idx] = *value;
                    break;
                case ebx_reg:
                    reg32[ebx_idx] = *value;
                    break;
                case esp_reg:
                    reg32[esp_idx] = *value;
                    break;
                case ebp_reg:
                    reg32[ebp_idx] = *value;
                    break;
                case esi_reg:
                    reg32[esi_idx] = *value;
                    break;
                case edi_reg:
                    reg32[edi_idx] = *value;
                    break;

                //case di_reg:
                //    reg32[edi_idx] = *value | (reg32[edi_idx] & 0xffff0000);
                //    break;

                case ax_reg:
                case al_reg:
                    reg32[eax_idx] = *value | reg32[eax_idx];
                    break;
                case cl_reg:
                case cx_reg:
                    reg32[ecx_idx] = *value | reg32[ecx_idx];
                    break;
                case dl_reg:
                case dx_reg:
                    reg32[edx_idx] = *value | reg32[edx_idx];
                    break;
                case bl_reg:
                case bx_reg:
                    reg32[ebx_idx] = *value | reg32[ebx_idx];
                    break;
                case sp_reg:
                    reg32[esp_idx] = *value | reg32[esp_idx];
                    break;
                case bp_reg:
                    reg32[ebp_idx] = *value | reg32[ebp_idx];
                    break;
                case si_reg:
                    reg32[esi_idx] = *value | reg32[esi_idx];
                    break;
                case di_reg:
                    reg32[edi_idx] = *value | reg32[edi_idx];
                    break;
                
                case ah_reg:
                    reg32[eax_idx] = reg32[eax_idx] | ((*value<<8) & 0xff00 );
                    break;
                case bh_reg:
                    reg32[ebx_idx] = reg32[ebx_idx] | ((*value<<8) & 0xff00 );
                    break;
                case ch_reg:
                    reg32[ecx_idx] = reg32[ecx_idx] | ((*value<<8) & 0xff00 );
                    break;
                case dh_reg:
                    reg32[edx_idx] = reg32[edx_idx] | ((*value<<8) & 0xff00 );
                    break;
            }
            break;
        case TImmediate:
            break;
        case TMemLoc:  //memory
            //meminfo->addr = *addr;
            //meminfo->value = *value;
            //meminfo-> size = 32;    //FIXME how many bits are valid
            //meminfo->status = -1;   //FIXME xed-operand-action.h
            printf("memory addr: %p\n", *addr);
            printf("memory value: 0x%08x\n", *value);
            return 1;
            break;
    }
    return 0;
}

void write_trace_from_decaf(FILE *trace)
{
    size_t read_size = 0;
    TraceHeader *th = malloc(sizeof(TraceHeader));
    read_size = fread(th, sizeof(TraceHeader), 1, trace);
    //print_trace_header(th);
    ProcRecord *pr = malloc(sizeof(ProcRecord));
    read_size = fread(pr, sizeof(ProcRecord), 1, trace);
    int n_mods = print_ProcRecord(pr);
    for(int i=0; i<n_mods; i++){
        printf("module %d:\n", i+1);
        ModuleRecord *mr = malloc(sizeof(ModuleRecord));
        read_size = fread(mr, sizeof(ModuleRecord), 1, trace);
        print_ModuleRecord(mr);
    }
    int count=1;
    while(1){
        printf("insn %d:\n", count++);
        EntryHeader *eh = malloc(sizeof(EntryHeader));
        read_size = fread(eh, sizeof(EntryHeader), 1, trace);
        printf("pc: %x\n", eh->address);
        size_t rawbytes_num = eh->inst_size;
        uint8_t *rawbytes = malloc(rawbytes_num);
        fread(rawbytes, rawbytes_num, 1, trace);
        //print rawbytes
        printf("rawbytes: ");
        for(int i=0;i<eh->inst_size;i++){
            printf("%02x ", rawbytes[i]);
        } printf("\n");
        //print operands
        for(int i=0;i<eh->num_operands;i++){
            int flag = read_operand(trace);
        }
        print_reg32();
        getchar();
    }
}

int main(int argc, char *argv[])
{
    if(!argv[1]){
        printf("./decaf2peekaboo decaf_trace_file\n");
        exit(1);
    }
    FILE *decaf_trace = fopen(argv[1], "r");
    write_trace_from_decaf(decaf_trace);
    fclose(decaf_trace);
}
