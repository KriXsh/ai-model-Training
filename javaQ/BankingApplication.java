import java.util.Scanner;
import java.util.HashMap;
import java.util.Map;
import java.util.ArrayList;
import java.util.List;
import java.util.Date;

public class BankingApplication {

    static Scanner scanner = new Scanner(System.in);

    // Map to store users' data: username -> User object
    static Map<String, User> users = new HashMap<>();

    // Main method
    public static void main(String[] args) {
        System.out.println("Welcome to the Banking Application");

        while (true) {
            System.out.println("Please select your role:");
            System.out.println("1. Admin");
            System.out.println("2. User");
            System.out.print("Enter your choice: ");
            int roleChoice = scanner.nextInt();
            scanner.nextLine();  // Consume newline character

            if (roleChoice == 1) {
                adminActions();
            } else if (roleChoice == 2) {
                userActions();
            } else {
                System.out.println("Invalid option. Please try again.");
            }
        }
    }

    // Admin actions
    private static void adminActions() {
        System.out.print("Enter admin password: ");
        String password = scanner.nextLine();  // Use nextLine() to capture the password

        // Hardcoded password for admin access
        if ("admin123".equals(password)) {
            System.out.println("Admin logged in.");
            System.out.println("1. Create User");
            System.out.println("2. Exit");
            System.out.print("Enter your choice: ");
            int adminChoice = scanner.nextInt();
            scanner.nextLine();  // Consume newline

            if (adminChoice == 1) {
                createUser();
            } else {
                System.out.println("Exiting admin interface.");
            }
        } else {
            System.out.println("Invalid password. Access denied.");
        }
    }

    // Create a new user
    private static void createUser() {
        System.out.print("Enter new username: ");
        String username = scanner.nextLine();

        if (users.containsKey(username)) {
            System.out.println("Username already exists. Try a different one.");
            return;
        }

        System.out.print("Enter initial balance: ");
        double balance = scanner.nextDouble();
        scanner.nextLine();  // Consume newline

        // Create a new user with an initial balance
        User newUser = new User(username, balance);
        users.put(username, newUser);

        System.out.println("User created successfully!");
    }

    // User actions
    private static void userActions() {
        System.out.print("Enter your username: ");
        String username = scanner.nextLine();

        if (!users.containsKey(username)) {
            System.out.println("User not found. Please contact admin.");
            return;
        }

        User user = users.get(username);
        System.out.println("Welcome, " + user.getUsername());
        System.out.println("Your current balance: $" + user.getBalance());
        System.out.println("1. Deposit");
        System.out.println("2. Withdraw");
        System.out.println("3. View Transaction History");
        System.out.println("4. Exit");
        System.out.print("Enter your choice: ");
        int userChoice = scanner.nextInt();
        scanner.nextLine();  // Consume newline

        switch (userChoice) {
            case 1:
                deposit(user);
                break;
            case 2:
                withdraw(user);
                break;
            case 3:
                user.showTransactionHistory();  // Show transaction history
                break;
            case 4:
                System.out.println("Exiting user interface.");
                break;
            default:
                System.out.println("Invalid option. Try again.");
        }
    }

    // Deposit amount
    private static void deposit(User user) {
        System.out.print("Enter deposit amount: ");
        double amount = scanner.nextDouble();
        scanner.nextLine();  // Consume newline

        if (amount <= 0) {
            System.out.println("Invalid deposit amount.");
            return;
        }

        user.setBalance(user.getBalance() + amount);
        System.out.println("Deposit successful. New balance: $" + user.getBalance());
        // Log transaction
        user.addTransaction(new Transaction("Deposit", amount));
        user.notifyBalanceChange("Deposit of $" + amount);
    }

    // Withdraw amount
    private static void withdraw(User user) {
        System.out.print("Enter withdrawal amount: ");
        double amount = scanner.nextDouble();
        scanner.nextLine();  // Consume newline

        if (amount <= 0) {
            System.out.println("Invalid withdrawal amount.");
            return;
        }

        if (user.getBalance() < amount) {
            System.out.println("Insufficient balance.");
            return;
        }

        user.setBalance(user.getBalance() - amount);
        System.out.println("Withdrawal successful. New balance: $" + user.getBalance());
        // Log transaction
        user.addTransaction(new Transaction("Withdrawal", amount));
        user.notifyBalanceChange("Withdrawal of $" + amount);
    }

    // User class to represent each user
    static class User {
        private String username;
        private double balance;
        private List<Transaction> transactionHistory;  // List to store user transactions

        public User(String username, double balance) {
            this.username = username;
            this.balance = balance;
            this.transactionHistory = new ArrayList<>();
        }

        public String getUsername() {
            return username;
        }

        public double getBalance() {
            return balance;
        }

        public void setBalance(double balance) {
            this.balance = balance;
        }

        // Add a transaction to the history
        public void addTransaction(Transaction transaction) {
            this.transactionHistory.add(transaction);
        }

        // Show the transaction history
        public void showTransactionHistory() {
            if (transactionHistory.isEmpty()) {
                System.out.println("No transaction history available.");
            } else {
                System.out.println("Transaction History for " + username + ":");
                for (Transaction transaction : transactionHistory) {
                    System.out.println(transaction);
                }
            }
        }

        // Notify the user of balance changes
        public void notifyBalanceChange(String transactionDetails) {
            System.out.println("Notification: " + transactionDetails + ". Your new balance is: $" + balance);
        }
    }

    // Transaction class to represent each transaction
    static class Transaction {
        private String type;
        private double amount;
        private Date timestamp;

        // Constructor to initialize the transaction
        public Transaction(String type, double amount) {
            this.type = type;
            this.amount = amount;
            this.timestamp = new Date();  // Current time when the transaction occurred
        }

        // Getter methods for transaction details
        public String getType() {
            return type;
        }

        public double getAmount() {
            return amount;
        }

        public Date getTimestamp() {
            return timestamp;
        }

        // Override toString() to represent the transaction as a string
        @Override
        public String toString() {
            return type + " of $" + amount + " on " + timestamp;
        }
    }
}
