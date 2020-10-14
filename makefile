.PHONY: test

clean:
	rm -rf build/*

test:
	@ mkdir -p build
	@ gcc -Wall -Isrc src/microcuts.c test/test_microcuts.c -o build/test_microcuts && \
	chmod +x build/test_microcuts && \
	./build/test_microcuts

cov:
	@ mkdir -p build
	@ gcc -Wall --coverage -Isrc src/microcuts.c test/test_microcuts.c -o build/test_microcuts && \
	chmod +x build/test_microcuts && \
	./build/test_microcuts > /dev/null && \
	gcov microcuts.c > /dev/null && \
	./tools/coverage.py && \
	rm -f *.gc*
	@ make clean > /dev/null
