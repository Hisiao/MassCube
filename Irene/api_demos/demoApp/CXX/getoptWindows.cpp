/***********************************************************************
$HeadURL$

 File: getoptWindows.cpp
 
 Description:

   Windows-based command-line option parsing

 Classification : 

   Unclassified

 Project Name:

   AE9/AP9/SPM Radiation Environment Models

   Developed under US Government contract # FA9453-12-C-0231

 Rights and Restrictions:

   Copyright 2014 Atmospheric and Environmental Research, Inc. (AER)

   DISTRIBUTION A. Approved for public release; distribution unlimited.

   The AE9/AP9/SPM software license is contained in the 'documents/Licenses' 
   folder of this distribution file collection.

 Author:

   This software was developed and/or modified by AER staff

 Contact:

   Atmospheric and Environmental Research, Inc.
   131 Hartwell Avenue
   Lexington, MA 02421-3126 USA
   Phone: 781.761.2288
   email: spwx@aer.com

 References:
 
   None

 Revision history:


SVNTag: $Id$
***********************************************************************/
#include <string.h>

#define EOF     (-1)

int opterr = 1;
int optind = 1;
int optopt;
char* optarg = NULL;

int getopt(int argc, char** argv, const char* opts)
{
  // Note: replaced windows implementation w/ original public domain
  // source from: 
  // http://www.koders.com/c/fid034963469B932D9D87F91C86680EB08DB4DE9AA3.aspx

  static int sp = 1;
  int c;
  char* cp;

  if (sp == 1) {
    if (optind >= argc ||
        argv[optind][0] != '-' || argv[optind][1] == '\0')
        return -1;
    else if (strcmp(argv[optind],"--") == 0) {
      optind++;
      return -1;
    }
  }

  c = argv[optind][sp];
  optopt = c;

  if (c == ':' || (cp = (char*)strchr(opts, c)) == NULL) {
    if (argv[optind][++sp] == '\0') {
      optind++;
      sp = 1;
    }
    return '?';
  }

  if (*++cp == ':') {
    if (argv[optind][sp+1] != '\0') {
      optarg = &argv[optind++][sp+1];
    }
    else if (++optind >= argc) {
      sp = 1;
      return '?';
    }
    else {
      optarg = argv[optind++];
    }
    sp = 1;
  }

  else {
    if (argv[optind][++sp] == '\0') {
      sp = 1;
      optind++;
    }
    optarg = NULL;
  }
  return (c);

}
