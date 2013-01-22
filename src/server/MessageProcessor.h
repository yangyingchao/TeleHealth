#ifndef _MESSAGEPROCESSOR_H_
#define _MESSAGEPROCESSOR_H_

#include <MessageBase.h>

class MessageProcessor;
typedef shared_ptr<MessageProcessor> MessageProcessorPtr;
typedef THMessagePtr (*CommandHandler)(THMessagePtr, void* privData);
typedef map<int, CommandHandler> HandlerMap;

class MessageProcessor
{
public:
    static MessageProcessor* GetInstance(MessageHeaderPtr header);
    static bool RegisterCommandHandler(Command cmd, CommandHandler handler);
    virtual ~MessageProcessor();
    MessageProcessor();
    THMessagePtr ProcessMessage(THMessagePtr msg);
private:
    static HandlerMap m_commandHandlers;
    THMessagePtr GenericErrorResponse(MessageHeaderPtr header, ErrorCode err);
};


#endif /* _MESSAGEPROCESSOR_H_ */
