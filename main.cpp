#include <fstream>
#include <iostream>
#include <string>

#include "feed_handler.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <input file>" << std::endl;
	}

	const std::string filename(argv[1]);
	std::ifstream infile(filename.c_str(), std::ios::in);

	trade::FeedHandler feed;
	size_t counter = 0;
	std::string line;

	while (std::getline(infile, line)) {
		feed.processMessage(line);
		if (++counter % 10 == 0) {
			feed.printCurrentOrderBook(std::cerr);
		}
	}
	feed.printCurrentOrderBook(std::cout);

	return 0;
}
