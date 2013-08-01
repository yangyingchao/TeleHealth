#ifndef _MESSAGEPROCESSOR_H_
#define _MESSAGEPROCESSOR_H_

#include <MessageBase.h>
#include "AccountManager.h"
#include <ZmqWrapper.h>

class MessageProcessor;
typedef shared_ptr<MessageProcessor> MessageProcessorPtr;
typedef shared_ptr<Account> AccountPtr;

class MsgPrivate;

typedef THMessagePtr (*CommandHandler)(const THMessage&,
                                       MsgPrivate& privData);
typedef map<int, CommandHandler> HandlerMap;

class ConfigParser;

class MsgPrivate
{
public:
    MsgPrivate(){}
    virtual ~MsgPrivate(){}
    ZmqContextPtr m_pContext;
    shared_ptr<ConfigParser> m_pConfig;

    ZmqSocketPtr m_dbSock;
};

class WorkerThread;
class MessageProcessor
{
public:
    friend class WorkerThread;
    static bool RegisterCommandHandler(Command cmd, CommandHandler handler);
    MessageProcessor();
    virtual ~MessageProcessor();
    ZmqMessagePtr ProcessMessage(const ZmqMessagePtr& msg);
private:
    static HandlerMap m_commandHandlers;
    THMessagePtr GenericErrorResponse(const THMessage& tmsg, ErrorCode err);

    MsgPrivate m_privateData; //Private data of this processor, should be bind to one connection.
};


#endif /* _MESSAGEPROCESSOR_H_ */
