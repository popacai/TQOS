#ifndef SYSCOMMON_H
#define SYSCOMMON_H

// Read from stack
int  read_argument(int index);

//push PC by adding 4 to prev, current, next PC
void PushPC();

/* ustrlen()
 * while(*str != NUL)
 *   i++;
 * return i; 
 */
int ustrlen(int src);


/* u2kmemcpy()
 * copy one char after another into dst
 */
// fail if n != return value
int u2kmemcpy(unsigned char* dst, int src, int n);

/* u2kmemread()
 * machine->ReadAt(src, &value, 4);
 * return value
 */
//ASSERT FALSE if the read operation is fail
int u2kmemread(int src);

/* u2kmatrixcpy()
 * copy one char[] after another int dst
 */
int u2kmatrixcpy(unsigned char** dst, int src, int n);

/* k2umemcpy()
 * copy one char after another into the dst
 */
// fail if n != return value
int k2umemcpy(int dst, unsigned char* src, int n);



#endif
