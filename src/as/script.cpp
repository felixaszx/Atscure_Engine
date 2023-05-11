#include "as/script.hpp"

void as::DeviceI::close_window() const
{
    glfwSetWindowShouldClose(window_, true);
}

void as::DeviceI::set_cursor_state(bool enable) const
{
    glfwSetInputMode(window_, GLFW_CURSOR, //
                     enable ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

as::PhysicalWorldComp::PhysicalWorldComp()
{
    world_ = std::make_unique<dWorld>();
    space_ = std::make_unique<dHashSpace>();
}

as::DynamicBodyComp::DynamicBodyComp(const PhysicalWorldComp& world)
{
    this->reset(new dBody(*world.world_));
}

void as::DynamicBodyComp::update_mass()
{
    this->get()->setMass(this);
}

void as::RigidShapeComp::set_sphere(const PhysicalWorldComp& world, float radius)
{
    this->reset(new dSphere(*world.space_, radius));
}

void as::RigidShapeComp::set_box(const PhysicalWorldComp& world, glm::vec3 extend)
{
    this->reset(new dBox(*world.space_, extend.x, extend.y, extend.z));
};

void as::RigidShapeComp::set_plane(const PhysicalWorldComp& world, float a, float b, float c, float d)
{
    this->reset(new dPlane(*world.space_, a, b, c, d));
}

void as::RigidShapeComp::set_capsule(const PhysicalWorldComp& world, float radius, float height)
{
    this->reset(new dCapsule(*world.space_, radius, height));
}

void as::RigidShapeComp::set_cylinder(const PhysicalWorldComp& world, float radius, float height)
{
    this->reset(new dCylinder(*world.space_, radius, height));
}

void as::RigidShapeComp::set_body(const DynamicBodyComp& body)
{
    this->get()->setBody(*body);
}
