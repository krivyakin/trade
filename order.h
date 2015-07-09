#ifndef __TRADE_ORDER_H__
#define __TRADE_ORDER_H__

#include <iostream>

namespace trade
{

enum class SideType
{
	SIDE_TYPE_UNKNOWN = 0,
	SIDE_TYPE_SELL = 1,
	SIDE_TYPE_BUY = 2
};

enum class OrderAction
{
	ORDER_ACTION_UNKNOWN = 0,
	ORDER_ACTION_ADD = 1,
	ORDER_ACTION_DELETE = 2,
	ORDER_ACTION_MODIFY = 3
};

// TODO: Do we really need double for price? ("double indicating max price at which to buy/min price to sell")
typedef double OrderPrice;
typedef unsigned int OrderID;

struct Order
{
	Order(const std::string& p_message);

	OrderAction action;
	OrderID id;
	SideType side;
	mutable unsigned int quantity;
	OrderPrice price;

	bool operator<(const Order& rhs) const;

private:
	OrderID m_second_id;
};

enum class OrderExceptionType
{
	CORRUPTED = 1,
	WRONG_NUMBER = 2	
};

class OrderException : public std::exception
{
public:
	OrderException(OrderExceptionType p_type);
	virtual const char* what() const throw();
	inline OrderExceptionType getType() const { return m_type; }

private:
	OrderExceptionType m_type;
};

inline std::ostream& operator<<(std::ostream& os, const Order& o)
{
	os << o.id << "," << (o.side == SideType::SIDE_TYPE_SELL ? "S" : "B") << "," <<
		o.quantity << "," << o.price;
	return os;
}

} // namespace trade

#endif // __TRADE_ORDER_H__
