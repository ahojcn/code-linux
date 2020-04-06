FLAG=-std=c++11 -lpthread -ljsoncpp
DEPEND=tcp_socket.hpp tcp_select_server.hpp tcp_client.hpp tcp_epoll_server.hpp

.PHONY:all
all:dict_server dict_client

dict_server:dict_server.cc $(DEPEND)
	g++ dict_server.cc -o dict_server $(FLAG)
dict_client:dict_client.cc $(DEPEND)
	g++ dict_client.cc -o dict_client $(FLAG)

.PHONY:clean
clean:
	rm dict_client dict_server
