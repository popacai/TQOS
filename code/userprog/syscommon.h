#ifndef SYSCOMMON_H
#define SYSCOMMON_H

// Read from stack
int read_argument(int index);

/* ustrlen()
 * while(*str != NUL)
 *   i++;
 * return i; 
 */
int ustrlen(unsigned char* src, int n);


/* u2kmemcpy()
 * copy one char after another into dst
 */
int u2kmemcpy(unsigned char* dst, int src, int n);

/* u2kmemread()
 * machine->ReadAt(src, &value, 4);
 * return value
 */
int u2kmemread(int src);

#endif
