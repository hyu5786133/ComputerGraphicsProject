#pragma once

#include "Math.h"

// ---------------------------------------------------------------------------
// Clipping (Rasterization slides 80 - 84)
//
// The clip planes come out of the projection matrix (slides 81 - 82) and they
// act on view space points, i.e. on the homogeneous vertex BEFORE the
// perspective divide:
//
//     inside the frustum is 0 <= p.z <= p.w  (our projection maps the near
//     plane to 0 and the far plane to 1)
//
// Clipping before the divide is also what makes the attribute interpolation
// along a clipped edge a plain linear interpolation.
//
// Slide 84: the rasteriser only needs the near and far planes here - left,
// right, top and bottom are handled by clamping the bounding box to the screen
// (and by never sampling outside it).
// ---------------------------------------------------------------------------

// A vertex in view space plus whatever attributes travel with it.
struct ClipVertex
{
	Vec4 position;		// view space: (x, y, z, 1)
	Colour colour;		// interpolated along a clipped edge
};

// A plane is (A, B, C, D) of Ax + By + Cz + D = 0 (slide 81). The inside of the
// plane is the half space where plane . (x, y, z, 1) >= 0, which is how the
// clip planes are extracted from the projection matrix (slide 82).
inline bool InsidePlane(const Vec4& plane, const Vec4& position)
{
	return plane.Dot(position) >= 0.0f;
}

// Sutherland-Hodgman clipping of a convex polygon against a single plane
// (slide 83). Writes at most inCount + 1 vertices and returns how many.
inline int ClipPolygon(const ClipVertex* in, int inCount, const Vec4& plane, ClipVertex* out)
{
	int outCount = 0;

	for (int i = 0; i < inCount; ++i)
	{
		const ClipVertex& current = in[i];
		const ClipVertex& next = in[(i + 1) % inCount];

		const float currentDistance = plane.Dot(current.position);
		const float nextDistance = plane.Dot(next.position);

		const bool currentInside = currentDistance >= 0.0f;
		const bool nextInside = nextDistance >= 0.0f;

		// keep the vertex if it is inside
		if (currentInside)
			out[outCount++] = current;

		// the edge leaves or enters the plane: add the crossing point
		if (currentInside != nextInside)
		{
			// where along the edge the plane is crossed. Both the position and
			// the attributes are interpolated with it.
			const float t = currentDistance / (currentDistance - nextDistance);

			ClipVertex intersection;
			intersection.position = current.position + (next.position - current.position) * t;
			intersection.colour = current.colour + (next.colour - current.colour) * t;
			out[outCount++] = intersection;
		}
	}

	return outCount;
}

// Clip a triangle against the near and far planes (slide 84).
// A triangle clipped by two planes is a convex polygon of up to 5 vertices, so
// 'out' must have room for 6. Returns the number of vertices written.
inline int ClipTriangle(const ClipVertex in[3], const Vec4& nearPlane, const Vec4& farPlane, ClipVertex out[6])
{
	ClipVertex afterNear[6];
	const int nearCount = ClipPolygon(in, 3, nearPlane, afterNear);

	if (nearCount == 0)
		return 0;

	return ClipPolygon(afterNear, nearCount, farPlane, out);
}

// The near and far clip planes taken out of the projection matrix, in view
// space (slides 81 - 82).
//
// Being inside the frustum means 0 <= p.z <= p.w, and p.z / p.w are the rows of
// the matrix applied to the vertex, so:
//
//     near: 0 <= p.z       ->  (row 2) . p >= 0
//     far:  p.z <= p.w     ->  (row 3 - row 2) . p >= 0
//
// The same trick gives left/right/top/bottom as (row 3 +/- row 0) and
// (row 3 +/- row 1); they are not needed because the rasteriser clamps its
// bounding box to the screen instead (slide 84).
inline void ExtractNearFarPlanes(const Matrix& projection, Vec4& nearPlane, Vec4& farPlane)
{
	nearPlane = projection.row(2);
	farPlane = projection.row(3) - projection.row(2);
}
