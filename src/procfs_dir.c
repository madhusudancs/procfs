/* procfs -- a translator for providing GNU/Linux compatible 
             proc pseudo-filesystem

   procfs_dir.c -- This file contains definitions to perform
                   directory operations such as creating,
                   removing and refreshing directories.
               
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

#include <unistd.h>
#include <hurd/netfs.h>
#include <hurd/ihash.h>

#include "procfs.h"

/* Initial HASHTABLE length for the new directories to be created. */
#define INIT_HTABLE_LEN 5


/* Return in DIR a new procfs directory, in the filesystem FS,
   with node NODE and path PATH. */
error_t procfs_dir_create (struct procfs *fs, struct node *node,
			  const char *path, struct procfs_dir **dir)
{
  struct procfs_dir *new = malloc (sizeof (struct procfs_dir));
  if (!new) 
    return ENOMEM;
  struct procfs_dir_entry **htable = calloc (INIT_HTABLE_LEN,
                            sizeof (struct procfs_dir_entry *));
  if (!htable)
    return ENOMEM;

  /* Hold a reference to the new dir's node.  */
  spin_lock (&netfs_node_refcnt_lock);
  node->references++;
  spin_unlock (&netfs_node_refcnt_lock);

  new->num_entries = 0;
  new->num_live_entries = 0;
  new->htable_len = INIT_HTABLE_LEN;
  new->htable = htable;
  new->ordered = NULL;
  new->fs_path = path;
  new->fs = fs;
  new->node = node;
  new->stat_timestamp = 0;
  new->name_timestamp = 0;

  *dir = new;

  return 0;
}

/* Lookup NAME in DIR, returning its entry, or an error. 
   *NODE will contain the result node, locked, and with
   an additional reference, or 0 if an error occurs.  */
error_t procfs_dir_lookup (struct procfs_dir *dir, const char *name,
			  struct node **node)
{

  /*  STUB */
  
  return 0;
}

/* Remove the specified DIR and free all its allocated
   storage. */
void procfs_dir_remove (struct procfs_dir *dir)
{

  /*  STUB */
  
  return 0;
}


/* Refresh DIR.  */
error_t procfs_dir_refresh (struct procfs_dir *dir)
{

    /*  STUB */
  
  return 0;
}
			  
