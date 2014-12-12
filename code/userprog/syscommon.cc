#include "syscall.h"
#include "system.h"
#include "directory.h"
#include "filesys.h"
#include "openfile.h"

int fexist_ck(unsigned char* name) {
    int ret = 1;
    int i = 0;
    char name_signed [64];
    while (*(name + i) != 0) {
        *(name_signed + i) = *(name + i);
        i++;
    }
    *(name_signed + i) = 0;
    //printf("file name is %s\n",name_signed);
    //printf("%d\n",(*name)==(*name_signed));
    OpenFile* openFile;
    openFile = fileSystem->Open(name_signed);
    if (openFile == NULL) {
        printf("file cannot be found\n");
        return -1;
    } 
    return ret;
}

int bufck(int buffer, int size) {
    int num_pages;
    unsigned long task_size, buffsize;
    num_pages = machine->pageTableSize;
    task_size = num_pages * PageSize;
    if (buffer < 0) {
        return -1;
    }
    buffsize = (unsigned long)buffer + (unsigned long)size * 4 ;
    //printf("user address space size = %ld\n",task_size);
    if (buffsize > task_size) {
        printf("buffer last exceed user address space\n");
        return -1;
    } 
    return 1;
}

int RW_bufck(int buffer, int size) {
    int num_pages;
    unsigned long task_size, buffsize;
    num_pages = machine->pageTableSize;
    task_size = num_pages * PageSize;
    buffsize = (unsigned long)buffer + (unsigned long)size;
    //printf("user address space size = %ld\n",task_size);
    
    if (buffsize > task_size || buffer < 0) {
        printf("buffer last exceed user address space\n");
        return -1;
    }
    if (size < 0) {
        printf("size is less than zero\n");
        return -1;
    }
 
    return 1;
}

int fname_addrck(char* name) {
    int num_pages;
    unsigned long task_size;
    unsigned long path_max = 64;
    int len = path_max;
    int n = 0;
    int value;
    num_pages = machine->pageTableSize;
    task_size = num_pages * PageSize;
    //printf("user address space size = %ld\n",task_size);
    if ((unsigned long)name > task_size) {
        printf("name address = %u\n",(unsigned long)name);
        printf("name pointer address exceed user address space\n");
        return 0;
    }
    if (task_size - (unsigned long)name < path_max) {
        len = task_size - (unsigned long)name;
    }
    
    do {
        if (!(machine->ReadMem((unsigned long)name + n, 1, &value))) {
            printf("physical address cannot be accessed\n");
            return -1;
        }
        n++;
    } while (value && n < len);
    if (n == 1) {
        printf("null string\n");
        return -1;
    }
    
    if (value) {
        printf("filename string is not end up with a null\n");
        return -1;
    }
    
    if (n > 0) {
        if (n < len || (n == len && !value)) {
            return n;
        }
        else {
            printf("name is too long\n");
            return -1; // name too long
        }
    }
    else {
        printf("name is empty\n");
        return -1; // no name
    }
          
}

void PushPC() {
    machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
    machine->WriteRegister(PCReg, machine->ReadRegister(PCReg) + 4);
    machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4); 
}

int ustrlen(int src) {
    int n, value;
    n = 0;

    do {
        if (!(machine->ReadMem(src + n, 1, &value)))
          return -1;
        n++;
    } while (value != 0);

    return n - 1;
}

int u2kmemcpy(unsigned char* dst, int src, int n) {
    int value;
    int i;
    for (i = 0; i < n; i++) {
        while (!(machine->ReadMem(src + i, 1, &value)))
        {
            //yes ppg!
        }
        *(dst + i) = value;

        /* Old version
        if (machine->ReadMem(src + i, 1, &value))
            *(dst + i) = value;
        else
            return i;
        */
    }
    return i;
}

int u2kmemread(int src) {
    int value;
    if (!(machine->ReadMem(src, 4, &value))) {
        ASSERT(false);
    }
    return value;
}

int u2kmatrixcpy(unsigned char** dst, int src, int n) {
   int i, len;
   int addr;

   for (i = 0; i < n; i++) {
        machine->ReadMem(src + i*4,4,&addr);
        len = ustrlen(addr);
        dst[i] = new unsigned char[len + 1];
        u2kmemcpy(dst[i],addr,len + 1);
   }
   return i;
}

int k2umemcpy(int dst, unsigned char* src, int n) {
    int value;
    int i;
    for (i = 0; i < n; i++) {
        value = *(src + i);
        while (!(machine->WriteMem(dst + i, 1, value))) {}

        /* Old version
        if (!(machine->WriteMem(dst + i, 1, value))){
            return i;
        }
        */
    }
    return i;
}
