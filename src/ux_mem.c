#include "ux_mem.h"
#include <stdio.h>


char *ux_strdup(const char *src) {
  char *dst;
  size_t len;

  if (!src) {
    return NULL;
  }

  len = strlen(src) + 1;
  dst = (char *)ux_malloc(len);

  memcpy(dst, src, len);

  return dst;
}

int ux_asprintf(char **strp, const char *format, ...) {
  va_list args;
  int ret;
  char buf[64];
  size_t strp_buflen;

  /* Use a constant-sized buffer to determine the length. */
  va_start(args, format);
  ret = vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  if (ret < 0) {
    *strp = NULL;
    return -1;
  }

  /* Allocate a new buffer, with space for the NUL terminator. */
  strp_buflen = (size_t)ret + 1;
  if ((*strp = (char *)ux_malloc(strp_buflen)) == NULL) {
    /* This shouldn't happen, because ux_malloc() calls abort(). */
    return -1;
  }

  /* Return early if we have all the bytes. */
  if (strp_buflen <= sizeof(buf)) {
    memcpy(*strp, buf, strp_buflen);
    return ret;
  }

  /* Try again using the larger buffer. */
  va_start(args, format);
  ret = vsnprintf(*strp, strp_buflen, format, args);
  va_end(args);
  if ((size_t)ret == strp_buflen - 1) {
    return ret;
  }

  /* This should never happen. */
  ux_free(*strp);
  *strp = NULL;
  return -1;
}
