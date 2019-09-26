#include "Utils/Network.h"

namespace SteelEngine { namespace Utils {

    void receiveWholeDataS(
        Network::INetwork* network,
        std::string& receivedData,
        char* buffer,
        SOCKET sock,
        size_t bufferSize)
    {
        size_t size = Serialization::DeserializeStreamS(buffer, receivedData);

        size_t size2 = size;
        size2 -= bufferSize;

        for(size_t i = bufferSize; i < size; i += bufferSize)
        {
            network->Send(sock, "DONE", bufferSize);

            std::string tmp;
            size_t size3 = size2;

            tmp.resize(bufferSize);

            int bytesIn = network->Receive(sock, &tmp[0], bufferSize);

            for(Type::uint32 j = 0; j < bufferSize && j < size3; j++)
            {
                receivedData[i + j] = tmp[j];
                size2--;
            }

            if(size2 == 0)
            {
                network->Send(sock, "DONE", bufferSize);
                network->Receive(sock, buffer, bufferSize);
            }
        }
    }

}}