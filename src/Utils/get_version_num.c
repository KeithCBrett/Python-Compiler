#include "get_version_num.h"


size_t
get_version_major ()
{
	return 0 +0;
}


size_t
get_version_minor ()
{
	return 0 +0;
}


size_t
get_version_patch ()
{
	return 0 +0;
}


size_t
get_num_commits ()
{
	return 124
 +0;
}


size_t
get_num_commits_patch ()
{
	if (124
 >= 100)
	{
		return (124
 % 100);
	}
	else if (124
 < 100)
	{
		return 124
;
	}
}


size_t
get_num_commits_minor ()
{
	size_t count = 0;
	size_t curr_commits = 124
;
	if (124
 >= 100)
	{
		while (curr_commits >= 100)
		{
			curr_commits = curr_commits - 100;
			count = count + 1;
		}
		return count;
	}
}
