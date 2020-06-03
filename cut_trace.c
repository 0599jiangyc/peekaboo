/* 
 * Copyright 2019 Chua Zheng Leong
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     https://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include "libpeekaboo.h"

void cut(char *trace_path, size_t start, size_t end)
{
    peekaboo_trace_t mytrace;
    load_trace(trace_path, &mytrace);
    size_t num_insn = get_num_insn(&mytrace);
    if(end>num_insn){
        printf("error. invalid range.");
        exit(1);
    }
    
    char path[MAX_PATH];

    snprintf(path, MAX_PATH, "%s-%d-%d", trace_path, start, end);
    printf("path: %s\n", path);

    peekaboo_trace_t *new_trace = create_new_trace(path);

    for(size_t count = start; count<=end; count++){
        //read
        peekaboo_insn_t *insn = get_peekaboo_insn(count, &mytrace);
        uint64_t pc = insn->addr;
        size_t rawbytes_num = insn->size;
        uint8_t *rawbytes = malloc(rawbytes_num);
        for(size_t i = 0; i<16; i++){
            rawbytes[i] = insn->rawbytes[i]; //to big endian
        }
        size_t mem_num = insn->num_mem;
        uint64_t *memory_addr = malloc(mem_num*sizeof(uint64_t));
        uint64_t *memory = malloc(mem_num*sizeof(uint64_t));
        uint32_t *mem_valid_bit = malloc(mem_num*sizeof(uint32_t));
        uint32_t *mem_status = malloc(mem_num*sizeof(uint32_t));
        for(size_t i=0; i<mem_num; i++){
            memory_addr[i] = insn->mem[i].addr;
            memory[i] = insn->mem[i].value;
            mem_valid_bit[i] = insn->mem[i].size;
            mem_status[i] = insn->mem[i].status;
        }
        //write
        peekaboo_insn_t *new_insn = create_new_insn("amd64");
        fill_insn(new_insn, pc, rawbytes, rawbytes_num, mem_num, memory_addr, memory, mem_valid_bit, mem_status);
        insert_insn(new_trace, new_insn);
    }
}

int main(int argc, char *argv[])
{
    
	char *trace_path = argv[1];
    size_t start = atoi(argv[2]);
    size_t end = atoi(argv[3]);
    cut(trace_path, start, end);
    return 0;
}
