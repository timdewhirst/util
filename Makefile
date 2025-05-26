
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

client_example: client_main.cpp tcp_socket.cpp
	$(CXX) -o client_example $(CXX_FLAGS) $^ -lpthread

server_example: server_main.cpp tcp_server.cpp tcp_socket.cpp
	$(CXX) -o server_example $(CXX_FLAGS) $^ -lpthread

hexdump_example: hexdump.cpp hexdump_example.cpp
	$(CXX) -o hexdump_example $(CXX_FLAGS) $^

simple_unit_test_example: simple_unit_test_example.cpp
	$(CXX) -o simple_unit_test_example $(CXX_FLAGS) $^

uuid_example: uuid.cpp uuid_example.cpp
	$(CXX) -o uuid_example $(CXX_FLAGS) $^

hexdump_test: hexdump.cpp hexdump_test.cpp
	$(CXX) -o hexdump_test $(CXX_FLAGS) $^

hex_test: hex_test.cpp
	$(CXX) -o hex_test $(CXX_FLAGS) $<

string_utils_test: string_utils_test.cpp
	$(CXX) -o string_utils_test $(CXX_FLAGS) $<

time_test: time_test.cpp
	$(CXX) -o time_test $(CXX_FLAGS) $<

raii_test: raii_test.cpp
	$(CXX) -o raii_test $(CXX_FLAGS) $<

uuid_test: uuid.cpp uuid_test.cpp
	$(CXX) -o uuid_test $(CXX_FLAGS) $^
