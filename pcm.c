/* 
 * Consumer producer problem As a linux driver!
 * Using linux kernel headers
 * * * * * * * * * * * * * * * * * * * * * * *
 * Author: AMIR MOHAMADI
 * 
 * NOTE: Please read README.md before using this
*/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("<@amiremohamadi> AMIR MOHAMADI");
MODULE_DESCRIPTION("producer-consumer problem as a simple device driver");
MODULE_VERSION("0.0.1");

#define SIZE 5

static struct task_struct *pcm_thread[SIZE];
static struct semaphore mutex, not_empty, not_full;
int arr[SIZE] = {0};
int i = 0;

int producer(void *pv)
{
    // make each process (thread) interruptable to prevent freezing
    set_current_state(TASK_INTERRUPTIBLE);

    while(!kthread_should_stop()) {
        down(&not_full);
        down(&mutex);

        printk(KERN_INFO "producer -> item %d\n", i++);

        up(&mutex);
        up(&not_empty);

        msleep(800);
    }
    return 0;
}

int consumer(void *pv)
{
    // make each process (thread) interruptable to prevent freezing
    set_current_state(TASK_INTERRUPTIBLE);

    while(!kthread_should_stop()) {
        down(&not_empty);
        down(&mutex);

        printk(KERN_INFO "consumer -> item %d\n", --i);

        up(&mutex);
        up(&not_full);

        msleep(400);
    }
    return 0;
}

static int __init pcm_driver_init(void)
{
    int i;

    // the idea is:
    // 1. have a mutex (semaphore with value=1) that prevents mututal exclusion
    // 2. have a semaphore with value 0 that checks if list is empty?
    // 3. and have a sempahore with value SZIE that checks if the list is full?
    sema_init(&mutex, 1);
    sema_init(&not_empty, 0);
    sema_init(&not_full, SIZE);

    // create and run producer threads
    for (i = 0; i < 3; ++i) {
        pcm_thread[i] = kthread_run(producer, NULL, "thread" + i);
        // check for thread creation errors
        if (IS_ERR(pcm_thread[i])) {
            printk(KERN_INFO "thread problem: not created!\n");
            return -1;
        }
    }

    // create and run consumer threads
    for (i = 3; i < 5; ++i) {
        pcm_thread[i] = kthread_run(consumer, NULL, "thread" + i);
        // check for thread creation errors
        if (IS_ERR(pcm_thread[i])) {
            printk(KERN_INFO "thread problem: not created!\n");
            return -1;
        }
    }

    return 0;
}

void __exit pcm_driver_exit(void)
{
    int i = 0;

    // stop each thread and free the memory at the end
    for (i = 0; i < SIZE; ++i) {
        kthread_stop(pcm_thread[i]);
    }

    printk(KERN_INFO "bye bye! device driver removed!\n");
}
 
module_init(pcm_driver_init);
module_exit(pcm_driver_exit);
