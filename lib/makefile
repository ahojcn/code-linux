test:test.c libadd.so
	gcc $^ -o a.out


libadd.so:add.c
	gcc add.c -shared -fPIC -o libadd.so

# 静态库
# libadd.a:add.c
# 	gcc -c add.c -o add.o
# 	ar -rc libadd.a add.o

.PHONY:clean
clean:
	rm test
