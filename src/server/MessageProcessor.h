#ifndef _MESSAGEPROCESSOR_H_
#define _MESSAGEPROCESSOR_H_

#include <MessageBase.h>
#include "AccountManager.h"

class MessageProcessor;
typedef shared_ptr<MessageProcessor> MessageProcessorPtr;
typedef shared_ptr<Account> AccountPtr;

class MsgPrsPrivate;

typedef THMessagePtr (*CommandHandler)(THMessagePtr, MsgPrsPrivate& privData);
typedef map<int, CommandHandler> HandlerMap;

class MsgPrsPrivate
{
public:
    MsgPrsPrivate();
    virtual ~MsgPrsPrivate();

    void Reset();

    string       m_errMsg;
    AccountPtr   m_account;

    AccountStatus m_status;

    string m_sessionId;
};


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

    MsgPrsPrivate m_privateData; //Private data of this processor, should be bind to one connection.
};


#endif /* _MESSAGEPROCESSOR_H_ */
