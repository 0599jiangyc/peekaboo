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
#include "peekaboo_parser.h"
#include "libpeekaboo.h"

void write_trace_from_file(char *trace_path, char *trace_name)
{
    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "%s", trace_name);
    peekaboo_trace_t *new_trace = create_new_trace(path, ARCH_X86);
    write_insn_from_file(new_trace, trace_path);
}

size_t get_num_insn_wrapper(char *trace_path)
{
    peekaboo_trace_t source_trace;
    load_trace(trace_path, &source_trace);
    return get_num_insn(&source_trace);
}

void write_insn_from_file(peekaboo_trace_t *new_trace, char* trace_path)
{
    size_t num_insn = get_num_insn_wrapper(trace_path);
    for(int i=1;i<=2;i++){
        peekaboo_insn_t *new_insn = parse_peekaboo_trace(trace_path, i);
        insert_insn(new_trace, new_insn, ARCH_X86);
        flush_trace(new_trace);
    }
}

int main(int argc, char *argv[])
{
	char *trace_path = argv[1];
    char *new_trace_name = argv[2];
    write_trace_from_file(trace_path, new_trace_name);
    return 0;
}
