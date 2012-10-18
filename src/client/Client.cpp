#include <iostream>
#include <errno.h>
#include <ftw.h>
#include <unistd.h>
#include <getopt.h>
#include <fstream>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include "THMessage.pb.h"
#include <tr1/memory>
#include "LogUtils.h"
#include "StringUtils.hpp"

using namespace std;
using namespace tr1;


shared_ptr<zmq::message_t> GenerateMessage(int i)
{
    shared_ptr<zmq::message_t> msg;

    Message bmsg;
    MessageHeader* header = bmsg.mutable_header();
    header->set_version("20120730");
    header->set_length(1024);
    if (i%2 == 0)
    {
        header->set_type(Plain);
    }
    else
    {
        header->set_type(Invalid);
    }
    header->set_session_id(string(""));

    bmsg.set_data(string("Message from client: ") +
                  StringUtils::Number2String(getpid()) + ", counter: " +
                  StringUtils::Number2String(i));

    assert(bmsg.has_header());
    string data;
    if (bmsg.SerializeToString(&data))
    {
        msg = shared_ptr<zmq::message_t>(new zmq::message_t((void*)data.c_str(),
                                                            data.size(), NULL));
    }

    if (i == 0)
    {
        char fileName[256] = {'\0'};
        sprintf(fileName, "/tmp/%lu.dump", (unsigned long)getpid());
        ofstream fs(fileName, ios::binary | ios::out);
        if (fs.is_open())
        {
            bmsg.SerializeToOstream(&fs);
            fs.flush();
            fs.close();
        }
        Message tmpMsg;
        if (!tmpMsg.ParseFromString(data))
        {
            PDEBUG ("failed to parse after serialize.\n");
        }
    }

    return msg;
}


int main ()
{
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    std::cout << "Connecting to hello world server..." << std::endl;
    socket.connect ("tcp://localhost:5555");

    for (int i = 0; i < 10; ++i)
    {
        shared_ptr<zmq::message_t> req = GenerateMessage(i);
        std::cout << "Sending Message "
                  << " size: " << req->size() << std::endl;
        socket.send (*req);
        sleep(1);
        //  Get the reply.
        zmq::message_t reply;
        socket.recv (&reply);
        if (reply.size())
        {
            Message msg;
            if (msg.ParseFromArray(reply.data(), reply.size()))
            {
                const MessageHeader& header = msg.header();
                PDEBUG ("Type: %d, version: %s\n", (int)header.type(),
                        header.version().c_str());
                if (header.type() % 2 != 0)
                {
                    cout << "Is this a response?" << endl;
                }
                if (header.result())
                {
                    cout << "OK" << endl;
                }
                else
                {
                    cout << "OOPS" << endl;
                }

                if (msg.has_data())
                {
                    cout << "Data = " << msg.data().c_str() << endl;
                }
            }
            else
            {
                PDEBUG ("Failed to parse replied data.\n");
            }
        }
    }

    return 0;
}
