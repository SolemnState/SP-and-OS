#include <stdio.h>
#include <stdint.h>

void eax0()
{
    printf("Vendor:");
    int out1,out2,out3,out4;
    char a[4];
    char b[4];
    char c[4];
    asm ("movl $0, %%eax;"
        "cpuid;"
        "movl %%eax, %0":"=r;"(out1));
    asm ("mov %%ebx, %0":"=r;"(out2));
    asm ("mov %%ecx, %0":"=r;"(out3));
    asm ("mov %%edx, %0":"=r;"(out4));
         for (int i = 0; i < 4; i++)
        {
            a[i]=out2>>(i*8);
            b[i]=out3>>(i*8);
            c[i]=out4>>(i*8);  
        }
        for (int i = 0; i < 4; i++)
            printf("%c",a[i]);
        for (int i = 0; i < 4; i++)
            printf("%c",c[i]);
        for (int i = 0; i < 4; i++)
            printf("%c",b[i]);
        printf("\nMax number of supported functions: %d\n",out1);
}

void eax1()
{
    int out1,out2,out3,out4;
    char a; 
    asm ("movl $1, %%eax;"
        "cpuid;"
        "movl %%eax, %0":"=r;"(out1));
    printf("CPUID signature: %x\n",out1);
    a=out1>>8;
    printf("Stepping: %d(0%xh)\n",out1&(0x0000000F),out1&(0x0000000F));
    printf("Model: %d(0%xh)\n",
    (((out1&(0x000000F0))>>4)|(out1&(0x000F0000))>>12)
    ,(((out1&(0x000000F0))>>4)|(out1&(0x000F0000))>>12));
    printf("Family: %d(0%xh)\n",(out1&(0x00000F00))>>8,(out1&(0x00000F00))>>8);
    int type=(out1&(0x00003000))>>8;
    if (type==0)
        printf("Type: Original OEM processor.\n");
    else if (type==1)
        printf("Type: OverDrive ® processor.\n");
    else if (type==2)
        printf("Type: Dual processor.\n");
    else if (type==3)
        printf("Type: Inter reserved.\n");
    asm ("movl $1, %%eax;"
        "cpuid;"
        "movl %%ebx, %0":"=r;"(out2));
    int brandID=out2&(0x000000FF);
    if (brandID!=0)
        printf("Brand ID: %d(0%xh)\n",brandID,brandID);
    else printf("Brand ID field is not supported.\n");
    printf("Chunks: %d(0%xh)\n",8*((out2&(0x0000FF00))>>8),8*((out2&(0x0000FF00))>>8));
    asm ("movl $1, %%eax;"
        "cpuid;"
        "movl %%edx, %0":"=r;"(out3));
            
        if (((out3&(0x00000001)))==1)
        printf("FPU is supported\n");
    if (((out3&(0x00000002))>>1)==1)
        printf("VME is supported\n");
    if (((out3&(0x00000004))>>2)==1)
        printf("DE is supported\n");
    if (((out3&(0x00000008))>>3)==1)
        printf("PSE is supported\n");
    if (((out3&(0x00000010))>>4)==1)
        printf("TSC is supported\n");
    if (((out3&(0x00000020))>>5)==1)
        printf("MSR is supported\n");
    if (((out3&(0x00000040))>>6)==1)
        printf("PAE is supported\n");
    if (((out3&(0x00000080))>>7)==1)
        printf("MCE is supported\n");
    if (((out3&(0x00000100))>>8)==1)
        printf("CXB is supported\n");
    if (((out3&(0x00000200))>>9)==1)
        printf("APIC is supported\n");
    if (((out3&(0x00000800))>>10)==1)
        printf("SEP is supported\n");

}

void eax2()
{
    int out1;
    printf("Cache descriptors:\n");
    asm ("movl $2, %%eax;"
        "cpuid;"
        "movl %%eax, %0":"=r;"(out1));
    printf("EAX: %xh\n",out1);
    asm ("movl %%ebx, %0":"=r;"(out1));
    printf("EBX: %xh\n",out1);
    asm ("movl %%ecx, %0":"=r;"(out1));
    printf("ECX: %xh\n",out1);
    asm ("movl %%edx, %0":"=r;"(out1));
    printf("EDX: %xh\n",out1);
}

void eax3()
{
    int out;
    asm ("movl $0x80000000, %%eax;"
        "cpuid;"
        "movl %%eax, %0":"=r;"(out));
    printf("Extended function set: %xh\n",out);
}

int main ()
{
    printf("Info about the processor using CPUID:\n");
    eax0();
    eax1();
    eax2();
    eax3();
    return 0;
}