#ifndef _MESSAGEPROCESSOR_H_
#define _MESSAGEPROCESSOR_H_

#include <MessageBase.h>
#include "AccountManager.h"
#include <ZmqWrapper.h>
#include "ConfigParser.h"

typedef shared_ptr<Account> AccountPtr;
typedef THMessagePtr (*CommandHandler)(const THMessage&,
                                       const ZmqContextPtr& gContext,
                                       const ConfigParserPtr& gConfig);

/**
 * @name RegisterCommandHandler - Register handler to process given command
 * @param cmd -  cmd
 * @param handler -  handler
 * @return true if succeeded.
 */
bool RegisterCommandHandler(Command cmd, CommandHandler handler);

/**
 * @name ProcessMessage - Process a message.
 * @param msg -  message to processed
 * @return ZmqMessagePtr to returned to client.
 */
ZmqMessagePtr ProcessMessage(const ZmqMessagePtr& msg,
                             const ZmqContextPtr& gContext,
                             const ConfigParserPtr& gConfig);


#endif /* _MESSAGEPROCESSOR_H_ */
