#pragma once

#include <vector>
#include <algorithm>

// ---------------------------------------------------------------------------
// Depth buffer / z buffer (Rasterization slides 85 - 87)
//
// One depth per pixel, cleared to 1 because the projection matrix maps the
// near plane to z = 0 and the far plane to z = 1, so "1" means "nothing drawn
// here yet".
//
// Algorithm 1 from slide 86, verbatim:
//     set z_buffer[x][y] <- 1 for each pixel
//     for each polygon:
//         for each pixel covered:
//             compute depth z of P at (x, y)
//             if z < z_buffer[x][y] then
//                 z_buffer[x][y] <- z
//                 shade the fragment
// ---------------------------------------------------------------------------
class DepthBuffer
{
public:
	DepthBuffer(unsigned int width, unsigned int height)
		: m_width(width)
		, m_height(height)
		, m_data(static_cast<size_t>(width) * static_cast<size_t>(height), 1.0f)
	{
	}

	unsigned int getWidth() const { return m_width; }
	unsigned int getHeight() const { return m_height; }

	// Back to "nothing drawn" (slide 86 line 2)
	void clear(float depth = 1.0f)
	{
		std::fill(m_data.begin(), m_data.end(), depth);
	}

	// The depth test (slide 86 lines 5 - 9). Returns true when this fragment is
	// in front of what is already there; the buffer is updated in that case.
	bool testAndSet(unsigned int x, unsigned int y, float depth)
	{
		if (x >= m_width || y >= m_height)
			return false;

		float& stored = m_data[static_cast<size_t>(y) * m_width + x];

		if (depth < stored)
		{
			stored = depth;
			return true;
		}

		return false;
	}

	// Read the stored depth (1 = untouched)
	float at(unsigned int x, unsigned int y) const
	{
		if (x >= m_width || y >= m_height)
			return 1.0f;

		return m_data[static_cast<size_t>(y) * m_width + x];
	}

private:
	unsigned int m_width;
	unsigned int m_height;
	std::vector<float> m_data;
};
