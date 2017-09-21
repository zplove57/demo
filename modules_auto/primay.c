#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/sched.h>
MODULE_LICENSE("GPL");

dev_t  devno;

int   major;
int   minor=0;
int   count=1;

struct cdev * cdev;
struct class *pclass;
struct device *pdevice;

int demo_open  (struct inode * inode, struct file *filep)
{
    printk("%s,%d\n",__func__,__LINE__);
    return 0;
}

int demo_release (struct inode * inode, struct file *filep)
{
    printk("%s,%d\n",__func__,__LINE__);
    return 0;
}



struct file_operations  fops={

    .owner=THIS_MODULE,
    .open=demo_open,
    .release=demo_release,
};

static int __init   primay_init(void)
{
    //device init
    int ret;
    printk("%s,%d\n",__func__,__LINE__);
    if((ret=alloc_chrdev_region(&devno,minor,count,"char_demo")))
     {
        printk("fail to alloc_chrdev_regin\n");
        return -1;
    }

    printk("devno=%d,%d,%d\n",devno,MAJOR(devno),MINOR(devno));

    cdev=cdev_alloc();
    if(cdev==NULL)
    {
        printk("faild to alloc\n");
        goto err1;
    }
    cdev_init(cdev,&fops);
    if((ret=cdev_add(cdev,devno,count)))
    {
        printk("fail to add_kernel\n");
        goto err2;
    }
    pclass= class_create(THIS_MODULE,"test_class");
    if(IS_ERR(pclass))
    {
        ret=PTR_ERR(pclass);
        goto err3;
    }
    pdevice=device_create(pclass,NULL,devno,NULL,"demo_test");
    if(IS_ERR(pdevice))
    {
        ret=PTR_ERR(pdevice);
        goto err4;
    }
    return 0;
err4:
    class_destroy(pclass);
err3:
    cdev_del(cdev);
err2:
    kfree(cdev);
err1:
    unregister_chrdev_region(devno,count);

    return ret;

}

static void   __exit    primay_exit(void)
{
    printk("%s,%d\n",__func__,__LINE__);
    device_destroy(pclass, devno);
    class_destroy(pclass);
    cdev_del(cdev);
    kfree(cdev);
    unregister_chrdev_region(devno,count);

}


module_init(primay_init);
module_exit(primay_exit);
