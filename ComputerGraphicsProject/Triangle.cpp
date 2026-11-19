#include "Triangle.h"

#include "DepthBuffer.h"
#include "GamesEngineeringBase.h"

Triangle::Triangle()
{
	for (int i = 0; i < 3; ++i)
	{
		v[i] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		c[i] = Colour(1.0f, 1.0f, 1.0f);
	}
}

Triangle::Triangle(const Vec4& v0, const Vec4& v1, const Vec4& v2)
	: Triangle(v0, v1, v2,
		Colour(1.0f, 1.0f, 1.0f),
		Colour(1.0f, 1.0f, 1.0f),
		Colour(1.0f, 1.0f, 1.0f))
{
}

Triangle::Triangle(const Vec4& v0, const Vec4& v1, const Vec4& v2,
	const Colour& c0, const Colour& c1, const Colour& c2)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;

	c[0] = c0;
	c[1] = c1;
	c[2] = c2;
}

float Triangle::edgeFunction(const Vec4& v0, const Vec4& v1, const Vec4& p)
{
	return ((p.x - v0.x) * (v1.y - v0.y)) - ((v1.x - v0.x) * (p.y - v0.y));
}

float Triangle::projectedArea() const
{
	return edgeFunction(v[0], v[1], v[2]);
}

void Triangle::findBounds(Vec4& topRight, Vec4& bottomLeft, unsigned int width, unsigned int height) const
{
	// The smallest box containing the three vertices ...
	const float maxX = std::max(v[0].x, std::max(v[1].x, v[2].x));
	const float maxY = std::max(v[0].y, std::max(v[1].y, v[2].y));
	const float minX = std::min(v[0].x, std::min(v[1].x, v[2].x));
	const float minY = std::min(v[0].y, std::min(v[1].y, v[2].y));

	// ... clamped to the canvas, so the pixel loop can never run outside the
	// back buffer (slide 30). This is also what takes care of the left, right,
	// top and bottom clip planes (slide 84).
	topRight.x = std::min(maxX, static_cast<float>(width) - 1.0f);
	topRight.y = std::min(maxY, static_cast<float>(height) - 1.0f);
	bottomLeft.x = std::max(minX, 0.0f);
	bottomLeft.y = std::max(minY, 0.0f);
}

bool Triangle::computeBarycentric(const Vec4& p, float& alpha, float& beta, float& gamma) const
{
	const float area = projectedArea();

	if (fabsf(area) < 1e-8f)
		return false;		// no area: there are no barycentric coordinates

	return computeBarycentric(p, 1.0f / area, alpha, beta, gamma);
}

bool Triangle::computeBarycentric(const Vec4& p, float inverseProjectedArea,
	float& alpha, float& beta, float& gamma) const
{
	// Each edge function is the (signed) area of the sub triangle formed by
	// that edge and p; dividing by the area of the whole triangle normalises
	// them to the barycentric weights (slides 15 - 18 and 31).
	alpha = edgeFunction(v[1], v[2], p) * inverseProjectedArea;
	beta = edgeFunction(v[2], v[0], p) * inverseProjectedArea;
	gamma = edgeFunction(v[0], v[1], p) * inverseProjectedArea;

	return true;
}

bool Triangle::isInside(float alpha, float beta, float gamma)
{
	// The weights sum to one, so being non-negative on all three means the
	// sample is inside (or exactly on an edge of) the triangle.
	return alpha >= 0.0f && beta >= 0.0f && gamma >= 0.0f;
}

bool Triangle::computeFragment(const Vec4& p, Fragment& out) const
{
	const float area = projectedArea();

	if (fabsf(area) < 1e-8f)
		return false;

	return computeFragment(p, 1.0f / area, out);
}

bool Triangle::computeFragment(const Vec4& p, float inverseProjectedArea, Fragment& out) const
{
	computeBarycentric(p, inverseProjectedArea, out.alpha, out.beta, out.gamma);

	if (!isInside(out.alpha, out.beta, out.gamma))
		return false;

	// Depth: linear in screen space (slide 87). That works because the
	// projected z is A + B/z, and 1/z is what varies linearly across the
	// projected triangle.
	out.depth = out.alpha * v[0].z + out.beta * v[1].z + out.gamma * v[2].z;

	// Colour: NOT linear in screen space, so it has to be scaled by the
	// vertices' 1/w before interpolating (slide 105). v[i].w is 1/w.
	const float fragW =
		(out.alpha * v[0].w) + (out.beta * v[1].w) + (out.gamma * v[2].w);

	out.colour = perspectiveCorrectInterpolateAttribute(
		c[0], c[1], c[2],
		v[0].w, v[1].w, v[2].w,
		out.alpha, out.beta, out.gamma, fragW);

	return true;
}

void Triangle::draw(GamesEngineeringBase::Window& canvas, DepthBuffer& depthBuffer) const
{
	const float area = projectedArea();

	if (fabsf(area) < 1e-8f)
		return;				// degenerate triangle, nothing to draw

	// The area is the same for every sample, so compute it once (slide 19)
	const float inverseArea = 1.0f / area;

	Vec4 topRight, bottomLeft;
	findBounds(topRight, bottomLeft, canvas.getWidth(), canvas.getHeight());

	// Iterate over the pixels inside the bounds (slide 30)
	for (int y = static_cast<int>(bottomLeft.y); y < static_cast<int>(topRight.y) + 1; ++y)
	{
		for (int x = static_cast<int>(bottomLeft.x); x < static_cast<int>(topRight.x) + 1; ++x)
		{
			// Sample at the centre of the pixel
			const Vec4 p(static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f, 0.0f, 1.0f);

			Fragment fragment;
			if (!computeFragment(p, inverseArea, fragment))
				continue;	// outside the triangle: discard the sample

			// Depth test: only shade the pixel when this fragment is in front
			// (slide 86 lines 6 - 8)
			if (!depthBuffer.testAndSet(static_cast<unsigned int>(x), static_cast<unsigned int>(y), fragment.depth))
				continue;

			canvas.draw(x, y, toByte(fragment.colour.r), toByte(fragment.colour.g), toByte(fragment.colour.b));
		}
	}
}
