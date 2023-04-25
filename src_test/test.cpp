#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "third_party/glms.hpp"
#include <ode/ode.h>

dWorld* w{};
dHashSpace* s{};
dJointGroup* g{};

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
        box_m.setBoxTotal(1000, 1, 1, 1);
        box.create(world);
        box.setMass(box_m);
        box.setPosition(0, 2, 0);
        box_g.create(space, 1, 1, 1);
        box_g.setBody(box);

        dBox p;
        p.create(space, 100, 1, 100);
        p.setPosition(0, 0, 0);

        while (true)
        {
            std::this_thread::sleep_for(8ms);
            space.collide(&dsad,
                          [](void* data, dGeomID o1, dGeomID o2)
                          {
                              dBodyID b1 = dGeomGetBody(o1);
                              dBodyID b2 = dGeomGetBody(o2);
                              dContact contact[10]{};
                              int numc = dCollide(o1, o2, 10, &contact[0].geom, sizeof(dContact));
                              for (int i = 0; i < 10; i++)
                              {
                                  contact[i].surface.mode = dContactBounce;
                                  contact[i].surface.mu = 0;
                              }

                              for (int i = 0; i < numc; i++)
                              {
                                  dContactJoint c;
                                  c.create(*w, *g, contact + i);
                                  c.attach(b1, b2);
                              }
                          });

            world.step(1 / (double)60);
            jgroup.empty();
            std::cout << " " << box.getPosition()[0] << " " << box.getPosition()[1] << " " << box.getPosition()[2]
                      << std::endl;
        }
    }

    dCloseODE();
    return 0;
}