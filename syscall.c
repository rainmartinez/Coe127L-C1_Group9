#include <stdio.h>
#include "filesys/filesys.h"
#include "filesys/file.h"


struct file_descriptor

{

  int fd_num;

  tid_t owner;

  struct file *file_struct;

  struct list_elem elem;

};


static void syscall_handler (struct intr_frame *);


/* System call functions */

static void halt (void);

static void exit (int);

static pid_t exec (const char *);

static int wait (pid_t);

static bool create (const char*, unsigned);

static bool remove (const char *);

static int open (const char *);

static int filesize (int);

static int read (int, void *, unsigned);

static int write (int, const void *, unsigned);

static void seek (int, unsigned);

static unsigned tell (int);

static void close (int);

static mapid_t mmap (int, void *);

static void munmap (mapid_t);

/* End of system call functions */

static void

syscall_handler (struct intr_frame *f)

{

  esp = f->esp;



  if (!is_valid_ptr (esp) || !is_valid_ptr (esp + 1) ||

      !is_valid_ptr (esp + 2) || !is_valid_ptr (esp + 3))

    {

      exit (-1);

    }

  else

    {

      int syscall_number = *esp;

      switch (syscall_number)

        {

        case SYS_HALT:

          halt ();

          break;

        case SYS_EXIT:

          exit (*(esp + 1));

          break;

        case SYS_EXEC:

          f->eax = exec ((char *) *(esp + 1));

          break;

        case SYS_WAIT:

          f->eax = wait (*(esp + 1));

          break;

        case SYS_CREATE:

          f->eax = create ((char *) *(esp + 1), *(esp + 2));

          break;

        case SYS_REMOVE:

          f->eax = remove ((char *) *(esp + 1));

          break;

        case SYS_OPEN:

          f->eax = open ((char *) *(esp + 1));

          break;

        case SYS_FILESIZE:

	  f->eax = filesize (*(esp + 1));

	  break;

        case SYS_READ:

          f->eax = read (*(esp + 1), (void *) *(esp + 2), *(esp + 3));

          break;

        case SYS_WRITE:

          f->eax = write (*(esp + 1), (void *) *(esp + 2), *(esp + 3));

          break;

        case SYS_SEEK:

          seek (*(esp + 1), *(esp + 2));

          break;

        case SYS_TELL:

          f->eax = tell (*(esp + 1));

          break;

        case SYS_CLOSE:

          close (*(esp + 1));

          break;

	case SYS_MMAP:

	  f->eax = mmap (*(esp + 1), (void *) *(esp + 2));

	  break;

	case SYS_MUNMAP:

	  munmap (*(esp + 1));

	  break;

        default:

          break;

        }

    }

}