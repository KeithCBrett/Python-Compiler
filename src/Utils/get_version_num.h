#ifndef get_version_num_h
#define get_version_num_h


#include <stdio.h>


size_t
get_version_major ();


size_t
get_version_minor ();


size_t
get_version_patch ();

size_t
get_num_commits_minor ();

size_t
get_num_commits_patch ();

#endif
