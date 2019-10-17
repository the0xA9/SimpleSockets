#pragma warning(disable:4996)
#include <iostream>
#include <thread>
#include <list>

#include "Socket.h"
#include "ServerHandler.h"

using namespace std;

void multyclient(Socket::Socket socket)
{
	try {
		socket.send("Connected!");
		std::string msg, response;
		while (socket.receive(msg)) {
			std::cout << "client: " << msg << std::endl;

			if (Server::Handle(msg, response))
				socket.send(response);
			else
				socket.send("Server Error: Invalid params!\n");
		}
		socket.shutdown();
	}
	catch (Socket::Socket::BaseError err) {
		std::cout << (std::string)err << std::endl;
	}
}

void main()
{
	try {
		Socket::Socket server("", "27015", MAKEWORD(2, 2),
			AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);

		server.init();
		server.socket();
		server.bind();
		server.listen();

		list<thread> threads;
		while (true)
			threads.push_back(thread(multyclient, server.accept()));
		
	}
	catch (Socket::Socket::BaseError err) {
		std::cout << (string)err << std::endl;
		system("pause");
	}
}