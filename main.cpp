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
#include <QGroupBox>
#include <QCheckBox>

class TwoWayTransferTester : public QWidget {
    Q_OBJECT

public:
    TwoWayTransferTester(QWidget *parent = nullptr) : QWidget(parent) {
        setupUI();
        setupConnections();
    }

private slots:
    void runQuickTwoWayTest() {
        testTwoWayWithSizes();
    }

    void runJavaToCppTest() {
        QString text = javaToCppInput->text();
        bool ok;
        int size = text.toInt(&ok);

        if (ok && size > 0) {
            testJavaToCppTransfer(size);
        } else {
            addLog("ERROR: Please enter valid size for Java->C++ test");
        }
    }

    void runCppToJavaTest() {
        QString text = cppToJavaInput->text();
        bool ok;
        int size = text.toInt(&ok);

        if (ok && size > 0) {
            testCppToJavaTransfer(size);
        } else {
            addLog("ERROR: Please enter valid size for C++->Java test");
        }
    }

    void runTwoWayExchange() {
        QString text = twoWayInput->text();
        QStringList parts = text.split(" ");
        if (parts.size() == 2) {
            bool ok1, ok2;
            int requestSize = parts[0].toInt(&ok1);
            int responseSize = parts[1].toInt(&ok2);

            if (ok1 && ok2 && requestSize > 0 && responseSize > 0) {
                testTwoWayExchange(requestSize, responseSize);
            } else {
                addLog("ERROR: Invalid parameters for two-way exchange");
            }
        } else {
            addLog("ERROR: Please enter: requestSize responseSize");
        }
    }

    void runTwoWayBenchmark() {
        QString text = benchmarkInput->text();
        QStringList parts = text.split(" ");
        if (parts.size() == 2) {
            bool ok1, ok2;
            int dataSize = parts[0].toInt(&ok1);
            int maxTime = parts[1].toInt(&ok2);

            if (ok1 && ok2 && dataSize > 0 && maxTime > 0) {
                runFullTwoWayBenchmark(dataSize, maxTime);
            } else {
                addLog("ERROR: Invalid parameters for two-way benchmark");
            }
        } else {
            addLog("ERROR: Please enter: dataSize maxTimeMs");
        }
    }

private:
    QTextEdit *logDisplay;
    QLineEdit *javaToCppInput;
    QLineEdit *cppToJavaInput;
    QLineEdit *twoWayInput;
    QLineEdit *benchmarkInput;
    QPushButton *quickTestButton;
    QPushButton *javaToCppButton;
    QPushButton *cppToJavaButton;
    QPushButton *twoWayButton;
    QPushButton *benchmarkButton;
    QProgressBar *progressBar;
    QLabel *statusLabel;

    void setupUI() {
        QVBoxLayout *layout = new QVBoxLayout(this);

        // Статус и прогресс
        statusLabel = new QLabel("Ready to test two-way data transfer performance");
        layout->addWidget(statusLabel);

        progressBar = new QProgressBar();
        progressBar->setVisible(false);
        layout->addWidget(progressBar);

        // Лог
        logDisplay = new QTextEdit();
        logDisplay->setReadOnly(true);
        layout->addWidget(logDisplay);

        // Java -> C++ тест
        QGroupBox *javaToCppGroup = new QGroupBox("Java -> C++ Transfer");
        QHBoxLayout *javaToCppLayout = new QHBoxLayout();
        javaToCppInput = new QLineEdit();
        javaToCppInput->setPlaceholderText("Data size in bytes");
        javaToCppButton = new QPushButton("Test Java->C++");
        javaToCppLayout->addWidget(new QLabel("Size:"));
        javaToCppLayout->addWidget(javaToCppInput);
        javaToCppLayout->addWidget(javaToCppButton);
        javaToCppGroup->setLayout(javaToCppLayout);
        layout->addWidget(javaToCppGroup);

        // C++ -> Java тест
        QGroupBox *cppToJavaGroup = new QGroupBox("C++ -> Java Transfer");
        QHBoxLayout *cppToJavaLayout = new QHBoxLayout();
        cppToJavaInput = new QLineEdit();
        cppToJavaInput->setPlaceholderText("Data size in bytes");
        cppToJavaButton = new QPushButton("Test C++->Java");
        cppToJavaLayout->addWidget(new QLabel("Size:"));
        cppToJavaLayout->addWidget(cppToJavaInput);
        cppToJavaLayout->addWidget(cppToJavaButton);
        cppToJavaGroup->setLayout(cppToJavaLayout);
        layout->addWidget(cppToJavaGroup);

        // Двусторонний обмен
        QGroupBox *twoWayGroup = new QGroupBox("Two-Way Exchange");
        QHBoxLayout *twoWayLayout = new QHBoxLayout();
        twoWayInput = new QLineEdit();
        twoWayInput->setPlaceholderText("requestSize responseSize");
        twoWayButton = new QPushButton("Test Two-Way");
        twoWayLayout->addWidget(new QLabel("Sizes:"));
        twoWayLayout->addWidget(twoWayInput);
        twoWayLayout->addWidget(twoWayButton);
        twoWayGroup->setLayout(twoWayLayout);
        layout->addWidget(twoWayGroup);

        // Бенчмарк
        QGroupBox *benchmarkGroup = new QGroupBox("Two-Way Benchmark");
        QHBoxLayout *benchmarkLayout = new QHBoxLayout();
        benchmarkInput = new QLineEdit();
        benchmarkInput->setPlaceholderText("dataSize maxTimeMs");
        benchmarkButton = new QPushButton("Run Benchmark");
        quickTestButton = new QPushButton("Quick Two-Way Test");
        benchmarkLayout->addWidget(new QLabel("Params:"));
        benchmarkLayout->addWidget(benchmarkInput);
        benchmarkLayout->addWidget(benchmarkButton);
        benchmarkLayout->addWidget(quickTestButton);
        benchmarkGroup->setLayout(benchmarkLayout);
        layout->addWidget(benchmarkGroup);
    }

    void setupConnections() {
        connect(quickTestButton, &QPushButton::clicked, this, &TwoWayTransferTester::runQuickTwoWayTest);
        connect(javaToCppButton, &QPushButton::clicked, this, &TwoWayTransferTester::runJavaToCppTest);
        connect(cppToJavaButton, &QPushButton::clicked, this, &TwoWayTransferTester::runCppToJavaTest);
        connect(twoWayButton, &QPushButton::clicked, this, &TwoWayTransferTester::runTwoWayExchange);
        connect(benchmarkButton, &QPushButton::clicked, this, &TwoWayTransferTester::runTwoWayBenchmark);

        connect(javaToCppInput, &QLineEdit::returnPressed, this, &TwoWayTransferTester::runJavaToCppTest);
        connect(cppToJavaInput, &QLineEdit::returnPressed, this, &TwoWayTransferTester::runCppToJavaTest);
        connect(twoWayInput, &QLineEdit::returnPressed, this, &TwoWayTransferTester::runTwoWayExchange);
        connect(benchmarkInput, &QLineEdit::returnPressed, this, &TwoWayTransferTester::runTwoWayBenchmark);
    }

    void addLog(const QString &message) {
        QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
        logDisplay->append("[" + timestamp + "] " + message);

        QScrollBar *scrollbar = logDisplay->verticalScrollBar();
        scrollbar->setValue(scrollbar->maximum());

        qDebug() << message;
    }

    QString generateTestData(int size) {
        QString data;
        data.reserve(size);
        QString chars = "CPP_TO_JAVA_0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        for (int i = 0; i < size; i++) {
            data.append(chars.at(rand() % chars.length()));
        }

        return data;
    }

    void testJavaToCppTransfer(int size) {
        statusLabel->setText(QString("Testing Java->C++ transfer of %1 bytes...").arg(size));

        QElapsedTimer timer;
        timer.start();

        QJniObject result = QJniObject::callStaticObjectMethod(
            "org/example/da/HelloEpta",
            "testJavaToCppTransfer",
            "(I)Ljava/lang/String;",
            size
            );

        qint64 elapsed = timer.nsecsElapsed();

        if (result.isValid()) {
            QString resultStr = result.toString();
            addLog(QString("Java->C++: %1 bytes, Time: %2 ms")
                       .arg(size)
                       .arg(elapsed / 1000000.0, 0, 'f', 2));
            addLog("Result: " + resultStr);
        } else {
            addLog("ERROR: Java->C++ transfer failed");
        }

        statusLabel->setText("Java->C++ test completed");
    }

    void testCppToJavaTransfer(int size) {
        statusLabel->setText(QString("Testing C++->Java transfer of %1 bytes...").arg(size));

        // Генерируем данные в C++
        QString testData = generateTestData(size);

        QElapsedTimer timer;
        timer.start();

        // Отправляем данные в Java
        QJniObject jniData = QJniObject::fromString(testData);
        QJniObject result = QJniObject::callStaticObjectMethod(
            "org/example/da/HelloEpta",
            "receiveDataFromCpp",
            "(Ljava/lang/String;)Ljava/lang/String;",
            jniData.object<jstring>()
            );

        qint64 elapsed = timer.nsecsElapsed();

        if (result.isValid()) {
            QString resultStr = result.toString();
            addLog(QString("C++->Java: %1 bytes, Time: %2 ms")
                       .arg(size)
                       .arg(elapsed / 1000000.0, 0, 'f', 2));
            addLog("Result: " + resultStr);
        } else {
            addLog("ERROR: C++->Java transfer failed");
        }

        statusLabel->setText("C++->Java test completed");
    }

    void testTwoWayExchange(int requestSize, int responseSize) {
        statusLabel->setText(QString("Testing two-way exchange (%1->%2 bytes)...").arg(requestSize).arg(responseSize));

        // Генерируем запрос в C++
        QString requestData = generateTestData(requestSize);

        QElapsedTimer timer;
        timer.start();

        // Выполняем двусторонний обмен
        QJniObject jniRequest = QJniObject::fromString(requestData);
        QJniObject result = QJniObject::callStaticObjectMethod(
            "org/example/da/HelloEpta",
            "twoWayExchange",
            "(Ljava/lang/String;I)Ljava/lang/String;",
            jniRequest.object<jstring>(),
            responseSize
            );

        qint64 elapsed = timer.nsecsElapsed();

        if (result.isValid()) {
            QString resultStr = result.toString();
            addLog(QString("Two-Way: Request %1 bytes, Response %2 bytes, Time: %3 ms")
                       .arg(requestSize)
                       .arg(responseSize)
                       .arg(elapsed / 1000000.0, 0, 'f', 2));
            addLog("Result: " + resultStr);
        } else {
            addLog("ERROR: Two-way exchange failed");
        }

        statusLabel->setText("Two-way exchange completed");
    }

    void testTwoWayWithSizes() {
        progressBar->setVisible(true);
        progressBar->setRange(0, 5);

        QVector<QPair<int, int>> testSizes = {
            {1024, 1024},       // 1KB в обе стороны
            {10240, 10240},     // 10KB в обе стороны
            {102400, 102400},   // 100KB в обе стороны
            {512000, 512000},   // 500KB в обе стороны
            {1048576, 1048576}  // 1MB в обе стороны
        };

        addLog("=== QUICK TWO-WAY TEST STARTED ===");

        for (int i = 0; i < testSizes.size(); i++) {
            progressBar->setValue(i);
            int requestSize = testSizes[i].first;
            int responseSize = testSizes[i].second;

            statusLabel->setText(QString("Testing %1/%2: %3 <-> %4 bytes")
                                     .arg(i + 1).arg(testSizes.size()).arg(requestSize).arg(responseSize));

            testTwoWayExchange(requestSize, responseSize);
            QApplication::processEvents();
        }

        progressBar->setVisible(false);
        addLog("=== QUICK TWO-WAY TEST COMPLETED ===");
        statusLabel->setText("Quick two-way test completed");
    }

    void runFullTwoWayBenchmark(int dataSize, int maxTime) {
        addLog(QString("=== TWO-WAY BENCHMARK STARTED ==="));
        addLog(QString("Data size: %1 bytes, Max time: %2 ms").arg(dataSize).arg(maxTime));

        QElapsedTimer timer;
        timer.start();

        QJniObject result = QJniObject::callStaticObjectMethod(
            "org/example/da/HelloEpta",
            "runTwoWayBenchmark",
            "(II)Ljava/lang/String;",
            dataSize,
            maxTime
            );

        qint64 totalTime = timer.nsecsElapsed();

        if (result.isValid()) {
            addLog("Benchmark completed in " + QString::number(totalTime / 1000000.0, 'f', 2) + " ms");
            addLog("=== BENCHMARK RESULTS ===");
            addLog(result.toString());
        } else {
            addLog("ERROR: Two-way benchmark failed");
        }

        addLog("=== TWO-WAY BENCHMARK COMPLETED ===");
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "Starting Two-Way QJNI Data Transfer Performance Test";

    // Инициализация Java класса
    QJniObject::callStaticMethod<void>(
        "org/example/da/HelloEpta",
        "hello",
        "()V");

    // Создаем и показываем тестер
    TwoWayTransferTester tester;
    tester.setWindowTitle("Two-Way QJNI Data Transfer Performance Tester");
    tester.resize(800, 700);
    tester.show();

    return a.exec();
}

#include "main.moc"
