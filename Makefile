
CFLAGS+= -Iinclude -std=c99 -g 
LFLAGS+= -L./ -lphotonhash -lm

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

libphotonhash.a: src/photonhash.o
	$(AR) rcs $@ $^

test_list:= basic large_map

define gen_test_rule
# $(1) test

test_$(1): test/$(1).o libphotonhash.a
	$(CC) test/$(1).o -o test_$(1) $(LFLAGS) 

$(1).run: test_$(1)
	./test_$(1)

$(1).clean: 
	rm -f test_$(1) test/$(1).o

clean: $(1).clean

test: $(1).run

endef

$(foreach test,$(test_list),\
	$(eval $(call gen_test_rule,$(test))))

clean_lib:
	rm -f src/*.o libphotonhash.a

clean: clean_lib
