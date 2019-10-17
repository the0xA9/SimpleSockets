#define _CRT_SECURE_NO_WARNINGS // :(
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // to resolve conflicts
#endif

//#define SOCKETS_DEBUG

#include <winsock2.h>

#include <string>
#include <memory>

namespace Socket 
{
	using namespace std;
	class Socket
	{
	private:
		class Arguments {
			std::string ret;
		public:
			Arguments(char const* const _Format, ...) {
				va_list argp;
				va_start(argp, _Format);
				std::unique_ptr<char> recvbuf(new char[1024]);
				vsprintf(recvbuf.get(), _Format, argp);
				va_end(argp);
				ret.append(recvbuf.get());
			}
			operator std::string() { return ret; }
		};

	public:
		class BaseError
		{
			std::string _err;
		public:
			explicit BaseError(Arguments args) : _err(args) {}
			virtual ~BaseError() {}
			operator const std::string() const { return _err; }
		};

		class InitialisationError : public BaseError { public: InitialisationError(Arguments args) : BaseError(args) {} };
		class ConnectionError : public BaseError { public: ConnectionError(Arguments args) : BaseError(args) {} };
		class BindError : public BaseError { public: BindError(Arguments args) : BaseError(args) {} };
		class ListenError : public BaseError { public: ListenError(Arguments args) : BaseError(args) {} };
		class AcceptError : public BaseError { public: AcceptError(Arguments args) : BaseError(args) {} };
		class SendError : public BaseError { public: SendError(Arguments args) : BaseError(args) {} };
		class ReceiveError : public BaseError { public: ReceiveError(Arguments args) : BaseError(args) {} };
		class ShutdownError : public BaseError { public: ShutdownError(Arguments args) : BaseError(args) {} };

	protected:
		Socket(SOCKET socket);

	public:
		Socket(std::string pNodeName, std::string pServiceName, WORD wVersionRequested = MAKEWORD(2, 2),
			int ai_family = AF_UNSPEC, int ai_socktype = SOCK_STREAM, IPPROTO ai_protocol = IPPROTO::IPPROTO_TCP, int ai_flags = 0) noexcept;

		~Socket();

		// initialize socket
		void init() throw (InitialisationError);

		// create socket to connect
		void socket() throw (InitialisationError);

		// establish connection
		void connect() throw (ConnectionError);

		// setup the TCP listening socket
		void bind() throw (BindError);

		// listen socket
		void listen() throw (ListenError);

		// accept a client socket 
		Socket accept() throw (AcceptError);

		// send message to the server
		void send(const std::string& _msg) throw (SendError);

		// receive message from the server
		bool receive(std::string& _msg, const size_t _recvbuflen = 1024, int _flags = 0, size_t * _size = nullptr) throw (ReceiveError);

		// shutdown the connection since no more data will be sent
		void shutdown() throw (ShutdownError);

		const bool operator==(const Socket& rhs) const noexcept;

	private:
		// clear all
		void cleanup();

		// print debug info
		inline void print(char const* const _Format, ...);

	private:
		std::string _pNodeName;
		std::string _pServiceName;
		WORD _wVersionRequested;
		int _ai_family;
		int _ai_socktype;
		IPPROTO _ai_protocol;
		int _ai_flags;

		SOCKET _socket;
		addrinfo* _result;

		friend class Waitable;
	};

	class Waitable
	{
	public:
		class AsyncError : public Socket::BaseError { public: AsyncError(Socket::Arguments args) : BaseError(args) {} };

	public:
		Waitable(std::string pNodeName, std::string pServiceName, WORD wVersionRequested = MAKEWORD(2, 2),
			int ai_family = AF_UNSPEC, int ai_socktype = SOCK_STREAM, IPPROTO ai_protocol = IPPROTO::IPPROTO_TCP, int ai_flags = 0) noexcept;

		// initialize socket
		inline void init() throw (Socket::InitialisationError) 
			{ _socket.init(); }

		// create socket to connect
		inline void socket() throw (Socket::InitialisationError)
			{ _socket.socket(); }

		// establish connection
		inline void connect() throw (Socket::ConnectionError)
			{ _socket.connect(); }

		// setup the TCP listening socket
		inline void bind() throw (Socket::BindError)
			{ _socket.bind(); }

		// listen socket
		inline void listen() throw (Socket::ListenError)
			{ _socket.listen(); }

		// accept a client socket 
		inline Socket accept() throw (Socket::AcceptError)
			{ return _socket.accept(); }

		// send message to the server
		inline void send(const std::string& _msg) throw (Socket::SendError)
			{ _socket.send(_msg); 	}

		// receive message from the server
		inline bool receive(std::string& _msg, const size_t _recvbuflen = 1024, int _flags = 0, size_t * _size = nullptr) throw (Socket::ReceiveError)
			{ return _socket.receive(_msg, _recvbuflen, _flags, _size); }

		// shutdown the connection since no more data will be sent
		void shutdown() throw (Socket::ShutdownError);

		// set socket to async listening
		void async() throw(AsyncError);

		void asyncSet(Socket client) noexcept;

		Socket asyncGet(size_t at) noexcept;

		size_t select() noexcept;

		
	protected:
		const Socket getSocket() const { return _socket; }

	private:
		Socket _socket;
		fd_set _master, _copy;

		friend bool operator==(const Waitable& wsoc, const Socket& soc) noexcept;
		friend bool operator==(const Socket& soc, const Waitable& wsoc) noexcept;
	};
}