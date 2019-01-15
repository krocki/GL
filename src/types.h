/*
* @Author: kmrocki
* @Date:   2016-11-07 18:30:52
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-11-07 18:33:38
*/

#ifndef __TYPES_H__
#define __TYPES_H__

template <typename T>
struct vec4 {

	T x, y, z, w;

} __attribute__((aligned(sizeof(T) * 4)));

#endif
