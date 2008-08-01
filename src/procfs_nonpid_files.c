/* procfs -- a translator for providing GNU/Linux compatible 
             proc pseudo-filesystem

   procfs_nonpid_files.c -- This file contains function definitions
                            to create and update the non-Per PID
                            files and their contents.
               
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
*/

#include <stdio.h>
#include <unistd.h>
#include <hurd/netfs.h>
#include <hurd/ihash.h>
#include <sys/stat.h>
#include "procfs.h"

error_t procfs_create_uptime (struct procfs_dir *dir, 
                           struct node **node,
                           time_t timestamp)
{
  int err;
  char *file_name, *file_path;
  struct procfs_dir_entry *dir_entry;
 
  if (asprintf (&file_name, "%s", "uptime") == -1)
    return errno;
  if (asprintf (&file_path, "%s", "uptime") == -1)
    return errno;
    
  dir_entry = update_pid_entries (dir, file_name, timestamp, NULL);
  err = procfs_create_node (dir_entry, file_path, node);

  free (file_name);
  free (file_path);

  return err;
}

error_t procfs_create_version(struct procfs_dir *dir, 
                           struct node **node,
                             time_t timestamp)
{
  int err;
  char *file_name, *file_path;
  struct procfs_dir_entry *dir_entry;
 
  if (asprintf (&file_name, "%s", "version") == -1)
    return errno;
  if (asprintf (&file_path, "%s", "version") == -1)
    return errno;
    
  dir_entry = update_pid_entries (dir, file_name, timestamp, NULL);
  err = procfs_create_node (dir_entry, file_path, node);

  free (file_name);
  free (file_path);

  return 0;
}

error_t procfs_create_stat (struct procfs_dir *dir, 
                           struct node **node,
                           time_t timestamp)
{
  int err;
  char *file_name, *file_path;
  struct procfs_dir_entry *dir_entry;
 
  if (asprintf (&file_name, "%s", "stat") == -1)
    return errno;
  if (asprintf (&file_path, "%s", "stat") == -1)
    return errno;
    
  dir_entry = update_pid_entries (dir, file_name, timestamp, NULL);
  err = procfs_create_node (dir_entry, file_path, node);

  free (file_name);
  free (file_path);

  return err;
}

error_t procfs_write_nonpid_stat (struct dir_entry *dir_entry,
                        off_t offset, size_t *len, void *data)
{
  return 0;
}
