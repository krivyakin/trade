#include <map>

#include "feed_handler.h"
#include "order.h"

namespace trade
{

FeedHandler::FeedHandler()
	: m_last_price(0)
	, m_last_price_quantity(0)
{

}

bool FeedHandler::try_buy(Order& p_order)
{
	auto it = m_sell_orders.begin();

	if (it == m_sell_orders.end() || it->price > p_order.price)
		return false;

	for (; it != m_sell_orders.end() && it->price <= p_order.price;)
	{
		const Order& sell_order = *it;
		trade(sell_order, p_order);

		if (sell_order.quantity == 0) {
			m_id_to_order.erase(sell_order.id);
			it = m_sell_orders.erase(it);
		} else {
			++it;
		}
		
		if (p_order.quantity == 0)
			break;
	}

	return p_order.price == 0;
}

bool FeedHandler::try_sell(Order& p_order)
{
	// Find and store iterators for all buy orders where price <= than sell price
	for (auto it = m_buy_orders.begin(); it != m_buy_orders.end();)
	{
		const Order& buy_order = *it;
		if (buy_order.price < p_order.price)
		{
			++it;
			continue;
		}

		trade(p_order, buy_order);

		if (buy_order.quantity == 0) {
			m_id_to_order.erase(buy_order.id);
			it = m_buy_orders.erase(it);
		} else {
			++it;
		}

		if (p_order.quantity == 0)
			break;
	}

	return p_order.quantity == 0;
}

inline void FeedHandler::trade(const Order& sell, const Order& buy)
{
	size_t count = std::min(buy.quantity, sell.quantity);

	sell.quantity -= count;
	buy.quantity -= count;

	if (sell.price != m_last_price)
	{
		m_last_price = sell.price;
		m_last_price_quantity = 0;
	}

	m_last_price_quantity += count;

#ifndef DISABLE_OUT
	std::cout << "Trade: " << sell.id << "->" << buy.id << ": " <<
		count << "@" << sell.price <<
	   	" (total: " << m_last_price_quantity << "@" << m_last_price << ")" << std::endl;
#endif
}

inline void FeedHandler::add(Order& p_order, std::multiset<Order> *p_orders_map)
{
	auto f = (p_order.side == SideType::SIDE_TYPE_BUY)
	   	? &FeedHandler::try_buy
	   	: &FeedHandler::try_sell;

	if (!(this->*f)(p_order))
	{
		auto it = p_orders_map->insert(p_order);
		m_id_to_order[p_order.id] = &*it;
	}
}

void FeedHandler::processMessage(const std::string& p_msg)
{
	try
	{
		Order order(p_msg);

		std::multiset<Order> *orders = nullptr;

		if (order.side == SideType::SIDE_TYPE_SELL) {
			orders = &m_sell_orders;
		} else if (order.side == SideType::SIDE_TYPE_BUY) {
			orders = &m_buy_orders;
		}
	   
		auto it = m_id_to_order.find(order.id);
		bool exists = (it != m_id_to_order.end());
	
		//At first modify old order if it necessary
		if (order.action == OrderAction::ORDER_ACTION_MODIFY) {
			if (!exists)
			{
				//TODO: should we count it?
			} else {
				const Order& old = *it->second;

				order.id = old.id;
				order.action = old.action;

				orders->erase(old);
				m_id_to_order.erase(it);
			}
		}

		if (order.action == OrderAction::ORDER_ACTION_ADD) {
			if (exists) {
				++m_stats.dublicate;
			}
			add(order, orders);
		} else if (order.action == OrderAction::ORDER_ACTION_DELETE) {
			if (!exists)
			{
				++m_stats.removes_no_corresponding;
			} else {
				orders->erase(*it->second);
			}
		}
	} catch (OrderException& e) {
		switch (e.getType())
		{
			case OrderExceptionType::CORRUPTED:
				++m_stats.corrupted;
				break;
			case OrderExceptionType::WRONG_NUMBER:
				++m_stats.wrong_number;
				break;
		}
	}
#ifndef DISABLE_OUT
	print_midquote();
#endif
}

void FeedHandler::printCurrentOrderBook(std::ostream& stream)
{
	stream << "\tSell orders (sorted by price,date):" << std::endl;
	for (auto& o : m_sell_orders)
	{
		stream << "\t" << o << std::endl;
	}

	stream << "\tBuy orders (sorted by price,date):" << std::endl;
	for (auto& o : m_buy_orders)
	{
		stream << "\t" << o << std::endl;
	}

	stream << std::endl << std::endl;
}

inline void FeedHandler::print_midquote() const
{
	if (!m_sell_orders.size() || !m_buy_orders.size()) {
		std::cout << "Midquote: NaN" << std::endl;
	} else {
		double value = (m_sell_orders.begin()->price + m_buy_orders.rbegin()->price) / 2;
		std::cout << "Midquote: " << value << std::endl;
	}
}

FeedHandler::~FeedHandler()
{
	std::cout << "Statistics:" << std::endl <<
		"---------------------------" << std::endl <<
		"Corrupted: " << m_stats.corrupted << std::endl <<
		"Removes with no correspondind order: " << m_stats.removes_no_corresponding << std::endl <<
		"Wrong numbers: " << m_stats.wrong_number << std::endl;
}

} // namespace trade
