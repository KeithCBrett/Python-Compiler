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
	return 139
 +0;
}


size_t
get_num_commits_patch ()
{
	if (139
 >= 100)
	{
		return (139
 % 100);
	}
	else if (139
 < 100)
	{
		return 139
;
	}
}


size_t
get_num_commits_minor ()
{
	size_t count = 0;
	size_t curr_commits = 139
;
	if (139
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
