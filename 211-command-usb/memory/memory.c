#include <stdio.h>
#include <stdint.h>
#include "hardware/regs/addressmap.h"
#include "pico/stdlib.h"
extern char __flash_binary_start;
extern char __flash_binary_end;
extern char __boot2_start__;
extern char __boot2_end__;
extern char __etext;
extern char __data_start__;
extern char __data_end__;
extern char __bss_start__;
extern char __bss_end__;
extern char __HeapLimit;
extern char __StackBottom;
extern char __StackTop;
int const ROM_SIZE = 16384;
static void row(const char *name, uintptr_t start, uintptr_t end)
{
    printf("%-10s 0x%08x 0x%08x %8u\n",
           name, (unsigned)start, (unsigned)end, (unsigned)(end - start));
}
unsigned size_area(uintptr_t start, uintptr_t end){
    return (unsigned)(end - start); 
}
void mem_info(void)
{   
    printf("%-10s %-10s %-10s %-10s\n","area", "start", "end", "size");
    row("flash",(uintptr_t)XIP_BASE, (uintptr_t)(XIP_BASE+PICO_FLASH_SIZE_BYTES));
    row("sram",(uintptr_t)SRAM_BASE, (uintptr_t)SRAM_END);
    row("rom",(uintptr_t)ROM_BASE, (uintptr_t)ROM_BASE+ROM_SIZE);
    row("image", (uintptr_t)&__flash_binary_start, (uintptr_t)&__flash_binary_end);
    row("free", (uintptr_t)&__flash_binary_end, (uintptr_t)(XIP_BASE+PICO_FLASH_SIZE_BYTES));
    row("boot2", (uintptr_t)&__boot2_start__, (uintptr_t)&__boot2_end__);
    row("text", (uintptr_t)&__boot2_end__, (uintptr_t)&__etext);
    row("data flash", (uintptr_t)&__etext, ((uintptr_t)&__etext+(uintptr_t)&__data_end__-(uintptr_t)&__data_start__));
    row("data ram", (uintptr_t)&__data_start__, (uintptr_t)&__data_end__);
    row("bss", (uintptr_t)&__bss_start__, (uintptr_t)&__bss_end__);
    row("heap", (uintptr_t)&__bss_end__, (uintptr_t)&__HeapLimit);
    row("stack", (uintptr_t)&__StackBottom, (uintptr_t)&__StackTop);
    printf("\n");
    printf("total\n");
    printf("  flash image %-6d = boot2 %-4d + text %-6d + data %-5d\n",
           (size_area((uintptr_t)&__boot2_start__, (uintptr_t)&__boot2_end__) +
           size_area((uintptr_t)&__boot2_end__, (uintptr_t)&__etext) +
           size_area((uintptr_t)&__etext, ((uintptr_t)&__etext+(uintptr_t)&__data_end__-(uintptr_t)&__data_start__))),
           size_area((uintptr_t)&__boot2_start__, (uintptr_t)&__boot2_end__),
           size_area((uintptr_t)&__boot2_end__, (uintptr_t)&__etext),
           size_area((uintptr_t)&__etext, ((uintptr_t)&__etext+(uintptr_t)&__data_end__-(uintptr_t)&__data_start__)) ) ;
    printf("  flash free %-8d of %-8d\n",size_area((uintptr_t)XIP_BASE, (uintptr_t)(XIP_BASE+PICO_FLASH_SIZE_BYTES)-
           (size_area((uintptr_t)&__boot2_start__, (uintptr_t)&__boot2_end__) +
           size_area((uintptr_t)&__boot2_end__, (uintptr_t)&__etext) +
           size_area((uintptr_t)&__etext, ((uintptr_t)&__etext+(uintptr_t)&__data_end__-(uintptr_t)&__data_start__)))) );
    printf("  ram used %-6d = data %-6d + bss %-6d\n",
           (size_area((uintptr_t)&__data_start__, (uintptr_t)&__data_end__)+
           size_area((uintptr_t)&__bss_start__, (uintptr_t)&__bss_end__)),
           size_area((uintptr_t)&__data_start__, (uintptr_t)&__data_end__),
           size_area((uintptr_t)&__bss_start__, (uintptr_t)&__bss_end__));
    printf("  ram free %-6d for heap and %-6d for stack",
           size_area((uintptr_t)&__bss_end__, (uintptr_t)&__HeapLimit), 
           size_area((uintptr_t)&__StackBottom, (uintptr_t)&__StackTop) );
       
}