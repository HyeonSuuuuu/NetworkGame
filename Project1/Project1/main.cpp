#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>

using std::string;

class SocketAddress {
public:
	SocketAddress(uint32_t inAddress, uint16_t inPort) {
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
		GetAsSockAddrIn()->sin_port = htons(inPort);
	}
	SocketAddress(const sockaddr& inSockAddr) {
		memcpy(&m_sockAddr, &inSockAddr, sizeof(sockaddr));
	}
	size_t GetSize() const { return sizeof(sockaddr); }

private:
	sockaddr m_sockAddr;

	sockaddr_in* GetAsSockAddrIn()
	{
		return reinterpret_cast<sockaddr_in*>(&m_sockAddr);
	}
};

using SocketAddressPtr = std::shared_ptr<SocketAddress>;

class SocketAddressFactory {
public:
	static SocketAddressPtr CreateIPv4FromString(const string& inString)
	{
		auto pos = inString.find_last_of(':');
		string host, service;
		if (pos != string::npos) {
			host = inString.substr(0, pos);
			service = inString.substr(pos + 1);
		}
		else {
			host = inString;
			service = "0";
		}
		addrinfo hint = {};
		hint.ai_family = AF_INET;

		addrinfo* result = nullptr;
		
		int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
		addrinfo* initResult = result;
		
		if (error != 0 && result != nullptr) {
			freeaddrinfo(initResult);
			return nullptr;
		}

		while (!result->ai_addr && result->ai_next) {
			result = result->ai_next;
		}
		
		if (!result->ai_addr) {
			freeaddrinfo(initResult);
			return nullptr;
		}

		auto toRet = std::make_shared<SocketAddress>(*result->ai_addr);

		freeaddrinfo(initResult);
		return toRet;
	}
};

int main()
{
	
	std::cout << "Hello";
}