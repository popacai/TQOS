#include "translate.h"
#include <vector>

class LRUheap
{
public:
    int push(TranslationEntry *entry);
    TranslationEntry* pop();

private:
    int compare(TranslationEntry* entry1, TranslationEntry* entry2);
    std::vector<TranslationEntry*> entry_vec;
};
