// ---------------------------------------------------------------------------
// Rasterization practical - drawing a triangle (slides 25 - 33)
//
// ---------------------------------------------------------------------------
#include "GamesEngineeringBase.h"

#include "Math.h"
#include "Triangle.h"

int main()
{
	GamesEngineeringBase::Window canvas;
	canvas.create(800, 800, "Computer Graphics - Rasterization");

	const float width = static_cast<float>(canvas.getWidth());
	const float height = static_cast<float>(canvas.getHeight());

	// The example from slide 33: red bottom left, green bottom right, blue on
	// top. Screen space has y pointing down, so a bigger y is lower on screen.
	const Triangle triangle(
		Vec4(0.20f * width, 0.86f * height, 0.0f, 1.0f),	// v0: bottom left, red
		Vec4(0.80f * width, 0.86f * height, 0.0f, 1.0f),	// v1: bottom right, green
		Vec4(0.50f * width, 0.13f * height, 0.0f, 1.0f),	// v2: top, blue
		Colour(1.0f, 0.0f, 0.0f),
		Colour(0.0f, 1.0f, 0.0f),
		Colour(0.0f, 0.0f, 1.0f));

	// Somewhere to test with more than one triangle later (slide 32):
	//
	// const Triangle second(
	//     Vec4(0.10f * width, 0.10f * height, 0.0f, 1.0f),
	//     Vec4(0.90f * width, 0.10f * height, 0.0f, 1.0f),
	//     Vec4(0.50f * width, 0.50f * height, 0.0f, 1.0f),
	//     Colour(1.0f, 1.0f, 0.0f),
	//     Colour(0.0f, 1.0f, 1.0f),
	//     Colour(1.0f, 0.0f, 1.0f));

	while (true)
	{
		canvas.checkInput();
		if (canvas.keyPressed(VK_ESCAPE))
			break;

		canvas.clear();				// black background
		triangle.draw(canvas);
		// second.draw(canvas);

		canvas.present();
	}

	return 0;
}
