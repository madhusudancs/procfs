/* procfs -- a translator for providing GNU/Linux compatible 
             proc pseudo-filesystem

   procfs_pid_files.c -- This file contains definitions to perform
                         file operations such as creating, writing to,
                         reading from and removing files that holds
                         information for each process with PID
               
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

#include <hurd/netfs.h>
#include <fcntl.h>

#include "procfs.h"

/* Update the files named NAME within the directory named
   PID also with SYMLINK TARGET if necessary. */
struct procfs_dir_entry*
update_pid_entries (struct procfs_dir *dir, const char *name,
                          time_t timestamp,
                          const char *symlink_target)
{
  struct procfs_dir_entry *dir_entry;
  struct stat *stat = (struct stat *) malloc (sizeof (struct stat));
  stat->st_mode = S_IFREG;

  dir_entry = update_entries_list (dir, name, stat, 
                                 timestamp, symlink_target);

  return dir_entry;
}

/* Creates files to store process information for DIR 
   whose names are pids and returns these files in *NODE. */
error_t
procfs_create_files (struct procfs_dir *dir, 
                                    struct node **node,
                                    time_t timestamp)
{
  int err;
  char *file_name, *file_path;
  struct procfs_dir_entry *dir_entry;
 
  if (asprintf (&file_name, "%s", "stat") == -1)
    return errno;
  if (asprintf (&file_path, "%s/%s", dir->node->nn->dir_entry->name, "stat") == -1)
    return errno;
    
  dir_entry = update_pid_entries (dir, file_name, timestamp, NULL);
  err = procfs_create_node (dir_entry, file_path, node);

  free (file_name);
  free (file_path);

#if 0
  nodes_list = &node_stat; 
  nodes_list++;
  node = nodes_list;
#endif

  return err;
}

/* Writes required process information to each of 
   files within DIR, with name pid for files specified
   by NODE. */
error_t 
procfs_write_files_contents (struct node **node, time_t timestamp)
{
  int err;
  
         /*  STUB  */
  
  return err;
}
