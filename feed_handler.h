#ifndef __TRADE_FEED_HANDLER_H__
#define __TRADE_FEED_HANDLER_H__

#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

#include "order.h"

namespace trade
{

typedef uint64_t Counter;
struct Stats
{
	Stats()
		: corrupted(0)
		, dublicate(0)
		, no_correspondind(0)
		, removes_no_corresponding(0)
		, best_sell(0)
		, wrong_number(0)
	{

	}

	Counter corrupted;
	Counter dublicate;
	Counter no_correspondind;
	Counter removes_no_corresponding;
	Counter best_sell;
	Counter wrong_number;
};

class FeedHandler
{
public:
	void processMessage(const std::string& p_msg);
	void printCurrentOrderBook(std::ostream& stream);
	FeedHandler();
	virtual ~FeedHandler();

private:
	inline void add(Order& p_order, std::multiset<Order> *p_orders_map);
	bool try_sell(Order& p_order);
	bool try_buy(Order& p_order);
	inline void trade(const Order& sell, const Order& buy);
	inline void print_midquote() const;

	// may be shared_ptr better but performance issue in the first place
	std::multiset<Order> m_sell_orders;
	std::multiset<Order> m_buy_orders;

	std::unordered_map<OrderID, const Order*> m_id_to_order;
	Stats m_stats;

	OrderPrice m_last_price;
	size_t m_last_price_quantity;
};

} // namespace trade

#endif // __TRADE_FEED_HANDLER_H__
