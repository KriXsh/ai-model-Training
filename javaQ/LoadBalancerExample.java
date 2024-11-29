// idea

/*
I am a software developer at a product-based company, and I need Develop a load balancing algorithm that 
prioritizes servers with the least CPU usage for a Java-based  application running in Docker containers, 
deployed across a hybrid cloud infrastructure ( like Microsoft Azure, AWS, and GCP). made a code that 
like it has 5 server and it default object basis like every server objects has CPU and storage IP in a
single file. in that single file you can add the logic for the same also print the expected output. 
The system should scale based on real-time CPU load. The load balancing  mechanism should use a custom 
load monitoring tool that will run within the containers and report the CPU load directly to a central
monitoring system. When the CPU load exceeds a certain threshold, the system should automatically balance 
the load  across to other servers .The system should also maintain high availability, load balancing, data consistency,
and fault tolerance.
 */

import java.util.ArrayList;
import java.util.List;

public class LoadBalancerExample {

    // Server class to represent each server
    static class Server {
        private String name;     // Added server name (serv1, serv2, etc.)
        private String ip;
        private double cpuUsage;  // CPU usage as a percentage
        private int storage;      // Storage in GB

        // Constructor with server name, IP, CPU usage, and storage
        public Server(String name, String ip, double cpuUsage, int storage) {
            this.name = name;
            this.ip = ip;
            this.cpuUsage = cpuUsage;
            this.storage = storage;
        }

        // Getters and setters
        public String getName() {
            return name;
        }

        public String getIp() {
            return ip;
        }

        public double getCpuUsage() {
            return cpuUsage;
        }

        public void setCpuUsage(double cpuUsage) {
            this.cpuUsage = cpuUsage;
        }

        public int getStorage() {
            return storage;
        }

        // Method to simulate a server workload
        public void simulateWorkload(double cpuLoad) {
            // Increase CPU usage by the given load (ensuring it doesn't exceed 100%)
            this.cpuUsage = Math.min(100.0, this.cpuUsage + cpuLoad);
        }

        @Override
        public String toString() {
            return "Server[" + name + ", IP: " + ip + ", CPU Usage: " + cpuUsage + "%, Storage: " + storage + "GB]";
        }
    }

    // LoadBalancer class that handles the distribution of workloads
    static class LoadBalancer {

        private List<Server> servers;

        public LoadBalancer() {
            this.servers = new ArrayList<>();
        }

        // Add a server to the load balancer
        public void addServer(Server server) {
            servers.add(server);
        }

        // Find the server with the least CPU usage
        public Server getServerWithLeastCpuUsage() {
            Server selectedServer = null;
            double minCpuUsage = Double.MAX_VALUE;
        
            for (Server server : servers) {
                if (server.getCpuUsage() < minCpuUsage) {
                    minCpuUsage = server.getCpuUsage();
                    selectedServer = server;
                }
                // Handle tie-breaking using storage (less storage is preferred)
                else if (server.getCpuUsage() == minCpuUsage && server.getStorage() < selectedServer.getStorage()) {
                    selectedServer = server;
                }
            }
        
            return selectedServer;
        }

        // Distribute a workload to the server with the least CPU usage
        public void distributeWorkload(double cpuLoad) {
            // Find the server with the least CPU usage
            Server server = getServerWithLeastCpuUsage();
            if (server != null) {
                // Simulate the workload being applied to that server
                server.simulateWorkload(cpuLoad);
                System.out.println("Distributed " + cpuLoad + "% load to server: " + server);
            }
        }

        // Display all servers
        public void displayServers() {
            for (Server server : servers) {
                System.out.println(server);
            }
        }
    }

    // Main method to test the LoadBalancer and Server classes
    public static void main(String[] args) {

        // Create LoadBalancer instance
        LoadBalancer loadBalancer = new LoadBalancer();

        // Create and add servers with custom names (serv1, serv2, etc.)
        loadBalancer.addServer(new Server("Server1", "192.168.0.1", 10.0, 100));
        loadBalancer.addServer(new Server("Server2", "192.168.0.2", 25.0, 150));
        loadBalancer.addServer(new Server("Server3", "192.168.0.3", 5.0, 200));
        loadBalancer.addServer(new Server("Server4", "192.168.0.4", 60.0, 250));
        loadBalancer.addServer(new Server("Server5", "192.168.0.5", 30.0, 300));

        // Display the current server states
        System.out.println("Initial server states:");
        loadBalancer.displayServers();

        // Simulate workload distribution
        System.out.println("\nDistributing workloads to the servers...");
        loadBalancer.distributeWorkload(20.0);  // Simulate a 20% CPU load
        loadBalancer.distributeWorkload(15.0);  // Simulate a 15% CPU load
        loadBalancer.distributeWorkload(10.0);  // Simulate a 10% CPU load

        // Display server states after distributing workloads
        System.out.println("\nServer states after workload distribution:");
        loadBalancer.displayServers();
    }
}