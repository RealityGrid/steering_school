/*
  The RCS Steering School Tutorial Exercises

  Copyright (c) 2007-2010, University of Manchester, United Kingdom.
  All rights reserved.

  This software is produced by Research Computing Services, University
  of Manchester as part of the RealityGrid project and associated
  follow on projects, funded by the EPSRC under grants GR/R67699/01,
  GR/R67699/02, GR/T27488/01, EP/C536452/1, EP/D500028/1,
  EP/F00561X/1.

  LICENCE TERMS

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * Neither the name of The University of Manchester nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.

  Author: Robert Haines
 */

#include "config.h"

#include <cstring>
#include <sys/types.h>
#include <dirent.h>

#if REG_NEED_MALLOC_H
#include <malloc.h>
#else
#include <cstdlib>
#endif

#include "flag-viz.h"

// this method is used to filter out nodedata files in scandir.
#if REG_NEED_CONST_DIRENT
int filter_nodes(const dirent* entry) {
#else
int filter_nodes(dirent* entry) {
#endif
  if(strstr(entry->d_name, "nodedata") == NULL)
    return 0;
  else
    return 1;
}

// this method is used to filter out vertices files in scandir.
#if REG_NEED_CONST_DIRENT
int filter_verts(const dirent* entry) {
#else
int filter_verts(dirent* entry) {
#endif
  if(strstr(entry->d_name, "vertices") == NULL)
    return 0;
  else
    return 1;
}

// read the contents of a directory and return lists of interesting files.
int read_directory(const char* dir, CallbackData* data) {
  int n_count;
  int v_count;
  int dirlen;
  dirent** n_files;
  dirent** v_files;
  char* canon_dir;

  // scan the directory for vertices and nodedata files.
  n_count = scandir(dir, &n_files, filter_nodes, alphasort);
  v_count = scandir(dir, &v_files, filter_verts, alphasort);

  // if no vertices files found, stop.
  if(v_count < 1) return 0;

  // canonicalize the directory name (add a trailing / if needs be).
  dirlen = strlen(dir);
  if((rindex(dir, '/') - dir) != (dirlen - 1)) {
    dirlen++;
    canon_dir = (char*) malloc((dirlen + 1) * sizeof(char));
    strncpy(canon_dir, dir, (dirlen - 1));
    strcat(canon_dir, "/");
  }
  else {
    canon_dir = (char*) dir;
  }

  // if the numbers of nodes and vertices files doesn't match
  // ignore the nodes files. This should never happen so long
  // as we actually have nodes files.
  if(n_count != v_count)
    n_count = 0;

  data->vertsfiles = (char**) malloc(v_count * sizeof(char*));
  if(n_count != 0)
    data->nodesfiles = (char**) malloc(n_count * sizeof(char*));
  else
    data->nodesfiles = NULL;

  // build the arrays of filenames.
  for(int i = 0; i < v_count; i++) {
    int len = strlen(v_files[i]->d_name) + 1;
    data->vertsfiles[i] = (char*) malloc((len + dirlen) * sizeof(char));
    strncpy(data->vertsfiles[i], canon_dir, dirlen + 1);
    strncat(data->vertsfiles[i], v_files[i]->d_name, len);
    free(v_files[i]);

    if(n_count != 0) {
      len = strlen(n_files[i]->d_name) + 1;
      data->nodesfiles[i] = (char*) malloc((len + dirlen) * sizeof(char));
      strncpy(data->nodesfiles[i], canon_dir, dirlen + 1);
      strncat(data->nodesfiles[i], n_files[i]->d_name, len);
      free(n_files[i]);
    }
  }

  // free memory
  free(v_files);
  free(n_files);
  if(canon_dir != dir)
    free(canon_dir);

  // set frame limits
  data->step = 0;
  data->max_step = v_count - 1;

  return v_count;
}
