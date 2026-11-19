// ---------------------------------------------------------------------------
// Rasterization practical - LookAt matrix (slides 72 - 79)
//
// The triangle from slide 66 now lives in WORLD space and a LookAt / view
// matrix moves the camera around it.
//
//   LEFT / RIGHT : orbit the camera around the triangle
//   UP / DOWN    : orbit up and down
//   W / S        : move the camera closer / further
//   ESC          : quit
//
// The camera starts at the origin looking down +z, which makes the view matrix
// the identity: the picture is the same one slide 67 shows.
// ---------------------------------------------------------------------------
#include "GamesEngineeringBase.h"

#include "Camera.h"
#include "Math.h"
#include "Triangle.h"

static const float NEAR_PLANE = 0.1f;
static const float FAR_PLANE = 100.0f;

// Keep the camera out of the degenerate configurations: looking straight up
// makes 'up' parallel to the view direction, and getting too close crosses the
// near plane.
static const float MAX_PITCH = 1.4f;
static const float MIN_DISTANCE = 0.35f;
static const float MAX_DISTANCE = 6.0f;

int main()
{
	GamesEngineeringBase::Window canvas;
	canvas.create(1280, 720, "Computer Graphics - LookAt");

	const float width = static_cast<float>(canvas.getWidth());
	const float height = static_cast<float>(canvas.getHeight());

	// Projection matrix with a 45 degree field of view (slide 66)
	Matrix projection;
	projection.setPerspective(45.0f, width / height, NEAR_PLANE, FAR_PLANE);

	// The vertices from slide 66, now in world space
	const Vec3 position[3] = {
		Vec3(0.0f, 0.3f, 1.0f),		// top
		Vec3(0.3f, -0.3f, 1.0f),	// bottom right
		Vec3(-0.3f, -0.3f, 1.0f),	// bottom left
	};

	// Slide 67 draws a flat dark green triangle: (0, 40, 0) in 0..255
	const Colour colour(0, 40, 0);

	// The camera always looks at the middle of the triangle and orbits it
	const Vec3 target(0.0f, 0.0f, 1.0f);
	const Vec3 worldUp(0.0f, 1.0f, 0.0f);

	float yaw = 0.0f;
	float pitch = 0.0f;
	float distance = 1.0f;			// puts the camera at the origin to start with

	GamesEngineeringBase::Timer timer;

	while (true)
	{
		canvas.checkInput();
		if (canvas.keyPressed(VK_ESCAPE))
			break;

		// --- orbit controls ------------------------------------------------
		const float dt = timer.dt();

		if (canvas.keyPressed(VK_LEFT))  yaw -= 1.5f * dt;
		if (canvas.keyPressed(VK_RIGHT)) yaw += 1.5f * dt;
		if (canvas.keyPressed(VK_UP))    pitch += 1.2f * dt;
		if (canvas.keyPressed(VK_DOWN))  pitch -= 1.2f * dt;
		if (canvas.keyPressed('W'))      distance -= 1.2f * dt;
		if (canvas.keyPressed('S'))      distance += 1.2f * dt;

		if (pitch > MAX_PITCH) pitch = MAX_PITCH;
		if (pitch < -MAX_PITCH) pitch = -MAX_PITCH;
		if (distance < MIN_DISTANCE) distance = MIN_DISTANCE;
		if (distance > MAX_DISTANCE) distance = MAX_DISTANCE;

		// Camera position: the target minus a direction, so that yaw = 0 and
		// pitch = 0 leaves the camera at (0, 0, 0) looking down +z.
		const float cosPitch = cosf(pitch);
		const Vec3 direction(
			sinf(yaw) * cosPitch,
			sinf(pitch),
			cosf(yaw) * cosPitch);
		const Vec3 cameraPosition = target - direction * distance;

		// --- world -> clip ------------------------------------------------
		// View matrix from the camera, then compose it with the projection
		// (slides 75 - 79). Our product applies the right hand side first, so
		// P * V means "view first, then project".
		const Matrix view = MakeViewMatrix(cameraPosition, target, worldUp);
		const Matrix viewProjection = projection * view;

		// --- project the three vertices -----------------------------------
		Vec4 screen[3];
		for (int i = 0; i < 3; ++i)
			screen[i] = ToScreen(viewProjection, Vec4(position[i].x, position[i].y, position[i].z, 1.0f),
				width, height);

		const Triangle triangle(screen[0], screen[1], screen[2], colour, colour, colour);

		canvas.clear();				// black background
		triangle.draw(canvas);
		canvas.present();
	}

	return 0;
}
