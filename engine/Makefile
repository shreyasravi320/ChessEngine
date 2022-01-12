build:
	clang++ -Ofast -std=c++17 -o ion src/*.cpp -I src/include -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment

run:
	./ion

clean:
	rm ion
