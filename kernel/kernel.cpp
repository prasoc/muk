#include <stddef.h>
#include <stdint.h>

#include <kernel/multiboot.h>
#include <kernel/tty.h>

extern "C"
void kernel_main(multiboot_info_t * mb_info, uint32_t stack_size, uintptr_t esp) {
    init_tty();

    while(true) { }
}
