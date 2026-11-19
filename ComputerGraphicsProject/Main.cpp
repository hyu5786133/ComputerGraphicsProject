// ---------------------------------------------------------------------------
// Rasterization practical - projection (slides 36 - 67)
//
// One triangle defined in 3D, projected with a 45 degree field of view
// (slides 65 - 67).
//
//   UP / DOWN : change z ("try changing z", slide 66)
//   ESC       : quit
// ---------------------------------------------------------------------------
#include "GamesEngineeringBase.h"

#include "Camera.h"
#include "Math.h"
#include "Triangle.h"

static const float NEAR_PLANE = 0.1f;
static const float FAR_PLANE = 100.0f;

int main()
{
	GamesEngineeringBase::Window canvas;
	canvas.create(1280, 720, "Computer Graphics - Projection");

	const float width = static_cast<float>(canvas.getWidth());
	const float height = static_cast<float>(canvas.getHeight());

	// Projection matrix with a 45 degree field of view (slide 66)
	Matrix projection;
	projection.setPerspective(45.0f, width / height, NEAR_PLANE, FAR_PLANE);

	// The vertices from slide 66, in view space (the camera sits at the origin
	// looking down +z, so for now world space and view space are the same):
	//     (0, 0.3, 1)  (0.3, -0.3, 1)  (-0.3, -0.3, 1)
	// Only x and y are kept here, z is the value that the arrow keys change.
	const Vec3 xy[3] = {
		Vec3(0.0f, 0.3f, 0.0f),		// top
		Vec3(0.3f, -0.3f, 0.0f),	// bottom right
		Vec3(-0.3f, -0.3f, 0.0f),	// bottom left
	};

	// Slide 67 draws a flat dark green triangle. 0..255 constructor, so this
	// is the same (0, 40, 0) as on the slide.
	const Colour colour(0, 40, 0);

	float z = 1.0f;					// slide 66 has z = 1 on all three vertices

	GamesEngineeringBase::Timer timer;

	while (true)
	{
		canvas.checkInput();
		if (canvas.keyPressed(VK_ESCAPE))
			break;

		// "Try changing z" (slide 66): move the triangle along the view axis.
		// Frame time is used so the speed does not depend on the frame rate.
		const float dt = timer.dt();
		if (canvas.keyPressed(VK_UP))
			z -= 1.0f * dt;
		if (canvas.keyPressed(VK_DOWN))
			z += 1.0f * dt;

		// Keep z in a range where the whole triangle stays on screen. It also
		// has to stay in front of the near plane: at z = 0 the perspective
		// divide blows up and points behind the camera project mirrored
		// (slide 48). Remove these two clamps to see that happen.
		if (z < 0.75f)		// the triangle fills the screen height here
			z = 0.75f;
		if (z > 8.0f)
			z = 8.0f;

		// Project the three vertices with the projection matrix
		Vec4 screen[3];
		for (int i = 0; i < 3; ++i)
			screen[i] = ToScreen(projection, Vec4(xy[i].x, xy[i].y, z, 1.0f), width, height);

		const Triangle triangle(screen[0], screen[1], screen[2],
			colour, colour, colour);

		canvas.clear();				// black background
		triangle.draw(canvas);
		canvas.present();
	}

	return 0;
}
