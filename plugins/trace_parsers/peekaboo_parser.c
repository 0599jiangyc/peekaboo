/*
 * peekaboo.c
 * Copyright (C) 2020 jiangyc <0599jiangyc@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include "libpeekaboo.h"

peekaboo_insn_t *parse_peekaboo_trace(char *trace_path, size_t index)
{
    peekaboo_trace_t source_trace;
    load_trace(trace_path, &source_trace);
    peekaboo_insn_t *ret = get_peekaboo_insn(index, &source_trace);
    close_trace(&source_trace);
    return ret;
}
