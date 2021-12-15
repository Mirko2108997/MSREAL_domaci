#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#define BUFF_SIZE 150

MODULE_LICENSE("Dual BSD/GPL");

dev_t my_dev_id;
static struct class *my_class;
static struct device *my_device;
static struct cdev *my_cdev;

char stred[120];
int endRead = 0;
int pos = 0, a, b;
int duz;

int stred_open(struct inode *pinode, struct file *pfile);
int stred_close(struct inode *pinode, struct file *pfile);
ssize_t stred_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset);
ssize_t stred_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset);

struct file_operations my_fops =
{
	.owner = THIS_MODULE,
	.open = stred_open,
	.read = stred_read,
	.write = stred_write,
	.release = stred_close,
};


int stred_open(struct inode *pinode, struct file *pfile) 
{
		printk(KERN_INFO "Succesfully opened file\n");
		return 0;
}

int stred_close(struct inode *pinode, struct file *pfile) 
{
		printk(KERN_INFO "Succesfully closed file\n");
		return 0;
}

ssize_t stred_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset) 
{
    int ret;
	char buff[BUFF_SIZE];
	long int len;
	
	duz = strlen(stred);
	
	if(duz == 0){
		endRead = 1;
		pos = 0;
		printk(KERN_INFO "Stred je prazan\n");
		return 0;
	}
	
	if (endRead == 1){
		endRead = 0;
		pos = 0;
		printk(KERN_INFO "Succesfully read from file\n");
		return 0;
	}
	len = scnprintf(buff,BUFF_SIZE , "%c ", stred[pos]);
	ret = copy_to_user(buffer, buff, len);
	if(ret)
		return -EFAULT;
	pos++;
	if (pos == duz)
	  endRead = 1;
	return len;
}

ssize_t stred_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset) 
{
	char buff[BUFF_SIZE];
	int ret;
	char *unos;
	int i;
	
	ret = copy_from_user(buff, buffer, length);
	if(ret)
		return -EFAULT;
	buff[length-1] = '\0';

	if (strstr(buff, "string=")){
		
		unos = buff + 7;
   
		for(i = 0;i < 100; i++)
			stred[i] = 0;
		

		if (strlen(unos) < 100){
		        strncpy(stred, unos, strlen(unos));
			printk(KERN_INFO "Succesfully wrote value");
		}
		else {
			printk(KERN_INFO "upisaces malo sutra");
		}

	}
	
	else if(strstr(buff, "clear")){

	        duz = strlen(stred);
		for (i = 0; i < duz ; i++){
			stred[i] = 0;
		}
		printk(KERN_INFO "Succesfully cleared");
	}
	
	else if(strstr(buff, "shrink")){
	  unos = strim(stred);
	  a = strlen(stred);
	  b = strlen(unos);
          strncpy(stred, unos, strlen(unos));
	  for ( i = 0; i < a - b; i++)
	    stred[strlen(stred) - i] = 0;
	  printk(KERN_INFO "Succesfully shrinked");
	}
	
	else if (strstr(buff, "append=")){
	  int dstring,  dunos;
		
		unos = buff + 7;
   		dstring = strlen(stred);
   		dunos = strlen(unos);
		
   	       if((dstring + dunos)< 100){	   
		  strcat (stred, unos);
		  }
	       else{
		  printk(KERN_INFO "upisaces malo sutra");
		  }	
	printk(KERN_INFO "Succesfully appended");

	}

       	else if(strstr(buff, "truncate=")){
	  int brojac = 3;
	  unos = buff + 9;

	  kstrtoint(unos, 10, &brojac);
	  
	   for(i = 0 ; i<=brojac; i ++) {
	    stred[strlen(stred) - i] = 0; 
	  }
	   printk(KERN_INFO "Succesfully truncated %d", brojac);
	}

	else if(strstr(buff, "remove=")){
	  int broj, j, k,z,x;
	  
	  unos = buff + 7;
	  if(strlen(unos) < strlen(stred)){
	    for (i = 0; i <= strlen(stred) - strlen(unos); i++){
	      broj = 0;
	      for (j = 0; j <= strlen(unos); j++){
		if (stred[i + j] == unos[j]){
		  broj++;
		  if (broj == strlen(unos)) {
		    z = 0;
		    
		    for (k = i; k < strlen(stred) - strlen (unos - 1); k++){
		      stred[i + z] = stred[i + strlen(unos) + z];
		      z++;
		    }
		    
		    for(x = 0 ; x <= strlen(unos); x++) {
		      stred[strlen(stred) - x] = 0; 
		    }
		    
		  }
		}
	      }
	    }
	  }
	  printk(KERN_WARNING "Succesfully removed %s", unos);
	}
	
	else{
		printk(KERN_WARNING "Wrong command format");
		
	}
	return length;
}

static int __init stred_init(void)
{
   int ret = 0;
	int i=0;

	//Initialize array
	for (i=0; i<100; i++)
		stred[i] = 0;

   ret = alloc_chrdev_region(&my_dev_id, 0, 1, "stred");
   if (ret){
      printk(KERN_ERR "failed to register char device\n");
      return ret;
   }
   printk(KERN_INFO "char device region allocated\n");

   my_class = class_create(THIS_MODULE, "stred_class");
   if (my_class == NULL){
      printk(KERN_ERR "failed to create class\n");
      goto fail_0;
   }
   printk(KERN_INFO "class created\n");
   
   my_device = device_create(my_class, NULL, my_dev_id, NULL, "stred");
   if (my_device == NULL){
      printk(KERN_ERR "failed to create device\n");
      goto fail_1;
   }
   printk(KERN_INFO "device created\n");

	my_cdev = cdev_alloc();	
	my_cdev->ops = &my_fops;
	my_cdev->owner = THIS_MODULE;
	ret = cdev_add(my_cdev, my_dev_id, 1);
	if (ret)
	{
      printk(KERN_ERR "failed to add cdev\n");
		goto fail_2;
	}
   printk(KERN_INFO "cdev added\n");
   printk(KERN_INFO "Hello world\n");

   return 0;

   fail_2:
      device_destroy(my_class, my_dev_id);
   fail_1:
      class_destroy(my_class);
   fail_0:
      unregister_chrdev_region(my_dev_id, 1);
   return -1;
}

static void __exit stred_exit(void)
{
   cdev_del(my_cdev);
   device_destroy(my_class, my_dev_id);
   class_destroy(my_class);
   unregister_chrdev_region(my_dev_id,1);
   printk(KERN_INFO "Goodbye, cruel world\n");
}


module_init(stred_init);
module_exit(stred_exit);



