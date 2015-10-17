/*
 * elm.c
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

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "em.h"

typedef enum {
  OK,
  ERROR,
  TIMEOUT,
  NOT_UNDERSTOOD,
} send_err_t;

send_err_t elm_send(em_t *em, char *wdata, char *rdata, int rdata_len)
{
  fd_set		fds;
  struct timeval	timeout;
  int			rc;
#define READ_SZ	100
  char			rbuf[READ_SZ];

  if (write(em->ser_fd, wdata, strlen(wdata)) == -1) {
    perror("Write");
    return ERROR;
  }

  while(1) {
    FD_ZERO(&fds);
    FD_SET(em->ser_fd, &fds);
    timeout.tv_sec = 2; /* ATZ takes approx 1 sec */
    timeout.tv_usec = 0;

    rc = select(em->ser_fd + 1, &fds, NULL, NULL, &timeout);
    if (rc == -1) {
      perror("Select");
      return ERROR;
    } else if (rc == 0) {
      return TIMEOUT;
    } else {
      rc = read(em->ser_fd, rbuf, READ_SZ);
      if (rc == -1) {
      perror("Read");
      return ERROR;
      }
      /* FIXME NJR TBD copy rbuf to rdata */
      /* FIXME NJR TBD check for '>' and also '?' */
      return OK;
    }
  }
}

int elm_init(em_t *em)
{
  struct termios	options;

  em->ser_fd = open(em->ser_name, O_RDWR | O_NOCTTY);
  if (em->ser_fd == -1) {
    perror(em->ser_name);
    return errno;
  }

  if (tcgetattr(em->ser_fd, &options)) {
    perror("Getting serial options");
    return errno;
  }
  options.c_cflag |= (CLOCAL | CREAD);
  options.c_lflag &= !(ECHO | ECHOE | ISIG);
  options.c_oflag &= !(OPOST);
  cfsetspeed(&options, B115200);
  if (tcsetattr(em->ser_fd, TCSANOW, &options)) {
    perror("Setting serial options");
    return errno;
  }

  /* Reset */
  elm_send(em, "ATZ\r", NULL, 0);

  return EOK;
}
