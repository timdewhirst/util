
TESTABLE = hex_test hexdump_test string_utils_test time_test raii_test uuid_test
EXAMPLES = hexdump_example simple_unit_test_example uuid_example client_example server_example
CXX_FLAGS = -Wall -Werror -std=c++23 -DTEST_MAIN
LD_FLAGS =

.PHONY: all
all: $(TESTABLE) $(EXAMPLES)

.PHONY: test
test: $(TESTABLE)
	@for t in $(TESTABLE); do \
		printf "%s" "testing $$t: "; \
		./$$t; \
	done

.PHONY: clean
.SILENT: clean
clean:
	rm -f $(TESTABLE) $(EXAMPLES)
	rm -rf a.out*
	rm -f *~
	rm -rf *.dSYM

client_example: client_main.cpp socket.hpp tcp_socket.hpp tcp_socket.cpp
	$(CXX) -o client_example $(CXX_FLAGS) $^ -lpthread

server_example: server_main.cpp socket.hpp tcp_socket.hpp tcp_server.hpp tcp_server.cpp tcp_socket.cpp
	$(CXX) -o server_example $(CXX_FLAGS) $^ -lpthread

hexdump_example: hexdump.cpp hexdump.hpp hexdump_example.cpp
	$(CXX) -o hexdump_example $(CXX_FLAGS) $^

simple_unit_test_example: simple_unit_test.hpp simple_unit_test_example.cpp
	$(CXX) -o simple_unit_test_example $(CXX_FLAGS) $^

uuid_example: uuid.cpp uuid.hpp uuid_example.cpp
	$(CXX) -o uuid_example $(CXX_FLAGS) $^

hexdump_test: hexdump.cpp hexdump.hpp hexdump_test.cpp
	$(CXX) -o hexdump_test $(CXX_FLAGS) $^

hex_test: hex_test.cpp hex.hpp
	$(CXX) -o hex_test $(CXX_FLAGS) $^

string_utils_test: string_utils.hpp string_utils_test.cpp
	$(CXX) -o string_utils_test $(CXX_FLAGS) $^

time_test: time_test.cpp time.hpp
	$(CXX) -o time_test $(CXX_FLAGS) $^

raii_test: raii_test.cpp raii.hpp
	$(CXX) -o raii_test $(CXX_FLAGS) $^

uuid_test: uuid.cpp uuid.hpp uuid_test.cpp
	$(CXX) -o uuid_test $(CXX_FLAGS) $^
