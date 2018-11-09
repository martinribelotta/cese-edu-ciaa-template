/*
 * Copyright (C) 2018 by Martin Alejandro Ribelotta
 */
 
#include <cmsis.h>

#define WEAK __attribute__ ((weak))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))

extern int main(void);
extern void __libc_init_array(void);
extern void SystemInit(void);

extern void _vStackTop(void);
extern void __valid_user_code_checksum(void);

__attribute__((noreturn)) void Reset_Handler(void);
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void MemManage_Handler(void);
WEAK void BusFault_Handler(void);
WEAK void UsageFault_Handler(void);
WEAK void SVC_Handler(void);
WEAK void DebugMon_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);

extern void (* const g_pfnVectors[])(void);

__attribute__ ((used,section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    &_vStackTop,                    // The initial stack pointer
    Reset_Handler,                       // The reset handler
    NMI_Handler,                    // The NMI handler
    HardFault_Handler,              // The hard fault handler
    MemManage_Handler,              // The MPU fault handler
    BusFault_Handler,               // The bus fault handler
    UsageFault_Handler,             // The usage fault handler
    __valid_user_code_checksum,     // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    SVC_Handler,                    // SVCall handler
    DebugMon_Handler,               // Debug monitor handler
    0,                              // Reserved
    PendSV_Handler,                 // The PendSV handler
    SysTick_Handler,                // The SysTick handler
};

__attribute__((section(".after_vectors")))
void data_init(unsigned int romstart, unsigned int start, unsigned int len) {
    unsigned int *pulDest = (unsigned int*) start;
    unsigned int *pulSrc = (unsigned int*) romstart;
    unsigned int loop;
    for (loop = 0; loop < len; loop = loop + 4)
        *pulDest++ = *pulSrc++;
}

__attribute__ ((section(".after_vectors")))
void bss_init(unsigned int start, unsigned int len) {
    unsigned int *pulDest = (unsigned int*) start;
    unsigned int loop;
    for (loop = 0; loop < len; loop = loop + 4)
        *pulDest++ = 0;
}

void _init(void) {}

extern unsigned int __data_section_table;
extern unsigned int __data_section_table_end;
extern unsigned int __bss_section_table;
extern unsigned int __bss_section_table_end;

void Reset_Handler(void) {
    __disable_irq();

    volatile unsigned int *RESET_CONTROL = (unsigned int *) 0x40053100;
    *(RESET_CONTROL + 0) = 0x10DF1000;
    *(RESET_CONTROL + 1) = 0x01DFF7FF;

    volatile unsigned int *NVIC_ICPR = (unsigned int *) 0xE000E280;
    unsigned int irqpendloop;
    for (irqpendloop = 0; irqpendloop < 8; irqpendloop++) {
        *(NVIC_ICPR + irqpendloop) = 0xFFFFFFFF;
    }
    __enable_irq();

    unsigned int LoadAddr, ExeAddr, SectionLen;
    unsigned int *SectionTableAddr;

    SectionTableAddr = &__data_section_table;
    while (SectionTableAddr < &__data_section_table_end) {
        LoadAddr = *SectionTableAddr++;
        ExeAddr = *SectionTableAddr++;
        SectionLen = *SectionTableAddr++;
        data_init(LoadAddr, ExeAddr, SectionLen);
    }
    while (SectionTableAddr < &__bss_section_table_end) {
        ExeAddr = *SectionTableAddr++;
        SectionLen = *SectionTableAddr++;
        bss_init(ExeAddr, SectionLen);
    }

    SystemInit();

    __libc_init_array();
    main();
    while (1) {
        __WFI();
    }
}

__attribute__ ((section(".after_vectors"), noreturn))
void NMI_Handler(void) {
    while (1) {
    }
}

__attribute__ ((section(".after_vectors"), noreturn))
void HardFault_Handler(void) {
    while (1) {
    }
}
__attribute__ ((section(".after_vectors"), noreturn))
void MemManage_Handler(void) {
    while (1) {
    }
}
__attribute__ ((section(".after_vectors"), noreturn))
void BusFault_Handler(void) {
    while (1) {
    }
}
__attribute__ ((section(".after_vectors"), noreturn))
void UsageFault_Handler(void) {
    while (1) {
    }
}
__attribute__ ((section(".after_vectors"), noreturn))
void SVC_Handler(void) {
    while (1) {
    }
}
__attribute__ ((section(".after_vectors"), noreturn))
void DebugMon_Handler(void) {
    while (1) {
    }
}
__attribute__ ((section(".after_vectors"), noreturn))
void PendSV_Handler(void) {
    while (1) {
    }
}
__attribute__ ((section(".after_vectors"), noreturn))
void SysTick_Handler(void) {
    while (1) {
    }
}
