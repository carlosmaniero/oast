TESTER_SRC := ./test.sh
TESTS := $(wildcard ./examples/*.oast)

.PHONY: all
all:
	@set -e; \
	for file in $(TESTS); do \
                $(TESTER_SRC) "$$file"; \
		echo; \
        done

.PHONY: clean
clean:
	@rm -rf ./examples/*.oast.test*
