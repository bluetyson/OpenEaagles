
#ifndef __oe_interop_dis_factory_H__
#define __oe_interop_dis_factory_H__

#include <string>

namespace oe {
namespace base { class Object; }
namespace dis {
base::Object* factory(const std::string&);
}
}

#endif
