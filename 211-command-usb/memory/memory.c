#include <stdio.h>
#include <stdint.h>
#include "hardware/regs/addressmap.h"
#include "pico/stdlib.h"
#include <stdlib.h>
#include "command.h"
#include "device.h"
#include "led.h"
#define VECTOR_TABLE 0x10000100
#define GPIO_IN_OFFSET 0x004

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
uint32_t data_variable = 100;
uint32_t bss_variable;
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
    printf("  ram free %-6d for heap and %-6d for stack\n",
           size_area((uintptr_t)&__bss_end__, (uintptr_t)&__HeapLimit), 
           size_area((uintptr_t)&__StackBottom, (uintptr_t)&__StackTop) );
    
}
int main(void);
void fw_info(void)
{
       data_variable++;
       bss_variable++;
       uint16_t *main_code = (uint16_t *)((uintptr_t)main & ~1u);
       uint16_t *fw_info_code = (uint16_t *)((uintptr_t)fw_info & ~1u);
       printf("%-14s %-10s %-10s \n","object", "address", "value");
       printf("main            0x%-08x 0x%-04x\n", (uintptr_t)&main,(uintptr_t)*main_code );
       printf("fw_info         0x%-08x 0x%-04x\n", (uintptr_t)&fw_info,(uintptr_t)*fw_info_code );
       printf("commands        0x%-08x\n", (uintptr_t)&commands);
       for (uint i = 0; i < command_count; i++)
       {
           printf("-%-13s  0x%-08x\n", commands[i].name,(uintptr_t)&commands[i].handler);      
       }
       
       printf("DEVICE_PROJECT  0x%-08x %-10s\n", (uintptr_t)&DEVICE_PROJECT, DEVICE_PROJECT);
       printf("DEVICE_BOARD    0x%-08x %-10s\n", (uintptr_t)&DEVICE_BOARD, DEVICE_BOARD);
       printf("data_variable   0x%-08x %-4d\n", (uintptr_t)&data_variable, data_variable);
       printf("bss_variable    0x%-08x %-4d\n", (uintptr_t)&bss_variable, bss_variable);
       uint32_t stack_variable = 1946;
       uint32_t *heap_variable = malloc(sizeof(uint32_t));

       if (heap_variable != NULL)
       {
        *heap_variable = 1951;
       }
       printf("stack_variable  0x%-08x %-4d\n", (uintptr_t)&stack_variable, stack_variable);
       printf("heap_variable   0x%-08x %-4d\n", (uintptr_t)heap_variable, *heap_variable);
       free(heap_variable);
}
void boot_info(void)
{
    const uint32_t *vectors = (const uint32_t *)VECTOR_TABLE;
    uint32_t stack_top = vectors[0];
    uint32_t reset_handler = vectors[1];
    uint32_t reset_handler_addr = (uint32_t *)((uintptr_t)reset_handler & ~1u);
    volatile uint32_t *gpio_in = (uint32_t *)(SIO_BASE + SIO_GPIO_IN_OFFSET);
    uint32_t level = (*gpio_in >> led_pin()) & 1u;
    //
    
    //

    printf("%-14s 0x%08x\n ", "vector table", VECTOR_TABLE);
    printf("  %-14s 0x%08x\n", "stack top", stack_top);
    printf("  %-14s 0x%08x\n", "reset", reset_handler);
    printf("  %-14s 0x%08x\n", "reset (even)", (uintptr_t)reset_handler_addr);
    printf("%-14s 0x%08x\n", "gpio in", (uintptr_t)gpio_in);
    printf("  %-14s %-2d\n", "led bit", level);
    printf("  %-14s %-2d\n", "gpio_get", gpio_get(led_pin()));
      
} 