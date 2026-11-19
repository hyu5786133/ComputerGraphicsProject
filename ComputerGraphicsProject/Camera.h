#pragma once

#include "Math.h"

// ---------------------------------------------------------------------------
// Camera (Rasterization slides 36 - 79)
//
// The view (LookAt) matrix and the clip space -> screen space step. The
// per-scene camera parameters live in the caller for now; if this grows
// (projection + view + viewport in one place) it can become a Camera class.
// ---------------------------------------------------------------------------

// View matrix (slides 75 - 79).
//
// The camera matrix places the camera in the world; the view matrix is its
// inverse and moves the world into the camera's space. LookAt builds that
// inverse directly from a position, a target and an up vector - which is why
// it is the usual way to make a view matrix.
inline Matrix MakeViewMatrix(const Vec3& from, const Vec3& to, const Vec3& up)
{
	Matrix view;
	view.lookAt(from, to, up);
	return view;
}

// Clip space -> screen space (slides 62 - 63):
//
//     ndc = clip / w    (slide 62: divide by w, w ends up as 1/w)
//     px  = (x + 1) / 2 * width          (slide 63: map to pixels, flip y)
//     py  = (1 - (y + 1) / 2) * height
//
// The returned z is the NDC depth in [0, 1] (z buffer, slide 87) and the
// returned w is 1/w (perspective correct interpolation, slide 105).
inline Vec4 ClipToScreen(const Vec4& clip, float width, float height)
{
	const Vec4 ndc = clip.perspectiveDivide();

	return Vec4(
		((ndc.x + 1.0f) * 0.5f) * width,
		(1.0f - (ndc.y + 1.0f) * 0.5f) * height,
		ndc.z,
		ndc.w);
}

// World space -> screen space in one go, for vertices that do not need
// clipping (handy for tests and for anything already known to be on screen).
// 'viewProjection' is the combined P * V matrix (our matrix product applies
// the right hand side first, so P * V means "view, then project").
inline Vec4 ToScreen(const Matrix& viewProjection, const Vec4& worldVertex, float width, float height)
{
	return ClipToScreen(viewProjection * worldVertex, width, height);
}
