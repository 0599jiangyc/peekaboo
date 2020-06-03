/*
 * peekaboo.h
 * Copyright (C) 2020 jiangyc <0599jiangyc@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PEEKABOO_H
#define PEEKABOO_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#include "libpeekaboo.h"

peekaboo_insn_t *parse_peekaboo_trace(char *trace_path, size_t index);

#endif /* !PEEKABOO_H */
