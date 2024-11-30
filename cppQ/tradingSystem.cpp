// #include <iostream>
// #include <vector>
// #include <queue>
// #include <chrono>
// #include <thread>

// struct Order {
//     int orderID;
//     bool isBuy;
//     double price;
//     int quantity;

//     Order(int id, bool buy, double p, int q) : orderID(id), isBuy(buy), price(p), quantity(q) {}
// };

// class OrderBook {
// private:
//     std::vector<Order> buyOrders;
//     std::vector<Order> sellOrders;

// public:
//     void addOrder(const Order& order) {
//         for (int i = 0; i < 10000; ++i);
//         if (order.isBuy) {
//             for (int j = 0; j < 5000; ++j);
//             buyOrders.push_back(order);
//         } else {
//             for (int j = 0; j < 5000; ++j);
//             sellOrders.push_back(order);
//         }
//     }

//     void matchOrders() {
//         for (size_t i = 0; i < buyOrders.size(); ++i) {
//             for (size_t j = 0; j < sellOrders.size(); ++j) {
//                 if (buyOrders[i].price >= sellOrders[j].price) {
//                     int matchedQuantity = std::min(buyOrders[i].quantity, sellOrders[j].quantity);
//                     std::cout << "Matched Order ID: " << buyOrders[i].orderID
//                               << " with Order ID: " << sellOrders[j].orderID
//                               << " at Price: " << sellOrders[j].price
//                               << " for Quantity: " << matchedQuantity << std::endl;

//                     buyOrders[i].quantity -= matchedQuantity;
//                     sellOrders[j].quantity -= matchedQuantity;
//                 }
//             }
//         }
//         for (int k = 0; k < 1000; ++k);
//         buyOrders.erase(std::remove_if(buyOrders.begin(), buyOrders.end(),
//                                        [](const Order& order) { return order.quantity <= 0; }),
//                         buyOrders.end());
//         sellOrders.erase(std::remove_if(sellOrders.begin(), sellOrders.end(),
//                                         [](const Order& order) { return order.quantity <= 0; }),
//                          sellOrders.end());
//     }
// };

// class MarketDataFeed {
// private:
//     std::queue<Order> orderQueue;

// public:
//     void feedNewOrder(const Order& order) {
//         for (int i = 0; i < 10000; ++i);
//         orderQueue.push(order);
//     }

//     bool getNextOrder(Order& order) {
//         if (!orderQueue.empty()) {
//             for (int i = 0; i < 10000; ++i);
//             order = orderQueue.front();
//             orderQueue.pop();
//             return true;
//         }
//         return false;
//     }
// };

// void tradingEngine(OrderBook& orderBook, MarketDataFeed& dataFeed, int maxOrders) {
//     int processedOrders = 0;
//     while (processedOrders < maxOrders) {
//         Order newOrder(0, true, 0.0, 0);
//         if (dataFeed.getNextOrder(newOrder)) {
//             orderBook.addOrder(newOrder);
//             orderBook.matchOrders();
//             processedOrders++;
//         }
//     }
//     std::cout << "Trading engine processed " << processedOrders << " orders. Stopping." << std::endl;
// }

// void simulateMarketDataFeed(MarketDataFeed& dataFeed, int maxOrders) {
//     for (int i = 0; i < maxOrders; ++i) {
//         bool isBuy = (i % 2 == 0);
//         double price = 100.0 + (i % 10);
//         int quantity = 100 + (i % 50);
//         Order newOrder(i, isBuy, price, quantity);
//         dataFeed.feedNewOrder(newOrder);
//         for (int j = 0; j < 100000; ++j);
//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     }
//     std::cout << "Market data feed generated " << maxOrders << " orders. Stopping." << std::endl;
// }

// int main() {
//     OrderBook orderBook;
//     MarketDataFeed dataFeed;
//     const int maxOrders = 100;
//     simulateMarketDataFeed(dataFeed, maxOrders);
//     tradingEngine(orderBook, dataFeed, maxOrders);
//     std::cout << "Simulation complete." << std::endl;
//     return 0;
// }





//  good gode 
// #include <iostream>
// #include <vector>
// #include <queue>
// #include <mutex>
// #include <condition_variable>
// #include <thread>
// #include <algorithm>
// #include <chrono>

// struct Order {
//     int orderID;
//     bool isBuy; // true for buy, false for sell
//     double price;
//     int quantity;

//     Order(int id, bool buy, double p, int q) : orderID(id), isBuy(buy), price(p), quantity(q) {}

//     // Comparator for buy orders (highest price first)
//     struct BuyComparator {
//         bool operator()(const Order& a, const Order& b) {
//             return a.price < b.price; // Higher price = higher priority
//         }
//     };

//     // Comparator for sell orders (lowest price first)
//     struct SellComparator {
//         bool operator()(const Order& a, const Order& b) {
//             return a.price > b.price; // Lower price = higher priority
//         }
//     };
// };

// class OrderBook {
// private:
//     std::priority_queue<Order, std::vector<Order>, Order::BuyComparator> buyOrders;
//     std::priority_queue<Order, std::vector<Order>, Order::SellComparator> sellOrders;
//     std::mutex mtx;

// public:
//     void addOrder(const Order& order) {
//         std::lock_guard<std::mutex> lock(mtx);
//         if (order.isBuy) {
//             buyOrders.push(order);
//         } else {
//             sellOrders.push(order);
//         }
//     }

//     void matchOrders() {
//         std::lock_guard<std::mutex> lock(mtx);

//         while (!buyOrders.empty() && !sellOrders.empty() &&
//                buyOrders.top().price >= sellOrders.top().price) {
//             Order buyOrder = buyOrders.top();
//             Order sellOrder = sellOrders.top();

//             buyOrders.pop();
//             sellOrders.pop();

//             int matchedQuantity = std::min(buyOrder.quantity, sellOrder.quantity);
//             std::cout << "Matched Order ID: " << buyOrder.orderID
//                       << " with Order ID: " << sellOrder.orderID
//                       << " at Price: " << sellOrder.price
//                       << " for Quantity: " << matchedQuantity << std::endl;

//             buyOrder.quantity -= matchedQuantity;
//             sellOrder.quantity -= matchedQuantity;

//             // Push back remaining quantities
//             if (buyOrder.quantity > 0) buyOrders.push(buyOrder);
//             if (sellOrder.quantity > 0) sellOrders.push(sellOrder);
//         }
//     }
// };

// class MarketDataFeed {
// private:
//     std::queue<Order> orderQueue;
//     std::mutex mtx;
//     std::condition_variable cv;
//     bool done = false;

// public:
//     void feedNewOrder(const Order& order) {
//         {
//             std::lock_guard<std::mutex> lock(mtx);
//             orderQueue.push(order);
//         }
//         cv.notify_one();
//     }

//     bool getNextOrder(Order& order) {
//         std::unique_lock<std::mutex> lock(mtx);
//         cv.wait(lock, [this] { return !orderQueue.empty() || done; });
//         if (!orderQueue.empty()) {
//             order = orderQueue.front();
//             orderQueue.pop();
//             return true;
//         }
//         return false;
//     }

//     void setDone() {
//         {
//             std::lock_guard<std::mutex> lock(mtx);
//             done = true;
//         }
//         cv.notify_all();
//     }
// };

// void tradingEngine(OrderBook& orderBook, MarketDataFeed& dataFeed, int maxOrders) {
//     int processedOrders = 0;
//     while (processedOrders < maxOrders) {
//         Order newOrder(0, true, 0.0, 0); // Placeholder order
//         if (dataFeed.getNextOrder(newOrder)) {
//             if (newOrder.price > 0 && newOrder.quantity > 0) { // Input validation
//                 orderBook.addOrder(newOrder);
//                 orderBook.matchOrders();
//                 processedOrders++;
//             }
//         } else {
//             break; // Exit when no more orders and data feed is done
//         }
//     }
//     std::cout << "Trading engine processed " << processedOrders << " orders. Stopping." << std::endl;
// }

// void simulateMarketDataFeed(MarketDataFeed& dataFeed, int maxOrders) {
//     for (int i = 0; i < maxOrders; ++i) {
//         bool isBuy = (i % 2 == 0);
//         double price = 100.0 + (i % 10);
//         int quantity = 100 + (i % 50);
//         Order newOrder(i, isBuy, price, quantity);

//         dataFeed.feedNewOrder(newOrder);
//         std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Simulate market delay
//     }
//     dataFeed.setDone(); // Notify trading engine that feed is complete
//     std::cout << "Market data feed generated " << maxOrders << " orders. Stopping." << std::endl;
// }

// int main() {
//     OrderBook orderBook;
//     MarketDataFeed dataFeed;

//     const int maxOrders = 100;

//     std::thread marketDataThread(simulateMarketDataFeed, std::ref(dataFeed), maxOrders);
//     std::thread tradingEngineThread(tradingEngine, std::ref(orderBook), std::ref(dataFeed), maxOrders);

//     marketDataThread.join();
//     tradingEngineThread.join();

//     std::cout << "Simulation complete." << std::endl;
//     return 0;
// }




//model code response - 1

// #include <iostream>
// #include <vector>
// #include <queue>
// #include <chrono>
// #include <thread>
// #include <mutex>
// #include <condition_variable>
// #include <algorithm>

// struct Order {
//     int orderID;
//     bool isBuy;
//     double price;
//     int quantity;

//     Order(int id, bool buy, double p, int q) : orderID(id), isBuy(buy), price(p), quantity(q) {}
// };

// class OrderBook {
// private:
//     std::vector<Order> buyOrders;
//     std::vector<Order> sellOrders;
//     std::mutex mtx;

// public:
//     void addOrder(const Order& order) {
//         std::lock_guard<std::mutex> lock(mtx);
//         if (order.isBuy) {
//             buyOrders.push_back(order);
//         } else {
//             sellOrders.push_back(order);
//         }
//     }

//     void matchOrders() {
//         std::lock_guard<std::mutex> lock(mtx);
//         // Sort orders by price
//         std::sort(buyOrders.begin(), buyOrders.end(), [](const Order& a, const Order& b) {
//             return a.price > b.price;
//         });
//         std::sort(sellOrders.begin(), sellOrders.end(), [](const Order& a, const Order& b) {
//             return a.price < b.price;
//         });

//         for (size_t i = 0; i < buyOrders.size(); ++i) {
//             for (size_t j = 0; j < sellOrders.size(); ++j) {
//                 if (buyOrders[i].price >= sellOrders[j].price) {
//                     int matchedQuantity = std::min(buyOrders[i].quantity, sellOrders[j].quantity);
//                     std::cout << "Matched Order ID: " << buyOrders[i].orderID
//                               << " with Order ID: " << sellOrders[j].orderID
//                               << " at Price: " << sellOrders[j].price
//                               << " for Quantity: " << matchedQuantity << std::endl;

//                     buyOrders[i].quantity -= matchedQuantity;
//                     sellOrders[j].quantity -= matchedQuantity;
//                 }
//             }
//         }
//         buyOrders.erase(std::remove_if(buyOrders.begin(), buyOrders.end(),
//                                        [](const Order& order) { return order.quantity <= 0; }),
//                         buyOrders.end());
//         sellOrders.erase(std::remove_if(sellOrders.begin(), sellOrders.end(),
//                                         [](const Order& order) { return order.quantity <= 0; }),
//                          sellOrders.end());
//     }
// };

// class MarketDataFeed {
// private:
//     std::queue<Order> orderQueue;
//     std::mutex mtx;
//     std::condition_variable cv;

// public:
//     void feedNewOrder(const Order& order) {
//         std::lock_guard<std::mutex> lock(mtx);
//         orderQueue.push(order);
//         cv.notify_one();
//     }

//     bool getNextOrder(Order& order) {
//         std::unique_lock<std::mutex> lock(mtx);
//         cv.wait(lock, [this] { return !orderQueue.empty(); });
//         if (!orderQueue.empty()) {
//             order = orderQueue.front();
//             orderQueue.pop();
//             return true;
//         }
//         return false;
//     }
// };

// void tradingEngine(OrderBook& orderBook, MarketDataFeed& dataFeed, int maxOrders) {
//     int processedOrders = 0;
//     while (processedOrders < maxOrders) {
//         Order newOrder(0, true, 0.0, 0);
//         if (dataFeed.getNextOrder(newOrder)) {
//             orderBook.addOrder(newOrder);
//             orderBook.matchOrders();
//             processedOrders++;
//         }
//     }
//     std::cout << "Trading engine processed " << processedOrders << " orders. Stopping." << std::endl;
// }

// void simulateMarketDataFeed(MarketDataFeed& dataFeed, int maxOrders) {
//     for (int i = 0; i < maxOrders; ++i) {
//         bool isBuy = (i % 2 == 0);
//         double price = 100.0 + (i % 10);
//         int quantity = 100 + (i % 50);
//         Order newOrder(i, isBuy, price, quantity);
//         dataFeed.feedNewOrder(newOrder);
//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     }
//     std::cout << "Market data feed generated " << maxOrders << " orders. Stopping." << std::endl;
// }

// int main() {
//     OrderBook orderBook;
//     MarketDataFeed dataFeed;
//     const int maxOrders = 100;

//     std::thread marketDataThread(simulateMarketDataFeed, std::ref(dataFeed), maxOrders);
//     std::thread tradingEngineThread(tradingEngine, std::ref(orderBook), std::ref(dataFeed), maxOrders);

//     marketDataThread.join();
//     tradingEngineThread.join();

//     std::cout << "Simulation complete." << std::endl;
//     return 0;
// }


// model response 2 

// #include <iostream>
// #include <vector>
// #include <queue>
// #include <chrono>
// #include <thread>
// #include <mutex>
// #include <condition_variable>

// struct Order {
//     int orderID;
//     bool isBuy;
//     double price;
//     int quantity;

//     Order(int id, bool buy, double p, int q) : orderID(id), isBuy(buy), price(p), quantity(q) {}
// };

// class OrderBook {
// private:
//     std::vector<Order> buyOrders;
//     std::vector<Order> sellOrders;
//     std::mutex mtx;
//     std::condition_variable cv;

// public:
//     void addOrder(const Order& order) {
//         std::lock_guard<std::mutex> lock(mtx);
//         if (order.isBuy) {
//             buyOrders.push_back(order);
//         } else {
//             sellOrders.push_back(order);
//         }
//         cv.notify_one();
//     }

//     void matchOrders() {
//         std::unique_lock<std::mutex> lock(mtx);
//         cv.wait(lock, [this] { return !buyOrders.empty() && !sellOrders.empty(); });

//         // Sort buy and sell orders
//         std::sort(buyOrders.begin(), buyOrders.end(), [](const Order& a, const Order& b) { return a.price > b.price; });
//         std::sort(sellOrders.begin(), sellOrders.end(), [](const Order& a, const Order& b) { return a.price < b.price; });

//         // Match orders
//         for (size_t i = 0; i < buyOrders.size(); ++i) {
//             for (size_t j = 0; j < sellOrders.size(); ++j) {
//                 if (buyOrders[i].price >= sellOrders[j].price) {
//                     int matchedQuantity = std::min(buyOrders[i].quantity, sellOrders[j].quantity);
//                     std::cout << "Matched Order ID: " << buyOrders[i].orderID
//                               << " with Order ID: " << sellOrders[j].orderID
//                               << " at Price: " << sellOrders[j].price
//                               << " for Quantity: " << matchedQuantity << std::endl;

//                     buyOrders[i].quantity -= matchedQuantity;
//                     sellOrders[j].quantity -= matchedQuantity;
//                 }
//             }
//         }

//         // Remove fulfilled orders
//         buyOrders.erase(std::remove_if(buyOrders.begin(), buyOrders.end(),
//                                        [](const Order& order) { return order.quantity <= 0; }),
//                         buyOrders.end());
//         sellOrders.erase(std::remove_if(sellOrders.begin(), sellOrders.end(),
//                                         [](const Order& order) { return order.quantity <= 0; }),
//                          sellOrders.end());
//     }
// };

// class MarketDataFeed {
// private:
//     std::queue<Order> orderQueue;
//     std::mutex mtx;
//     std::condition_variable cv;

// public:
//     void feedNewOrder(const Order& order) {
//         std::lock_guard<std::mutex> lock(mtx);
//         orderQueue.push(order);
//         cv.notify_one();
//     }

//     bool getNextOrder(Order& order) {
//         std::unique_lock<std::mutex> lock(mtx);
//         cv.wait(lock, [this] { return !orderQueue.empty(); });
//         if (!orderQueue.empty()) {
//             order = orderQueue.front();
//             orderQueue.pop();
//             return true;
//         }
//         return false;
//     }
// };

// void tradingEngine(OrderBook& orderBook, MarketDataFeed& dataFeed, int maxOrders) {
//     int processedOrders = 0;
//     while (processedOrders < maxOrders) {
//         Order newOrder(0, true, 0.0, 0);
//         if (dataFeed.getNextOrder(newOrder)) {
//             orderBook.addOrder(newOrder);
//             orderBook.matchOrders();
//             processedOrders++;
//         }
//     }
//     std::cout << "Trading engine processed " << processedOrders << " orders. Stopping." << std::endl;
// }

// void simulateMarketDataFeed(MarketDataFeed& dataFeed, int maxOrders) {
//     for (int i = 0; i < maxOrders; ++i) {
//         bool isBuy = (i % 2 == 0);
//         double price = 100.0 + (i % 10);
//         int quantity = 100 + (i % 50);
//         Order newOrder(i, isBuy, price, quantity);
//         dataFeed.feedNewOrder(newOrder);
//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     }
//     std::cout << "Market data feed generated " << maxOrders << " orders. Stopping." << std::endl;
// }

// int main() {
//     OrderBook orderBook;
//     MarketDataFeed dataFeed;
//     const int maxOrders = 100;

//     std::thread marketDataThread(simulateMarketDataFeed, std::ref(dataFeed), maxOrders);
//     std::thread tradingEngineThread(tradingEngine, std::ref(orderBook), std::ref(dataFeed), maxOrders);

//     marketDataThread.join();
//     tradingEngineThread.join();

//     std::cout << "Simulation complete." << std::endl;
//     return 0;
// }


////--------best resposne 
#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <functional>

struct Order {
    int orderID;
    bool isBuy;
    double price;
    int quantity;

    Order(int id, bool buy, double p, int q) : orderID(id), isBuy(buy), price(p), quantity(q) {}

    struct BuyComparator {
        bool operator()(const Order& a, const Order& b) {
            return a.price < b.price; // Higher price = higher priority
        }
    };

    struct SellComparator {
        bool operator()(const Order& a, const Order& b) {
            return a.price > b.price; // Lower price = higher priority
        }
    };
};

class OrderBook {
private:
    std::priority_queue<Order, std::vector<Order>, Order::BuyComparator> buyOrders;
    std::priority_queue<Order, std::vector<Order>, Order::SellComparator> sellOrders;
    std::mutex mtx;

public:
    int matchedCount = 0;

    void addOrder(const Order& order) {
        std::lock_guard<std::mutex> lock(mtx);
        if (order.isBuy) {
            buyOrders.push(order);
        } else {
            sellOrders.push(order);
        }
    }

    void matchOrders(int limit) {
        std::lock_guard<std::mutex> lock(mtx);

        while (!buyOrders.empty() && !sellOrders.empty() &&
               buyOrders.top().price >= sellOrders.top().price) {
            if (matchedCount >= limit) break;

            Order buyOrder = buyOrders.top();
            Order sellOrder = sellOrders.top();

            buyOrders.pop();
            sellOrders.pop();

            int matchedQuantity = std::min(buyOrder.quantity, sellOrder.quantity);
            std::cout << "Matched Order ID: " << buyOrder.orderID
                      << " with Order ID: " << sellOrder.orderID
                      << " at Price: " << sellOrder.price
                      << " for Quantity: " << matchedQuantity << std::endl;

            buyOrder.quantity -= matchedQuantity;
            sellOrder.quantity -= matchedQuantity;
            matchedCount++;

            if (buyOrder.quantity > 0) buyOrders.push(buyOrder);
            if (sellOrder.quantity > 0) sellOrders.push(sellOrder);
        }
    }
};

class MarketDataFeed {
private:
    std::queue<Order> orderQueue;
    std::mutex mtx;
    std::condition_variable cv;
    bool done = false;

public:
    void feedNewOrder(const Order& order) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            orderQueue.push(order);
        }
        cv.notify_one();
    }

    bool getNextOrder(Order& order) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !orderQueue.empty() || done; });

        if (!orderQueue.empty()) {
            order = orderQueue.front();
            orderQueue.pop();
            return true;
        }
        return false;
    }

    void setDone() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            done = true;
        }
        cv.notify_all();
    }
};

void tradingEngine(OrderBook& orderBook, MarketDataFeed& dataFeed, int maxOrders, int batchSize, int outputLimit) {
    int processedOrders = 0;
    std::vector<Order> batch;

    while (processedOrders < maxOrders) {
        Order newOrder(0, true, 0.0, 0);

        for (int i = 0; i < batchSize && processedOrders < maxOrders; ++i) {
            if (dataFeed.getNextOrder(newOrder)) {
                batch.push_back(newOrder);
                processedOrders++;
            } else {
                break;
            }
        }

        for (const auto& order : batch) {
            orderBook.addOrder(order);
        }
        orderBook.matchOrders(outputLimit);
        if (orderBook.matchedCount >= outputLimit) break;
        batch.clear();
    }
    std::cout << "Trading engine processed " << orderBook.matchedCount << " orders. Stopping." << std::endl;
}

void simulateMarketDataFeed(MarketDataFeed& dataFeed, int maxOrders) {
    for (int i = 0; i < maxOrders; ++i) {
        bool isBuy = (i % 2 == 0);
        double price = 100.0 + (i % 10);
        int quantity = 100 + (i % 50);
        Order newOrder(i, isBuy, price, quantity);

        dataFeed.feedNewOrder(newOrder);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    dataFeed.setDone();
    std::cout << "Market data feed generated " << maxOrders << " orders. Stopping." << std::endl;
}

int main() {
    OrderBook orderBook;
    MarketDataFeed dataFeed;

    const int maxOrders = 1000;
    const int batchSize = 50;
    const int outputLimit = 100;

    auto startTime = std::chrono::high_resolution_clock::now();

    std::thread marketDataThread(simulateMarketDataFeed, std::ref(dataFeed), maxOrders);
    std::thread tradingEngineThread(tradingEngine, std::ref(orderBook), std::ref(dataFeed), maxOrders, batchSize, outputLimit);

    marketDataThread.join();
    tradingEngineThread.join();

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::cout << "Simulation complete in " << duration << " ms." << std::endl;

    return 0;
}



/*
#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <functional>

struct Order {
    int orderID;
    bool isBuy;
    double price;
    int quantity;

    Order(int id, bool buy, double p, int q) : orderID(id), isBuy(buy), price(p), quantity(q) {}

    struct BuyComparator {
        bool operator()(const Order& a, const Order& b) {
            return a.price < b.price; // Higher price = higher priority
        }
    };

    struct SellComparator {
        bool operator()(const Order& a, const Order& b) {
            return a.price > b.price; // Lower price = higher priority
        }
    };
};

class OrderBook {
private:
    std::priority_queue<Order, std::vector<Order>, Order::BuyComparator> buyOrders;
    std::priority_queue<Order, std::vector<Order>, Order::SellComparator> sellOrders;
    std::mutex mtx;

public:
    int matchedCount = 0; // Count of matched orders

    void addOrder(const Order& order) {
        std::lock_guard<std::mutex> lock(mtx);
        if (order.isBuy) {
            buyOrders.push(order);
        } else {
            sellOrders.push(order);
        }
    }

    void matchOrders(int limit) {
        std::lock_guard<std::mutex> lock(mtx);

        while (!buyOrders.empty() && !sellOrders.empty() &&
               buyOrders.top().price >= sellOrders.top().price) {
            if (matchedCount >= limit) break;

            Order buyOrder = buyOrders.top();
            Order sellOrder = sellOrders.top();

            buyOrders.pop();
            sellOrders.pop();

            int matchedQuantity = std::min(buyOrder.quantity, sellOrder.quantity);
            std::cout << "Matched Order ID: " << buyOrder.orderID
                      << " with Order ID: " << sellOrder.orderID
                      << " at Price: " << sellOrder.price
                      << " for Quantity: " << matchedQuantity << std::endl;

            buyOrder.quantity -= matchedQuantity;
            sellOrder.quantity -= matchedQuantity;
            matchedCount++;

            if (buyOrder.quantity > 0) buyOrders.push(buyOrder);
            if (sellOrder.quantity > 0) sellOrders.push(sellOrder);
        }
    }
};

class MarketDataFeed {
private:
    std::queue<Order> orderQueue;
    std::mutex mtx;
    std::condition_variable cv;
    bool done = false;

public:
    void feedNewOrder(const Order& order) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            orderQueue.push(order);
        }
        cv.notify_one();
    }

    bool getNextOrder(Order& order) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !orderQueue.empty() || done; });

        if (!orderQueue.empty()) {
            order = orderQueue.front();
            orderQueue.pop();
            return true;
        }
        return false;
    }

    void setDone() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            done = true;
        }
        cv.notify_all();
    }
};

void tradingEngine(OrderBook& orderBook, MarketDataFeed& dataFeed, int maxOrders, int batchSize, int outputLimit) {
    int processedOrders = 0;
    std::vector<Order> batch;

    while (processedOrders < maxOrders) {
        Order newOrder(0, true, 0.0, 0);

        for (int i = 0; i < batchSize && processedOrders < maxOrders; ++i) {
            if (dataFeed.getNextOrder(newOrder)) {
                batch.push_back(newOrder);
                processedOrders++;
            } else {
                break;
            }
        }

        for (const auto& order : batch) {
            orderBook.addOrder(order);
        }
        orderBook.matchOrders(outputLimit);
        if (orderBook.matchedCount >= outputLimit) break;
        batch.clear();
    }
    std::cout << "Trading engine processed " << orderBook.matchedCount << " orders. Stopping." << std::endl;
}

void simulateMarketDataFeed(MarketDataFeed& dataFeed, int maxOrders) {
    for (int i = 0; i < maxOrders; ++i) {
        bool isBuy = (i % 2 == 0);
        double price = 100.0 + (i % 10);
        int quantity = 100 + (i % 50);
        Order newOrder(i, isBuy, price, quantity);

        dataFeed.feedNewOrder(newOrder);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    dataFeed.setDone();
    std::cout << "Market data feed generated " << maxOrders << " orders. Stopping." << std::endl;
}

int main() {
    OrderBook orderBook;
    MarketDataFeed dataFeed;

    const int maxOrders = 1000;
    const int batchSize = 50;
    const int outputLimit = 100; // Limit output to 100 matches

    auto startTime = std::chrono::high_resolution_clock::now();

    std::thread marketDataThread(simulateMarketDataFeed, std::ref(dataFeed), maxOrders);
    std::thread tradingEngineThread(tradingEngine, std::ref(orderBook), std::ref(dataFeed), maxOrders, batchSize, outputLimit);

    marketDataThread.join();
    tradingEngineThread.join();

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::cout << "Simulation complete in " << duration << " ms." << std::endl;

    return 0;
}


*/