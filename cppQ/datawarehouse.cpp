#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include <atomic>

// Simple structure to represent a data record (e.g., row in a table)
struct DataRecord {
    int id;
    std::string name;
    float value;
    
    // Constructor for ease of creating records
    DataRecord(int _id, std::string _name, float _value) 
        : id(_id), name(_name), value(_value) {}
};

// Simulating a Data Warehouse
class DataWarehouse {
private:
    std::vector<DataRecord> data;  // Holds all the data records

public:
    // Extract data (for this example, we just initialize some data manually)
    void extractData() {
        // Example data
        data.push_back(DataRecord(1, "Product A", 100.50));
        data.push_back(DataRecord(2, "Product B", 150.75));
        data.push_back(DataRecord(3, "Product C", 120.40));
        data.push_back(DataRecord(4, "Product D", 130.30));
        std::cout << "Data extracted successfully.\n";
    }

    // Parallelized Transform data (filter or aggregate data)
    void transformData() {
        std::atomic<int> index(0);  // Atomic index to prevent race conditions
        
        // Function to process records in parallel
        auto processBatch = [&](int startIdx, int endIdx) {
            for (int i = startIdx; i < endIdx; ++i) {
                if (data[i].value < 120) {
                    data[i] = DataRecord(0, "", 0);  // "Remove" by zeroing out record
                }
            }
        };

        // Number of threads to divide the work
        int numThreads = std::thread::hardware_concurrency();  // Use available hardware threads
        int batchSize = data.size() / numThreads;

        std::vector<std::thread> threads;
        
        // Launch multiple threads to process data in parallel
        for (int i = 0; i < numThreads; ++i) {
            int startIdx = i * batchSize;
            int endIdx = (i == numThreads - 1) ? data.size() : (i + 1) * batchSize; // Last thread processes the remaining records
            threads.push_back(std::thread(processBatch, startIdx, endIdx));
        }

        // Wait for all threads to finish
        for (auto& t : threads) {
            t.join();
        }

        // Remove "empty" records (those with value 0 after transformation)
        data.erase(std::remove_if(data.begin(), data.end(), [](const DataRecord& record) {
            return record.value == 0;
        }), data.end());

        std::cout << "Data transformed (filtered by value >= 120) using parallel processing.\n";
    }

    // Load data (insert data into the 'warehouse')
    void loadData() {
        std::cout << "Loading data into the Data Warehouse:\n";
        for (const auto& record : data) {
            if (record.id != 0) {  // Skip "removed" records
                std::cout << "ID: " << record.id << ", Name: " << record.name 
                          << ", Value: $" << record.value << std::endl;
            }
        }
    }

    // Query data (simple query example to find all records)
    void queryData() {
        std::cout << "Querying all records from the Data Warehouse:\n";
        for (const auto& record : data) {
            if (record.id != 0) {  // Skip "removed" records
                std::cout << "ID: " << record.id << ", Name: " << record.name 
                          << ", Value: $" << record.value << std::endl;
            }
        }
    }
};

int main() {
    // Simulating a Data Warehouse
    DataWarehouse warehouse;

    // Extract data (from some source)
    warehouse.extractData();

    // Transform data (e.g., filtering) using parallel processing
    warehouse.transformData();

    // Load data (into the warehouse)
    warehouse.loadData();

    // Query data (simple select all operation)
    warehouse.queryData();

    return 0;
}
