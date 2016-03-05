%: %.cpp
	g++ -std=c++11 -O2 -ggdb -DLOCAL -Wall -Wextra -Wshadow \
			 	-Wno-unused-result -Wsequence-point -Wl,-stack_size -Wl,120000000 $< -o $@
