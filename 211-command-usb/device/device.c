// device.c

#include "device.h"
#include "pico/version.h"
#include <stdio.h>
#include "pico/unique_id.h"
#include "hardware/regs/addressmap.h"
#include "hardware/regs/sysinfo.h"
#include <stddef.h>

struct info_t device_card={
    VERSION, NAME_215, REVISION
};

void dev_info(void)
{
    volatile uint32_t *chip_id = (uint32_t *)(SYSINFO_BASE + SYSINFO_CHIP_ID_OFFSET);
    uint32_t id = *chip_id;
    uint32_t revision = (id & SYSINFO_CHIP_ID_REVISION_BITS)\
    >> SYSINFO_CHIP_ID_REVISION_LSB;
    device_card.revision=revision;
    device_card.name[sizeof(device_card.name) - 1] = '\0';
    printf("%-16s %-12s %-5s %-6s %-6s \n","struct", "address", "size", "offset", "value");
    printf("device_card     0x%08x %5u\n", (uintptr_t)&device_card, sizeof(device_card));
    
    printf("- %-13s 0x%08x %5u %6u 0x%08x\n",
           "version",
           &device_card.version,
           sizeof(device_card.version),
           offsetof(struct info_t, version),
           device_card.version);
    printf("- %-13s 0x%08x %5u %6u %-13s\n",
           "name",
           device_card.name,
           sizeof(device_card.name),
           offsetof(struct info_t, name),
           device_card.name);
    printf("- %-13s 0x%08x %5u %6u %-8x\n",
           "revision",
           &device_card.revision,
           sizeof(device_card.revision),
           offsetof(struct info_t, revision),
           device_card.revision);
    unsigned fields = sizeof(device_card.version) + sizeof(device_card.name) + sizeof(device_card.revision);
    
    unsigned padding =sizeof(device_card)-fields;
    printf("fields %2d, sizeof %2d, padding %-2d\n", fields, sizeof(device_card), padding);
}
void device_info(void)
{
    // читаем серийный номер платы функцией SDK
    char board_id[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2 + 1];
    pico_get_unique_board_id_string(board_id, sizeof(board_id));
    // читаем регистр CHIP_ID по адресу и разбираем три поля
    volatile uint32_t *chip_id = (uint32_t *)(SYSINFO_BASE + SYSINFO_CHIP_ID_OFFSET);
    uint32_t id = *chip_id;
    uint32_t manufacturer = (id & SYSINFO_CHIP_ID_MANUFACTURER_BITS)\
    >> SYSINFO_CHIP_ID_MANUFACTURER_LSB;
    uint32_t part = (id & SYSINFO_CHIP_ID_PART_BITS)\
    >> SYSINFO_CHIP_ID_PART_LSB;
    uint32_t revision = (id & SYSINFO_CHIP_ID_REVISION_BITS)\
    >> SYSINFO_CHIP_ID_REVISION_LSB;
    // печатаем пять строк паспорта
    printf("project: %s\n", DEVICE_PROJECT);
    printf("repo: %s\n", DEVICE_REPO);
    printf("board: %s\n", DEVICE_BOARD);
    printf("serial: %s\n", board_id);
    printf("chip: manufacturer 0x%03x, part 0x%04x, revision %u\n", manufacturer, part, revision);
    printf("pico-sdk: %s\n", PICO_SDK_VERSION_STRING);
}