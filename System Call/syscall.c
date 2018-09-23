#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"


struct file_descriptor
{
  int fd_num;
  tid_t owner;
  struct file *file_struct;
  struct list_elem elem;
};

/* a list of open files, represents all the files open by the user process
   through syscalls. */
struct list open_files; 

/* the lock used by syscalls involving file system to ensure only one thread at
 * a time is accessing file system 
 */
struct lock fs_lock;

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  printf ("system call!\n");
  thread_exit ();
}
