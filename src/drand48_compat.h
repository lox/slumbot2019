#ifndef DRAND48_COMPAT_H_
#define DRAND48_COMPAT_H_

#if defined(__APPLE__)
#include <stdlib.h>

// macOS does not provide the GNU extensions drand48_r/srand48_r or
// the accompanying drand48_data struct. Provide a lightweight
// compatibility layer that forwards to erand48.
struct drand48_data {
  unsigned short xsubi[3];
};

static inline int srand48_r(long seedval, struct drand48_data *buffer) {
  unsigned long seed = static_cast<unsigned long>(seedval);
  buffer->xsubi[0] = static_cast<unsigned short>(seed & 0xFFFFu);
  buffer->xsubi[1] = static_cast<unsigned short>((seed >> 16) & 0xFFFFu);
  buffer->xsubi[2] = static_cast<unsigned short>((seed >> 32) & 0xFFFFu);
  if (buffer->xsubi[0] == 0 && buffer->xsubi[1] == 0 && buffer->xsubi[2] == 0) {
    buffer->xsubi[0] = 0x330E;  // Match glibc default so state is never zero.
  }
  return 0;
}

static inline int drand48_r(struct drand48_data *buffer, double *result) {
  *result = erand48(buffer->xsubi);
  return 0;
}

#endif  // defined(__APPLE__)

#endif  // DRAND48_COMPAT_H_
