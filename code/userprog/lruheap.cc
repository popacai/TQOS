#include "lruheap.h"

int LRUheap::compare(TranslationEntry* entry1, TranslationEntry* entry2) {
    return (entry1->use > entry2->use);
} 

int LRUheap::push(TranslationEntry *entry) {
    //printf("push page entry\n");
    entry_vec.push_back(entry);
}

TranslationEntry* LRUheap::pop() {
    //printf("pop page entry\n");
    int size, i, pop_ind;
    std::vector<TranslationEntry*>::iterator it;
    TranslationEntry* ret;

printf("==========================\n");
    it = entry_vec.begin();
    for (i = 0; it != entry_vec.end(); ++it, i++) {
        printf("entry %d stat %d physical page %d\n",i, (*it)->use, (*it)->physicalPage);
    }
printf("==========================\n");

    it = entry_vec.begin();
    ret = *it;
    for (++it , pop_ind = 0, i = 1; it != entry_vec.end(); ++it, ++i) {
        if (!(*it)->valid) {
            entry_vec.erase(entry_vec.begin() + i);
            continue;
        }
        if (this->compare(ret, *it)) {
            ret = *it;
            pop_ind = i;
        }
    }

    entry_vec.erase(entry_vec.begin() + pop_ind);

    printf("entry use =%d\n",ret->use);
    printf("pop physical page %d\n", ret->physicalPage);
    return ret;
}

