#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

#include "TestMacros.h"

int main(int argc, char *argv[])
{
    string testPath = (argc > 1) ? std::string(argv[1]) : "";

    // Add customized listener.
    VerboseTextProgressListener progress;

    //
    TestFactoryRegistry &registry = TestFactoryRegistry::getRegistry();
    Test* test =  registry.makeTest();

    TextTestRunner runner;
    runner.eventManager().addListener(&progress);

    runner.addTest(test);

    runner.run();

    TestResultCollector& result = runner.result();

    return result.wasSuccessful() ? 0 : 1;
}

#if 0
#include "Socket.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "StringUtils.hpp"
#include "MessageBase.h"

#define handle_error(msg)                               \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define BUF_SIZE 500

// TODO: Remove this ifdef!
int
main(int argc, char *argv[])
{
    int sfd, s, j;
    size_t len;
    ssize_t nread;
    char buf[BUF_SIZE];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s  msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Socket* sock = Socket::CreateSocket(ST_TCP, "localhost", false);
    if (!sock)
    {
        printf("Failed to open socket!\n");
        return 1;
    }

    /* Send remaining command-line arguments as separate
       datagrams, and read responses from server */

    THMessagePtr message;
    MessageHeaderPtr header;
    shared_ptr<User> user;
    for (j = 1; j < argc; j++)
    {
        message.reset(NEW THMessage);
        header.reset(NEW MessageHeader);
        // Checks are skipped for this test app.
        header->set_version("20120730");
        if (j%2 == 0)
        {
            header->set_cmd(Plain);
        }
        else
        {
            header->set_cmd(RegisterUserReq);
        }
        header->set_session_id(string(""));
        message->SetMessageHeader(header);

        user.reset(NEW User);
        user->set_name("Hello");
        message->SetMessageBody(user);

        header->set_length(user->ByteSize() + sizeof(int16));

        printf("Sending  msg: %d\n", j-1);

        sock->Send(message);
        (void)(sock->Receive());
        printf("Received ...\n");
        sleep(1);
    }

    exit(EXIT_SUCCESS);
}

#endif // End of #if 0
