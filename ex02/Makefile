pro = PmergeMe
com = c++
flagse = -Wall -Wextra -Werror -std=c++98 #-fsanitize=address

scr = main.cpp PmergeMe.cpp
obj = $(scr:.cpp=.o)

all: $(pro) clean

$(pro): $(obj)
	$(com) $(flagse) $(obj) -o $(pro)

%.o:%.cpp
	$(com) $(flagse) -c $< -o $@

clean:
	rm -rf $(obj)

fclean: clean
	rm -rf $(pro)

re: fclean all