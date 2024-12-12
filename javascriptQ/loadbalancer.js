// const { promisify } = require('util');
// const sleep = promisify(setTimeout);

// // सर्वर कॉन्फ़िगरेशन
// const servers = [
//   { id: 1, ip: '192.168.1.1', cpuUsage: 50, availableStorage: 100 },
//   { id: 2, ip: '192.168.1.2', cpuUsage: 60, availableStorage: 90 },
//   { id: 3, ip: '192.168.1.3', cpuUsage: 70, availableStorage: 80 },
//   { id: 4, ip: '192.168.1.4', cpuUsage: 85, availableStorage: 70 },
//   { id: 5, ip: '192.168.1.5', cpuUsage: 75, availableStorage: 85 }
// ];

// // वास्तविक समय CPU लोड सिमुलेशन फंक्शन
// async function simulateCPUUsage(server) {
//   // CPU लोड को एक प्रतिशत में बढ़ाने का सिमुलेशन
//   server.cpuUsage += Math.floor(Math.random() * 10);
//   if (server.cpuUsage > 100) {
//     server.cpuUsage = 100;
//   }
//   // वास्तविक समय के लिए स्वच्छन
//   await sleep(5000); // 5 सेकंड का इंटरवैल
//   return server.cpuUsage;
// }

// // लोड बैलेंसिंग फंक्शन
// async function loadBalance() {
//   while (true) {
//     const loadBalancedServer = servers.reduce((lowestCPU, server) => {
//       if (lowestCPU.cpuUsage > server.cpuUsage) {
//         return server;
//       } else if (lowestCPU.cpuUsage === server.cpuUsage) {
//         return lowestCPU.availableStorage > server.availableStorage ? lowestCPU : server;
//       }
//       return lowestCPU;
//     }, servers[0]);

//     // CPU लोड सिमुलेशन करें और निर्णय को प्रिंट करें
//     const currentCPUUsage = await simulateCPUUsage(loadBalancedServer);
//     console.log(`Current load balanced server: ${loadBalancedServer.ip} with CPU usage: ${currentCPUUsage}%`);

//     // यदि कोई सर्वर का CPU लोड 80% से अधिक है, तो ट्रैफिक को अन्य सर्वरों पर वितरित करें
//     servers.forEach((server, index) => {
//       if (server.cpuUsage > 80) {
//         console.warn(`Server ${server.ip} has high CPU usage (${server.cpuUsage}%). Traffic redirection required.`);
//       }
//     });
//   }
// }

// // लोड बैलेंसिंग शुरू करें
// loadBalance();

//////=----------response 2 

// const servers = [
//     { ip: '192.168.1.1', cpuUsage: 20, storageAvailable: 500 },
//     { ip: '192.168.1.2', cpuUsage: 50, storageAvailable: 300 },
//     { ip: '192.168.1.3', cpuUsage: 70, storageAvailable: 400 },
//     { ip: '192.168.1.4', cpuUsage: 10, storageAvailable: 600 },
//     { ip: '192.168.1.5', cpuUsage: 80, storageAvailable: 450 }
//   ];
  
//   function getServerWithLeastCPULoad() {
//     // Sort servers by CPU usage
//     const sortedServers = servers.sort((a, b) => a.cpuUsage - b.cpuUsage);
//     // Return the server with the least CPU load
//     return sortedServers[0];
//   }
  
//   function simulateCPULoadChanges() {
//     // Simulate random CPU load changes
//     servers.forEach(server => {
//       server.cpuUsage += Math.floor(Math.random() * 20 - 10); // Random change between -10 and 10
//       server.cpuUsage = Math.max(0, Math.min(100, server.cpuUsage)); // Ensure CPU usage is between 0 and 100
//     });
//   }
  
//   function balanceLoad() {
//     console.log("Current Server Status:");
//     console.table(servers);
  
//     servers.forEach(server => {
//       if (server.cpuUsage > 80) {
//         console.log(`Server ${server.ip} is over 80% load. Redistributing traffic...`);
//         const targetServer = getServerWithLeastCPULoad();
//         console.log(`Redirecting traffic from ${server.ip} to ${targetServer.ip}`);
//       }
//     });
//   }
  
//   function startLoadBalancer() {
//     setInterval(() => {
//       simulateCPULoadChanges();
//       balanceLoad();
//     }, 5000); // Every 5 seconds
//   }
  
//   startLoadBalancer();



///----------better code 
const servers = [
    { ip: '192.168.1.1', cpuUsage: 20, storageAvailable: 500 },
    { ip: '192.168.1.2', cpuUsage: 50, storageAvailable: 300 },
    { ip: '192.168.1.3', cpuUsage: 70, storageAvailable: 400 },
    { ip: '192.168.1.4', cpuUsage: 10, storageAvailable: 600 },
    { ip: '192.168.1.5', cpuUsage: 80, storageAvailable: 450 }
  ];
  
  // Function to simulate a central monitoring system
  function reportToCentralMonitoring() {
    console.log("Central Monitoring Report:");
    console.table(servers.map(server => ({
      ip: server.ip,
      cpuUsage: `${server.cpuUsage}%`,
      storageAvailable: `${server.storageAvailable} GB`
    })));
  }
  
  // Function to get the server with the least CPU load
  function getServerWithLeastCPULoad() {
    return servers.reduce((leastLoaded, server) =>
      server.cpuUsage < leastLoaded.cpuUsage ? server : leastLoaded
    );
  }
  
  // Simulate random CPU load changes
  function simulateCPULoadChanges() {
    servers.forEach(server => {
      // Simulate CPU usage changes (-10% to +10%)
      server.cpuUsage += Math.floor(Math.random() * 21) - 10;
      // Ensure CPU usage remains between 0 and 100
      server.cpuUsage = Math.max(0, Math.min(100, server.cpuUsage));
    });
  }
  
  // Balance load by redistributing traffic
  function balanceLoad() {
    servers.forEach(server => {
      if (server.cpuUsage > 80) {
        console.warn(`⚠️ Server ${server.ip} is over 80% load (${server.cpuUsage}%). Redistributing traffic...`);
        const targetServer = getServerWithLeastCPULoad();
        if (targetServer.ip !== server.ip) {
          console.log(`➡️ Redirecting traffic from ${server.ip} to ${targetServer.ip}`);
          // Simulate traffic redistribution by transferring 10% CPU load
          targetServer.cpuUsage = Math.min(100, targetServer.cpuUsage + 10);
          server.cpuUsage -= 10;
        } else {
          console.log(`❗ No suitable server found for traffic redirection from ${server.ip}`);
        }
      }
    });
  }
  
  // Handle server unavailability (fault tolerance)
  function handleServerFailures() {
    if (servers.length > 0) {
      // Randomly mark a server as unavailable
      const failingServerIndex = Math.floor(Math.random() * servers.length);
      const failingServer = servers[failingServerIndex];
      if (Math.random() < 0.2) { // 20% chance of failure
        console.error(`🚨 Server ${failingServer.ip} is down! Removing from load balancing.`);
        servers.splice(failingServerIndex, 1); // Remove the server
      }
    } else {
      console.log("⚠️ No servers available to fail.");
    }
  }
  
  // Main function to start the load balancer
  function startLoadBalancer() {
    setInterval(() => {
      simulateCPULoadChanges(); // Simulate CPU load changes
      handleServerFailures();   // Handle potential server failures
      balanceLoad();            // Redistribute traffic if necessary
      reportToCentralMonitoring(); // Report server status to the central monitoring system
    }, 5000); // Every 5 seconds
  }
  
  // Start the load balancer
  startLoadBalancer();
  