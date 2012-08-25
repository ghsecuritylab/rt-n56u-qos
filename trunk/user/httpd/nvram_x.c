/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <nvram/bcmnvram.h>

#include "nvram_x.h"


char *nvram_get_list_x(const char *name, int index)
{
	char nv_name[64];
	sprintf(nv_name, "%s%d", name, index);
	return nvram_safe_get(nv_name);
}

int nvram_match_list_x(const char *name, char *match, int index)
{
	const char *value = nvram_get_list_x(name, index);
	return (!strcmp(value, match));
}

void nvram_add_list_x(const char *name, const char *value, int index)
{
	char nv_name[64];

	if (*name)
	{
		sprintf(nv_name, "%s%d", name, index);
		nvram_set(nv_name, value);
	}
}

void nvram_del_list_x(const char *name, int index)
{
	char nv_name[64];

	if (*name)
	{
		sprintf(nv_name, "%s%d", name, index);
		nvram_unset(nv_name);
	}
}

void nvram_del_list_map_x(const char *name, int group_count, int *delMap)
{
	char oname[64], nname[64], *oval;
	int i, ni, di, group_count_new;

	if (!(*name) || group_count < 1)
		return;
	
	ni=0;
	di=0;
	
	for (i=0; i < group_count; i++)
	{
		sprintf(oname, "%s%d", name, i);
		sprintf(nname, "%s%d", name, ni);
		
		oval = nvram_safe_get(oname);
		
		if (delMap[di]==i)
		{
			di++;
		}
		else
		{
			nvram_set(nname, oval);
			ni++;
		}
	}
	
	group_count_new = group_count - di;
	if (group_count_new < 0) group_count_new = 0;
	
	/* clear old array data (try +16 items for clear old trash) */
	for (i = (group_count+15); i >= group_count_new; i--)
	{
		sprintf(oname, "%s%d", name, i);
		nvram_unset(oname);
	}
}
