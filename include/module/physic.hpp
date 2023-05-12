#ifndef PHYSIC_HPP
#define PHYSIC_HPP

#include "ode/ode.h"
#include "as/utils.hpp"

namespace as
{
    class PhysicModule
    {
    };

    namespace phy
    {
        class Context
        {
          public:
            Context();
            ~Context();
        };

        using World = UniqueObj<dWorld>;
        using HashSpace = UniqueObj<dHashSpace>;
        using SimpleSpace = UniqueObj<dSimpleSpace>;
        using QuadTreeSpace = UniqueObj<dQuadTreeSpace>;
        using Body = UniqueObj<dBody>;
        using JointGroup = UniqueObj<dJointGroup>;
        using Joint = UniqueObj<dJoint>;
        using BallJoint = UniqueObj<dBallJoint>;
        using HingeJoint = UniqueObj<dHingeJoint>;
        using SliderJoint = UniqueObj<dSliderJoint>;
        using UniversalJoint = UniqueObj<dUniversalJoint>;
        using PRJoint = UniqueObj<dPRJoint>;
        using PUJoint = UniqueObj<dPUJoint>;
        using PistonJoint = UniqueObj<dPistonJoint>;
        using FixedJoint = UniqueObj<dFixedJoint>;
        using ContactJoint = UniqueObj<dContactJoint>;
        using NullJoint = UniqueObj<dNullJoint>;
        using AMotorJoint = UniqueObj<dAMotorJoint>;
        using LMotorJoint = UniqueObj<dLMotorJoint>;

        using Geom = VirtualObj<dGeom>;
        using SphereShape = UniqueObj<dSphere>;
        using BoxShape = UniqueObj<dBox>;
        using PlaneShape = UniqueObj<dPlane>;
        using CapsuleShape = UniqueObj<dCapsule>;
        using CylinderShape = UniqueObj<dCylinder>;
        using RayShape = UniqueObj<dRay>;

        using rWorld = VirtualObj<dWorld>;
        using rHashSpace = VirtualObj<dHashSpace>;
        using rSimpleSpace = VirtualObj<dSimpleSpace>;
        using rQuadTreeSpace = VirtualObj<dQuadTreeSpace>;
        using rBody = VirtualObj<dBody>;
        using rJointGroup = VirtualObj<dJointGroup>;
        using rJoint = VirtualObj<dJoint>;
        using rBallJoint = VirtualObj<dBallJoint>;
        using rHingeJoint = VirtualObj<dHingeJoint>;
        using rSliderJoint = VirtualObj<dSliderJoint>;
        using rUniversalJoint = VirtualObj<dUniversalJoint>;
        using rPRJoint = VirtualObj<dPRJoint>;
        using rPUJoint = VirtualObj<dPUJoint>;
        using rPistonJoint = VirtualObj<dPistonJoint>;
        using rFixedJoint = VirtualObj<dFixedJoint>;
        using rContactJoint = VirtualObj<dContactJoint>;
        using rNullJoint = VirtualObj<dNullJoint>;
        using rAMotorJoint = VirtualObj<dAMotorJoint>;
        using rLMotorJoint = VirtualObj<dLMotorJoint>;

        using rGeom = VirtualObj<dGeom>;
        using rSphereShape = VirtualObj<dSphere>;
        using rBoxShape = VirtualObj<dBox>;
        using rPlaneShape = VirtualObj<dPlane>;
        using rCapsuleShape = VirtualObj<dCapsule>;
        using rCylinderShape = VirtualObj<dCylinder>;
        using rRayShape = VirtualObj<dRay>;

    }; // namespace phy
};     // namespace as

#endif // PHYSIC_HPP
