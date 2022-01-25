#include "Camera.h"

Camera::Camera(int width, int height) {

	position = glm::vec3(0.0f, 0.0f, 0.0f);
	lookAt = glm::vec3(0.0f, 0.0f, 0.0f);

	makeView();

	aspectRatio = float(width) / float(height);
	setFovX(glm::radians(60.0f));

	zNear = 0.0005f;
	zFar = 10000000.0f;

	makeProjection();
}

Camera::~Camera() {}

const glm::mat4&
Camera::getViewMatrix() const {
	return view;
}

const glm::mat4&
Camera::getProjectionMatrix() const {
	return projection;
}

glm::mat4
Camera::getViewProjectionMatrix() const {
	return projection * view;
}

void
Camera::setPosition(glm::vec3 p) {
	position = p;
	makeView();
}

glm::vec3
Camera::getPosition() {
    return position;
}

void
Camera::setLookAt(glm::vec3 l) {
	lookAt = l;
	makeView();
}

void
Camera::setLookAtFromPolarCoords(float ra, float dec) {
    ra = -ra;
    lookAt = glm::vec3(glm::cos(ra) * glm::cos(dec), glm::sin(dec), glm::sin(ra) * glm::cos(dec));
    makeViewFromPolarCoords(ra);
}

void
Camera::setLookAt(float ra, float dec) {
    ra = -ra;
    setLookAt(glm::vec3(glm::cos(ra) * glm::cos(dec), glm::sin(dec), glm::sin(ra) * glm::cos(dec)));
}

void
Camera::setRaLootAk(float ra) {
    raLookAt = ra;
}

void
Camera::setLookAtForFreeCamera(float ra, float dec) {
    ra = -ra;
    lookAt = position + glm::vec3(glm::cos(ra) * glm::cos(dec), glm::sin(dec), glm::sin(ra) * glm::cos(dec));
    makeViewFromPolarCoords(ra);
}

void
Camera::setPosition(float ra, float dec, float distance) {
    setPosition(distance * glm::vec3(glm::cos(ra) * glm::cos(dec), glm::sin(dec), glm::sin(ra) * glm::cos(dec)));
}

void
Camera::locate(const glm::vec3& p, const glm::vec3& l) {
    position = p;
    lookAt = l;
    makeView();
}

void
Camera::setAspectRatio(GLfloat a) {
    aspectRatio = a;
    makeProjection();
}

void
Camera::setFovY(GLfloat f) {
	fovY = f;
	makeProjection();
}

void
Camera::setFovX(GLfloat f) {
	fovY = 2.0f * glm::atan(glm::tan(f / 2.0f) / aspectRatio);
	makeProjection();
}

void
Camera::setZNear(GLfloat n) {
	zNear = n;
	makeProjection();
}

void
Camera::setZFar(GLfloat f) {
	zFar = f;
	makeProjection();
}

void
Camera::makeView() {

    glm::vec3 yVec{ 0.0f, 1.0f, 0.0f };
    glm::vec3 raVec{ glm::cos(raLookAt), 0.0f, glm::sin(raLookAt) };

    glm::vec3 upVec;

    n = glm::normalize(position - lookAt);

    if (areCollinear(n, yVec)) {
        upVec = raVec;
    }
    else if (areCollinear(n, -yVec)) {
        upVec = -raVec;
    }
    else {
        upVec = yVec;
    }

    u = glm::normalize(glm::cross(upVec, n));
    v = glm::cross(n, u);

    view = glm::lookAt(position, lookAt, upVec);
}

void
Camera::setOrbit(float ra, float dec, float distance, glm::vec3 orbitPoint) {

    ra = -ra;
    position = orbitPoint + distance * glm::vec3(glm::cos(ra) * glm::cos(dec), glm::sin(dec), glm::sin(ra) * glm::cos(dec));
    lookAt = orbitPoint;

    glm::vec3 yVec{ 0.0f, 1.0f, 0.0f };
    glm::vec3 raVec{ glm::cos(raLookAt), 0.0f, glm::sin(raLookAt) };

    glm::vec3 upVec;

    n = glm::normalize(position - lookAt);

    if (areCollinear(n, yVec)) {
        upVec = raVec;
    }
    else if (areCollinear(n, -yVec)) {
        upVec = -raVec;
    }
    else {
        upVec = yVec;
    }

    u = glm::normalize(glm::cross(upVec, n));
    v = glm::cross(n, u);

    view = glm::lookAt(position, lookAt, upVec);
}

void
Camera::makeViewFromPolarCoords(float ra) {

    glm::vec3 yVec{ 0.0f, 1.0f, 0.0f };
    glm::vec3 raVec{ glm::cos(ra), 0.0f, glm::sin(ra) };

    glm::vec3 upVec;

    n = glm::normalize(position - lookAt);

    if (areCollinear(n, yVec)) {
        upVec = raVec;
    }
    else if (areCollinear(n, -yVec)) {
        upVec = -raVec;
    }
    else {
        upVec = yVec;
    }

    u = glm::normalize(glm::cross(upVec, n));
    v = glm::cross(n, u);

    view = glm::lookAt(position, lookAt, upVec);
}

void
Camera::makeProjection() {
	projection = glm::perspective(fovY, aspectRatio, zNear, zFar);
}

bool
Camera::areCollinear(const glm::vec3& v, const glm::vec3& u) {
	return	glm::epsilonEqual(v.x, u.x, glm::epsilon<float>()) &&
		glm::epsilonEqual(v.y, u.y, glm::epsilon<float>()) &&
		glm::epsilonEqual(v.z, u.z, glm::epsilon<float>());
}

void
Camera::pan(float angle) {
    // Rotating around Y axis instead of V camera vector offers a better behavior
    glm::mat3 rotate(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)));
    lookAt = rotate * (lookAt - position) + position;
    makeView();
}

void
Camera::tilt(float angle) {
    glm::mat3 rotate(glm::rotate(glm::mat4(1.0f), angle, u));
    lookAt = rotate * (lookAt - position) + position;
    makeView();
}

void
Camera::orbitY(float angle) {
    // Rotating around Y axis instead of V camera vector offers a better behavior
    glm::mat3 rotate(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)));
    position = rotate * (position - lookAt) + lookAt;
    makeView();
}

void
Camera::orbitU(float angle) {
    glm::mat3 rotate(glm::rotate(glm::mat4(1.0f), angle, u));
    position = rotate * (position - lookAt) + lookAt;
    makeView();
}

// Left/Right movement
void
Camera::truck(float distance) {
    position += (distance * u);
    lookAt += (distance * u);
    makeView();
}

// Forward/Back movement
void
Camera::dolly(float distance) {
    position -= (distance * n);
    lookAt -= (distance * n);
    makeView();
}

// Up/Down movement
void
Camera::crane(float distance) {
    position += (distance * v);
    lookAt += (distance * v);
    makeView();
}

void
Camera::zoom(float zoom) {
    GLfloat fovX = 2.0f * glm::atan(glm::tan(fovY / 2.0f) * aspectRatio);
    fovX += zoom;

    if (fovX < glm::radians(10.0f)) {
        fovX = glm::radians(10.0f);
    }
    else if (fovX > glm::radians(170.0f)) {
        fovX = glm::radians(170.0f);
    }

    setFovX(fovX);
}