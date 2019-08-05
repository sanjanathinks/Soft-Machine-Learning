all: house-prices.c

house-prices: house-prices.c
  gcc -std=c99 -Wall -Werror -fsanitize=address house-prices.c -o house-prices
  
clean:
  rm -rf house-prices
