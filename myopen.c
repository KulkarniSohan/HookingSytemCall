#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/unistd.h>
#include<linux/syscalls.h>
#include<asm/paravirt.h>




void **sys_call_table;

asmlinkage int (*original_call) (const char*,int,int);


asmlinkage int my_open(const char* file,int flags,int mode)
{
	printk(KERN_ALERT "File was opened... BY SOHAN KULKARNI\n");
	printk(KERN_ALERT "PATH = %s",file);
	return original_call(file,flags,mode);
}


int set_page_rw(unsigned long address)
{
	/*struct page *pg;
	pgprot_t prot;
	
	pg = virt_to_page((*__addr));
	prot.pgprot = VM_READ | VM_WRITE;
	return change_page_attr(pg,1,prot);*/
	
	unsigned int level;
	pte_t *pte = lookup_address(address,&level); //pte stands for process table entry;
	if(pte->pte & ~_PAGE_RW)  //consider that pte->pte = 101 and PAGE_RW = 0 and ~PAGE_RW = 1 then by anding these we get 101 so it is true we enter in if
	{
		pte->pte |= _PAGE_RW; //here we are changing pte to read/write i.e by 101 oring 001 then we get 101 that is set pte->pte;
	}
	return 0;
} 

void set_addr_ro(unsigned long addr)
{

	unsigned int level;
	pte_t *pte = lookup_address(addr,&level);
	
	pte->pte = pte->pte & ~_PAGE_RW; 
}

int init_module()
{
	sys_call_table = (void *)0xc1536160;
	original_call = sys_call_table[__NR_open]; 
	
	set_page_rw((long unsigned int )sys_call_table);
	sys_call_table[__NR_open] = my_open;
	
	return 0;
}

void cleanup_module()
{
	sys_call_table = (void *)0xc1536160;
	set_addr_ro((long unsigned int )sys_call_table);
	sys_call_table[__NR_open] = original_call;
}

MODULE_LICENSE("GPL");
