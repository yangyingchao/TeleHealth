#ifndef _MESSAGEPROCESSOR_H_
#define _MESSAGEPROCESSOR_H_

#include <MessageBase.h>
#include "AccountManager.h"
#include <ZmqWrapper.h>

class MessageProcessor;
typedef shared_ptr<MessageProcessor> MessageProcessorPtr;
typedef shared_ptr<Account> AccountPtr;

class MsgPrsPrivate;

typedef THMessagePtr (*CommandHandler)(const THMessage&, MsgPrsPrivate& privData);
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
    static bool RegisterCommandHandler(Command cmd, CommandHandler handler);
    virtual ~MessageProcessor();
    MessageProcessor();
    ZmqMessagePtr ProcessMessage(const ZmqMessagePtr& msg);
private:
    static HandlerMap m_commandHandlers;
    THMessagePtr GenericErrorResponse(const THMessage& tmsg, ErrorCode err);

    MsgPrsPrivate m_privateData; //Private data of this processor, should be bind to one connection.
};


#endif /* _MESSAGEPROCESSOR_H_ */
