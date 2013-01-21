#ifndef _MESSAGEPROCESSOR_H_
#define _MESSAGEPROCESSOR_H_

#include <MessageBase.h>

class MessageProcessor;
typedef shared_ptr<MessageProcessor> MessageProcessorPtr;

typedef THMessagePtr (*CommandHandler)(THMessagePtr, void* privData);

class MessageProcessor
{
public:
    static MessageProcessorPtr GetInstance(MessageHeaderPtr header);
    virtual ~MessageProcessor();
    THMessagePtr ProcessMessage(THMessagePtr msg);
    static bool RegisterCommandHandler(Command cmd, CommandHandler handler);
private:
    MessageProcessor();
    typedef map<int, CommandHandler> HanlderMap;
    static HandlerMap m_commandHandlers;
};


#endif /* _MESSAGEPROCESSOR_H_ */
