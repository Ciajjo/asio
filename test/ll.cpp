#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>

#include "demo.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using hello::Greeter;
using hello::HelloRequest;
using hello::HelloResponse;

class GreeterServicelmpl final : public Greeter::Service
{
    ::grpc::Status SayHello(::grpc::ServerContext *context,
                            const ::hello::HelloRequest *request, ::hello::HelloResponse *response)
                            {
                                std::string lf("llfc grpc server has received:");
                                response->set_message(lf + request->message());
                                return Status::OK;
                            }
};

void RunServer()
{
    std::string server_address("0.0.0.0:50051");
    GreeterServicelmpl service;
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listrning on" << std::endl;
    server->Wait();
}

int main()
{
    RunServer();
    return 0;
}