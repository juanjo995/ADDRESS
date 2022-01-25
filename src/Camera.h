#ifndef CAMERA_H_
#define CAMERA_H_

#include <iostream>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::lookAt, glm::perspective
#include <glm/gtc/epsilon.hpp> // glm::epsilonEqual

class Camera {
public:

	Camera(int width, int height);
	~Camera();

	const glm::mat4& getViewMatrix() const;
	const glm::mat4& getProjectionMatrix() const;
	glm::mat4 getViewProjectionMatrix() const;
	
	void setPosition(glm::vec3 p);
    glm::vec3 getPosition();
	void setLookAt(glm::vec3 l);
	void setRaLootAk(float ra);

	void setLookAt(float ra, float dec); // ra and dec must be given in degres
    void setLookAtFromPolarCoords(float ra, float dec);
    void setLookAtForFreeCamera(float ra, float dec);
    void setPosition(float ra, float dec, float distance);

    void setOrbit(float ra, float dec, float distance, glm::vec3 orbitPoint);

    void locate(const glm::vec3& p, const glm::vec3& l);

	void setAspectRatio(GLfloat a);

	void setFovY(GLfloat f);
	void setFovX(GLfloat f);

	void setZNear(GLfloat n);
	void setZFar(GLfloat f);

    void pan(float angle);
    void tilt(float angle);

    void orbitY(float angle);
    void orbitU(float angle);

    void truck(float distance);
    void dolly(float distance);
    void crane(float distance);

    void zoom(float zoom);

private:

	glm::mat4 view;
	glm::mat4 projection;

	void makeView();
    void makeViewFromPolarCoords(float ra);
	void makeProjection();

	glm::vec3 position;
	glm::vec3 lookAt;
    float raLookAt;

	glm::vec3 u, v, n;

	GLfloat aspectRatio;
	GLfloat fovY;

	GLfloat zNear;
	GLfloat zFar;

	bool areCollinear(const glm::vec3& v, const glm::vec3& u);
};

#endif  // CAMERA_H_
