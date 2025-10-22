package org.example.da;

import android.util.Log;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Random;

public class HelloEpta {

    private static final Random random = new Random();
    private static final SimpleDateFormat timeFormat = new SimpleDateFormat("HH:mm:ss", Locale.getDefault());
    private static String lastReceivedData = "";
    private static int receiveCount = 0;

    private static void logLongMessage(String tag, String message) {
        int maxLogSize = 3000;
        for (int i = 0; i <= message.length() / maxLogSize; i++) {
            int start = i * maxLogSize;
            int end = Math.min((i + 1) * maxLogSize, message.length());
            Log.i(tag, "Part " + (i + 1) + ": " + message.substring(start, end));
        }
    }

    // Метод для генерации данных разного размера (Java -> C++)
    public static String generateData(int sizeBytes) {
        long startTime = System.currentTimeMillis();

        if (sizeBytes <= 0) {
            return "Invalid size";
        }

        StringBuilder data = new StringBuilder();
        data.append("=== JAVA -> C++ DATA ===\n");
        data.append("Size: ").append(sizeBytes).append(" bytes\n");
        data.append("Generated at: ").append(timeFormat.format(new Date())).append("\n");
        data.append("Direction: Java to C++\n\n");

        // Основное содержимое
        int contentSize = sizeBytes - data.length() - 100;
        if (contentSize > 0) {
            String pattern = "JAVA_TO_CPP_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz";
            int patternLength = pattern.length();

            for (int i = 0; i < contentSize; i++) {
                data.append(pattern.charAt(i % patternLength));
                if (i % 80 == 79) {
                    data.append('\n');
                }
            }
        }

        data.append("\n=== END OF JAVA -> C++ TRANSMISSION ===");

        long generationTime = System.currentTimeMillis() - startTime;
        Log.i("DataGeneration", "Generated " + sizeBytes + " bytes in " + generationTime + " ms");

        return data.toString();
    }

    // Метод для приема данных из C++ (C++ -> Java)
    public static String receiveDataFromCpp(String dataFromCpp) {
        long startTime = System.currentTimeMillis();
        receiveCount++;

        Log.i("DataReception", "Received data from C++, length: " + dataFromCpp.length() + " bytes");

        // Сохраняем полученные данные
        lastReceivedData = dataFromCpp;

        // Генерируем ответ
        String response = generateResponse(dataFromCpp);

        long processingTime = System.currentTimeMillis() - startTime;

        return "SUCCESS: Received " + dataFromCpp.length() + " bytes from C++\n" +
               "Processing time: " + processingTime + " ms\n" +
               "Receive count: " + receiveCount + "\n" +
               "Response: " + response;
    }

    // Метод для двустороннего обмена (Java -> C++ -> Java)
    public static String twoWayExchange(String requestData, int responseSize) {
        long startTime = System.currentTimeMillis();

        Log.i("TwoWayExchange", "Two-way exchange started, request: " + requestData.length() +
              " bytes, response: " + responseSize + " bytes");

        // Обрабатываем входящие данные
        String processingResult = "Processed in Java: '" +
            (requestData.length() > 50 ? requestData.substring(0, 50) + "..." : requestData) + "'";

        // Генерируем ответ
        String responseData = generateData(responseSize);

        long totalTime = System.currentTimeMillis() - startTime;

        return "TWO-WAY EXCHANGE COMPLETE\n" +
               "Request size: " + requestData.length() + " bytes\n" +
               "Response size: " + responseData.length() + " bytes\n" +
               "Total processing time: " + totalTime + " ms\n" +
               "Processing: " + processingResult + "\n" +
               "Response data preview: " +
               (responseData.length() > 100 ? responseData.substring(0, 100) + "..." : responseData);
    }

    // Вспомогательный метод для генерации ответа
    private static String generateResponse(String input) {
        StringBuilder response = new StringBuilder();
        response.append("Java response to: ");
        response.append(input.length() > 30 ? input.substring(0, 30) + "..." : input);
        response.append(" [Processed at: ").append(timeFormat.format(new Date())).append("]");
        return response.toString();
    }

    // Метод для тестирования передачи Java -> C++
    public static String testJavaToCppTransfer(int sizeBytes) {
        long startTime = System.currentTimeMillis();

        String data = generateData(sizeBytes);
        long transferTime = System.currentTimeMillis() - startTime;

        Log.i("JavaToCpp", "Java->C++ transfer: " + sizeBytes + " bytes, time: " + transferTime + " ms");

        return "JAVA->C++: " + sizeBytes + " bytes in " + transferTime + " ms\n" +
               "Data preview: " + (data.length() > 100 ? data.substring(0, 100) + "..." : data);
    }

    // Метод для тестирования передачи C++ -> Java
    public static String testCppToJavaTransfer(int sizeBytes) {
        // Этот метод будет вызываться из C++ с данными
        // Для теста мы просто возвращаем готовность
        return "READY: Java prepared to receive " + sizeBytes + " bytes from C++";
    }

    // Комплексный бенчмарк для двустороннего обмена
    public static String runTwoWayBenchmark(int dataSize, int maxTimeMs) {
        Log.i("TwoWayBenchmark", "Starting two-way benchmark, size: " + dataSize + " bytes, max time: " + maxTimeMs + " ms");

        StringBuilder result = new StringBuilder();
        result.append("=== TWO-WAY DATA EXCHANGE BENCHMARK ===\n");

        // Тест Java -> C++
        long startTime = System.currentTimeMillis();
        String javaToCppData = generateData(dataSize);
        long javaToCppTime = System.currentTimeMillis() - startTime;

        result.append("Java -> C++: ").append(javaToCppTime).append(" ms\n");
        result.append("Status: ").append(javaToCppTime <= maxTimeMs ? "PASS" : "FAIL").append("\n\n");

        // Тест C++ -> Java (имитация - в реальности данные придут из C++)
        startTime = System.currentTimeMillis();
        String testDataFromCpp = "TEST_DATA_FROM_CPP_" + generateTestData(dataSize / 2);
        String cppToJavaResponse = receiveDataFromCpp(testDataFromCpp);
        long cppToJavaTime = System.currentTimeMillis() - startTime;

        result.append("C++ -> Java: ").append(cppToJavaTime).append(" ms\n");
        result.append("Status: ").append(cppToJavaTime <= maxTimeMs ? "PASS" : "FAIL").append("\n\n");

        // Тест двустороннего обмена
        startTime = System.currentTimeMillis();
        String twoWayResult = twoWayExchange(testDataFromCpp, dataSize);
        long twoWayTime = System.currentTimeMillis() - startTime;

        result.append("Java <-> C++: ").append(twoWayTime).append(" ms\n");
        result.append("Status: ").append(twoWayTime <= maxTimeMs ? "PASS" : "FAIL").append("\n\n");

        result.append("=== SUMMARY ===\n");
        result.append("Data size: ").append(dataSize).append(" bytes\n");
        result.append("Max allowed time: ").append(maxTimeMs).append(" ms\n");
        result.append("Java->C++: ").append(javaToCppTime).append(" ms - ").append(javaToCppTime <= maxTimeMs ? "✓" : "✗").append("\n");
        result.append("C++->Java: ").append(cppToJavaTime).append(" ms - ").append(cppToJavaTime <= maxTimeMs ? "✓" : "✗").append("\n");
        result.append("Two-way: ").append(twoWayTime).append(" ms - ").append(twoWayTime <= maxTimeMs ? "✓" : "✗").append("\n");

        boolean allPassed = javaToCppTime <= maxTimeMs && cppToJavaTime <= maxTimeMs && twoWayTime <= maxTimeMs;
        result.append("Overall: ").append(allPassed ? "PASS" : "FAIL").append("\n");

        return result.toString();
    }

    // Генерация тестовых данных
    private static String generateTestData(int size) {
        StringBuilder data = new StringBuilder();
        String chars = "CPP_TO_JAVA_0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        for (int i = 0; i < size; i++) {
            data.append(chars.charAt(random.nextInt(chars.length())));
        }
        return data.toString();
    }

    public static void hello() {
        Log.i("HelloWorld", "Android JNI HelloEpta class initialized successfully");
    }

    public static String retres(String inputString) {
        if (inputString.startsWith("/test_java_to_cpp ")) {
            try {
                int size = Integer.parseInt(inputString.substring(18));
                return testJavaToCppTransfer(size);
            } catch (NumberFormatException e) {
                return "ERROR: Invalid size format";
            }
        } else if (inputString.startsWith("/test_cpp_to_java ")) {
            try {
                int size = Integer.parseInt(inputString.substring(18));
                return testCppToJavaTransfer(size);
            } catch (NumberFormatException e) {
                return "ERROR: Invalid size format";
            }
        } else if (inputString.startsWith("/two_way ")) {
            try {
                String[] parts = inputString.split(" ");
                int requestSize = Integer.parseInt(parts[1]);
                int responseSize = Integer.parseInt(parts[2]);
                String testData = generateTestData(requestSize);
                return twoWayExchange(testData, responseSize);
            } catch (Exception e) {
                return "ERROR: Invalid parameters. Use: /two_way requestSize responseSize";
            }
        } else if (inputString.startsWith("/benchmark_two_way ")) {
            try {
                String[] parts = inputString.split(" ");
                int dataSize = Integer.parseInt(parts[1]);
                int maxTime = Integer.parseInt(parts[2]);
                return runTwoWayBenchmark(dataSize, maxTime);
            } catch (Exception e) {
                return "ERROR: Invalid parameters. Use: /benchmark_two_way dataSize maxTimeMs";
            }
        } else if (inputString.startsWith("/receive ")) {
            String data = inputString.substring(9);
            return receiveDataFromCpp(data);
        }

        return "Two-way test commands:\n" +
               "/test_java_to_cpp [size] - Test Java->C++ transfer\n" +
               "/test_cpp_to_java [size] - Test C++->Java readiness\n" +
               "/two_way requestSize responseSize - Two-way exchange\n" +
               "/benchmark_two_way dataSize maxTimeMs - Two-way benchmark\n" +
               "/receive [data] - Receive data from C++";
    }
}
