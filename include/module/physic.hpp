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

        using Geom = ReferedObj<dGeom>;
        using SphereShape = UniqueObj<dSphere>;
        using BoxShape = UniqueObj<dBox>;
        using PlaneShape = UniqueObj<dPlane>;
        using CapsuleShape = UniqueObj<dCapsule>;
        using CylinderShape = UniqueObj<dCylinder>;
        using RayShape = UniqueObj<dRay>;

        using rWorld = ReferedObj<dWorld>;
        using rHashSpace = ReferedObj<dHashSpace>;
        using rSimpleSpace = ReferedObj<dSimpleSpace>;
        using rQuadTreeSpace = ReferedObj<dQuadTreeSpace>;
        using rBody = ReferedObj<dBody>;
        using rJointGroup = ReferedObj<dJointGroup>;
        using rJoint = ReferedObj<dJoint>;
        using rBallJoint = ReferedObj<dBallJoint>;
        using rHingeJoint = ReferedObj<dHingeJoint>;
        using rSliderJoint = ReferedObj<dSliderJoint>;
        using rUniversalJoint = ReferedObj<dUniversalJoint>;
        using rPRJoint = ReferedObj<dPRJoint>;
        using rPUJoint = ReferedObj<dPUJoint>;
        using rPistonJoint = ReferedObj<dPistonJoint>;
        using rFixedJoint = ReferedObj<dFixedJoint>;
        using rContactJoint = ReferedObj<dContactJoint>;
        using rNullJoint = ReferedObj<dNullJoint>;
        using rAMotorJoint = ReferedObj<dAMotorJoint>;
        using rLMotorJoint = ReferedObj<dLMotorJoint>;

        using rGeom = ReferedObj<dGeom>;
        using rSphereShape = ReferedObj<dSphere>;
        using rBoxShape = ReferedObj<dBox>;
        using rPlaneShape = ReferedObj<dPlane>;
        using rCapsuleShape = ReferedObj<dCapsule>;
        using rCylinderShape = ReferedObj<dCylinder>;
        using rRayShape = ReferedObj<dRay>;

    }; // namespace phy
};     // namespace as

#endif // PHYSIC_HPP
