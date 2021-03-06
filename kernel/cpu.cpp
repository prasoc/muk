#include <stddef.h>
#include <std.hpp>

#include <kernel/cpu.hpp>
#include <kernel/memory/alloc.hpp>

uint8_t inportb (uint16_t port) {
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

void outportb (uint16_t port, uint8_t data) {
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}

uint16_t inports (uint16_t port) {
    unsigned short rv;
    __asm__ __volatile__ ("inw %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

void outports (uint16_t port, uint16_t data) {
    __asm__ __volatile__ ("outw %1, %0" : : "dN" (port), "a" (data));
}

uint32_t inportl (uint16_t port) {
    uint32_t rv;
    __asm__ __volatile__ ("inl %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

void outportl (uint16_t port, uint32_t data) {
      __asm__ __volatile__ ("outl %1, %0" : : "dN" (port), "a" (data));
}

__attribute__((noreturn)) inline void halt() {
    interrupts_disable();
    while (true) {
        __asm__ __volatile__ ("hlt");
    }
}

void BochsConsolePrint(char *s){
    do {
        outportb(0xe9 ,*s);
        s++;
    } while( *s != 0);
}

void bcprintf(const char* fmt, ...) {
	va_list parameters;
	char temp_buffer[80] = {0};

	va_start(parameters, fmt);
	vsprintf(temp_buffer, fmt, parameters);
	BochsConsolePrint((char*)temp_buffer);
	va_end(parameters);
}

int snprintf(char* buffer, size_t n, const char* fmt, ...) {
	va_list parameters;
	char temp_buffer[n] = {0};

	va_start(parameters, fmt);
	vsprintf(temp_buffer, fmt, parameters);
	va_end(parameters);

    memcpy(buffer, &temp_buffer[0], n * sizeof(char));

    for(int i = n-1; i > 0; i--) {
        if((int)temp_buffer[i] != 0) {
            return i;
        }
    }

    return -1;
}

int asprintf(char** buffer,  const char* fmt, ...) {
	va_list parameters;
	char temp_buffer[128] = {0};

	va_start(parameters, fmt);
	vsprintf(temp_buffer, fmt, parameters);
	va_end(parameters);

    size_t s = 0;
    for(int i = 127; i > 0; i--) {
        if((int)temp_buffer[i] != 0) {
            s = i;
            break;
        }
    }

    char* rs = new char[s];
    memcpy(rs, &temp_buffer[0], s * sizeof(char));
    buffer = &rs;

    return s;
}