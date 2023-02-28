#ifndef Camera_hpp
#define Camera_hpp

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include <string>

namespace gps {

    enum MOVE_DIRECTION { MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT, MOVE_UP, MOVE_DOWN };
    enum ROTATE_DIRECTION { ROATE_UP, ROTATE_DOWN, ROTATE_RIGHT, ROTATE_LEFT };

    class Camera
    {
    public:
        //Camera constructor
        Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp);
        //return the view matrix, using the glm::lookAt() function
        glm::mat4 getViewMatrix();
        //update the camera internal parameters following a camera move event
        void move(MOVE_DIRECTION direction);
        //update the camera internal parameters following a camera rotate event
        //yaw - camera rotation around the y axis
        //pitch - camera rotation around the x axis
        void move(MOVE_DIRECTION direction,  float deltaTime);
        void rotate(ROTATE_DIRECTION direction, float speed);
        void ProcessMouseMovement(float xoffset, float yoffset);
        void ProcessMouseScroll(float yoffset);
        glm::vec3 getCameraTarget();
        void setCameraTarget(glm::vec3 dir);
        void setCameraPos(glm::vec3 pos);
        glm::vec3 getCameraUp();
        void setCameraUp(glm::vec3 dir);
        void setCameraDir(glm::vec3 dir);
        glm::vec3 getCameraFront();
        void setCameraFront(glm::vec3 dir);


        glm::vec3 getCameraPos();
        glm::vec3 getCameraDir();

    private:
        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget;
        glm::vec3 cameraFrontDirection;
        glm::vec3 cameraRightDirection;
        glm::vec3 cameraUpDirection;
        glm::vec3 worldUp;
        glm::vec3 cameraDirection;
        
        void updateCameraVectors();
        
        float Zoom;
        float Speed;
        float Sensitivity;
        float Yaw;
        float Pitch;

    };

}

#endif /* Camera_hpp */



