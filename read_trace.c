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
#include <inttypes.h>
#include <capstone/capstone.h>
#include <string.h>
#include "libpeekaboo.h"

void hexprint(unsigned char *buf, size_t size)
{
	for(size_t i=0;i<size;i++){
		printf("%02x ",buf[i]);
	}
	printf("\n");
}

int block_match(int cur_insn, unsigned char *rawbytes_buf, int size_list[], int count, peekaboo_trace_t mytrace)
{
	int sum=0;
	for(int i=0;i<count;i++)	sum+=size_list[i];
	unsigned char *trace_buf = malloc(0x100);
	memset(trace_buf, 0, 0x100);
	int i=0;
	for(i=0;i<count;i++){
		peekaboo_insn_t *insn = get_peekaboo_insn(cur_insn, &mytrace);
		if(insn->size!=size_list[i])
			return 0;
		strncat(trace_buf, insn->rawbytes, insn->size);
		//trace_buf += insn->rawbytes;	
		cur_insn++;
	}
	//if(i==4){
	//	printf("%d\n", cur_insn);
	//	hexprint(trace_buf, sum);
	//	hexprint(rawbytes_buf, sum);
	//}
	if(strncmp(rawbytes_buf, trace_buf, sum)!=0)
		return 0;
	return 1;//match
}

int main(int argc, char *argv[])
{
    char *trace_path = argv[1];
    peekaboo_trace_t mytrace;
    load_trace(trace_path, &mytrace);

    size_t num_insn = get_num_insn(&mytrace);

    for (int x=1; x<=num_insn; x++)
    {
        peekaboo_insn_t *insn = get_peekaboo_insn(x, &mytrace);
	//for(int i=0;i<insn->size;i++){
	//   printf("%x ", insn->rawbytes[i]);
	//}
	//printf("\n");
	
	//if(insn->rawbytes[0]!=(uint8_t)'\x48' || insn->rawbytes[1]!=(uint8_t)'\x89' || insn->rawbytes[2]!=(uint8_t)'\xC7')
	//   continue;
	//CVE2013: taint sink:  insn1458 (system call)
	//if(insn->rawbytes[0]!=(uint8_t)'\xe8')
	//   continue;
		//unsigned char *expect = "\x4C\x89\xC4\x4C\x89\xCD\xFF\xE2";	// 2,3,3,2
		unsigned char *expect = "\x48\x89\x02";
		//unsigned char *expect = "\x89\xF0\x4C\x89\xC4\x4C\x89\xCD\xFF\xE2";	//CVE-2013-sink
		int size_list[] = {3};
		if(!block_match(x, expect, size_list, 1, mytrace))
		//if(x<425)
			continue;
        yellow();
        printf("instruction %d:\n", x);
        pretty_print(insn);
        getchar();
    }
    return 0;
}
