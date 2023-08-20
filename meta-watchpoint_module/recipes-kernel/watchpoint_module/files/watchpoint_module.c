#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/moduleparam.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

static unsigned long watch_address = 0x12345678;
//module_param(watch_address, ulong, S_IRUGO);

void custom_function(void) {
    printk(KERN_INFO "Backtrace\n");
    dump_stack();
}

static ssize_t  read_memory(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
    custom_function(); 
    return 0;
}

static ssize_t  write_memory(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos) {
    custom_function();
    return sizeof(unsigned long);
}

static const struct proc_ops memory_ops = {
    .proc_read = read_memory,
    .proc_write = write_memory,
};

static int __init watchpoint_init(void) {
    pr_err("Watchpoint set on address 0x%lx\n", watch_address);
    proc_create_data("memory_watcher", 0666, NULL, &memory_ops, NULL);

    return 0;
}

static void __exit watchpoint_exit(void) {
    remove_proc_entry("memory_watcher", NULL);
    printk(KERN_INFO "Watchpoint module removed\n");
}

module_init(watchpoint_init);
module_exit(watchpoint_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Evhen");
MODULE_PARM_DESC(watch_address, "Address to watch");
