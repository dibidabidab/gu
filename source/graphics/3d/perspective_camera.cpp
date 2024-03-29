
#include "perspective_camera.h"

PerspectiveCamera::PerspectiveCamera(float near_, float far_, float viewportWidth, float viewportHeight, float fOV)
    
    : Camera(near_, far_, viewportWidth, viewportHeight),
      fOV(fOV)
{
}

void PerspectiveCamera::update()
{
    projection = glm::perspective(glm::radians(fOV), viewportHeight == 0 ? 0 : viewportWidth / viewportHeight, near_, far_);
    view = glm::lookAt(
        position,
        position + direction,
        up
    );
    combined = projection * view;
}

bool PerspectiveCamera::isInViewPort(const vec4 &homogeneousCoordinates, const vec3 &h3Coordinates) const
{
    return homogeneousCoordinates.z >= 0 && Camera::isInViewPort(homogeneousCoordinates, h3Coordinates);
}
