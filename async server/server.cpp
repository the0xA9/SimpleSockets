#pragma warning(disable:4996)

#include <iostream>
#include <vector>
#include <thread>

#include "Socket.h"
#include "ServerHandler.h"

using namespace std;

void main()
{	
	Socket::Waitable server("", "27015", MAKEWORD(2, 2),
			AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);
	try {

		server.init();
		server.socket();
		server.bind();
		server.listen();

		server.async();

		while (true) {
			
			size_t count = server.select();

			for (int it = 0; it < count; ++it) {

				Socket::Socket client = server.asyncGet(it);

				if (client == server) {
					Socket::Socket newClient = server.accept();

					server.asyncSet(newClient);

					newClient.send("Connected!\n");
				}
				else {
					std::string msg, response;
					if (client.receive(msg)) { //TODO check for closed connection
						std::cout << "client: " << msg << std::endl;

						if (Server::Handle(msg, response))
							client.send(response);
						else
							client.send("Server Error: Invalid params!\n");
					}
				}
			}
		}
		server.shutdown();
		system("pause");
	}
	catch (Socket::Socket::BaseError err) {
		std::cout << (std::string)err << std::endl;

		system("pause");
	}
}