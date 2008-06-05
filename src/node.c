/* procfs -- a translator for providing GNU/Linux compatible 
             proc pseudo-filesystem

   node.c -- This file contains function defintions to handle
             node creation and destruction.
               
   Copyright (C) 2008, FSF.
   Written as a Summer of Code Project
   
   procfs is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   procfs is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <hurd/ihash.h>
#include <hurd/fshelp.h>
#include <hurd/iohelp.h>

#include <hurd/netfs.h>

#include "procfs.h"

/* Return a new node in NODE, with a name NAME, and return the
   new node with a single reference in NODE. */
error_t procfs_create_node (struct procfs_dir_entry *dir_entry, 
                            const char *fs_path, struct node **node)
{
  struct node *new;
  struct netnode *nn = malloc (sizeof (struct netnode));
  error_t err;

  if (! nn)
    return ENOMEM;

  nn->fs = dir_entry->dir->fs;
  nn->dir_entry = dir_entry;
  nn->dir = NULL;
  nn->fs_path = strdup (fs_path);

  new = netfs_make_node (nn);
  if (! new)
    {
      free (nn);
      return ENOMEM;
    }

  fshelp_touch (&new->nn_stat, TOUCH_ATIME|TOUCH_MTIME|TOUCH_CTIME,
		procfs_maptime);

  spin_lock (&nn->fs->inode_mappings_lock);
  err = hurd_ihash_add (&nn->fs->inode_mappings, dir_entry->stat.st_ino, dir_entry);
  spin_unlock (&nn->fs->inode_mappings_lock);

  if (err)
    {
      free (nn);
      free (new);
      return err;
    }

  dir_entry->node = new;
  *node = new;

  return 0;
}

/* Refresh stat information for NODE */
error_t procfs_refresh_node (struct node *node)
{

    /*  STUB  */
    
  return 0;
}

/* Remove NODE from its entry */
error_t procfs_remove_node (struct node *node)
{

    /* STUB */
    
  return 0;    
}
