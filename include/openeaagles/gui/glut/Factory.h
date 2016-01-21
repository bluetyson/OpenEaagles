//------------------------------------------------------------------------------
// Class: Factory
//
// Description: Class factory
//------------------------------------------------------------------------------
#ifndef __oe_Glut_Factory_H__
#define __oe_Glut_Factory_H__

namespace oe {

namespace Basic { class Object; }

namespace Glut {

class Factory
{
public:
   static Basic::Object* createObj(const char* name);

protected:
   Factory();   // prevent object creation
};

}  // end namespace Glut
}  // end namespace Eaagles

#endif
