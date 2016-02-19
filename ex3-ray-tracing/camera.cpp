#include "camera.h"
#include "scene.h"
#include <cmath>
#include <cstdio>

//#define ENABLE_DOF
#define DEFAULT_POS_VALUE 0.0
#define DEFAULT_FOV 1.0
#define LENS_RADIUS			0.15
#define LENS_SAMPLES_RADIUS	3
#define LENS_SAMPLES_THETA	3

Camera::Camera()
	: _position(DEFAULT_POS_VALUE), _coi(Vector3d(0.0, 0.0, 1.0)), _up(Vector3d(0.0, 1.0, 0.0)), _fov_h(30.0), _samples_per_pixel(1)
{

}

// constructor - create a camera with the given paramters
Camera::Camera(Point3d & pos, Point3d & coi, Vector3d & up, double fov, size_t samples_per_pixel)
	: _position(pos), _coi(coi), _up(up), _fov_h(fov), _samples_per_pixel(samples_per_pixel)
{

}

void Camera::setSamplesPerPixel(size_t samples_per_pixel)
{
	_samples_per_pixel = samples_per_pixel;
}

// Calculate a view with the given lens offset for the image
Camera::CameraView Camera::calculateView(const BImage &img, const Vector3d &offset) const
{
	CameraView view;

	view.position = _position + offset;

	int w = img.getWidth();
	int h = img.getHeight();

	Vector3d front = _coi - view.position;
	double dist = front.norm();

	front.normalize();

	Vector3d up = (_up - (_up | front) * front).normalize();
	Vector3d right = front % up;

	double halfViewWidth = dist * tan(_fov_h);
	double halfViewHeight = halfViewWidth * (double(h) / double(w));

	view.topLeft = _coi - halfViewWidth * right + halfViewHeight * up;
	view.dx = (2.0 * halfViewWidth / double(w)) * right;
	view.dy = -(2.0 * halfViewHeight / double(h)) * up;

	return view;
}

// render - create a bitmap image with the given properties from the given scene
void Camera::render(size_t row_start, size_t number_of_rows, BImage& img, Scene & scene) const
{
	vector<CameraView> views;

	if (!scene.isReady())
	{
		fprintf(stderr, "ERROR: Scene is not ready. Call Scene::completeSetup() first.\n");
		return;
	}

	views.push_back(calculateView(img, Vector3d(0, 0, 0)));

#ifdef ENABLE_DOF
	Vector3d front = (_coi - _position).normalized();
	Vector3d up = (_up - (_up | front) * front).normalize();
	Vector3d right = front % up;

	for (int i = 0; i < LENS_SAMPLES_THETA; i++)
	{
		for (int j = 0; j < LENS_SAMPLES_RADIUS; j++)
		{
			double theta = (double)i * (2 * PI / (double)(LENS_SAMPLES_THETA - 1));
			double r = (double)(j + 1) * (LENS_RADIUS / (double)LENS_SAMPLES_RADIUS);

			Vector3d offset = (right * cos(theta) + up * sin(theta)) * r;

			views.push_back(calculateView(img, offset));
		}
	}
#endif

	// Render all rows in range
	for (size_t i = row_start; i < row_start + number_of_rows; i++)
	{
		// Render all pixels in the row
		for (int j = 0; j < img.getWidth(); j++)
		{
			Color3d pixelColor(0.0);

			// Average all the time frames
			for (int time = 0; time < scene.totalTime(); time++)
			{
				Color3d timeColor(0.0);

				// Average all the views
				for (size_t v = 0; v < views.size(); v++)
				{
					Color3d viewColor(0.0);

					// Render either a single ray or average multiple rays
					if (_samples_per_pixel <= 1)
					{
						// Single ray through the center
						Vector3d d = (views[v].topLeft +
							((double)j + 0.5) * views[v].dx +
							((double)i + 0.5) * views[v].dy - views[v].position).normalize();
						Ray ray = Ray(views[v].position, d, time);
						viewColor = scene.traceRay(ray, 1.0, 1.0);
					}
					else
					{
						// Multiple rays in a rectangle
						for (size_t r = 0; r < _samples_per_pixel; r++)
						{
							Vector3d d = (views[v].topLeft +
								((double)j + (double)rand() / (double)RAND_MAX) * views[v].dx +
								((double)i + (double)rand() / (double)RAND_MAX) * views[v].dy - views[v].position).normalize();
							Ray ray = Ray(views[v].position, d, time);
							viewColor += scene.traceRay(ray, 1.0, 1.0);
						}

						// Average the rays
						viewColor /= _samples_per_pixel;
					}

					timeColor += viewColor;
				}

				// Average the views
				timeColor /= (double)views.size();
				pixelColor += timeColor;
			}

			// Average the time frames
			pixelColor /= (double)scene.totalTime();
			pixelColor *= 255.0;

			double r = pixelColor[0];
			double g = pixelColor[1];
			double b = pixelColor[2];

			// Clamp at [0, 255]
			r = (r > 255.0) ? 255.0 : ((r < 0) ? 0 : r);
			g = (g > 255.0) ? 255.0 : ((g < 0) ? 0 : g);
			b = (b > 255.0) ? 255.0 : ((b < 0) ? 0 : b);
			img(j, i) = Bpixel((uchar)r, (uchar)g, (uchar)b);
		}
	}
}
