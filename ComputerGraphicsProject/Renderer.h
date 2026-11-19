#pragma once

#include "GamesEngineeringBase.h"

#include "Camera.h"
#include "Clipper.h"
#include "DepthBuffer.h"
#include "Math.h"
#include "Triangle.h"

// ---------------------------------------------------------------------------
// Renderer (Rasterization slide 107)
//
//     Load each triangle
//     Transform each vertex
//     Rasterize
//     ZBuffer
//     Perspective correct interpolate attributes
//     Shade
//
// RenderTriangle takes one triangle in WORLD space and pushes it through the
// whole pipeline. Everything the scene needs to know about is in here, so once
// the models arrive this is the function the mesh loop calls per triangle.
// ---------------------------------------------------------------------------
inline void RenderTriangle(GamesEngineeringBase::Window& canvas,
	const Matrix& view, const Matrix& projection,
	const Vec4& nearPlane, const Vec4& farPlane,
	DepthBuffer& depthBuffer,
	const Vec3& p0, const Vec3& p1, const Vec3& p2,
	const Colour& c0, const Colour& c1, const Colour& c2)
{
	const float width = static_cast<float>(canvas.getWidth());
	const float height = static_cast<float>(canvas.getHeight());

	// 1. world space -> view space (the clip planes are defined in view space)
	ClipVertex in[3] = {
		{ view * Vec4(p0.x, p0.y, p0.z, 1.0f), c0 },
		{ view * Vec4(p1.x, p1.y, p1.z, 1.0f), c1 },
		{ view * Vec4(p2.x, p2.y, p2.z, 1.0f), c2 },
	};

	// 2. clip against the near and far planes (slides 80 - 84)
	ClipVertex polygon[6];
	const int vertexCount = ClipTriangle(in, nearPlane, farPlane, polygon);

	// 3. an n-gon becomes a fan of n - 2 triangles (at most 3)
	for (int i = 1; i + 1 < vertexCount; ++i)
	{
		// project, divide by w and map to pixels (slides 58 - 63)
		const Vec4 s0 = ClipToScreen(projection * polygon[0].position, width, height);
		const Vec4 s1 = ClipToScreen(projection * polygon[i].position, width, height);
		const Vec4 s2 = ClipToScreen(projection * polygon[i + 1].position, width, height);

		// 4. rasterise, depth test, interpolate and draw
		const Triangle triangle(s0, s1, s2,
			polygon[0].colour, polygon[i].colour, polygon[i + 1].colour);

		triangle.draw(canvas, depthBuffer);
	}
}
