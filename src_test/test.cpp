#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <ode/ode.h>

dWorld* w;
dHashSpace* s;
dJointGroup* g;
int main(int argc, char** argv)
{
    using namespace std::chrono_literals;

    dInitODE();
    {
        dWorld world{};
        dHashSpace space{};
        dJointGroup jgroup{};
        w = &world;
        s = &space;
        g = &jgroup;

        struct ss
        {
            dWorld* w;
            dHashSpace* s;
            dJointGroup* g;
        } dsad = {&world, &space, &jgroup};

        world.setGravity(0, -9.8, 0);

        dBody box{};
        dMass box_m{};
        dBox box_g;
        box_m.setBoxTotal(1, 0.5, 1, 1);
        box.create(world);
        box.setMass(box_m);
        box.setPosition(0, 10, 0);
        box_g.create(space, 0.5, 1, 1);
        box_g.setBody(box);

        dBox p;
        p.create(space, 1, 1, 1);
        p.setPosition(0, 0, 0);

        while (true)
        {
            std::this_thread::sleep_for(8ms);
            space.collide(&dsad,
                          [](void* data, dGeomID o1, dGeomID o2)
                          {
                              dBodyID b1 = dGeomGetBody(o1);
                              dBodyID b2 = dGeomGetBody(o2);
                              dContact contact;
                              contact.surface.mode = dContactBounce;
                              // friction parameter
                              contact.surface.mu = dInfinity;
                              // bounce is the amount of "bouncyness".
                              contact.surface.bounce = 0.0;
                              // bounce_vel is the minimum incoming velocity to cause a bounce
                              contact.surface.bounce_vel = 0.1;
                              // constraint force mixing parameter
                              contact.surface.soft_cfm = 0.001;
                              if (int numc = dCollide(o1, o2, 1, &contact.geom, sizeof(dContact)))
                              {
                                  dContactJoint c;
                                  c.create(*w, *g, &contact);
                                  dJointAttach(c, b1, b2);
                              }
                          });

            world.step(1 / (double)60);
            jgroup.empty();
            std::cout << " " << box.getRotation()[0] << " " << box.getPosition()[1] << " " << box.getPosition()[2]
                      << std::endl;
        }
    }

    dCloseODE();
    return 0;
}