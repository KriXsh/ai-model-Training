// bug code
// #include <iostream>
// #include <vector>
// #include <atomic>
// #include <memory>
// #include <thread>
// #include <queue>
// #include <mutex>
// #include <condition_variable>

// // Order structure to represent a trade order
// struct Order {
//     int orderID;
//     bool isBuy; // true for buy, false for sell
//     double price;
//     int quantity;

//     Order(int id, bool buy, double p, int q) : orderID(id), isBuy(buy), price(p), quantity(q) {}
// };

// // Simple Order Book with Lock-free Data Structures
// class OrderBook {
// private:
//     std::atomic<int> buyOrdersCount{0};  // Atomic counter for buy orders
//     std::atomic<int> sellOrdersCount{0}; // Atomic counter for sell orders

//     std::mutex mtx; // Mutex to protect order matching

//     std::vector<Order> buyOrders;
//     std::vector<Order> sellOrders;

// public:
//     // Add an order to the order book
//     void addOrder(const Order& order) {
//         std::lock_guard<std::mutex> lock(mtx);  // Lock to ensure thread safety

//         if (order.isBuy) {
//             buyOrders.push_back(order);
//             buyOrdersCount++;
//         } else {
//             sellOrders.push_back(order);
//             sellOrdersCount++;
//         }
//     }

//     // Match orders between buy and sell orders
//     void matchOrders() {
//         std::lock_guard<std::mutex> lock(mtx);

//         // Sort buy orders by price descending and sell orders by price ascending
//         std::sort(buyOrders.begin(), buyOrders.end(), [](const Order& a, const Order& b) {
//             return a.price > b.price;
//         });
//         std::sort(sellOrders.begin(), sellOrders.end(), [](const Order& a, const Order& b) {
//             return a.price < b.price;
//         });

//         // Match orders
//         for (size_t i = 0; i < buyOrders.size() && i < sellOrders.size(); ++i) {
//             if (buyOrders[i].price >= sellOrders[i].price) {
//                 // A match is found, print the trade details
//                 std::cout << "Matched Order ID: " << buyOrders[i].orderID
//                           << " with Order ID: " << sellOrders[i].orderID
//                           << " at Price: " << sellOrders[i].price
//                           << " for Quantity: " << std::min(buyOrders[i].quantity, sellOrders[i].quantity) << std::endl;
//                 // Update quantities
//                 buyOrders[i].quantity -= sellOrders[i].quantity;
//                 sellOrders[i].quantity -= buyOrders[i].quantity;
//             }
//         }

//         // Remove executed orders
//         buyOrders.erase(std::remove_if(buyOrders.begin(), buyOrders.end(), [](const Order& order) {
//             return order.quantity <= 0;
//         }), buyOrders.end());

//         sellOrders.erase(std::remove_if(sellOrders.begin(), sellOrders.end(), [](const Order& order) {
//             return order.quantity <= 0;
//         }), sellOrders.end());
//     }

//     void displayOrderBook() {
//         std::lock_guard<std::mutex> lock(mtx);
//         std::cout << "Buy Orders:\n";
//         for (const auto& order : buyOrders) {
//             std::cout << "Order ID: " << order.orderID << " Price: " << order.price << " Quantity: " << order.quantity << std::endl;
//         }
//         std::cout << "Sell Orders:\n";
//         for (const auto& order : sellOrders) {
//             std::cout << "Order ID: " << order.orderID << " Price: " << order.price << " Quantity: " << order.quantity << std::endl;
//         }
//     }
// };

// // Low-Latency Message Queue to simulate market data feed
// class MarketDataFeed {
// private:
//     std::atomic<bool> isNewDataAvailable{false}; // Simulate availability of new market data
//     std::queue<Order> orderQueue;

// public:
//     void feedNewOrder(const Order& order) {
//         orderQueue.push(order);
//         isNewDataAvailable.store(true, std::memory_order_release);
//     }

//     bool getNextOrder(Order& order) {
//         if (isNewDataAvailable.load(std::memory_order_acquire)) {
//             order = orderQueue.front();
//             orderQueue.pop();
//             if (orderQueue.empty()) {
//                 isNewDataAvailable.store(false, std::memory_order_release);
//             }
//             return true;
//         }
//         return false;
//     }
// };

// // Simulate Trading System - Perform real-time order matching
// void tradingEngine(OrderBook& orderBook, MarketDataFeed& dataFeed) {
//     while (true) {
//         Order newOrder(0, true, 100.0, 100);  // Sample Order (Buy)
//         if (dataFeed.getNextOrder(newOrder)) {
//             orderBook.addOrder(newOrder);
//             orderBook.matchOrders();
//         }
//     }
// }

// // Simulate a High-Frequency Trading Market Data Feed
// void simulateMarketDataFeed(MarketDataFeed& dataFeed) {
//     for (int i = 0; i < 10; ++i) {
//         Order newOrder(i, i % 2 == 0, 100.0 + i, 100 + i);  // Alternating Buy and Sell orders
//         dataFeed.feedNewOrder(newOrder);
//         std::this_thread::sleep_for(std::chrono::milliseconds(1));  // Simulate delay
//     }
// }

// int main() {
//     // Initialize system components
//     OrderBook orderBook;
//     MarketDataFeed dataFeed;

//     // Start the market data simulation
//     std::thread marketDataThread(simulateMarketDataFeed, std::ref(dataFeed));

//     // Start the trading engine (order matching)
//     std::thread tradingEngineThread(tradingEngine, std::ref(orderBook), std::ref(dataFeed));

//     marketDataThread.join();
//     tradingEngineThread.join();

//     return 0;
// }


// working
#include <iostream>
#include <vector>
#include <atomic>
#include <memory>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono> // for sleep

// Order structure to represent a trade order
struct Order {
    int orderID;
    bool isBuy; // true for buy, false for sell
    double price;
    int quantity;

    Order(int id, bool buy, double p, int q) : orderID(id), isBuy(buy), price(p), quantity(q) {}
};

// Simple Order Book with Lock-free Data Structures
class OrderBook {
private:
    std::atomic<int> buyOrdersCount{0};  // Atomic counter for buy orders
    std::atomic<int> sellOrdersCount{0}; // Atomic counter for sell orders

    std::mutex mtx; // Mutex to protect order matching

    std::vector<Order> buyOrders;
    std::vector<Order> sellOrders;

public:
    // Add an order to the order book
    void addOrder(const Order& order) {
        std::lock_guard<std::mutex> lock(mtx);  // Lock to ensure thread safety

        if (order.isBuy) {
            buyOrders.push_back(order);
            buyOrdersCount++;
        } else {
            sellOrders.push_back(order);
            sellOrdersCount++;
        }
    }

    // Match orders between buy and sell orders
    void matchOrders() {
        std::lock_guard<std::mutex> lock(mtx);

        // Sort buy orders by price descending and sell orders by price ascending
        std::sort(buyOrders.begin(), buyOrders.end(), [](const Order& a, const Order& b) {
            return a.price > b.price;
        });
        std::sort(sellOrders.begin(), sellOrders.end(), [](const Order& a, const Order& b) {
            return a.price < b.price;
        });

        // Match orders
        for (size_t i = 0; i < buyOrders.size() && i < sellOrders.size(); ++i) {
            if (buyOrders[i].price >= sellOrders[i].price) {
                // A match is found, print the trade details
                std::cout << "Matched Order ID: " << buyOrders[i].orderID
                          << " with Order ID: " << sellOrders[i].orderID
                          << " at Price: " << sellOrders[i].price
                          << " for Quantity: " << std::min(buyOrders[i].quantity, sellOrders[i].quantity) << std::endl;
                // Update quantities
                buyOrders[i].quantity -= sellOrders[i].quantity;
                sellOrders[i].quantity -= buyOrders[i].quantity;
            }
        }

        // Remove executed orders
        buyOrders.erase(std::remove_if(buyOrders.begin(), buyOrders.end(), [](const Order& order) {
            return order.quantity <= 0;
        }), buyOrders.end());

        sellOrders.erase(std::remove_if(sellOrders.begin(), sellOrders.end(), [](const Order& order) {
            return order.quantity <= 0;
        }), sellOrders.end());
    }

    void displayOrderBook() {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Buy Orders:\n";
        for (const auto& order : buyOrders) {
            std::cout << "Order ID: " << order.orderID << " Price: " << order.price << " Quantity: " << order.quantity << std::endl;
        }
        std::cout << "Sell Orders:\n";
        for (const auto& order : sellOrders) {
            std::cout << "Order ID: " << order.orderID << " Price: " << order.price << " Quantity: " << order.quantity << std::endl;
        }
    }
};

// Low-Latency Message Queue to simulate market data feed
class MarketDataFeed {
private:
    std::atomic<bool> isNewDataAvailable{false}; // Simulate availability of new market data
    std::queue<Order> orderQueue;

public:
    void feedNewOrder(const Order& order) {
        orderQueue.push(order);
        isNewDataAvailable.store(true, std::memory_order_release);
    }

    bool getNextOrder(Order& order) {
        if (isNewDataAvailable.load(std::memory_order_acquire)) {
            order = orderQueue.front();
            orderQueue.pop();
            if (orderQueue.empty()) {
                isNewDataAvailable.store(false, std::memory_order_release);
            }
            return true;
        }
        return false;
    }
};

// Simulate Trading System - Perform real-time order matching
void tradingEngine(OrderBook& orderBook, MarketDataFeed& dataFeed) {
    while (true) {
        Order newOrder(0, true, 100.0, 100);  // Sample Order (Buy)
        if (dataFeed.getNextOrder(newOrder)) {
            orderBook.addOrder(newOrder);
            orderBook.matchOrders();
        }
    }
}

// Simulate a High-Frequency Trading Market Data Feed
void simulateMarketDataFeed(MarketDataFeed& dataFeed) {
    int i = 0;
    while (true) {  // Run infinitely to simulate a continuous data feed
        Order newOrder(i++, i % 2 == 0, 100.0 + i, 100 + i);  // Alternating Buy and Sell orders
        dataFeed.feedNewOrder(newOrder);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Simulate market delay
    }
}

int main() {
    // Initialize system components
    OrderBook orderBook;
    MarketDataFeed dataFeed;

    // Start the market data simulation
    std::thread marketDataThread(simulateMarketDataFeed, std::ref(dataFeed));

    // Start the trading engine (order matching)
    std::thread tradingEngineThread(tradingEngine, std::ref(orderBook), std::ref(dataFeed));

    marketDataThread.join();
    tradingEngineThread.join();

    return 0;
}
