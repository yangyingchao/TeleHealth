#ifndef _MESSAGEPROCESSOR_H_
#define _MESSAGEPROCESSOR_H_

#include <MessageBase.h>

class MessageProcessor;
typedef shared_ptr<MessageProcessor> MessageProcessorPtr;


class MessageProcessor
{
public:
    static MessageProcessorPtr GetInstance(MessageHeaderPtr header);
    virtual ~MessageProcessor();
private:
    MessageProcessor();
};


#endif /* _MESSAGEPROCESSOR_H_ */
