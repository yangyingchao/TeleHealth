#ifndef _ZMQWRAPPER_H_
#define _ZMQWRAPPER_H_

#include "SmartPointer.h"
#include <zmq.h>
#include <zmq.hpp>

using namespace std;
using namespace zmq;

typedef shared_ptr<context_t> ContextPtr;
typedef shared_ptr<message_t> MessagePtr;

#endif /* _ZMQWRAPPER_H_ */
