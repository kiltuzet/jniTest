package org.example.da;

import android.util.Log;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Random;

public class HelloEpta {

    private static final Random random = new Random();
    private static final SimpleDateFormat timeFormat = new SimpleDateFormat("HH:mm:ss", Locale.getDefault());

    private static void logLongMessage(String tag, String message) {
        int maxLogSize = 3000;
        for (int i = 0; i <= message.length() / maxLogSize; i++) {
            int start = i * maxLogSize;
            int end = Math.min((i + 1) * maxLogSize, message.length());
            Log.i(tag, "Part " + (i + 1) + ": " + message.substring(start, end));
        }
    }

    // Метод для генерации данных разного размера
    public static String generateData(int sizeBytes) {
        long startTime = System.currentTimeMillis();

        if (sizeBytes <= 0) {
            return "Invalid size";
        }

        // Генерируем данные нужного размера
        StringBuilder data = new StringBuilder();
        data.append("=== DATA BLOCK ===\n");
        data.append("Size: ").append(sizeBytes).append(" bytes\n");
        data.append("Generated at: ").append(timeFormat.format(new Date())).append("\n");
        data.append("Type: Performance test data\n\n");

        // Основное содержимое
        int contentSize = sizeBytes - data.length() - 100; // Оставляем место для заголовка
        if (contentSize > 0) {
            // Используем повторяющийся паттерн для эффективности
            String pattern = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz";
            int patternLength = pattern.length();

            for (int i = 0; i < contentSize; i++) {
                data.append(pattern.charAt(i % patternLength));

                // Добавляем переносы строки для читаемости
                if (i % 80 == 79) {
                    data.append('\n');
                }
            }
        }

        data.append("\n=== END OF DATA BLOCK ===");

        long generationTime = System.currentTimeMillis() - startTime;
        Log.i("DataGeneration", "Generated " + sizeBytes + " bytes in " + generationTime + " ms");

        return data.toString();
    }

    // Метод для тестирования передачи данных
    public static String testDataTransfer(int sizeBytes) {
        long startTime = System.currentTimeMillis();

        String data = generateData(sizeBytes);
        long transferTime = System.currentTimeMillis() - startTime;

        Log.i("DataTransfer", "Transfer test: " + sizeBytes + " bytes, time: " + transferTime + " ms");

        return "SUCCESS: Transferred " + sizeBytes + " bytes in " + transferTime + " ms\n" +
               "Data preview: " + (data.length() > 100 ? data.substring(0, 100) + "..." : data);
    }

    // Метод для поиска максимального размера (бинарный поиск)
    public static String findMaxDataSize(int minSize, int maxSize, int maxTimeMs) {
        Log.i("DataBenchmark", "Searching max data size between " + minSize + " and " + maxSize +
              " bytes with max time " + maxTimeMs + " ms");

        int low = minSize;
        int high = maxSize;
        int bestSize = 0;
        String result = "=== DATA TRANSFER BENCHMARK ===\n";

        for (int i = 0; i < 10; i++) { // Максимум 10 итераций
            if (low > high) break;

            int mid = (low + high) / 2;
            long startTime = System.currentTimeMillis();

            String data = generateData(mid);
            long transferTime = System.currentTimeMillis() - startTime;

            String status = transferTime <= maxTimeMs ? "PASS" : "FAIL";
            result += String.format("Test %d: %d bytes - %d ms - %s\n",
                                   i + 1, mid, transferTime, status);

            Log.i("DataBenchmark", "Iteration " + (i + 1) + ": " + mid + " bytes, " +
                  transferTime + " ms, " + status);

            if (transferTime <= maxTimeMs) {
                bestSize = mid;
                low = mid + 1; // Пробуем больший размер
            } else {
                high = mid - 1; // Пробуем меньший размер
            }
        }

        result += "\n=== RESULT ===\n";
        result += "Maximum data size: " + bestSize + " bytes\n";
        result += "Target latency: " + maxTimeMs + " ms\n";
        result += "Estimated throughput: " +
                 (bestSize > 0 ? String.format("%.2f", bestSize / (maxTimeMs / 1000.0) / 1024 / 1024) : "0") +
                 " MB/s";

        return result;
    }

    public static void hello() {
        Log.i("HelloWorld", "Android JNI HelloEpta class initialized successfully");
    }

    public static String retres(String inputString) {
        // Обработка команд тестирования
        if (inputString.startsWith("/test ")) {
            try {
                int size = Integer.parseInt(inputString.substring(6));
                return testDataTransfer(size);
            } catch (NumberFormatException e) {
                return "ERROR: Invalid size format";
            }
        } else if (inputString.startsWith("/benchmark")) {
            return findMaxDataSize(1024, 10 * 1024 * 1024, 1000); // От 1KB до 10MB
        } else if (inputString.startsWith("/custom_benchmark ")) {
            try {
                String[] parts = inputString.split(" ");
                int minSize = Integer.parseInt(parts[1]);
                int maxSize = Integer.parseInt(parts[2]);
                int maxTime = Integer.parseInt(parts[3]);
                return findMaxDataSize(minSize, maxSize, maxTime);
            } catch (Exception e) {
                return "ERROR: Invalid benchmark parameters. Use: /custom_benchmark minSize maxSize maxTimeMs";
            }
        }

        return "Available commands:\n" +
               "/test [size] - Test data transfer with specific size\n" +
               "/benchmark - Find max data size with 1000ms limit\n" +
               "/custom_benchmark minSize maxSize maxTimeMs - Custom benchmark";
    }
}
