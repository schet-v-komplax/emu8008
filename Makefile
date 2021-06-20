emu8008: emu/main.c emu/control.c emu/io.c emu/jump.c emu/load.c emu/math.c etc/common.c
	gcc -o $@ $^

asm8008: asm/main.c asm/vector.c etc/common.c
	gcc -o $@ $^

dis8008: dis/main.c etc/common.c
	gcc -o $@ $^

run: emu8008 asm8008
	./asm8008 .8008.asm > as
	./emu8008 as
