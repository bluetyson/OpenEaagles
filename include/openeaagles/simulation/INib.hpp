
#ifndef __oe_simulation_INib_H__
#define __oe_simulation_INib_H__

#include "openeaagles/base/Component.hpp"

namespace oe {

namespace osg { class Vec3d; }

namespace simulation {
class INetIO;

class INib : public base::Component
{
   DECLARE_SUBCLASS(INib, base::Component)

public:
   INib();

   virtual const base::String* getFederateName() const =0;           // Federate name as String

   virtual unsigned short getPlayerID() const =0;

   virtual INetIO* getNetIO() =0;

   // update incoming entity dead reckoning
   virtual bool updateDeadReckoning(
      const double dt,              // delta time (sec)
      osg::Vec3d* const pNewPos,    // New DR position
      osg::Vec3d* const pNewAngles  // New DR orientation
   ) =0;

   // DR's velocity vector @ T0 (m/sec)  (ECEF or Body based on the DR algorithm)
   virtual const osg::Vec3d& getDrVelocity() const =0;

   // DR's acceleration vector @ T0 ((m/sec)/sec) (ECEF or Body based on the DR algorithm)
   virtual const osg::Vec3d& getDrAcceleration() const =0;

   // DR's angular rates @ T0 (rad/sec)  [ phi theta psi ] (Body/ECEF)
   virtual const osg::Vec3d& getDrAngularVelocities() const =0;

};

}
}

#endif
