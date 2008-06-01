/* procfs -- a translator for providing GNU/Linux compatible 
             proc pseudo-filesystem

   procfs.h -- This file is the main header file of this
               translator. This has important header 
               definitions for constants and functions 
               used in the translator.
               
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

#ifndef __PROCFS_H__
#define __PROCFS_H__

#define PROCFS_SERVER_NAME "procfs"
#define PROCFS_SERVER_VERSION "0.0.1"

#include <stdlib.h>
#include <maptime.h>

/* libnetfs node structure */
struct netnode 
{ 
  /* Name of this node */
  char *name; 
  
  /* The path in the filesystem that corresponds
     this node. */
  char *path;

  /* The proc filesystem */
  struct procfs *fs;
  
  /* inode number, assigned to this netnode structure. */
  unsigned int inode_num;
  
  /* pointer to node structure, assigned to this node. */
  struct node *node;
  
  /* links to the previous and next nodes in the list */
  struct netnode *nextnode, *prevnode;
  
  /* link to parent netnode of this file or directory */
  struct netnode *parent;
  
  /* link to the first child netnode of this directory */
  struct netnode *child_first;
};

/* The actual procfs filesystem structure */
struct procfs 
{
  /* Root of the filesystem. */
  struct node *root;
 
  
};

extern struct procfs *procfs;

extern volatile mapped_time_value *procfs_maptime;

/* Create a new procfs filesystem.  */
error_t procfs_create (struct procfs **fs);

/* Refresh stat information for NODE */
error_t procfs_refresh_node (struct node *node);

/* Return a new node in NODE, with a name NAME,
   and return the new node with a single
   reference in NODE. */
error_t procfs_create_node (struct node **node);

/* Refresh stat information for NODE.  This may
   actually refresh the whole directory.  */
error_t procfs_refresh_node (struct node *node);

/* Remove NODE from its entry */
error_t procfs_remove_node (struct node *node);

#endif /* __PROCFS_H__ */
