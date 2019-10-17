#include "pch.h"
#include "..\\library\Socket.h"
#include "..\\library\Socket.cpp"

#pragma warning(disable:4996)

#include <gtest/gtest.h>


TEST(InterfaceSocketTest, socket)
{
	Socket::Socket server("", "27015", MAKEWORD(2, 2),
		AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);
	try {
		server.socket();
	}
	catch (Socket::Socket::BaseError& err) {
		EXPECT_EQ((std::string)err, "socket is not initialized!");
	}
}

TEST(InterfaceSocketTest, bind)
{
	Socket::Socket server("", "27015", MAKEWORD(2, 2),
		AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);
	try {
		server.bind();
	}
	catch (Socket::Socket::BaseError& err) {
		EXPECT_EQ((std::string)err, "socket is not initialized!");
	}
}

TEST(InterfaceSocketTest, listen)
{
	Socket::Socket server("", "27015", MAKEWORD(2, 2),
		AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);

	try {
		server.listen();
	}
	catch (Socket::Socket::BaseError& err) {
		EXPECT_EQ((std::string)err, "Invalid Socket");
	}
}

TEST(InterfaceSocketTest, accept)
{
	Socket::Socket server("", "27015", MAKEWORD(2, 2),
		AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);

	try {
		server.accept();
	}
	catch (Socket::Socket::BaseError& err) {
		EXPECT_EQ((std::string)err, "Invalid Socket");
	}
}

TEST(InterfaceSocketTest, send)
{
	Socket::Socket server("", "27015", MAKEWORD(2, 2),
		AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);

	try {
		server.send("test");
	}
	catch (Socket::Socket::BaseError& err) {
		EXPECT_EQ((std::string)err, "Invalid Socket");
	}
}

TEST(InterfaceSocketTest, receive)
{
	Socket::Socket server("", "27015", MAKEWORD(2, 2),
		AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);

	try {
		std::string rec;
		server.receive(rec);
	}
	catch (Socket::Socket::BaseError& err) {
		EXPECT_EQ((std::string)err, "Invalid Socket");
	}
}

TEST(InterfaceSocketTest, shutdown)
{
	Socket::Socket server("", "27015", MAKEWORD(2, 2),
		AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);

	try {
		server.shutdown();
	}
	catch (Socket::Socket::BaseError& err) {
		EXPECT_EQ((std::string)err, "Invalid Socket");
	}
}

TEST(InterfaceWaitableTest, socket)
{
	Socket::Waitable server("", "27015", MAKEWORD(2, 2),
		AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);
	try {
		server.socket();
	}
	catch (Socket::Socket::BaseError& err) {
		EXPECT_EQ((std::string)err, "socket is not initialized!");
	}
}

TEST(InterfaceWaitableTest, bind)
{
	Socket::Waitable server("", "27015", MAKEWORD(2, 2),
		AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);
	try {
		server.bind();
	}
	catch (Socket::Socket::BaseError& err) {
		EXPECT_EQ((std::string)err, "socket is not initialized!");
	}
}

TEST(InterfaceWaitableTest, listen)
{
	Socket::Waitable server("", "27015", MAKEWORD(2, 2),
		AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);

	try {
		server.listen();
	}
	catch (Socket::Socket::BaseError& err) {
		EXPECT_EQ((std::string)err, "Invalid Socket");
	}
}

TEST(InterfaceWaitableTest, accept)
{
	Socket::Waitable server("", "27015", MAKEWORD(2, 2),
		AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);

	try {
		server.accept();
	}
	catch (Socket::Socket::BaseError& err) {
		EXPECT_EQ((std::string)err, "Invalid Socket");
	}
}

TEST(InterfaceWaitableTest, send)
{
	Socket::Waitable server("", "27015", MAKEWORD(2, 2),
		AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);

	try {
		server.send("test");
	}
	catch (Socket::Socket::BaseError& err) {
		EXPECT_EQ((std::string)err, "Invalid Socket");
	}
}

TEST(InterfaceWaitableTest, receive)
{
	Socket::Waitable server("", "27015", MAKEWORD(2, 2),
		AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);

	try {
		std::string rec;
		server.receive(rec);
	}
	catch (Socket::Socket::BaseError& err) {
		EXPECT_EQ((std::string)err, "Invalid Socket");
	}
}

TEST(InterfaceWaitableTest, shutdown)
{
	Socket::Waitable server("", "27015", MAKEWORD(2, 2),
		AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);

	try {
		server.shutdown();
	}
	catch (Socket::Socket::BaseError& err) {
		EXPECT_EQ((std::string)err, "Invalid Socket");
	}
}

TEST(InterfaceWaitableTest, async)
{
	Socket::Waitable server("", "27015", MAKEWORD(2, 2),
		AF_INET, SOCK_STREAM, IPPROTO::IPPROTO_TCP, AI_PASSIVE);

	try {
		server.async();
	}
	catch (Socket::Socket::BaseError& err) {
		EXPECT_EQ((std::string)err, "Invalid Socket");
	}
}
