#pragma once

#include "Math.h"

// Forward declaration only: this keeps <Windows.h> / D3D11 out of every file
// that includes Triangle.h. Triangle.cpp is the only place that needs the
// GamesEngineeringBase window.
namespace GamesEngineeringBase
{
	class Window;
}

class DepthBuffer;

// ---------------------------------------------------------------------------
// Triangle (Rasterization lecture, slides 25 - 33 and 85 - 105)
//
// Three vertices in SCREEN space (pixel coordinates, y pointing DOWN, so a
// larger y is lower on the screen) and one colour per vertex.
//
// A screen space vertex carries everything the later stages need:
//     x, y  : pixel position
//     z     : NDC depth in [0, 1]        (z buffer, slide 87)
//     w     : 1 / w from the divide      (perspective correct interpolation,
//                                         slides 92 - 105)
//
// Rasterising follows the slides:
//   * slide 29: edge function
//   * slide 30: find the bounds, then loop over the pixels inside them
//   * slide 31: barycentric coordinates = edge functions normalised by area
//   * slide 87: interpolate z with the barycentrics, then depth test
//   * slide 105: interpolate the colour perspective correctly
// ---------------------------------------------------------------------------
class Triangle
{
public:
	// What the rasteriser works out for one sample (slides 87 and 105)
	struct Fragment
	{
		float alpha, beta, gamma;	// barycentric weights of v[0], v[1], v[2]
		float depth;				// interpolated NDC z
		Colour colour;				// perspective correct interpolated colour
	};

	Vec4 v[3];		// vertices in screen space
	Colour c[3];	// one colour per vertex, interpolated across the triangle

	Triangle();
	Triangle(const Vec4& v0, const Vec4& v1, const Vec4& v2);
	Triangle(const Vec4& v0, const Vec4& v1, const Vec4& v2,
		const Colour& c0, const Colour& c1, const Colour& c2);

	// Edge function (slide 29):
	//     (p - v0) x (v1 - v0)
	//   = (p.x - v0.x) * (v1.y - v0.y) - (v1.x - v0.x) * (p.y - v0.y)
	// It is positive on one side of the directed edge v0 -> v1 and negative on
	// the other, so three of them give us the three half spaces of the triangle.
	static float edgeFunction(const Vec4& v0, const Vec4& v1, const Vec4& p);

	// Twice the signed area of the projected triangle (slide 31). A value of
	// (almost) zero means the triangle is degenerate and cannot be rasterised.
	float projectedArea() const;

	// Screen space bounding box, clamped to the canvas (slide 30).
	// Only x and y of the two output vectors are written.
	void findBounds(Vec4& topRight, Vec4& bottomLeft, unsigned int width, unsigned int height) const;

	// Barycentric coordinates of p (slide 31). alpha belongs to v[0], beta to
	// v[1] and gamma to v[2]. Returns false when the triangle is degenerate.
	bool computeBarycentric(const Vec4& p, float& alpha, float& beta, float& gamma) const;

	// Same, but with a precomputed 1 / projectedArea - the area is shared by
	// every sample of the triangle, so it should only be computed once
	// (slide 19).
	bool computeBarycentric(const Vec4& p, float inverseProjectedArea,
		float& alpha, float& beta, float& gamma) const;

	// All three weights non-negative? Barycentrics are normalised, so this is
	// the "is the sample inside the triangle" test (slide 26).
	static bool isInside(float alpha, float beta, float gamma);

	// Work out the depth and the colour at screen point p. Returns false when
	// p is outside the triangle.
	bool computeFragment(const Vec4& p, float inverseProjectedArea, Fragment& out) const;
	bool computeFragment(const Vec4& p, Fragment& out) const;

	// Rasterise into the canvas, depth testing against the z buffer
	// (slides 30 - 32, 86 and 105).
	void draw(GamesEngineeringBase::Window& canvas, DepthBuffer& depthBuffer) const;
};
