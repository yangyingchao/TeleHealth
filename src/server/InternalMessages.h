#ifndef _INTERNALMESSAGES_H_
#define _INTERNALMESSAGES_H_

#include "InternalMessages.pb.h"

using namespace std;
using namespace google::protobuf;

class ThreadMgtMsg
{
public:
    ThreadMgtMsg();
    virtual ~ThreadMgtMsg();
};

#endif /* _INTERNALMESSAGES_H_ */
