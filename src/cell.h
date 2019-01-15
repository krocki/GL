/*
* @Author: kmrocki
* @Date:   2016-11-07 18:32:11
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-11-07 18:32:57
*/

#ifndef __CELL_H__
#define __CELL_H__

#include <types.h>
#include <rand.h>
#include <iostream>
#include <cmath>

class Cell {

public:

	Cell() :
		Cell(0) { }

	// generate points on a sphere
	Cell(size_t _id, float r) {

		float _x = MT19937::randn();
		float _y = MT19937::randn();
		float _z = MT19937::randn();

		float length = sqrtf(_x * _x + _y * _y + _z * _z) / r;

		location.x = _x / length;
		location.y = _y / length;
		location.z = _z / length;
		location.w = 0.0;
		id = _id;
	}

	// generate points, normal distribution
	Cell(size_t _id) :
		Cell(MT19937::randn(0, 0.5), MT19937::randn(0, 0.5), MT19937::randn(0, 0.5), _id) { }

	Cell(float _x, float _y, float _z, size_t _id) {

		location.x = _x;
		location.y = _y;
		location.z = _z;
		location.w = 0.0f;
		id = _id;

	}

	~Cell() { };

	virtual void serialize(std::ostream& os) const {

		os << "CELL " << id << " [" << location.x << ", " << location.y << ", " << location.z << ", " << location.w << " ]";

	}

	friend std::ostream& operator<<(std::ostream& os, const Cell& c) {

		c.serialize(os);

		return os;

	}

	void move(float dir) {

		location.x *= (1.0 + dir);
		location.y *= (1.0 + dir);
		location.z *= (1.0 + dir);
		location.w *= (1.0 + dir);

	}

	bool active = false;
	bool predicted = false;
	size_t id;
	// confirmed = active && predicted

	vec4<float> location; // physical location (for visualization)

};

#endif