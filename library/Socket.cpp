#include "Socket.h"

#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

namespace Socket {

	Socket::Socket(SOCKET socket)
		: _pNodeName()
		, _pServiceName()
		, _wVersionRequested()
		, _ai_family()
		, _ai_socktype()
		, _ai_protocol()
		, _ai_flags()
		, _socket(socket)
		, _result(nullptr)
	{ }

	Socket::Socket(std::string pNodeName, std::string pServiceName, WORD wVersionRequested,
		int ai_family, int ai_socktype, IPPROTO ai_protocol, int ai_flags) noexcept
		: _pNodeName(pNodeName)
		, _pServiceName(pServiceName)
		, _wVersionRequested(wVersionRequested)
		, _ai_family(ai_family)
		, _ai_socktype(ai_socktype)
		, _ai_protocol(ai_protocol)
		, _ai_flags(ai_flags)
		, _socket(INVALID_SOCKET)
		, _result(nullptr)
	{ }

	Socket::~Socket()
	{
		freeaddrinfo(_result);
	}

	void Socket::init() throw (InitialisationError)
	{
		int handle;
		WSADATA _wsaData;

		if (handle = WSAStartup(_wVersionRequested, &_wsaData))
			throw InitialisationError(Arguments("WSAStartup failed with error: %d", handle));

		addrinfo hints;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = _ai_family;
		hints.ai_socktype = _ai_socktype;
		hints.ai_protocol = _ai_protocol;
		hints.ai_flags = _ai_flags;

		if (handle = getaddrinfo(_pNodeName.size() ? _pNodeName.c_str() : nullptr, _pServiceName.size() ? _pServiceName.c_str() : nullptr, &hints, &_result))
			throw InitialisationError(Arguments("getaddrinfo failed with error: %d", handle));
	}

	void Socket::socket() throw (InitialisationError)
	{
		if (!_result)
			throw ConnectionError("socket is not initialized!");

		if ((_socket = ::socket(_result->ai_family, _result->ai_socktype, _result->ai_protocol)) != INVALID_SOCKET)
			return;

		throw InitialisationError(Arguments("socket failed with error: %ld", WSAGetLastError()));
	}

	void Socket::connect() throw(ConnectionError)
	{
		if (!_result)
			throw ConnectionError("socket is not initialized!");

		// Attempt to connect to an address until one succeeds
		for (auto ptr = _result; ptr != NULL; ptr = ptr->ai_next) {

			// Create a SOCKET for connecting to server
			_socket = ::socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);

			if (_socket == INVALID_SOCKET)
				throw ConnectionError(Arguments("socket failed with error: %ld", WSAGetLastError()));


			// Connect to server.
			if (::connect(_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
				closesocket(_socket);
				_socket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		if (_socket != INVALID_SOCKET)
			return;

		throw ConnectionError("Unable to connect to server!");
	}

	void Socket::bind() throw(BindError)
	{
		if (!_result)
			throw BindError("socket is not initialized!");

		if (::bind(_socket, _result->ai_addr, (int)_result->ai_addrlen) != SOCKET_ERROR)
			return;

		throw BindError(Arguments("bind failed with error: %d", WSAGetLastError()));
	}

	void Socket::listen() throw (ListenError)
	{
		if (_socket == INVALID_SOCKET)
			throw ListenError("Invalid Socket");

		if (::listen(_socket, SOMAXCONN) != SOCKET_ERROR)
			return;

		throw ListenError(Arguments("listen failed with error: %d", WSAGetLastError()));
	}

	Socket Socket::accept() throw (AcceptError)
	{
		if (_socket == INVALID_SOCKET)
			throw AcceptError("Invalid Socket");
		
		SOCKET ret = INVALID_SOCKET;
		if ((ret = ::accept(_socket, NULL, NULL)) != INVALID_SOCKET)
			return ret;

		throw AcceptError(Arguments("accept failed with error: %d", WSAGetLastError()));
	}

	void Socket::send(const std::string& msg) throw (SendError)
	{
		if (_socket == INVALID_SOCKET)
			throw SendError("Invalid Socket");

		int size;
		if ((size = ::send(_socket, msg.c_str(), msg.length(), 0)) == SOCKET_ERROR)
			throw SendError(Arguments("send failed with error: %d", WSAGetLastError()));

		print("Bytes Sent: %ld\n", size);
	}

	bool Socket::receive(std::string& _msg, const size_t _recvbuflen, int _flags, size_t* _size) throw (ReceiveError)
	{
		if (_socket == INVALID_SOCKET)
			throw ReceiveError("Invalid Socket");
		
		std::unique_ptr<char> recvbuf(new char[_recvbuflen]);
		int size = recv(_socket, recvbuf.get(), _recvbuflen, _flags);

		if (size > 0) {
			print("Bytes received: %d\n", size);

			_msg.assign(recvbuf.get(), size);

			if (_size)
				* _size = size;
			return true;
		}

		if (size == 0 || WSAGetLastError() == WSAECONNRESET) {
			print("Connection closed\n");
			return false;
		}
		throw ReceiveError(Arguments("recv failed with error: %d", WSAGetLastError()));
	}

	void Socket::shutdown() throw (ShutdownError)
	{
		if (_socket == INVALID_SOCKET)
			throw ShutdownError("Invalid Socket");

		if (::shutdown(_socket, SD_SEND) != SOCKET_ERROR)
			return;

		throw ShutdownError(Arguments("shutdown failed with error: %d", WSAGetLastError()));
	}

	const bool Socket::operator==(const Socket& rhs) const noexcept
	{
		if (this->_socket != rhs._socket)
			return false;

		return true;
	}

	void Socket::cleanup()
	{
		closesocket(_socket);
		WSACleanup();
	}

	inline void Socket::print(char const* const _Format, ...)
	{
		#ifdef SOCKETS_DEBUG

		va_list argp;
		va_start(argp, _Format);

		vprintf(_Format, argp);
		va_end(argp);

		#endif
	}

	Waitable::Waitable(std::string pNodeName, std::string pServiceName, WORD wVersionRequested, int ai_family, int ai_socktype, IPPROTO ai_protocol, int ai_flags) noexcept
		: _socket(pNodeName, pServiceName, wVersionRequested, ai_family, ai_socktype, ai_protocol, ai_flags)
		, _master()
		, _copy()
	{}

	void Waitable::shutdown() throw(Socket::ShutdownError)
	{
		while (_master.fd_count > 0) {
			// Get the server number
			SOCKET client = _master.fd_array[0];

			// Remove it from the master file list and close the server
			FD_CLR(client, &_master);
			closesocket(client);
		}
		FD_CLR(_socket._socket, &_master);
		_socket.shutdown();
	}

	void Waitable::async() throw(AsyncError)
	{
		if (_socket == INVALID_SOCKET)
			throw AsyncError("Invalid Socket");

		FD_ZERO(&_master);
		FD_SET(_socket._socket, &_master);
	}

	void Waitable::asyncSet(Socket client) noexcept
	{
		FD_SET(client._socket, &_master);
	}

	Socket Waitable::asyncGet(size_t at) noexcept
	{
		return _copy.fd_array[at];
	}

	size_t Waitable::select() noexcept
	{
		_copy = _master;
		return (size_t)::select(0, &_copy, nullptr, nullptr, nullptr);
	}

	bool operator==(const Waitable& wsoc, const Socket& soc) noexcept 
	{
		return wsoc._socket == soc;
	}

	bool operator==(const Socket& soc, const Waitable& wsoc) noexcept
	{
		return wsoc._socket == soc;
	}
}
