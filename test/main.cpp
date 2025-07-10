// #include <iostream>
// #include <csignal>
// #include <thread>
// #include <mutex>

// #include "CServer.h"
// using namespace std;

// bool bstop = false;
// std::condition_variable cond;
// std::mutex mut;

// void sig_handler(int sig)
// {
// 	if (sig == SIGINT || sig == SIGTERM)
// 	{
// 		std::unique_lock<mutex> lock(mut);
// 		bstop = true;
// 		cond.notify_one();
// 	}
// }

// int main()
// {
// 	try
// 	{
// 		auto pool = AsioIOServicePool::getIntance();
// 		boost::asio::io_context io_context;
// 		std::thread netWorkThread([&io_context]()
// 								  {
// 		CServer s(io_context, 10086);
// 		io_context.run(); });

// 		struct sigaction sa;
// 		sa.sa_handler = sig_handler;
// 		sigemptyset(&sa.sa_mask);
// 		sa.sa_flags = 0;

// 		if (sigaction(SIGINT, &sa, nullptr) == -1)
// 		{
// 			perror("sigaction(SIGINT) failed");
// 			exit(EXIT_FAILURE);
// 		}

// 		if (sigaction(SIGTERM, &sa, nullptr) == -1)
// 		{
// 			perror("sigaction(SIGTERM) failed");
// 			exit(EXIT_FAILURE);
// 		}

// 		while (!bstop)
// 		{
// 			std::unique_lock<std::mutex> lock(mut);
// 			cond.wait(lock);
// 		}

// 		io_context.stop();

// 		netWorkThread.join();
// 	}
// 	catch (std::exception &e)
// 	{
// 		std::cerr << "Exception: " << e.what() << endl;
// 	}
// 	boost::asio::io_context io_context;
// }