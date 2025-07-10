#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>

#include "demo.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using hello::Greeter;
using hello::HelloRequest;
using hello::HelloResponse;

class FCClient
{
public:
    FCClient(std::shared_ptr<Channel> channel) : stub_(Greeter::NewStub(channel)) {}

    std::string SayHello(std::string name)
    {
        ClientContext context;
        HelloRequest request;
        HelloResponse response;
        request.set_message(name);

        Status status = stub_->SayHello(&context, request, &response);
        if (status.ok())
        {
            return response.message();
        }
        else
        {
            return "failure" + status.error_message();
        }
    }

private:
    std::unique_ptr<Greeter::Stub> stub_;
};

int main()
{
    auto channel = grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());
    FCClient flient(channel);
    std::string resulit = flient.SayHello("hello, llfc");
    std::cout << resulit.c_str() << std::endl;
}