/* procfs -- a translator for providing GNU/Linux compatible 
             proc pseudo-filesystem
             
   procfs.c -- This file is the main file of the translator.
               This has important definitions and initializes
               the translator
               
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
#include <argp.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <error.h>
#include <sys/stat.h>
#include <hurd/netfs.h>

#include "procfs.h"

/* Defines this Tanslator Name */
char *netfs_server_name = PROCFS_SERVER_NAME;
char *netfs_server_version = PROCFS_SERVER_VERSION;


static const struct argp_child argp_children[] = 
  {
    {&netfs_std_startup_argp, 0, NULL, 0}
    {0}
  };


const char *argp_program_version = "procfs (" PACKAGE ") " VERSION "\n"
"Copyright (C) 2008 Free Software Foundation\n"
"This is free software; see the source for copying conditions.  There is NO\n"
"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE."
"\n";

static char *args_doc = "PROCFSROOT";
static char *doc = "proc pseudo-filesystem for Hurd implemented as a translator"
"This is still under very humble and initial stages of development.\n"
"Any Contribution or help is welcomed. The code may not even compile";

/* Startup options.  */
static const struct argp_option procfs_options[] = 
  {
    { 0 }
  };

  
/* argp parser function for parsing single procfs command line options  */  
static error_t
parse_procfs_opt (int key, char *arg, struct argp_state *state)
{
  switch (key) 
    {
    case ARGP_KEY_ARG:
      if (state->arg_num > 1) 
        argp_usage (state);
      break;
      
    case ARGP_KEY_NO_ARGS:
      argp_usage(state);
      break;
      
    default:
      return ARGP_ERROR_UNKNOWN;
    }
}

/* Program entry point. */
int 
main (int argc, char **argv)
{
  error_t err;
  mach_port_t bootstrap, underlying_node;
  struct argp argp = 
    {
      procfs_options, parse_procfs_opt,
      args_doc, doc, argp_children,
      NULL, NULL  
    }; 
}
