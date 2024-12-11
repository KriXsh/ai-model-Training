import java.util.*;

public class ArticleManager {
    private String articleText;
    private List<String> pages;
    private List<String> words;
    private int wordsPerLine;
    private int linesPerPage;
    private Map<String, Integer> paymentStructure;

    public ArticleManager(String articleText, Map<String, Object> articleOptions) {
        this.articleText = articleText;
        this.pages = new ArrayList<>();
        this.words = new ArrayList<>();
        this.wordsPerLine = (int) articleOptions.get("wordsPerLine");
        this.linesPerPage = (int) articleOptions.get("linesPerPage");
        this.paymentStructure = (Map<String, Integer>) articleOptions.get("paymentStructure");
    }

    public void splitIntoPages() {
        // Split words by whitespace
        words = Arrays.asList(articleText.trim().split("\\s+"));

        List<String> lines = new ArrayList<>();

        // Group words into lines
        for (int i = 0; i < words.size(); i += wordsPerLine) {
            int end = Math.min(i + wordsPerLine, words.size());
            lines.add(String.join(" ", words.subList(i, end)));
        }

        // Group lines into pages
        for (int i = 0; i < lines.size(); i += linesPerPage) {
            int end = Math.min(i + linesPerPage, lines.size());
            pages.add(String.join("\n", lines.subList(i, end)));
        }
    }

    public int calculatePayment() {
        int totalPages = pages.size();
        int payment = 0;

        for (Map.Entry<String, Integer> entry : paymentStructure.entrySet()) {
            String range = entry.getKey();
            int amount = entry.getValue();

            if ("default".equals(range)) {
                payment = amount;
                continue;
            }

            String[] bounds = range.split("-");
            int lowerBound = Integer.parseInt(bounds[0]);
            int upperBound = Integer.parseInt(bounds[1]);

            if (totalPages >= lowerBound && totalPages <= upperBound) {
                payment = amount;
                break;
            }
        }

        return payment;
    }

    public void displayPages() {
        int payment = calculatePayment();

        System.out.println("Total Pages: " + pages.size());
        System.out.println("Payment Due: $" + payment);

        for (int i = 0; i < pages.size(); i++) {
            System.out.println("\nPage " + (i + 1) + ":\n" + pages.get(i) + "\n");
        }
    }

    public void processArticle() {
        try {
            if (articleText == null || articleText.isBlank()) {
                throw new IllegalArgumentException("Article text cannot be null or empty.");
            }
            splitIntoPages();
            displayPages();
        } catch (Exception e) {
            System.err.println("Error processing article: " + e.getMessage());
        }
    }

    public static Map<String, Object> createArticleMap(int wordsPerLine, int linesPerPage, Map<String, Integer> paymentStructure) {
        Map<String, Object> articleOptions = new HashMap<>();
        articleOptions.put("wordsPerLine", wordsPerLine);
        articleOptions.put("linesPerPage", linesPerPage);
        articleOptions.put("paymentStructure", paymentStructure);

        return articleOptions;
    }

    public static Map<String, Integer> addPaymentStructure(Object... entries) {
        Map<String, Integer> paymentStructure = new HashMap<>();

        if (entries.length % 2 != 0) {
            throw new IllegalArgumentException("Invalid number of arguments; expected pairs of key and value.");
        }

        for (int i = 0; i < entries.length; i += 2) {
            String key = (String) entries[i];
            Integer value = (Integer) entries[i + 1];
            paymentStructure.put(key, value);
        }

        return paymentStructure;
    }

    public static void main(String[] args) {
        String articleText = "This is a sample article text that should be replaced with actual content. ";
        articleText += "It is used for demonstration purposes in splitting text into pages and calculating payments.";

        Map<String, Integer> paymentStructure = addPaymentStructure(
            "1-1", 30,
            "2-2", 50,
            "3-4", 60,
            "default", 100
        );

        Map<String, Object> articleOptions = createArticleMap(12, 20, paymentStructure);
        ArticleManager articleManager = new ArticleManager(articleText, articleOptions);
        articleManager.processArticle();
    }
}
