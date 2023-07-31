
TESTABLE = hex hexdump string_utils
CXX_FLAGS = -Wall -Werror -std=c++17 -DTEST_MAIN
LD_FLAGS =

hexdump: hexdump.cpp
	$(CXX) -o hexdump $(CXX_FLAGS) $<

hex: hex.cpp
	$(CXX) -o hex $(CXX_FLAGS) $<

string_utils: string_utils.cpp
	$(CXX) -o string_utils $(CXX_FLAGS) $<

.PHONY: tests
tests: $(TESTABLE)
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
