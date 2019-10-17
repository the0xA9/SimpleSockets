#pragma warning(disable:4996)

#include <iostream>

#include "Socket.h"

int main()
{
	Socket::Socket sock("127.0.0.1", "27015");

	try {
		sock.init();

		std::cout << "Connecting..." << std::endl;
		sock.connect();

		std::string msg;
		while (sock.receive(msg)) {
			std::cout << "server: " << msg << std::endl;
			std::cin >> msg;
			sock.send(msg);
		}

		sock.shutdown();
		return 0;

	} catch (Socket::Socket::BaseError err) {
		std::cout << (std::string)err << std::endl;
		system("pause");
		return 0;
	}
}