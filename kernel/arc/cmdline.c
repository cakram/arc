/*
 * Copyright (c) 2011-2012 Graham Edgecombe <graham@grahamedgecombe.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <arc/cmdline.h>
#include <arc/util/hashtab.h>
#include <arc/util/strhashtab.h>
#include <arc/panic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define CMDLINE_BUCKETS 8

static hashtab_t cmdline_table;

// TODO: more error checking for malformed command line
void cmdline_init(multiboot_t *multiboot)
{
  if (!hashtab_init(&cmdline_table, &djb2, &strcmpbool, CMDLINE_BUCKETS))
    panic("couldn't init cmdline hash table");

  multiboot_tag_t *tag = multiboot_get(multiboot, MULTIBOOT_TAG_CMDLINE);
  if (tag)
  {
    const char *cmdline = tag->cmdline.string;
    if (!(*cmdline))
      return;

    char *key = 0;

    const char *token_start = cmdline;
    for (;;)
    {
      char c = *cmdline;
      if (c == 0 || c == ' ' || c == '=')
      {
        if (token_start == cmdline)
          panic("malformed kernel command line");

        size_t token_len = cmdline - token_start;
        char *token = malloc(token_len);
        memcpy(token, token_start, token_len);
        token[token_len] = 0;

        token_start = cmdline + 1;

        if (key == 0)
        {
          if (c == ' ')
            hashtab_put(&cmdline_table, token, "");
          else
            key = token;
        }
        else
        {
          hashtab_put(&cmdline_table, key, token);
          key = 0;
        }
      }

      if (c == 0)
        break;

      cmdline++;
    }
  }
}

const char *cmdline_get(const char *key)
{
  hashtab_node_t *node = hashtab_get(&cmdline_table, (void *) key);
  if (!node)
    return 0;

  return node->value;
}
