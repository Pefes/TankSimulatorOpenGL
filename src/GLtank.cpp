#include "GLtank.h"
#include <thread>
#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace GLmodels;
using glm::inverse;

//obiekty tworzone sa przy inicjalizacji czolgu
GLtank::GLtank():
    m_barrel(glm::vec3(0, 0, 0), "models/tank/barrel.obj", "models/tank/cam.png"),
    m_turret(glm::vec3(0, 0, 0), "models/tank/turret.obj", "models/tank/cam.png"),
    m_bottom(glm::vec3(0, 0, 0), "models/tank/bottom.obj", "models/tank/cam.png"),
    m_bullet(glm::vec3(0, -10.0f, 0), "models/bullet/bullet.obj", "models/bullet/metal_scratches.png"){}

void GLtank::left()
{
    //te funkcje sa wykonywane rownolegle po to zeby mogla pojawic sie animacja
    auto f_a = [&](){m_barrel.move(m_turret.m_translatePos);};
    auto f_b = [&](){m_turret.move(m_bottom.m_translatePos);};
    auto f_c = [&](){m_bottom.rotate(GLobject::Rotate::LEFT, 0.01f);};

    std::thread t_a(f_a), t_b(f_b), t_c(f_c);

    /*jak sie zakonczy animacja kazdego elementu to wtedy mozna ponownie animowac(taka blokada jest slaba ale powoduje ze kilkakrotne nacisniecie
    przycisku nie powoduje wlaczenie kilku animacji na raz a dopiero jak sie zakonczy
    */
    t_a.join();
    t_b.join();
    t_c.join();
}

void GLtank::right()
{
    auto f_a = [&](){m_barrel.move(m_turret.m_translatePos);};
    auto f_b = [&](){m_turret.move(m_bottom.m_translatePos);};
    auto f_c = [&](){m_bottom.rotate(GLobject::Rotate::RIGHT, 0.01f);};

    std::thread t_a(f_a), t_b(f_b), t_c(f_c);

    t_a.join();
    t_b.join();
    t_c.join();
}

void GLtank::forward()
{
    auto f_a = [&](){m_barrel.move(m_turret.m_translatePos);};
    auto f_b = [&](){m_turret.move(m_bottom.m_translatePos);};
    auto f_c = [&](){m_bottom.move(GLobject::Move::FORWARD, 0.01f);};

    std::thread t_a(f_a), t_b(f_b), t_c(f_c);

    t_a.join();
    t_b.join();
    t_c.join();
}

void GLtank::backward()
{
    auto f_a = [&](){m_barrel.move(m_turret.m_translatePos);};
    auto f_b = [&](){m_turret.move(m_bottom.m_translatePos);};
    auto f_c = [&](){m_bottom.move(GLobject::Move::BACKWARD, 0.01f);};

    std::thread t_a(f_a), t_b(f_b), t_c(f_c);

    t_a.join();
    t_b.join();
    t_c.join();
}

void GLtank::turretLeft()
{
    auto f_a = [&](){m_turret.rotate(GLobject::Rotate::LEFT, 0.01f, m_bottom.m_translatePos);};
    auto f_b = [&](){m_barrel.move(m_turret.m_translatePos);};

    std::thread t_a(f_a), t_b(f_b);

    t_a.join();
    t_b.join();
}

void GLtank::turretRight()
{
    auto f_a = [&](){m_turret.rotate(GLobject::Rotate::RIGHT, 0.01f, m_bottom.m_translatePos);};
    auto f_b = [&](){m_barrel.move(m_turret.m_translatePos);};

    std::thread t_a(f_a), t_b(f_b);

    t_a.join();
    t_b.join();
}

void GLtank::barrelUp()
{
    auto f = [&](){m_barrel.rotate(GLobject::Rotate::UP, 0.01f, m_turret.m_translatePos);};

    std::thread t(f);

    t.join();
}

void GLtank::barrelDown()
{
    auto f = [&](){m_barrel.rotate(GLobject::Rotate::DOWN, 0.01f, m_turret.m_translatePos);};

    std::thread t(f);

    t.join();
}

void GLtank::fire()
{
    auto f = [&](){m_bullet.move(m_barrel.m_translatePos);};

    std::thread t(f);

    t.join();
}

GLtank::~GLtank()
{
    //dtor
}

GLtank::GLbarrel::GLbarrel(glm::vec3 position, std::string verticesPath, std::string texturePath):
    GLobject(position, verticesPath, texturePath)
{
    m_maxAngleBarrel = 0;
}

void GLtank::GLbarrel::rotate(Rotate direction, float angle, glm::mat4 turretMatrix)
{
//dzieki tej blokadzie naraz moze sie wykonywac tylko 1 operacja (rotacja lub przeniesienie)
    std::unique_lock<std::mutex> actionLock(m_actionPerforming);

    //dzieki tej blokadzie naraz moze sie wykonywac albo operacja (rotacja lub przeniesienie) albo rysowanie animacji
    std::unique_lock<std::mutex> drawingLock(m_changingValues);

        //rotacja wzgledem poczatku ukladu wspolrzednych(osie)
        switch(direction)
        {
            case Rotate::DOWN:
                m_rotateAxis = glm::vec3(0, 0, 1.0f);
                break;
            case Rotate::UP:
                m_rotateAxis = glm::vec3(0, 0, -1.0f);
                break;
        }

        //zamieniamy na radiany
        angle = glm::radians(angle);

        if(m_rotateAxis.z > 0) m_maxAngleBarrel -= angle;
        else m_maxAngleBarrel += angle;

        if(m_maxAngleBarrel < 0.3 && m_maxAngleBarrel > -0.01)
        {
            m_translatePos = glm::translate(m_translatePos, glm::vec3(-10.0f, 10.0f, 0));
            m_translatePos = glm::rotate(m_translatePos, angle,  m_rotateAxis);
            m_translatePos = glm::translate(m_translatePos, glm::vec3(10.0f, -10.0f, 0));

            m_rotateMatrix = inverse(turretMatrix) * m_translatePos;
        }
        else if(m_maxAngleBarrel > 0.3) m_maxAngleBarrel = 0.3;
        else if(m_maxAngleBarrel < -0.01) m_maxAngleBarrel = -0.01;
}

void GLtank::GLbarrel::move(glm::mat4 turretMatrix)
{
        m_translatePos =  turretMatrix * m_rotateMatrix;
}

GLtank::GLturret::GLturret(glm::vec3 position, std::string verticesPath, std::string texturePath):
    GLobject(position, verticesPath, texturePath)
{

}

void GLtank::GLturret::rotate(Rotate direction, float angle, glm::mat4 bottomMatrix)
{
//dzieki tej blokadzie naraz moze sie wykonywac tylko 1 operacja (rotacja lub przeniesienie)
    std::unique_lock<std::mutex> actionLock(m_actionPerforming);

    //dzieki tej blokadzie naraz moze sie wykonywac albo operacja (rotacja lub przeniesienie) albo rysowanie animacji
    std::unique_lock<std::mutex> drawingLock(m_changingValues);

    //zamieniamy na radiany
    angle = glm::radians(angle);

        //rotacja wzgledem poczatku ukladu wspolrzednych(osie)
        switch(direction)
        {
            case Rotate::LEFT:
                m_rotateAxis = glm::vec3(0, 1.0f, 0);
                break;

            case Rotate::RIGHT:
                m_rotateAxis = glm::vec3(0, -1.0f, 0);
                break;
        }

        m_translatePos = glm::translate(m_translatePos, glm::vec3(-2.0f, 0, 11.5f));
        m_translatePos = glm::rotate(m_translatePos, angle,  m_rotateAxis);
        m_translatePos = glm::translate(m_translatePos, glm::vec3(2.0f, 0, -11.5f));

        m_rotateMatrix = inverse(bottomMatrix) * m_translatePos;
}

void GLtank::GLturret::move(glm::mat4 bottomMatrix)
{
        m_translatePos =  bottomMatrix * m_rotateMatrix;
}

GLtank::GLbottom::GLbottom(glm::vec3 position, std::string verticesPath, std::string texturePath):
    GLobject(position, verticesPath, texturePath)
{

}

void GLtank::GLbottom::rotate(Rotate direction, float angle)
{
//dzieki tej blokadzie naraz moze sie wykonywac tylko 1 operacja (rotacja lub przeniesienie)
    std::unique_lock<std::mutex> actionLock(m_actionPerforming);

    //dzieki tej blokadzie naraz moze sie wykonywac albo operacja (rotacja lub przeniesienie) albo rysowanie animacji
    std::unique_lock<std::mutex> drawingLock(m_changingValues);

    //zamieniamy na radiany
    angle = glm::radians(angle);

        //rotacja wzgledem poczatku ukladu wspolrzednych(osie)
        switch(direction)
        {
            case Rotate::LEFT:
                m_rotateAxis = glm::vec3(0, 1.0f, 0);
                break;

            case Rotate::RIGHT:
                m_rotateAxis = glm::vec3(0, -1.0f, 0);
                break;
        }

        m_translatePos = glm::translate(m_translatePos, glm::vec3(0, 0, 11.5f));
        m_translatePos = glm::rotate(m_translatePos, angle,  m_rotateAxis);
        m_translatePos = glm::translate(m_translatePos, glm::vec3(0, 0, -11.5f));
}

void GLtank::GLbottom::move(Move direction, double distance)
{
    std::unique_lock<std::mutex> actionLock(m_actionPerforming);
    std::unique_lock<std::mutex> drawingLock(m_changingValues);

        switch(direction)
        {
            case Move::BACKWARD:
                m_moveAxis = glm::vec3(distance, 0, 0);
                break;

            case Move::FORWARD:
                m_moveAxis = glm::vec3(-distance, 0, 0);
                break;
        }

        m_translatePos = glm::translate(m_translatePos, glm::vec3(-m_position.x, -m_position.y, -m_position.z));
        m_translatePos = glm::translate(m_translatePos, glm::vec3(m_position.x + m_moveAxis.x, m_position.y + m_moveAxis.y, m_position.z + m_moveAxis.z));


    m_position += m_moveAxis;
}

GLtank::GLbullet::GLbullet(glm::vec3 position, std::string verticesPath, std::string texturePath):
    GLobject(position, verticesPath, texturePath)
{

}

void GLtank::GLbullet::move(glm::mat4 barrelMatrix)
{
    m_translatePos = barrelMatrix;
    m_translatePos = glm::rotate(m_translatePos, 1.570796326795f, glm::vec3(0, 0, 1.0f));
    m_translatePos = glm::translate(m_translatePos, glm::vec3(10.2f, 37.0f, 11.1f));
    m_translatePos = glm::scale(m_translatePos, glm::vec3(3.0f, 3.0f, 3.0f));

    for(int i = 0; i < 100; i++)
    {
        m_translatePos = glm::translate(m_translatePos, glm::vec3(0, 1.0f, 0));
        m_position += glm::vec3(0, 1.0f, 0);
        Sleep(25);
    }

    m_translatePos = glm::translate(m_translatePos, glm::vec3(-m_position.x - 50.0f, -m_position.y - 50.0f, -m_position.z));
    m_translatePos = glm::scale(m_translatePos, glm::vec3(-3.0f, -3.0f, -3.0f));

}
