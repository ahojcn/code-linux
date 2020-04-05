FLAG=-std=c++11 -lpthread -ljsoncpp
DEPEND=tcp_server.hpp tcp_socket.hpp tcp_process_server.hpp

.PHONY:all
all:dict_server dict_client calc_server calc_client calc_server_json calc_client_json http_server

http_server: http_server.cc $(DEPEND)
	g++ http_server.cc -o http_server $(FLAG)

calc_server_json: calc_server_json.cc $(DEPEND)
	g++ calc_server_json.cc -o calc_server_json $(FLAG)
calc_client_json: calc_client_json.cc $(DEPEND)
	g++ calc_client_json.cc -o calc_client_json $(FLAG)

calc_server: calc_server.cc $(DEPEND)
	g++ calc_server.cc -o calc_server $(FLAG)
calc_client: calc_client.cc $(DEPEND)
	g++ calc_client.cc -o calc_client $(FLAG)

dict_server:dict_server.cc $(DEPEND)
	g++ dict_server.cc -o dict_server $(FLAG)
dict_client:dict_client.cc $(DEPEND)
	g++ dict_client.cc -o dict_client $(FLAG)

.PHONY:clean
clean:
	rm dict_client dict_server
