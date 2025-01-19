// class Order {
//     constructor(orderID, isBuy, price, quantity) {
//         this.orderID = orderID;
//         this.isBuy = isBuy;
//         this.price = price;
//         this.quantity = quantity;
//     }
// }

// class OrderBook {
//     constructor() {
//         this.buyOrders = []; // Max-heap for buy orders (highest price priority)
//         this.sellOrders = []; // Min-heap for sell orders (lowest price priority)
//     }

//     // Add an order to the appropriate book
//     addOrder(order) {
//         if (order.isBuy) {
//             this.buyOrders.push(order);
//             this.buyOrders.sort((a, b) => b.price - a.price); // Higher price = higher priority
//         } else {
//             this.sellOrders.push(order);
//             this.sellOrders.sort((a, b) => a.price - b.price); // Lower price = higher priority
//         }
//     }

//     // Match orders in the book
//     matchOrders(limit) {
//         let matchedCount = 0;

//         while (
//             this.buyOrders.length > 0 &&
//             this.sellOrders.length > 0 &&
//             this.buyOrders[0].price >= this.sellOrders[0].price
//         ) {
//             if (matchedCount >= limit) break;

//             const buyOrder = this.buyOrders.shift();
//             const sellOrder = this.sellOrders.shift();

//             const matchedQuantity = Math.min(buyOrder.quantity, sellOrder.quantity);
//             console.log(
//                 `Matched Buy Order ${buyOrder.orderID} with Sell Order ${sellOrder.orderID} at Price ${sellOrder.price} for Quantity ${matchedQuantity}`
//             );

//             buyOrder.quantity -= matchedQuantity;
//             sellOrder.quantity -= matchedQuantity;

//             // Push back partially filled orders
//             if (buyOrder.quantity > 0) this.addOrder(buyOrder);
//             if (sellOrder.quantity > 0) this.addOrder(sellOrder);

//             matchedCount++;
//         }

//         return matchedCount;
//     }
// }

// class MarketDataFeed {
//     constructor() {
//         this.orderQueue = [];
//         this.done = false;
//     }

//     // Feed a new order into the queue
//     feedNewOrder(order) {
//         this.orderQueue.push(order);
//     }

//     // Get the next order from the queue
//     getNextOrder() {
//         return new Promise((resolve) => {
//             const checkQueue = () => {
//                 if (this.orderQueue.length > 0) {
//                     resolve(this.orderQueue.shift());
//                 } else if (this.done) {
//                     resolve(null);
//                 } else {
//                     setTimeout(checkQueue, 5); // Polling interval
//                 }
//             };
//             checkQueue();
//         });
//     }

//     // Mark the feed as done
//     setDone() {
//         this.done = true;
//     }
// }

// // Simulate market data feed
// async function simulateMarketDataFeed(dataFeed, maxOrders) {
//     for (let i = 0; i < maxOrders; i++) {
//         const isBuy = i % 2 === 0;
//         const price = 100 + (i % 10);
//         const quantity = 100 + (i % 50);

//         const newOrder = new Order(i, isBuy, price, quantity);
//         dataFeed.feedNewOrder(newOrder);

//         await new Promise((resolve) => setTimeout(resolve, 5)); // Simulate delay
//     }
//     dataFeed.setDone();
//     console.log(`Market data feed generated ${maxOrders} orders. Stopping.`);
// }

// // Trading engine
// async function tradingEngine(orderBook, dataFeed, maxOrders, batchSize, outputLimit) {
//     let processedOrders = 0;
//     const batch = [];

//     while (processedOrders < maxOrders) {
//         for (let i = 0; i < batchSize && processedOrders < maxOrders; i++) {
//             const order = await dataFeed.getNextOrder();
//             if (order) {
//                 batch.push(order);
//                 processedOrders++;
//             } else {
//                 break;
//             }
//         }

//         batch.forEach((order) => orderBook.addOrder(order));
//         batch.length = 0; // Clear batch
//         const matches = orderBook.matchOrders(outputLimit);
//         if (matches >= outputLimit) break;
//     }
//     console.log(`Trading engine processed ${processedOrders} orders.`);
// }

// // Main function
// async function main() {
//     const orderBook = new OrderBook();
//     const dataFeed = new MarketDataFeed();

//     const maxOrders = 1000;
//     const batchSize = 50;
//     const outputLimit = 100;

//     console.time("Simulation Time");

//     const marketDataThread = simulateMarketDataFeed(dataFeed, maxOrders);
//     const tradingEngineThread = tradingEngine(orderBook, dataFeed, maxOrders, batchSize, outputLimit);

//     await Promise.all([marketDataThread, tradingEngineThread]);

//     console.timeEnd("Simulation Time");
// }

// main();



// worst 

// class Order {
//     constructor(orderID, isBuy, price, quantity) {
//         this.orderID = orderID;
//         this.isBuy = isBuy;
//         this.price = price;
//         this.quantity = quantity;
//     }
// }

// class OrderBook {
//     constructor() {
//         this.orders = [];
//     }

//     // Add an order to the book
//     addOrder(order) {
//         this.orders.push(order);
//     }

//     // Match orders
//     matchOrders() {
//         for (let i = 0; i < this.orders.length; i++) {
//             const buyOrder = this.orders[i];
//             if (!buyOrder.isBuy) continue; 

//             for (let j = 0; j < this.orders.length; j++) {
//                 const sellOrder = this.orders[j];
//                 if (buyOrder === sellOrder || !sellOrder.isBuy) continue;

//                 if (buyOrder.price >= sellOrder.price) {
//                     const matchedQuantity = Math.min(buyOrder.quantity, sellOrder.quantity);

//                     console.log(
//                         `Matched Buy Order ${buyOrder.orderID} with Sell Order ${sellOrder.orderID} at Price ${sellOrder.price} for Quantity ${matchedQuantity}`
//                     );

//                     buyOrder.quantity -= matchedQuantity;
//                     sellOrder.quantity -= matchedQuantity;

//                     if (buyOrder.quantity === 0) this.orders.splice(i--, 1);
//                     if (sellOrder.quantity === 0) this.orders.splice(j--, 1);

//                     break; 
//                 }
//             }
//         }
//     }
// }

// class MarketDataFeed {
//     constructor() {
//         this.orders = []; 
//     }

//     // Feed new order
//     feedNewOrder(order) {
//         this.orders.push(order); 
//     }

   
//     getAllOrders() {
//         const orders = [...this.orders];
//         this.orders = [];
//         return orders;
//     }
// }

// function simulateMarketDataFeed(dataFeed, maxOrders) {
//     for (let i = 0; i < maxOrders; i++) {
//         const isBuy = i % 2 === 0; 
//         const price = 100 + Math.floor(Math.random() * 10);
//         const quantity = 100 + Math.floor(Math.random() * 50); 
//         const order = new Order(i, isBuy, price, quantity);

//         dataFeed.feedNewOrder(order); 
//     }
// }

// function tradingEngine(orderBook, dataFeed) {
//     const allOrders = dataFeed.getAllOrders(); 
//     allOrders.forEach(order => orderBook.addOrder(order)); 
//     orderBook.matchOrders();
// }

// function main() {
//     const orderBook = new OrderBook();
//     const dataFeed = new MarketDataFeed();

//     const maxOrders = 50; 

//     simulateMarketDataFeed(dataFeed, maxOrders); 
//     tradingEngine(orderBook, dataFeed); 
//     console.log("Trading simulation completed.");
// }

// main();


// resposn1

// class Order {
//     constructor(id, type, price, quantity) {
//       this.id = id;
//       this.type = type; // 'buy' ya 'sell'
//       this.price = price;
//       this.quantity = quantity;
//     }
//   }
  
//   class PriorityQueue {
//     constructor(comparator) {
//       this.items = [];
//       this.comparator = comparator;
//     }
  
//     enqueue(item) {
//       this.items.push(item);
//       this.bubbleUp();
//     }
  
//     bubbleUp() {
//       let index = this.items.length - 1;
//       const element = this.items[index];
//       while (index > 0) {
//         let parentIndex = Math.floor((index - 1) / 2);
//         let parent = this.items[parentIndex];
//         if (this.comparator(element, parent) <= 0) break;
//         this.items[index] = parent;
//         index = parentIndex;
//       }
//       this.items[index] = element;
//     }
  
//     dequeue() {
//       const max = this.items[0];
//       const end = this.items.pop();
//       if (this.items.length > 0) {
//         this.items[0] = end;
//         this.sinkDown();
//       }
//       return max;
//     }
  
//     sinkDown() {
//       let index = 0;
//       const length = this.items.length;
//       const element = this.items[0];
  
//       while (true) {
//         let leftChildIndex = 2 * index + 1;
//         let rightChildIndex = 2 * index + 2;
//         let leftChild, rightChild;
//         let swap = null;
  
//         if (leftChildIndex < length) {
//           leftChild = this.items[leftChildIndex];
//           if (this.comparator(leftChild, element) > 0) {
//             swap = leftChildIndex;
//           }
//         }
  
//         if (rightChildIndex < length) {
//           rightChild = this.items[rightChildIndex];
//           if (
//             (swap === null && this.comparator(rightChild, element) > 0) ||
//             (swap !== null && this.comparator(rightChild, leftChild) > 0)
//           ) {
//             swap = rightChildIndex;
//           }
//         }
  
//         if (swap === null) break;
//         this.items[index] = this.items[swap];
//         index = swap;
//       }
//       this.items[index] = element;
//     }
  
//     isEmpty() {
//       return this.items.length === 0;
//     }
//   }
  
//   class TradingEngine {
//     constructor() {
//       this.buyOrders = new PriorityQueue((a, b) => b.price - a.price); // highest price first
//       this.sellOrders = new PriorityQueue((a, b) => a.price - b.price); // lowest price first
//     }
  
//     async matchOrders() {
//       while (!this.buyOrders.isEmpty() && !this.sellOrders.isEmpty()) {
//         const buyOrder = this.buyOrders.dequeue();
//         const sellOrder = this.sellOrders.dequeue();
  
//         if (buyOrder.price >= sellOrder.price) {
//           const matchedQuantity = Math.min(buyOrder.quantity, sellOrder.quantity);
//           buyOrder.quantity -= matchedQuantity;
//           sellOrder.quantity -= matchedQuantity;
  
//           await this.processMatchedOrder(matchedQuantity, buyOrder.id, sellOrder.id);
  
//           if (buyOrder.quantity > 0) {
//             this.buyOrders.enqueue(buyOrder);
//           }
//           if (sellOrder.quantity > 0) {
//             this.sellOrders.enqueue(sellOrder);
//           }
//         } else {
//           // Agar match nahi hota hai to sell order ko wapas daal do
//           this.sellOrders.enqueue(sellOrder);
//         }
//       }
//     }
  
//     async processMatchedOrder(quantity, buyOrderId, sellOrderId) {
//       // Yahan par matched order ko process karne ke liye logic daal sakte hain
//       return new Promise(resolve => {
//         setTimeout(() => {
//           console.log(`Matched ${quantity} units between Buy Order ${buyOrderId} and Sell Order ${sellOrderId}`);
//           resolve();
//         }, 0);
//       });
//     }
  
//     addOrder(order) {
//       if (order.type === 'buy') {
//         this.buyOrders.enqueue(order);
//       } else if (order.type === 'sell') {
//         this.sellOrders.enqueue(order);
//       }
//     }
//   }
  
//   // Example usage:
//   const engine = new TradingEngine();
//   engine.addOrder(new Order(1, 'buy', 100, 10));
//   engine.addOrder(new Order(2, 'sell', 90, 5));
//   engine.addOrder(new Order(3, 'sell', 95, 10));
//   engine.addOrder(new Order(4, 'buy', 105, 5));
  
//   engine.matchOrders();


class Order {
    constructor(id, type, price, quantity) {
      this.id = id;
      this.type = type; // 'buy' or 'sell'
      this.price = price;
      this.quantity = quantity;
    }
  
    toString() {
      return `${this.type}: ${this.price}: ${this.quantity}`;
    }
  }
  
  class PriorityQueue {
    constructor(comparator = (a, b) => a - b) {
      this.array = [];
      this.comparator = comparator;
    }
  
    enqueue(element) {
      this.array.push(element);
      this.siftUp(this.array.length - 1);
    }
  
    dequeue() {
      if (this.isEmpty()) return null;
      const root = this.array[0];
      const last = this.array.pop();
      if (!this.isEmpty()) {
        this.array[0] = last;
        this.siftDown(0);
      }
      return root;
    }
  
    peek() {
      return this.array[0];
    }
  
    isEmpty() {
      return this.array.length === 0;
    }
  
    size() {
      return this.array.length;
    }
  
    siftUp(index) {
      let childIndex = index;
      let parentIndex = (childIndex - 1) >> 1;
      while (childIndex > 0 && this.comparator(this.array[childIndex], this.array[parentIndex]) > 0) {
        [this.array[childIndex], this.array[parentIndex]] = [this.array[parentIndex], this.array[childIndex]];
        childIndex = parentIndex;
        parentIndex = (childIndex - 1) >> 1;
      }
    }
  
    siftDown(index) {
      let parentIndex = index;
      let childIndex = 2 * parentIndex + 1;
      while (childIndex < this.array.length) {
        if (childIndex + 1 < this.array.length && this.comparator(this.array[childIndex + 1], this.array[childIndex]) > 0) {
          childIndex++;
        }
        if (this.comparator(this.array[parentIndex], this.array[childIndex]) >= 0) {
          break;
        }
        [this.array[parentIndex], this.array[childIndex]] = [this.array[childIndex], this.array[parentIndex]];
        parentIndex = childIndex;
        childIndex = 2 * parentIndex + 1;
      }
    }
  }
  
  class TradingEngine {
    constructor() {
      this.buyOrders = new PriorityQueue((a, b) => b.price - a.price);
      this.sellOrders = new PriorityQueue((a, b) => a.price - b.price);
    }
  
    async matchOrders() {
      while (!this.buyOrders.isEmpty() && !this.sellOrders.isEmpty()) {
        const buyOrder = this.buyOrders.dequeue();
        const sellOrder = this.sellOrders.dequeue();
  
        if (buyOrder && sellOrder && buyOrder.price >= sellOrder.price) {
          const matchedQuantity = Math.min(buyOrder.quantity, sellOrder.quantity);
          buyOrder.quantity -= matchedQuantity;
          sellOrder.quantity -= matchedQuantity;
  
          if (buyOrder.quantity === 0) {
            this.buyOrders.dequeue(); // Remove fully matched buy order
          }
          if (sellOrder.quantity === 0) {
            this.sellOrders.dequeue(); // Remove fully matched sell order
          }
  
          console.log(`Matched: ${buyOrder.toString()} with ${sellOrder.toString()}`);
          await this.delay(1); // Simulate time delay
        }
      }
    }
  
    addOrder(order) {
      if (order.type === 'buy') {
        this.buyOrders.enqueue(order);
      } else if (order.type === 'sell') {
        this.sellOrders.enqueue(order);
      }
    }
  
    async delay(ms) {
      return new Promise(resolve => setTimeout(resolve, ms));
    }
  }
  
  // Example usage
  const engine = new TradingEngine();
  
  engine.addOrder(new Order(1, 'buy', 110, 5));   // Buy order at price 110
  engine.addOrder(new Order(2, 'sell', 105, 3));  // Sell order at price 105
  engine.addOrder(new Order(3, 'buy', 102, 4));   // Buy order at price 102
  engine.addOrder(new Order(4, 'sell', 100, 2));  // Sell order at price 100
  
  engine.matchOrders();