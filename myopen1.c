#include<linux/semaphore.h>
#include<asm/cacheflush.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/unistd.h>
#include<linux/moduleparam.h>
#include<linux/sched.h>
#include<asm/uaccess.h>
#include<asm/pgtable.h>
#include<linux/io.h>
#include<asm/fixmap.h>
#include<asm/tlbflush.h>
#include<linux/highmem.h>
#include<asm/unistd.h>

unsigned long *sys_call_table = (unsigned long *)0xc1536160;

asmlinkage int (*real_open)(const char *__user,int,int);

asmlinkage int myopen(const char *__user filename,int flags,int mode)
{
	printk(KERN_ALERT "In Function My Open\n");
	return real_open(filename,flags,mode);
}

int make_rw(unsigned long address) //make page read write.
{
	unsigned int level;
	pte_t *pte = lookup_address(address,&level); //pte stands for process table entry;
	if(pte->pte & ~__PAGE_RW)  //consider that pte->pte = 101 and PAGE_RW = 0 and ~PAGE_RW = 1 then by anding these we get 101 so it is true we enter in if
	{
		pte->pte |= __PAGE_RW; //here we are changing pte to read/write i.e by 101 oring 001 then we get 101 that is set pte->pte;
	}
	return 0;
}

int make_ro(unsigned long address) //write protected.
{
	unsigned int level;
	pte_t *pte = lookup_address(address,&level);
	pte->pte = pte->pte & ~__PAGE_RW //here we are anding with the 101 and ~PAGE_RW i.e ~001 = 000 and anding these we will get 000 which was the original value at pte->pte.
}

int init_module()
{
	
}
