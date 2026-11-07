#ifndef UTIL_H
#define UTIL_H

// Returns a malloc-allocated Base64 buffer, or NULL on failure.
// Sets *size to the byte count on success, or zero on failure.
// The caller must free the returned buffer; size must not be NULL.
unsigned char *readFile(const char *filename, int *size);

#endif