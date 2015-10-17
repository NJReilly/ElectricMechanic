/*
 * main.c
 * Copyright (C) 2015 Nicholas Reilly
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include "em.h"

int main (int argc, char **argv)
{
  em_t *em;
  int rc;

  em = calloc(1, sizeof(*em));
  if (em == NULL) {
    fprintf(stderr, "Failed to allocate memory");
    exit(EXIT_FAILURE);
  }

  rc = elm_init(em);
  if (rc != EOK) {
    fprintf(stderr, "Failed to initialize the OBD-II adaptor");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
