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

#include <hurd/netfs.h>

#include "procfs.h"

/* Return a new node in NODE, with a name NAME, and return the new node
   with a single reference in NODE. */
error_t procfs_create_node (struct node **node)
{

    /* STUB */
    
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
