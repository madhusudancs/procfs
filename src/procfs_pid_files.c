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
#include <string.h>
#include <stdio.h>

#include "procfs.h"
#include "procfs_pid.h"

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

/* Check if the PSTAT_FLAG is set in the corresponding PS
   structure, if not set it and check again and return error
   status accordingly. */
error_t set_field_value (struct proc_stat *ps, int pstat_flag)
{
  error_t err;

  if (! (ps->flags & pstat_flag))
    {
      err = proc_stat_set_flags (ps, pstat_flag);
      if (err)
        return err;

      /* This second check is done since ps.h specifies to
         do so since the previous call would not have set
         the required value. */
      if (! (ps->flags & pstat_flag))
        return EGRATUITOUS;
    }

  return 0;
}

/* Get the data for stat file into the structure
   PROCFS_STAT. */
error_t get_stat_data (pid_t pid, 
                       struct procfs_stat **procfs_stat)
{
  error_t err;
  struct procfs_stat *new = (struct procfs_stat *)
                    malloc (sizeof (struct procfs_stat));

  struct proc_stat *ps;
  err = _proc_stat_create (pid, ps_context, &ps);

  new->pid = pid;

  if (! err)
    {
      err = set_field_value (ps, PSTAT_ARGS);
      if (! err)
        asprintf (&new->comm, "(%s)", ps->args);
    }
  if (! err)
    {
      err = set_field_value (ps, PSTAT_STATE);
      if (! err)
        {
          if (ps->state & PSTAT_STATE_P_STOP) 
            new->state = strdup ("T");
          if (ps->state & PSTAT_STATE_P_ZOMBIE)
            new->state = strdup ("Z");
          if (ps->state & PSTAT_STATE_P_FG)
            new->state = strdup ("+");
          if (ps->state & PSTAT_STATE_P_SESSLDR)
            new->state = strdup ("s");
          if (ps->state & PSTAT_STATE_P_LOGINLDR)
            new->state = strdup ("l");
          if (ps->state & PSTAT_STATE_P_FORKED)
            new->state = strdup ("f");
          if (ps->state & PSTAT_STATE_P_NOMSG)
            new->state = strdup ("m");
          if (ps->state & PSTAT_STATE_P_NOPARENT)
            new->state = strdup ("p");
          if (ps->state & PSTAT_STATE_P_ORPHAN)
            new->state = strdup ("o");
          if (ps->state & PSTAT_STATE_P_TRACE)
            new->state = strdup ("x");
          if (ps->state & PSTAT_STATE_P_WAIT)
            new->state = strdup ("w");
          if (ps->state & PSTAT_STATE_P_GETMSG)
            new->state = strdup ("g");     
        }
    }
  if (! err)
    {
      err = set_field_value (ps, PSTAT_PROC_INFO);
      if (! err)
        {
          new->ppid = ps->proc_info->ppid;
          new->pgid = ps->proc_info->pgrp;
          new->sid = ps->proc_info->session;
        }
    }

  *procfs_stat = new;
  _proc_stat_free (ps);

  return err;
}

/* Writes required process information to stat file
   within the directory represented by pid. Return
   the data in DATA and actual length to be written
   in LEN. */
error_t
procfs_write_stat_file (struct procfs_dir_entry *dir_entry, 
                        off_t offset, size_t *len, void *data)
{
  error_t err;
  char *stat_data;
  struct procfs_stat *procfs_stat;
  pid_t pid = atoi (dir_entry->dir->node->nn->dir_entry->name);

  err = get_stat_data (pid, &procfs_stat);
  
  if (asprintf (&stat_data, "%d %s %s %d %d %d\n", procfs_stat->pid,
                   procfs_stat->comm, procfs_stat->state,
                   procfs_stat->ppid, procfs_stat->pgid,
                   procfs_stat->sid) == -1)
    return errno;

  memcpy (data, stat_data, strlen(stat_data));
  *len = strlen (data);
  
  free (stat_data);
  free (procfs_stat);
  
  return err;  
}

/* Writes required process information to each of files
   within directory represented by pid, for files specified
   by NODE. Return the data in DATA and actual length of 
   data in LEN. */
error_t 
procfs_write_files_contents (struct node *node,
                 off_t offset, size_t *len, void *data)
{
  error_t err;

  if (! strcmp (node->nn->dir_entry->name, "stat"))
    err = procfs_write_stat_file (node->nn->dir_entry, 
                                      offset, len, data);

  return err;
}
