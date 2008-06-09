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
   
   A portion of the code in this file is based on ftpfs code
   present in the hurd repositories copyrighted to FSF. The
   Copyright notice from that file is given below.
   
   Copyright (C) 1997,98,2002 Free Software Foundation, Inc.
   Written by Miles Bader <miles@gnu.org>
   This file is part of the GNU Hurd.
*/



#include <unistd.h>
#include <hurd/netfs.h>
#include <hurd/ihash.h>

#include "procfs.h"

/* Initial HASHTABLE length for the new directories to be created. */
#define INIT_HTABLE_LEN 5

struct procfs_dir_entry **cur_entry;

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
  error_t err;
  
  make_dir_invalid (dir);
  
  struct timeval tv;
  maptime_read (procfs_maptime, &tv); 
  
  time_t timestamp = tv.tv_sec;
  cur_entry = &dir->ordered;
  if (!dir->fs->root && !dir->fs->next_inode && !dir->fs->fsid
      && dir->fs->fs_path == "")
    err = procfs_fill_root_dir(dir, timestamp);
  else
    err = update_dir_entries (dir, timestamp);
      
  return err;
}

/* Make all the directory entries invalid  */
static void
make_dir_invalid (struct procfs_dir *dir)
{
  int count;
  size_t len = dir->htable_len;
  struct procfs_dir_entry **htable = dir->htable;
  struct procfs_dir_entr *dir_entry;

  for (count = 0; count < len; count++)
    {
      dir_entry = htable[i];
      while (dir_entry)
        {
          dir_entry->valid = 0;
          dir_entry = dir_entry->next;
        }
    }  
}

/* Fills DIR, the root directory with all the pids of 
   processes running in the system as directories. */
error_t 
procfs_fill_root_dir(struct procfs_dir *dir, time_t timestamp)
{
  error_t err;
  char *data;

  pid_t *pids;
  int pidslen;
  int count;
  char *pid_string;

  pids = NULL;
  pidslen = 0;
  err = proc_getallpids (getproc (), &pids, &pidslen);
  
  /* STUB */

  return err;
}

error_t update_dir_entries (struct procfs_dir *dir)
{
  /* STUB */
  return 0;
}

/* Update the directory entry for NAME to reflect STAT and SYMLINK_TARGET. 
   This also creates a valid linked list of entries imposing ordering on
   them. */
error_t update_entries_list (struct procfs_dir *dir, const char *name,
                             const struct stat *stat, time_t timestamp)
{
  ino_t ino;
  struct procfs_dir_entry *dir_entry = lookup_entry (dir, name, 1);
  struct procfs *fs = dir->fs;
  
  if (! dir_entry)
    return ENOMEM;

  if (dir_entry->stat.st_ino)
    ino = dir_entry->stat.st_ino;
  else
    ino = fs->next_inode++;

  dir_entry->name_timestamp = timestamp;

  if (stat)
    /* The ST and SYMLINK_TARGET parameters are only valid if ST isn't 0.  */
    {
      dir_entry->stat = *stat;
      dir_entry->stat_timestamp = timestamp;

      if (!dir_entry->symlink_target || !symlink_target
	  || strcmp (dir_entry->symlink_target, symlink_target) != 0)
	{
	  if (dir_entry->symlink_target)
	    free (dir_entry->symlink_target);
	  dir_entry->symlink_target = symlink_target ? strdup (symlink_target) : 0;
	}
    }

  /* The st_ino field is always valid.  */
  dir_entry->stat.st_ino = ino;
  dir_entry->stat.st_fsid = fs->fsid;
  dir_entry->stat.st_fstype = PROCFILESYSTEM;
 
 
  dir_entry->valid = 1;

  if (! dir_entry->ordered_self_p)
    /* Position DIR_ENTRY in the ordered chain following the previously seen entry.  */
    {
      /* The PREV_ENTRY_NEXT_P field holds a pointer to the NEXT-field of the
	 previous entry, or a pointer to the ORDERED field in the directory. */
      dir_entry->ordered_self_p = cur_entry;

      if (*dir_entry->ordered_self_p)
	/* Update the self_p pointer of the previous successor.  */
	(*dir_entry->ordered_self_p)->ordered_self_p = &dir_entry->ordered_next;

      /* DIR_ENTRY comes before the previous successor.  */
      dir_entry->ordered_next = *dir_entry->ordered_self_p;

      *dir_entry->ordered_self_p = dir_entry;	/* Put DIR_ENTRY there.  */
    }

  /* Put the next entry after this one. */
  cur_entry->prev_entry_next_p = &e->ordered_next;

  return 0;

}

/* Lookup NAME in DIR and return its entry.  If there is no such entry, and
   DNEW, the decision variable, is true, then a new entry is allocated and
   returned, otherwise 0 is returned (if DNEW is true then 0 can be returned
   if a memory allocation error occurs).  */
struct procfs_dir_entry *
lookup_entry (struct procfs_dir *dir, const char *name, int dnew)
{
  size_t hv = hash (name);
  struct procfs_dir_entry *dir_entry = dir->htable[hv % dir->htable_len];

  while (dir_entry && strcmp (name, dir_entry->name) != 0)
    dir_entry = dir_entry->next;

  if (!dir_entry && dnew)
    {
      if (dir->num_entries > dir->htable_len)
	/* Grow the hash table.  */
	if (rehash (dir, (dir->htable_len + 1) * 2 - 1) != 0)
	  return 0;

      dir_entry = (struct procfs_dir_entry *) malloc (sizeof *struct procfs_dir_entry);
      if (dir_entry)
	{
	  dir_entry->hv = hv;
	  dir_entry->name = strdup (name);
	  dir_entry->node = 0;
	  dir_entry->dir = dir;
	  dir_entry->stat_timestamp = 0;
	  bzero (&dir_entry->stat, sizeof dir_entry->stat);
	  dir_entry->symlink_target = 0;
	  dir_entry->noent = 0;
	  dir_entry->valid = 0;
	  dir_entry->name_timestamp = 0;
	  dir_entry->ordered_next = 0;
	  dir_entry->ordered_self_p = 0;
	  dir_entry->next = 0;
	  dir_entry->self_p = 0;
	  insert (dir_entry, dir->htable, dir->htable_len);
	  dir->num_entries++;
	}
    }

  return dir_entry;
}

/* Calculate NAME's hash value.  */
static size_t
hash (const char *name)
{
  size_t hash_value = 0;
  while (*name)
    hash_value = ((hash_value << 5) + *name++) & 0xFFFFFF;
  return hash_value;
}

/* Extend the existing hashtable for DIR to accomodate values for new length
   NEW_LEN. We retain all the previous entries. */
static error_t
rehash (struct procfs_dir *dir, size_t new_len)
{
  int count;
  size_t old_len = dir->htable_len;
  struct procfs_dir_entry **old_htable = dir->htable;
  struct procfs_dir_entry **new_htable = (struct procfs_dir_entry **)
    malloc (new_len * sizeof (struct procfs_dir_entry *));

  if (! new_htable)
    return ENOMEM;

  bzero (new_htable, new_len * sizeof (struct procfs_dir_entry *));

  for (count = 0; count < old_len; count++)
    while (old_htable[i])
      {
	struct procfs_dir_entry *dir_entry = old_htable[i];

	/* Remove DIR_ENTRY from the old table */
	old_htable[i] = DIR_ENTRY->next;

	insert (dir_entry, new_htable, new_len);
      }

  free (old_htable);

  dir->htable = new_htable;
  dir->htable_len = new_len;

  return 0;
}

/* Put the directory entry DIR_ENTRY into the hash table HTABLE. */
static void
insert (struct procfs_dir_entry *dir_entry,
	struct procfs_dir_entry **htable, size_t htable_len)
{
  struct procfs_dir_entry **new_htable = &htable[e->hv % htable_len];
  if (*new_htable)
    (*new_htable)->self_p = &dir_entry->next;
  dir_entry->next = *new_htable;
  dir_entry->self_p = new_htable;
  *new_htable = dir_entry;
}
