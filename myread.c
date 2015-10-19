#include<linux/kernel.h>
//#include<module.h>
#include<linux/syscalls.h>
#include<asm/paravirt.h>

unsigned long **sys_call_table;
unsigned long original_cr0;

asmlinkage long (*ref_sys_read)(unsigned int fd,char __user *buf,size_t count);


asmlinkage long myread(unsigned int fd,char __user *buf,size_t count)
{
	printk(KERN_ALERT "MY READ BY SOHAM KULKARNI\n");
	long ret;
	ret = ref_sys_read(fd,buf,count);
	
	return ret;
}

static unsigned long **get_sys_call_table(void)
{
	unsigned long **sct;
	unsigned int offset = PAGE_OFFSET,i = PAGE_OFFSET;
	for(i = PAGE_OFFSET; i < ULLONG_MAX;i = i + sizeof(void *))
	{
		sct = (unsigned long **)i;
	 
	
		if(sct[__NR_close] == (unsigned long *)sys_close)
			return sct;
	
	}
	return NULL;
}


int init_module(void)
{
	if(!(sys_call_table = get_sys_call_table()))
	{
		printk(KERN_ALERT "System call not hooked it has some problems in it\n");
		return -1;
	}
	
	original_cr0 = read_cr0();
	write_cr0(original_cr0 & ~0x00010000);
	
	ref_sys_read = (void *)sys_call_table[__NR_read];
	sys_call_table[__NR_read] = (unsigned long *)myread;
	
	
	write_cr0(original_cr0);
	
	return 0;
}

void cleanup_module(void)
{
	if(!sys_call_table)
	{
		return;
	}

	write_cr0(original_cr0 & ~0x00010000);
	sys_call_table[__NR_read] = (unsigned long *)ref_sys_read;
	write_cr0(original_cr0);
}
