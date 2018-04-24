#pragma once

struct INPUTDATA;

class Camera
{
public:
	Camera();
	~Camera();

	void Run(INPUTDATA* InputData);
	void SetCamera(float zoom, float xAngle, float yAngle);

	const float PI = 3.14159f;

private:
	float camXAngle;
	float camYAngle;
	float camZoom;
};

