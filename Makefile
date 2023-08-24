
TESTABLE = hex_test hexdump_test string_utils_test time_test raii_test
CXX_FLAGS = -Wall -Werror -std=c++20 -DTEST_MAIN
LD_FLAGS =

.PHONY: all
all: $(TESTABLE)

.PHONY: test
test: $(TESTABLE)
	@for t in $(TESTABLE); do \
		printf "%s" "testing $$t: "; \
		./$$t; \
	done

.PHONY: clean
.SILENT: clean
clean:
	rm -f $(TESTABLE)
	rm -rf a.out*
	rm -f *~
	rm -rf *.dSYM

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
