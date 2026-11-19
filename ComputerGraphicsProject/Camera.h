#pragma once

#include "Math.h"

// ---------------------------------------------------------------------------
// Camera (Rasterization slides 36 - 79)
//
// So far this holds the view space -> screen space step. The LookAt / view
// matrix from slides 76 - 79 goes here as well once it is implemented.
// ---------------------------------------------------------------------------

// Transform a vertex from view space into screen space (slides 62 - 63):
//
//     clip = P * v      (slide 62: p_c = P * p)
//     ndc  = clip / w   (slide 62: divide by w, w ends up as 1/w)
//     px   = (x + 1) / 2 * width        (slide 63: map to pixels, flip y)
//     py   = (1 - (y + 1) / 2) * height
//
// The returned z is the NDC depth in [0, 1] (used by the z buffer later) and
// the returned w is 1/w (used by perspective correct interpolation later).
inline Vec4 ToScreen(const Matrix& projection, const Vec4& viewVertex, float width, float height)
{
	const Vec4 clip = projection * viewVertex;
	const Vec4 ndc = clip.perspectiveDivide();

	return Vec4(
		((ndc.x + 1.0f) * 0.5f) * width,
		(1.0f - (ndc.y + 1.0f) * 0.5f) * height,
		ndc.z,
		ndc.w);
}
