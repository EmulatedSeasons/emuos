#include <mm/pmm.h>
#include <kernel.h>
#include <stdio.h>

pmm_list_node* head;
pmm_list_node* tail;

void pmm_init(limine_memmap_response* memmap) {
    for (size_t i = 0; i < memmap->entry_count; i++) {
        if (memmap->entries[i]->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }

        // Puts a node into each page of usable physical memory
        
        if (head != nullptr) {
            head->next = (pmm_list_node*)(memmap->entries[i]->base + _hhdm_offset);
        }

        if (tail != nullptr) {
            tail = (pmm_list_node*)(memmap->entries[i]->base + _hhdm_offset);
        }

        for (size_t j = 0; j < memmap->entries[i]->length; j += 0x1000) {
            pmm_list_node* page = (pmm_list_node*)(memmap->entries[i]->base + j + _hhdm_offset);
            //head->next = page;
            page->next = page + 0x1000;
            head = page;
            //tail = page;
        }
    }

    // sets head->next to first set page
    for (size_t i = 0; i < memmap->entry_count; i++) {
        if (memmap->entries[i] == LIMINE_MEMMAP_USABLE) {
            head->next = tail;
            // head = (pmm_list_node*)(memmap->entries[i]->base + _hhdm_offset);
            //tail->next = head;
            break;
        }
    }

    printf("Initialized PMM\n");
}

void* palloc() {
    pmm_list_node* node = head;
    printf("Head: %lx\nNext: %lx\n", head, head->next);
    head = head->next;
    return node;
}

void pfree(void* page) {
    pmm_list_node* node = (pmm_list_node*)page + _hhdm_offset;
    node->next = head;
    head = node;
}