/*************************************************
 * File Name  : map.c
 * Version    : 2.0
 *  Created on: 06/dec/2018
 *      Author: Francesco Gaudeni
 *************************************************/

inline float map(float val, float from_src, float to_src, float from_dst, float to_dst)
{
	return (((to_dst-from_dst)/(to_src-from_src))*(val-from_src)) + from_dst;
}
