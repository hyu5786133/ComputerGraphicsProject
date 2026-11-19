#pragma once

#include <vector>

#include "Math.h"

// ---------------------------------------------------------------------------
// Scene
//
// Temporary hand-made geometry, used to test the rasteriser until the GEM
// models arrive. When GEMLoader is in, this becomes the loaded mesh data and
// the triangles come from the file instead.
// ---------------------------------------------------------------------------
struct SceneTriangle
{
	Vec3 position[3];	// world space
	Colour colour[3];	// one colour per vertex
};

inline std::vector<SceneTriangle> MakeTestScene()
{
	std::vector<SceneTriangle> triangles;

	const Colour floorColour(0.16f, 0.16f, 0.20f);

	// --- floor: two triangles at y = -1.2 -------------------------------
	triangles.push_back({
		{ Vec3(-2.8f, -1.2f, -2.4f), Vec3(2.8f, -1.2f, -2.4f), Vec3(2.8f, -1.2f, 2.4f) },
		{ floorColour, floorColour, floorColour } });

	triangles.push_back({
		{ Vec3(-2.8f, -1.2f, -2.4f), Vec3(2.8f, -1.2f, 2.4f), Vec3(-2.8f, -1.2f, 2.4f) },
		{ floorColour, floorColour, floorColour } });

	// --- a card standing at z = 0 ---------------------------------------
	// Flat in depth, so its colour gradient is the same with or without
	// perspective correction - it is here to be occluded.
	triangles.push_back({
		{ Vec3(-2.2f, -1.2f, 0.0f), Vec3(2.2f, -1.2f, 0.0f), Vec3(0.0f, 2.0f, 0.0f) },
		{ Colour(1.0f, 0.1f, 0.1f), Colour(0.1f, 1.0f, 0.1f), Colour(0.1f, 0.3f, 1.0f) } });

	// --- a second card turned about 45 degrees --------------------------
	// It cuts through the first one: in front of it on the right, behind it
	// on the left, and the crossing line has to be right per pixel.
	triangles.push_back({
		{ Vec3(1.9f, -1.2f, -1.9f), Vec3(-1.9f, -1.2f, 1.9f), Vec3(0.2f, 2.0f, -0.2f) },
		{ Colour(0.2f, 0.9f, 1.0f), Colour(1.0f, 1.0f, 0.2f), Colour(1.0f, 0.4f, 1.0f) } });

	return triangles;
}
