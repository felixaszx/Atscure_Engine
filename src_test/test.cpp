#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "third_party/glms.hpp"
#include <ode/ode.h>
#include <entt/entt.hpp>

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

        entt::registry reg;
        auto e = reg.create();
        dBody& ebox = reg.emplace<dBody>(e);
        dMass& ebox_m = reg.emplace<dMass>(e);
        dBox& ebox_g = reg.emplace<dBox>(e);
        ebox.create(world);
        ebox_m.setBoxTotal(1, 1, 1, 1);
        ebox.setMass(ebox_m);
        ebox.setPosition(0, 10, 0);
        ebox_g.create(space, 1, 1, 1);
        ebox_g.setBody(ebox);

        dBox p;
        p.create(space, 100, 1, 100);
        p.setPosition(0, 0, 0);

        reg.destroy(e);

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

                              for (int i = 0; i < numc; i++)
                              {
                                  contact[i].surface.mode = dContactBounce;
                                  contact[i].surface.bounce = 1.0f;
                                  dContactJoint c;
                                  c.create(*w, *g, contact + i);
                                  c.attach(b1, b2);
                              }
                          });

            world.step(1 / (double)60);
            jgroup.empty();
            break;

            // std::cout << ebox.getPosition()[0] << " " << ebox.getPosition()[1] << " " << ebox.getPosition()[2] <<
            // std::endl;
        }
    }

    dCloseODE();
    return 0;
}