#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"

#include "Camera.hpp"
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
enum MOVE_DIRECTION { MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT };
enum ROTATE_DIRECTION { ROTATE_UP, ROTATE_DOWN, ROTATE_RIGHT, ROTATE_LEFT };
namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) { //target=front
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget; ;
        this->cameraUpDirection = cameraUp;
        this->cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        //TODO - Update the rest of camera parameters
        this->worldUp = cameraUp;
        this->Yaw = YAW;
        this->Pitch = PITCH;
        this->Zoom = ZOOM;
        this->Sensitivity = SENSITIVITY;
        this->Speed = SPEED;

       updateCameraVectors();
    }
    glm::vec3 Camera::getCameraTarget()
    {
        return cameraTarget;
    }
    void Camera::setCameraPos(glm::vec3 pos)
    {
        cameraPosition = pos;
    }
  
    void Camera::setCameraTarget(glm::vec3 dir)
    {
        cameraTarget = dir;

    }
    void Camera::setCameraFront(glm::vec3 dir)
    {
        cameraFrontDirection = dir;
    }

    glm::vec3 Camera::getCameraPos()
    {
        return cameraPosition;
    }



    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition,cameraTarget + cameraPosition, cameraUpDirection);
    }
    void Camera::rotate(ROTATE_DIRECTION direction, float speed) {

        switch (direction) {
        case ROTATE_UP:
            Pitch += speed;
            break;
        case ROTATE_DOWN:
            Pitch -= speed;
            break;
        case ROTATE_RIGHT:
            Yaw += speed;
            break;
        case ROTATE_LEFT:
            Yaw -= speed;
            break;
        }

        cameraTarget.x = cameraPosition.x + sin(this->Yaw);
        cameraTarget.z = cameraPosition.z - cos(this->Yaw);

        cameraTarget.y = cameraPosition.y + sin(this->Pitch);
        cameraTarget.y = cameraPosition.y - cos(this->Pitch);
    }



    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float deltaTime) {

        float velocity = Speed * deltaTime;
        switch (direction) {
        case MOVE_FORWARD:
            cameraPosition += cameraTarget * velocity;
            break;

        case MOVE_BACKWARD:
            cameraPosition -= cameraTarget * velocity;
            break;

        case MOVE_RIGHT:
            cameraPosition += cameraRightDirection * velocity;
            break;

        case MOVE_LEFT:
            cameraPosition -= cameraRightDirection * velocity;
            break;
        
        case MOVE_UP:
            cameraPosition += glm::vec3(0, 1, 0) * velocity;
            break;

        case MOVE_DOWN:
            cameraPosition -= glm::vec3(0, 1, 0) * velocity;
            break;
    }
      // if (cameraPosition.y <= 0.0f) {
        //  cameraPosition.y = 0.0;
       // }; // ca sa ramana pe ground

    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis


    void Camera::ProcessMouseMovement(float xoffset, float yoffset)
    {

        xoffset *= Sensitivity;
        yoffset *= Sensitivity;

        Yaw += xoffset;
        Pitch += yoffset;


        // make sure that when pitch is out of bounds, screen doesn't get flipped
       
           if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
       

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void Camera::ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
        updateCameraVectors();
    }
    void Camera:: updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        cameraTarget = glm::normalize(front);
        // also re-calculate the Right and Up vector
        cameraRightDirection = glm::normalize(glm::cross(cameraTarget, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        cameraUpDirection = glm::normalize(glm::cross(cameraRightDirection, cameraTarget));
    

       
    }

}