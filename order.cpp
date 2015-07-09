#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "converter.hpp"
#include "order.h"

namespace trade
{

static OrderID second_id_counter = 0;

Order::Order(const std::string& p_message)
	: m_second_id(++second_id_counter)
{
	const std::string delim = ",";

	std::vector<std::string> tokens;
	boost::split(tokens, p_message, boost::is_any_of(delim));

	if (tokens.size() != 5)
	{
		throw OrderException(OrderExceptionType::CORRUPTED);
	}

	action = OrderAction::ORDER_ACTION_UNKNOWN;
	if (tokens[0] == "A") {
		action = OrderAction::ORDER_ACTION_ADD;
	} else if (tokens[0] == "M") {
		action = OrderAction::ORDER_ACTION_MODIFY;
	} else if (tokens[0] == "X") {
		action = OrderAction::ORDER_ACTION_DELETE;
	} else {
		throw OrderException(OrderExceptionType::CORRUPTED);
	}

	id = 0;
	try {
		id = Converter<decltype(id)>::convert(tokens[1]);
	} catch (...) {
		throw OrderException(OrderExceptionType::WRONG_NUMBER);
	}

	side = SideType::SIDE_TYPE_UNKNOWN;
	if (tokens[2] == "S") {
		side = SideType::SIDE_TYPE_SELL;
	} else if (tokens[2] == "B") {
		side = SideType::SIDE_TYPE_BUY;
	} else {
		throw OrderException(OrderExceptionType::CORRUPTED);
	}

	quantity = 0;
	try {
		quantity = std::stoi(tokens[3]);
	} catch (...) {
		throw OrderException(OrderExceptionType::WRONG_NUMBER);
	}

	price = 0;
	try {
		price = std::stod(tokens[4]);
	} catch (...) {
		throw OrderException(OrderExceptionType::WRONG_NUMBER);
	}
}

OrderException::OrderException(OrderExceptionType p_type)
	: m_type(p_type)
{

}

const char* OrderException::what() const throw()
{
	switch (m_type)
	{
	case OrderExceptionType::CORRUPTED:
		return "Corrupted";
	case OrderExceptionType::WRONG_NUMBER:
		return "Wrong number";
	}

	return "";
}

bool Order::operator<(const Order& rhs) const
{
	if (side == SideType::SIDE_TYPE_SELL)
		return price < rhs.price;
	else
		return id < rhs.id;
}

} // namespace trade
