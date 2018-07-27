#include <cmath>
using namespace std;

#include "Camera.h"
#include "inputdata.h"

//direct3d.cpp prototypes
void AdjustCamera(float x, float y, float z);

Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::Run()
{
	/*if(camXAngle > 2 * PI)
		camXAngle = 0.0f;
	if(camXAngle < 0.0f)
		camXAngle = 2 * PI;
	if(camYAngle > 3 * PI / 8)
		camYAngle = 3 * PI / 8;
	if(camYAngle < PI / 6)
		camYAngle = PI / 6;

	camZoom -= InputData->MouseZ / 50.0f;
	if(camZoom < 20.0f)
		camZoom = 20.0f;
	if(camZoom > 90.0f)
		camZoom = 90.0f;*/

	AdjustCamera((float)cos((double)camXAngle) * camZoom * (float)cos((double)camYAngle),
		(float)sin((double)camYAngle) * camZoom,
				 (float)sin((double)camXAngle) * camZoom * (float)cos((double)camYAngle));
}

void Camera::SetCamera(float zoom, float xAngle, float yAngle)
{
	camXAngle = xAngle;
	camYAngle = yAngle;
	camZoom = zoom;
	AdjustCamera((float)cos((double)camXAngle) * camZoom * (float)cos((double)camYAngle),
		(float)sin((double)camYAngle) * camZoom,
				 (float)sin((double)camXAngle) * camZoom * (float)cos((double)camYAngle));
}
