
#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QElapsedTimer>
#include <QJniObject>
#include <QTimer>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollBar>
#include <QDateTime>
#include <QProgressBar>
#include <QLabel>
#include <QHBoxLayout>

class DataTransferTester : public QWidget {
    Q_OBJECT

public:
    DataTransferTester(QWidget *parent = nullptr) : QWidget(parent) {
        setupUI();
        setupConnections();
    }

private slots:
    void runQuickTest() {
        testSpecificSizes();
    }

    void runFullBenchmark() {
        runBenchmark();
    }

    void testCustomSize() {
        QString text = sizeInput->text();
        bool ok;
        int size = text.toInt(&ok);

        if (ok && size > 0) {
            testSingleTransfer(size);
        } else {
            addLog("ERROR: Please enter valid size in bytes");
        }
    }
    void runCustomBenchmark1(int minSize, int maxSize, int maxTime) {
        addLog(QString("=== CUSTOM BENCHMARK STARTED ==="));
        addLog(QString("Range: %1 - %2 bytes, Max time: %3 ms").arg(minSize).arg(maxSize).arg(maxTime));

        QElapsedTimer timer;
        timer.start();

        QJniObject result = QJniObject::callStaticObjectMethod(
            "org/example/da/HelloEpta",
            "findMaxDataSize",
            "(III)Ljava/lang/String;",
            minSize,
            maxSize,
            maxTime
            );

        qint64 totalTime = timer.nsecsElapsed();

        if (result.isValid()) {
            addLog("Custom benchmark completed in " + QString::number(totalTime / 1000000.0, 'f', 2) + " ms");
            addLog("=== BENCHMARK RESULTS ===");
            addLog(result.toString());
        } else {
            addLog("ERROR: Custom benchmark failed");
        }

        addLog("=== CUSTOM BENCHMARK COMPLETED ===");
    }

    void runCustomBenchmark() {
        QString text = benchmarkInput->text();
        QStringList parts = text.split(" ");
        if (parts.size() == 3) {
            bool ok1, ok2, ok3;
            int minSize = parts[0].toInt(&ok1);
            int maxSize = parts[1].toInt(&ok2);
            int maxTime = parts[2].toInt(&ok3);

            if (ok1 && ok2 && ok3 && minSize > 0 && maxSize > minSize && maxTime > 0) {
                runCustomBenchmark1(minSize, maxSize, maxTime);
            } else {
                addLog("ERROR: Invalid parameters. Format: minSize maxSize maxTimeMs");
            }
        } else {
            addLog("ERROR: Please enter: minSize maxSize maxTimeMs");
        }
    }

private:
    QTextEdit *logDisplay;
    QLineEdit *sizeInput;
    QLineEdit *benchmarkInput;
    QPushButton *testButton;
    QPushButton *benchmarkButton;
    QPushButton *customBenchmarkButton;
    QPushButton *quickTestButton;
    QProgressBar *progressBar;
    QLabel *statusLabel;

    void setupUI() {
        QVBoxLayout *layout = new QVBoxLayout(this);

        // Статус и прогресс
        statusLabel = new QLabel("Ready to test data transfer performance");
        layout->addWidget(statusLabel);

        progressBar = new QProgressBar();
        progressBar->setVisible(false);
        layout->addWidget(progressBar);

        // Лог
        logDisplay = new QTextEdit();
        logDisplay->setReadOnly(true);
        layout->addWidget(logDisplay);

        // Быстрый тест
        QHBoxLayout *quickTestLayout = new QHBoxLayout();
        quickTestButton = new QPushButton("Quick Test");
        testButton = new QPushButton("Test Custom Size");
        sizeInput = new QLineEdit();
        sizeInput->setPlaceholderText("Size in bytes (e.g., 1024)");

        quickTestLayout->addWidget(quickTestButton);
        quickTestLayout->addWidget(new QLabel("Custom Size:"));
        quickTestLayout->addWidget(sizeInput);
        quickTestLayout->addWidget(testButton);
        layout->addLayout(quickTestLayout);

        // Бенчмарк
        QHBoxLayout *benchmarkLayout = new QHBoxLayout();
        benchmarkButton = new QPushButton("Full Benchmark");
        customBenchmarkButton = new QPushButton("Custom Benchmark");
        benchmarkInput = new QLineEdit();
        benchmarkInput->setPlaceholderText("minSize maxSize maxTimeMs (e.g., 1024 1000000 1000)");

        benchmarkLayout->addWidget(benchmarkButton);
        benchmarkLayout->addWidget(customBenchmarkButton);
        benchmarkLayout->addWidget(benchmarkInput);
        layout->addLayout(benchmarkLayout);
    }

    void setupConnections() {
        // Исправленные connect с правильным синтаксисом
        connect(quickTestButton, &QPushButton::clicked, this, &DataTransferTester::runQuickTest);
        connect(benchmarkButton, &QPushButton::clicked, this, &DataTransferTester::runFullBenchmark);
        connect(testButton, &QPushButton::clicked, this, &DataTransferTester::testCustomSize);
        connect(customBenchmarkButton, &QPushButton::clicked, this, &DataTransferTester::runCustomBenchmark);
        connect(sizeInput, &QLineEdit::returnPressed, this, &DataTransferTester::testCustomSize);
    }

    void addLog(const QString &message) {
        QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
        logDisplay->append("[" + timestamp + "] " + message);

        QScrollBar *scrollbar = logDisplay->verticalScrollBar();
        scrollbar->setValue(scrollbar->maximum());

        qDebug() << message;
    }

    void testSingleTransfer(int size) {
        statusLabel->setText(QString("Testing transfer of %1 bytes...").arg(size));

        QElapsedTimer timer;
        timer.start();

        // Используем правильный тип для int параметра
        QJniObject result = QJniObject::callStaticObjectMethod(
            "org/example/da/HelloEpta",
            "testDataTransfer",
            "(I)Ljava/lang/String;",
            size
            );

        qint64 elapsed = timer.nsecsElapsed();

        if (result.isValid()) {
            QString resultStr = result.toString();
            addLog(QString("Size: %1 bytes, Time: %2 ms")
                       .arg(size)
                       .arg(elapsed / 1000000.0, 0, 'f', 2));
            addLog("Result: " + resultStr);
        } else {
            addLog("ERROR: Failed to transfer data");
        }

        statusLabel->setText("Test completed");
    }

    void testSpecificSizes() {
        progressBar->setVisible(true);
        progressBar->setRange(0, 7);

        // Тестируем различные размеры
        QVector<int> testSizes = {
            1024,           // 1 KB
            10240,          // 10 KB
            102400,         // 100 KB
            512000,         // 500 KB
            1048576,        // 1 MB
            5242880,        // 5 MB
            10485760        // 10 MB
        };

        addLog("=== QUICK TEST STARTED ===");
        addLog("Testing various data sizes...");

        for (int i = 0; i < testSizes.size(); i++) {
            progressBar->setValue(i);
            statusLabel->setText(QString("Testing %1/%2: %3 bytes")
                                     .arg(i + 1).arg(testSizes.size()).arg(testSizes[i]));

            testSingleTransfer(testSizes[i]);
            QApplication::processEvents(); // Обновляем UI
        }

        progressBar->setVisible(false);
        addLog("=== QUICK TEST COMPLETED ===");
        statusLabel->setText("Quick test completed");
    }

    void runBenchmark() {
        addLog("=== FULL BENCHMARK STARTED ===");
        addLog("Finding maximum data size with 1000ms limit...");

        QElapsedTimer timer;
        timer.start();

        QJniObject result = QJniObject::callStaticObjectMethod(
            "org/example/da/HelloEpta",
            "findMaxDataSize",
            "(III)Ljava/lang/String;",
            1024,      // minSize: 1KB
            10485760,  // maxSize: 10MB
            1000       // maxTime: 1000ms
            );

        qint64 totalTime = timer.nsecsElapsed();

        if (result.isValid()) {
            addLog("Benchmark completed in " + QString::number(totalTime / 1000000.0, 'f', 2) + " ms");
            addLog("=== BENCHMARK RESULTS ===");
            addLog(result.toString());
        } else {
            addLog("ERROR: Benchmark failed");
        }

        addLog("=== FULL BENCHMARK COMPLETED ===");
    }


};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "Starting QJNI Data Transfer Performance Test";

    // Быстрый тест производительности при запуске
    QElapsedTimer startupTest;
    startupTest.start();

    // Инициализация Java класса
    QJniObject::callStaticMethod<void>(
        "org/example/da/HelloEpta",
        "hello",
        "()V");

    qint64 initTime = startupTest.nsecsElapsed();
    qDebug() << "Java class initialization:" << initTime / 1000000.0 << "ms";

    // Создаем и показываем тестер
    DataTransferTester tester;
    tester.setWindowTitle("QJNI Data Transfer Performance Tester");
    tester.resize(800, 600);
    tester.show();

    return a.exec();
}

#include "main.moc"
