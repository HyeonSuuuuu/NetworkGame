
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
	static SocketAddressPtr CreateIPv4FromString(const string& inString);
};