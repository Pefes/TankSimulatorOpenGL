#ifndef GLTANK_H
#define GLTANK_H

#include "GLobject.h"

namespace GLmodels
{
    class GLtank
    {
        public:
            GLtank();

            void left();
            void right();
            void forward();
            void backward();

            void turretLeft();
            void turretRight();

            void barrelUp();
            void barrelDown();

            void fire();

            virtual ~GLtank();

        private:

            class GLbarrel: public GLobject
            {
            public:
                GLbarrel(glm::vec3 position, std::string verticesPath, std::string texturePath);
                void rotate(Rotate direction, float angle, glm::mat4 turretMatrix);
                void move(glm::mat4 turretMatrix);

            private:
                float m_maxAngleBarrel;
            } m_barrel;


            class GLturret: public GLobject
            {
            public:
                GLturret(glm::vec3 position, std::string verticesPath, std::string texturePath);
                void rotate(Rotate direction, float angle, glm::mat4 bottomMatrix);
                void move(glm::mat4 bottomMatrix);
            } m_turret;


            class GLbottom: public GLobject
            {
            public:
                GLbottom(glm::vec3 position, std::string verticesPath, std::string texturePath);
                void rotate(Rotate direction, float angle);
                void move(Move direction, double distance);
            } m_bottom;


            class GLbullet: public GLobject
            {
            public:
                GLbullet(glm::vec3 position, std::string verticesPath, std::string texturePath);
                void move(glm::mat4 barrelMatrix);
            } m_bullet;
    };
}

#endif // GLTANK_H
