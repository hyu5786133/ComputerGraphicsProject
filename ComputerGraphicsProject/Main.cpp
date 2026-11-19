// ---------------------------------------------------------------------------
// Rasterization practical - clipping and the z buffer (slides 80 - 107)
//
// Every triangle goes through the full pipeline: transform -> clip against
// the near and far planes -> rasterise -> depth test -> perspective correct
// interpolation.
//
//   LEFT / RIGHT : orbit the camera around the scene
//   UP / DOWN    : orbit up and down
//   W / S        : move the camera closer / further (fly into the geometry
//                  to see the near plane clipping do its job)
//   ESC          : quit
// ---------------------------------------------------------------------------
#include "GamesEngineeringBase.h"

#include "Camera.h"
#include "Clipper.h"
#include "DepthBuffer.h"
#include "Math.h"
#include "Renderer.h"
#include "Scene.h"
#include "Triangle.h"

static const float NEAR_PLANE = 0.1f;
static const float FAR_PLANE = 100.0f;

static const float MAX_PITCH = 1.4f;
static const float MIN_DISTANCE = 0.25f;
static const float MAX_DISTANCE = 8.0f;

int main()
{
	GamesEngineeringBase::Window canvas;
	canvas.create(1280, 720, "Computer Graphics - Clipping and Z buffer");

	const float width = static_cast<float>(canvas.getWidth());
	const float height = static_cast<float>(canvas.getHeight());

	// Projection with a 45 degree field of view (slide 66)
	Matrix projection;
	projection.setPerspective(45.0f, width / height, NEAR_PLANE, FAR_PLANE);

	// The near and far clip planes come out of the projection matrix
	// (slides 81 - 82). They never change, so extract them once.
	Vec4 nearPlane, farPlane;
	ExtractNearFarPlanes(projection, nearPlane, farPlane);

	DepthBuffer depthBuffer(canvas.getWidth(), canvas.getHeight());

	const std::vector<SceneTriangle> scene = MakeTestScene();

	// Camera orbit state
	const Vec3 target(0.0f, 0.0f, 0.0f);
	const Vec3 worldUp(0.0f, 1.0f, 0.0f);

	float yaw = 0.5f;
	float pitch = 0.34f;
	float distance = 6.5f;

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
		if (canvas.keyPressed('W'))      distance -= 2.0f * dt;
		if (canvas.keyPressed('S'))      distance += 2.0f * dt;

		if (pitch > MAX_PITCH) pitch = MAX_PITCH;
		if (pitch < -MAX_PITCH) pitch = -MAX_PITCH;
		if (distance < MIN_DISTANCE) distance = MIN_DISTANCE;
		if (distance > MAX_DISTANCE) distance = MAX_DISTANCE;

		const float cosPitch = cosf(pitch);
		const Vec3 direction(
			sinf(yaw) * cosPitch,
			sinf(pitch),
			cosf(yaw) * cosPitch);
		const Vec3 cameraPosition = target - direction * distance;

		// --- matrices -------------------------------------------------------
		const Matrix view = MakeViewMatrix(cameraPosition, target, worldUp);

		// --- draw -----------------------------------------------------------
		canvas.clear();					// black background
		depthBuffer.clear();			// z_buffer[x][y] <- 1 for every pixel (slide 86)

		for (const SceneTriangle& triangle : scene)
		{
			RenderTriangle(canvas, view, projection, nearPlane, farPlane, depthBuffer,
				triangle.position[0], triangle.position[1], triangle.position[2],
				triangle.colour[0], triangle.colour[1], triangle.colour[2]);
		}

		canvas.present();
	}

	return 0;
}


