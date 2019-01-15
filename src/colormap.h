#ifndef __COLORMAP_H__
#define __COLORMAP_H__

#include <stdio.h>
#include <cmath>
#include <interpolation.h>

typedef enum {COLORMAP_JET, COLORMAP_HAXBY, COLORMAP_GRAY, COLORMAP_SEISMIC} COLORMAP_TYPE;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {

	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

} color_rgba;

const static uint8_t haxby[20][3] = {	{  9,     0,   121},
	{ 40,     0,   150},
	{  0,     9,   200},
	{  0,    25,   212},
	{ 26,   102,   240},
	{ 25,   175,   255},
	{ 50,   190,   255},
	{ 97,   225,   240},
	{106,   236,   225},
	{138,   236,   174},
	{205,   255,   162},
	{223,   246,   141},
	{248,   215,   104},
	{255,   189,    87},
	{244,   117,    75},
	{255,    90,    90},
	{255,   124,   124},
	{246,   179,   174},
	{255,   196,   196},
	{255,   236,   236}
};

const static uint8_t seismic[20][3] = {	{  0, 	 36,   227},
	{  0,    72,   250},
	{  0,   135,   205},
	{  0,   208,   139},
	{ 22,   244,    90},
	{ 63,   250,    54},
	{115,   255,    26},
	{189,   255,    12},
	{255,   255,     0},
	{255,   255,     0},
	{255,   255,     0},
	{255,   221,     0},
	{255,   183,     0},
	{255,   144,     0},
	{255,   106,     0},
	{255,    68,     0},
	{255,    29,     0},
	{247,     0,     0},
	{208,     0,     0},
	{170,     0,     0}
};

const static uint8_t haxby_colors = 20;
const static uint8_t seismic_colors = 20;

float interpolate_jet( float val, float y0, float x0, float y1, float x1);
float interpolate( float val, float y_low, float y_high);
float jet_base( float val ) ;
float jet_red( float gray );
float jet_green( float gray );
float jet_blue( float gray );

color_rgba map_value_to_color(float val, float min, float max, COLORMAP_TYPE t);

color_rgba jet_colormap(float value);
color_rgba haxby_colormap(float value);
color_rgba seismic_colormap(float value);
color_rgba gray_colormap(float value);

float interpolate_jet( float val, float y0, float x0, float y1, float x1 ) {

	return (val - x0) * (y1 - y0) / (x1 - x0) + y0;
}

float jet_base (float val ) {

	if ( val <= -0.75f ) return 0;
	else if ( val <= -0.25f ) return interpolate_jet( val, 0.0f, -0.75f, 1.0f, -0.25f );
	else if ( val <= 0.25f ) return 1.0f;
	else if ( val <= 0.75f ) return interpolate_jet( val, 1.0f, 0.25f, 0.0f, 0.75f );
	else return 0.0f;

}

float jet_red( float value ) {

	return jet_base( value - 0.6f );
}

float jet_green( float value ) {

	return jet_base( value );
}

float jet_blue( float value ) {

	return jet_base( value + 0.6f );
}

color_rgba map_value_to_color(float val, float min, float max, COLORMAP_TYPE t) {

	float range = max - min;

	float value = (val - min) / (range);

	if (value <= 0.0f)
		value = 0.0f;
	if (value > 1.0f)
		value = 1.0f;

	switch (t) {

	case COLORMAP_JET: { return jet_colormap(value); }
	case COLORMAP_HAXBY: { return haxby_colormap(value); }
	case COLORMAP_SEISMIC: { return seismic_colormap(value); }
	case COLORMAP_GRAY: { return gray_colormap(value); }

	}
	//default
	//printf("map_value_to_color: unknown switch\n");
	//return jet_colormap(value);
}

color_rgba jet_colormap(float value) {

	value *= 2.0f;
	value -= 1.0f;

	color_rgba color;

	//MATLAB's jet palette
	float r = jet_red(value);
	float g = jet_green(value);
	float b = jet_blue(value);

	color.r = (uint8_t)(r * 255.0f);
	color.g = (uint8_t)(g * 255.0f);
	color.b = (uint8_t)(b * 255.0f);
	color.a = 255;

	return color;
}

color_rgba gray_colormap(float value) {

	color_rgba color;

	float r = value;
	float g = value;
	float b = value;

	color.r = (uint8_t)(r * 255.0f);
	color.g = (uint8_t)(g * 255.0f);
	color.b = (uint8_t)(b * 255.0f);
	color.a = 255;

	return color;
}

color_rgba haxby_colormap(float value) {

	color_rgba color;

	unsigned low = (unsigned)(float)floor(value * (haxby_colors - 1));
	unsigned high = (unsigned)(float)ceil(value * (haxby_colors - 1));

	float v = (float)(haxby_colors - 1) * (value - (float)floor(value * (haxby_colors - 1)) / (haxby_colors - 1));

	float r = (float)linear_interpolation( v, haxby[low][0], haxby[high][0] );
	float g = (float)linear_interpolation( v, haxby[low][1], haxby[high][1] );
	float b = (float)linear_interpolation( v, haxby[low][2], haxby[high][2] );

	color.r = (uint8_t)(r);
	color.g = (uint8_t)(g);
	color.b = (uint8_t)(b);
	color.a = 255;

	return color;

}

color_rgba seismic_colormap(float value) {

	color_rgba color;

	unsigned low = (unsigned)(float)floor(value * (seismic_colors - 1));
	unsigned high = (unsigned)(float)ceil(value * (seismic_colors - 1));

	float v = (float)(seismic_colors - 1) * (value - (float)floor(value * (seismic_colors - 1)) / (seismic_colors - 1));

	float r = (float)linear_interpolation( v, seismic[low][0], seismic[high][0] );
	float g = (float)linear_interpolation( v, seismic[low][1], seismic[high][1] );
	float b = (float)linear_interpolation( v, seismic[low][2], seismic[high][2] );

	color.r = (uint8_t)(r);
	color.g = (uint8_t)(g);
	color.b = (uint8_t)(b);
	color.a = 255;

	return color;

}

#ifdef __cplusplus
}
#endif

#endif

