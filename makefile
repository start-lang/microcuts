.PHONY: test

clean:
	rm -rf build/*

build/libassert.a:
	gcc -Wall -c src/microcuts.c -o build/microcuts.o && \
	ar cr build/libmicrocuts.a build/microcuts.o
	rm build/*.o

test: build/libassert.a
	gcc -Wall -Isrc src/microcuts.c test/test_microcuts.c -o build/test_microcuts && \
	chmod +x build/test_microcuts && \
	./build/test_microcuts

cov:
	@make build/libassert.a > /dev/null
	@gcc -Wall --coverage -Isrc src/microcuts.c test/test_microcuts.c -o build/test_microcuts && \
	chmod +x build/test_microcuts && \
	./build/test_microcuts > /dev/null && \
	gcov microcuts.c > /dev/null && \
	./tools/coverage.py && \
	rm -f *.gc*
	@make clean > /dev/null
