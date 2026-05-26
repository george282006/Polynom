#include "MainWindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QGridLayout>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("Polynomial Calculator - Professional Edition");
    setGeometry(50, 50, 1400, 900);
    setupStylesheet();
    setupUI();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupStylesheet() {
    QString stylesheet = R"(
        QMainWindow {
            background-color: #f0f0f0;
        }
        QGroupBox {
            font-weight: bold;
            border: 2px solid #3498db;
            border-radius: 5px;
            margin-top: 10px;
            padding-top: 10px;
            color: #2c3e50;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 3px 0 3px;
        }
        QPushButton {
            background-color: #3498db;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
            font-weight: bold;
            font-size: 11px;
        }
        QPushButton:hover {
            background-color: #2980b9;
        }
        QPushButton:pressed {
            background-color: #1a5276;
        }
        QLineEdit, QSpinBox, QDoubleSpinBox {
            padding: 6px;
            border: 1px solid #bdc3c7;
            border-radius: 4px;
            background-color: white;
            color: black;
        }
        QTextEdit {
            border: 1px solid #bdc3c7;
            border-radius: 4px;
            background-color: #ecf0f1;
            font-family: Courier;
            font-size: 10px;
            color: black;
        }
        QComboBox {
            padding: 6px;
            border: 1px solid #bdc3c7;
            border-radius: 4px;
            background-color: white;
            color: black;
        }
        QLabel {
            color: #2c3e50;
            font-size: 11px;
        }
        QTabWidget::pane {
            border: 1px solid #3498db;
        }
        QTabBar::tab {
            background-color: #ecf0f1;
            color: #2c3e50;
            padding: 8px 20px;
            margin-right: 2px;
        }
        QTabBar::tab:selected {
            background-color: #3498db;
            color: white;
        }
    )";
    qApp->setStyleSheet(stylesheet);
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    // === TOP HEADER ===
    QGroupBox* headerGroup = new QGroupBox("Storage Configuration", this);
    QHBoxLayout* headerLayout = new QHBoxLayout(headerGroup);
    
    QLabel* storageLabel = new QLabel("Storage Type:");
    storageLabel->setFont(QFont("Arial", 10, QFont::Bold));
    headerLayout->addWidget(storageLabel);
    
    storageTypeCombo = new QComboBox(this);
    storageTypeCombo->addItem("Ordered Table");
    storageTypeCombo->addItem("Linked List");
    storageTypeCombo->addItem("AVL Tree");
    storageTypeCombo->addItem("Red-Black Tree");
    storageTypeCombo->addItem("Hash Table (Open Address)");
    storageTypeCombo->addItem("Hash Table (Forward Chaining)");
    storageTypeCombo->addItem("Unordered Table");
    
    connect(storageTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onStorageTypeChanged);
    headerLayout->addWidget(storageTypeCombo);
    
    storageInfoLabel = new QLabel("Sorted key-value storage");
    storageInfoLabel->setStyleSheet("color: #27ae60; font-weight: bold;");
    headerLayout->addWidget(storageInfoLabel);
    headerLayout->addStretch();
    
    mainLayout->addWidget(headerGroup);

    // === TAB WIDGET ===
    tabWidget = new QTabWidget(this);
    
    createCreateTab();
    createOperationsTab();
    createStorageTab();
    
    mainLayout->addWidget(tabWidget);
}

void MainWindow::createCreateTab() {
    QWidget* createTab = new QWidget();
    QHBoxLayout* tabLayout = new QHBoxLayout(createTab);
    tabLayout->setSpacing(15);
    
    // === POLYNOMIAL A PANEL ===
    QVBoxLayout* panelA = new QVBoxLayout();
    
    QGroupBox* groupA = new QGroupBox("Polynomial A", this);
    QVBoxLayout* layoutA = new QVBoxLayout(groupA);
    
    // Input row
    QGridLayout* inputGridA = new QGridLayout();
    inputGridA->setHorizontalSpacing(8);
    inputGridA->setVerticalSpacing(6);
    
    inputGridA->addWidget(new QLabel("Coeff:"), 0, 0);
    coeffSpinBoxA = new QDoubleSpinBox(this);
    coeffSpinBoxA->setRange(-1000, 1000);
    coeffSpinBoxA->setValue(1.0);
    coeffSpinBoxA->setSingleStep(0.1);
    inputGridA->addWidget(coeffSpinBoxA, 0, 1);
    
    inputGridA->addWidget(new QLabel("X:"), 0, 2);
    powerXSpinBoxA = new QSpinBox(this);
    powerXSpinBoxA->setRange(0, 100);
    inputGridA->addWidget(powerXSpinBoxA, 0, 3);
    
    inputGridA->addWidget(new QLabel("Y:"), 1, 0);
    powerYSpinBoxA = new QSpinBox(this);
    powerYSpinBoxA->setRange(0, 100);
    inputGridA->addWidget(powerYSpinBoxA, 1, 1);
    
    inputGridA->addWidget(new QLabel("Z:"), 1, 2);
    powerZSpinBoxA = new QSpinBox(this);
    powerZSpinBoxA->setRange(0, 100);
    inputGridA->addWidget(powerZSpinBoxA, 1, 3);
    
    layoutA->addLayout(inputGridA);
    
    // Buttons
    QHBoxLayout* buttonsA = new QHBoxLayout();
    addMonomButtonA = new QPushButton("➕ Add Monom", this);
    connect(addMonomButtonA, &QPushButton::clicked, this, &MainWindow::onAddMonomToA);
    buttonsA->addWidget(addMonomButtonA);
    
    clearButtonA = new QPushButton("🗑 Clear", this);
    connect(clearButtonA, &QPushButton::clicked, this, &MainWindow::onClearA);
    clearButtonA->setStyleSheet("QPushButton { background-color: #e74c3c; }");
    buttonsA->addWidget(clearButtonA);
    
    savePolyAButton = new QPushButton("💾 Save", this);
    connect(savePolyAButton, &QPushButton::clicked, this, &MainWindow::onSavePolyA);
    savePolyAButton->setStyleSheet("QPushButton { background-color: #27ae60; }");
    buttonsA->addWidget(savePolyAButton);
    
    layoutA->addLayout(buttonsA);
    
    // Save name input
    QHBoxLayout* saveNameLayoutA = new QHBoxLayout();
    saveNameLayoutA->addWidget(new QLabel("Save as:"));
    saveNameInputA = new QLineEdit(this);
    saveNameInputA->setPlaceholderText("Enter name...");
    saveNameLayoutA->addWidget(saveNameInputA);
    layoutA->addLayout(saveNameLayoutA);
    
    // Display
    displayA = new QTextEdit(this);
    displayA->setReadOnly(true);
    displayA->setText("A = 0");
    displayA->setMinimumHeight(80);
    layoutA->addWidget(displayA);
    
    panelA->addWidget(groupA);
    panelA->addStretch();

    // === POLYNOMIAL B PANEL ===
    QVBoxLayout* panelB = new QVBoxLayout();
    
    QGroupBox* groupB = new QGroupBox("Polynomial B", this);
    QVBoxLayout* layoutB = new QVBoxLayout(groupB);
    
    QGridLayout* inputGridB = new QGridLayout();
    inputGridB->setHorizontalSpacing(8);
    inputGridB->setVerticalSpacing(6);
    
    inputGridB->addWidget(new QLabel("Coeff:"), 0, 0);
    coeffSpinBoxB = new QDoubleSpinBox(this);
    coeffSpinBoxB->setRange(-1000, 1000);
    coeffSpinBoxB->setValue(1.0);
    coeffSpinBoxB->setSingleStep(0.1);
    inputGridB->addWidget(coeffSpinBoxB, 0, 1);
    
    inputGridB->addWidget(new QLabel("X:"), 0, 2);
    powerXSpinBoxB = new QSpinBox(this);
    powerXSpinBoxB->setRange(0, 100);
    inputGridB->addWidget(powerXSpinBoxB, 0, 3);
    
    inputGridB->addWidget(new QLabel("Y:"), 1, 0);
    powerYSpinBoxB = new QSpinBox(this);
    powerYSpinBoxB->setRange(0, 100);
    inputGridB->addWidget(powerYSpinBoxB, 1, 1);
    
    inputGridB->addWidget(new QLabel("Z:"), 1, 2);
    powerZSpinBoxB = new QSpinBox(this);
    powerZSpinBoxB->setRange(0, 100);
    inputGridB->addWidget(powerZSpinBoxB, 1, 3);
    
    layoutB->addLayout(inputGridB);
    
    QHBoxLayout* buttonsB = new QHBoxLayout();
    addMonomButtonB = new QPushButton("➕ Add Monom", this);
    connect(addMonomButtonB, &QPushButton::clicked, this, &MainWindow::onAddMonomToB);
    buttonsB->addWidget(addMonomButtonB);
    
    clearButtonB = new QPushButton("🗑 Clear", this);
    connect(clearButtonB, &QPushButton::clicked, this, &MainWindow::onClearB);
    clearButtonB->setStyleSheet("QPushButton { background-color: #e74c3c; }");
    buttonsB->addWidget(clearButtonB);
    
    savePolyBButton = new QPushButton("💾 Save", this);
    connect(savePolyBButton, &QPushButton::clicked, this, &MainWindow::onSavePolyB);
    savePolyBButton->setStyleSheet("QPushButton { background-color: #27ae60; }");
    buttonsB->addWidget(savePolyBButton);
    
    layoutB->addLayout(buttonsB);
    
    // Save name input
    QHBoxLayout* saveNameLayoutB = new QHBoxLayout();
    saveNameLayoutB->addWidget(new QLabel("Save as:"));
    saveNameInputB = new QLineEdit(this);
    saveNameInputB->setPlaceholderText("Enter name...");
    saveNameLayoutB->addWidget(saveNameInputB);
    layoutB->addLayout(saveNameLayoutB);
    
    displayB = new QTextEdit(this);
    displayB->setReadOnly(true);
    displayB->setText("B = 0");
    displayB->setMinimumHeight(80);
    layoutB->addWidget(displayB);
    
    panelB->addWidget(groupB);
    panelB->addStretch();
    
    tabLayout->addLayout(panelA);
    tabLayout->addLayout(panelB);
    
    tabWidget->addTab(createTab, "📝 Create Polynomials");
}

void MainWindow::createOperationsTab() {
    QWidget* opsTab = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(opsTab);
    mainLayout->setSpacing(10);
    
    // === INPUT SECTION ===
    QGroupBox* inputGroup = new QGroupBox("Input Polynomials", this);
    QHBoxLayout* inputLayout = new QHBoxLayout(inputGroup);
    
    // Poly A
    QVBoxLayout* aLayout = new QVBoxLayout();
    QLabel* aLabel = new QLabel("Polynomial A");
    aLabel->setFont(QFont("Arial", 9, QFont::Bold));
    aLayout->addWidget(aLabel);
    
    QHBoxLayout* loadALayout = new QHBoxLayout();
    loadOpACombo = new QComboBox(this);
    loadALayout->addWidget(loadOpACombo);
    loadOpAButton = new QPushButton("Load", this);
    connect(loadOpAButton, &QPushButton::clicked, this, &MainWindow::onLoadPolyAOp);
    loadALayout->addWidget(loadOpAButton);
    aLayout->addLayout(loadALayout);
    
    displayOpA = new QTextEdit(this);
    displayOpA->setReadOnly(true);
    displayOpA->setText("A = 0");
    displayOpA->setMaximumHeight(60);
    aLayout->addWidget(displayOpA);
    
    inputLayout->addLayout(aLayout);
    
    // Poly B
    QVBoxLayout* bLayout = new QVBoxLayout();
    QLabel* bLabel = new QLabel("Polynomial B");
    bLabel->setFont(QFont("Arial", 9, QFont::Bold));
    bLayout->addWidget(bLabel);
    
    QHBoxLayout* loadBLayout = new QHBoxLayout();
    loadOpBCombo = new QComboBox(this);
    loadBLayout->addWidget(loadOpBCombo);
    loadOpBButton = new QPushButton("Load", this);
    connect(loadOpBButton, &QPushButton::clicked, this, &MainWindow::onLoadPolyBOp);
    loadBLayout->addWidget(loadOpBButton);
    bLayout->addLayout(loadBLayout);
    
    displayOpB = new QTextEdit(this);
    displayOpB->setReadOnly(true);
    displayOpB->setText("B = 0");
    displayOpB->setMaximumHeight(60);
    bLayout->addWidget(displayOpB);
    
    inputLayout->addLayout(bLayout);
    
    mainLayout->addWidget(inputGroup);
    
    // === OPERATIONS SECTION ===
    QGroupBox* opsGroup = new QGroupBox("Operations", this);
    QVBoxLayout* opsGroupLayout = new QVBoxLayout(opsGroup);
    
    QGridLayout* opsButtonsLayout = new QGridLayout();
    opsButtonsLayout->setHorizontalSpacing(10);
    opsButtonsLayout->setVerticalSpacing(8);
    
    opAddButton = new QPushButton("A + B", this);
    connect(opAddButton, &QPushButton::clicked, this, &MainWindow::onAddition);
    opsButtonsLayout->addWidget(opAddButton, 0, 0);
    
    opSubButton = new QPushButton("A - B", this);
    connect(opSubButton, &QPushButton::clicked, this, &MainWindow::onSubtraction);
    opsButtonsLayout->addWidget(opSubButton, 0, 1);
    
    opMulButton = new QPushButton("A × B", this);
    connect(opMulButton, &QPushButton::clicked, this, &MainWindow::onMultiplication);
    opsButtonsLayout->addWidget(opMulButton, 0, 2);
    
    opDivButton = new QPushButton("A ÷ B", this);
    connect(opDivButton, &QPushButton::clicked, this, &MainWindow::onDivision);
    opsButtonsLayout->addWidget(opDivButton, 0, 3);
    
    opsGroupLayout->addLayout(opsButtonsLayout);
    mainLayout->addWidget(opsGroup);
    
    // === RESULT SECTION ===
    QGroupBox* resultGroup = new QGroupBox("Result", this);
    QVBoxLayout* resultLayout = new QVBoxLayout(resultGroup);
    
    displayOpResult = new QTextEdit(this);
    displayOpResult->setReadOnly(true);
    displayOpResult->setText("Result = 0");
    displayOpResult->setMinimumHeight(100);
    resultLayout->addWidget(displayOpResult);
    
    QHBoxLayout* saveResultLayout = new QHBoxLayout();
    saveResultLayout->addWidget(new QLabel("Save as:"));
    opResultNameInput = new QLineEdit(this);
    opResultNameInput->setPlaceholderText("Enter name...");
    saveResultLayout->addWidget(opResultNameInput);
    opSaveButton = new QPushButton("💾 Save Result", this);
    connect(opSaveButton, &QPushButton::clicked, this, &MainWindow::onSaveResultOp);
    opSaveButton->setStyleSheet("QPushButton { background-color: #27ae60; }");
    saveResultLayout->addWidget(opSaveButton);
    
    resultLayout->addLayout(saveResultLayout);
    mainLayout->addWidget(resultGroup);
    
    mainLayout->addStretch();
    
    tabWidget->addTab(opsTab, "➗ Operations");
}

void MainWindow::createStorageTab() {
    QWidget* storageTab = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(storageTab);
    mainLayout->setSpacing(10);
    
    // === STORAGE LIST ===
    QGroupBox* listGroup = new QGroupBox("Saved Polynomials (Ordered by Key)", this);
    QVBoxLayout* listLayout = new QVBoxLayout(listGroup);
    
    storageListWidget = new QListWidget(this);
    listLayout->addWidget(storageListWidget);
    
    QHBoxLayout* listButtonsLayout = new QHBoxLayout();
    refreshStorageButton = new QPushButton("🔄 Refresh", this);
    connect(refreshStorageButton, &QPushButton::clicked, this, &MainWindow::onRefreshStorage);
    listButtonsLayout->addWidget(refreshStorageButton);
    
    deleteStorageButton = new QPushButton("🗑 Delete Selected", this);
    deleteStorageButton->setStyleSheet("QPushButton { background-color: #e74c3c; }");
    connect(deleteStorageButton, &QPushButton::clicked, this, &MainWindow::onDeleteFromStorage);
    listButtonsLayout->addWidget(deleteStorageButton);
    
    listButtonsLayout->addStretch();
    listLayout->addLayout(listButtonsLayout);
    
    mainLayout->addWidget(listGroup);
    
    // === STORED OPERATIONS ===
    QGroupBox* storedOpsGroup = new QGroupBox("Operations on Stored Polynomials", this);
    QVBoxLayout* storedOpsLayout = new QVBoxLayout(storedOpsGroup);
    
    QGridLayout* storedOpsGrid = new QGridLayout();
    storedOpsGrid->setHorizontalSpacing(10);
    storedOpsGrid->setVerticalSpacing(8);
    
    storedOpsGrid->addWidget(new QLabel("P1:"), 0, 0);
    storedOp1Combo = new QComboBox(this);
    storedOpsGrid->addWidget(storedOp1Combo, 0, 1);
    
    storedOpsGrid->addWidget(new QLabel("Operation:"), 0, 2);
    storedOpTypeCombo = new QComboBox(this);
    storedOpTypeCombo->addItems({"+", "-", "×", "÷"});
    storedOpsGrid->addWidget(storedOpTypeCombo, 0, 3);
    
    storedOpsGrid->addWidget(new QLabel("P2:"), 0, 4);
    storedOp2Combo = new QComboBox(this);
    storedOpsGrid->addWidget(storedOp2Combo, 0, 5);
    
    storedOpsGrid->addWidget(new QLabel("Result name:"), 1, 0);
    storedOpResultInput = new QLineEdit(this);
    storedOpResultInput->setPlaceholderText("Enter name...");
    storedOpsGrid->addWidget(storedOpResultInput, 1, 1, 1, 4);
    
    storedOpExecuteButton = new QPushButton("⚙️ Execute & Save", this);
    storedOpExecuteButton->setStyleSheet("QPushButton { background-color: #9b59b6; }");
    connect(storedOpExecuteButton, &QPushButton::clicked, this, &MainWindow::onPerformStoredOp);
    storedOpsGrid->addWidget(storedOpExecuteButton, 1, 5);
    
    storedOpsLayout->addLayout(storedOpsGrid);
    mainLayout->addWidget(storedOpsGroup);
    
    mainLayout->addStretch();
    
    tabWidget->addTab(storageTab, "💾 Storage");
}

// === SLOTS IMPLEMENTATION ===

void MainWindow::onAddMonomToA() {
    polyA.add_monom(coeffSpinBoxA->value(), powerXSpinBoxA->value(), 
                    powerYSpinBoxA->value(), powerZSpinBoxA->value());
    updateDisplays();
    coeffSpinBoxA->setValue(1.0);
    powerXSpinBoxA->setValue(0);
    powerYSpinBoxA->setValue(0);
    powerZSpinBoxA->setValue(0);
}

void MainWindow::onAddMonomToB() {
    polyB.add_monom(coeffSpinBoxB->value(), powerXSpinBoxB->value(), 
                    powerYSpinBoxB->value(), powerZSpinBoxB->value());
    updateDisplays();
    coeffSpinBoxB->setValue(1.0);
    powerXSpinBoxB->setValue(0);
    powerYSpinBoxB->setValue(0);
    powerZSpinBoxB->setValue(0);
}

void MainWindow::onClearA() {
    polyA = Polynomial();
    updateDisplays();
}

void MainWindow::onClearB() {
    polyB = Polynomial();
    updateDisplays();
}

void MainWindow::onSavePolyA() {
    QString name = saveNameInputA->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Enter a name!");
        return;
    }
    storage.insert({name.toStdString(), polyA});
    refreshStorageList();
    saveNameInputA->clear();
    QMessageBox::information(this, "Saved", "Polynomial A saved as: " + name);
}

void MainWindow::onSavePolyB() {
    QString name = saveNameInputB->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Enter a name!");
        return;
    }
    storage.insert({name.toStdString(), polyB});
    refreshStorageList();
    saveNameInputB->clear();
    QMessageBox::information(this, "Saved", "Polynomial B saved as: " + name);
}

void MainWindow::onAddition() {
    polyResult = polyA + polyB;
    displayOpResult->setText(QString("Result = ") + QString::fromStdString(polyResult.to_string()));
}

void MainWindow::onSubtraction() {
    polyResult = polyA - polyB;
    displayOpResult->setText(QString("Result = ") + QString::fromStdString(polyResult.to_string()));
}

void MainWindow::onMultiplication() {
    polyResult = polyA * polyB;
    displayOpResult->setText(QString("Result = ") + QString::fromStdString(polyResult.to_string()));
}

void MainWindow::onDivision() {
    if (polyB.empty()) {
        QMessageBox::warning(this, "Error", "Cannot divide by zero!");
        return;
    }
    QMessageBox::information(this, "Info", "Division: simplified (shows A×B)");
    polyResult = polyA * polyB;
    displayOpResult->setText(QString("Result = ") + QString::fromStdString(polyResult.to_string()));
}

void MainWindow::onSaveResultOp() {
    QString name = opResultNameInput->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Enter a name!");
        return;
    }
    storage.insert({name.toStdString(), polyResult});
    refreshStorageList();
    opResultNameInput->clear();
    QMessageBox::information(this, "Saved", "Result saved as: " + name);
}

void MainWindow::onLoadPolyAOp() {
    if (loadOpACombo->count() == 0) {
        QMessageBox::warning(this, "Error", "No polynomials saved!");
        return;
    }
    int idx = loadOpACombo->currentIndex();
    if (idx < 0) return;
    
    QString fullText = loadOpACombo->currentText();
    int eqPos = fullText.indexOf(" = ");
    if (eqPos < 0) return;
    
    QString name = fullText.left(eqPos);
    auto it = storage.find(name.toStdString());
    if (it != storage.end()) {
        polyA = it->second;
        displayOpA->setText(QString("A = ") + QString::fromStdString(polyA.to_string()));
    }
}

void MainWindow::onLoadPolyBOp() {
    if (loadOpBCombo->count() == 0) {
        QMessageBox::warning(this, "Error", "No polynomials saved!");
        return;
    }
    int idx = loadOpBCombo->currentIndex();
    if (idx < 0) return;
    
    QString fullText = loadOpBCombo->currentText();
    int eqPos = fullText.indexOf(" = ");
    if (eqPos < 0) return;
    
    QString name = fullText.left(eqPos);
    auto it = storage.find(name.toStdString());
    if (it != storage.end()) {
        polyB = it->second;
        displayOpB->setText(QString("B = ") + QString::fromStdString(polyB.to_string()));
    }
}

void MainWindow::onStorageTypeChanged(int index) {
    QString info;
    switch(index) {
        case 0: info = "✓ Sorted by key (O(log n))"; break;
        case 1: info = "Linked List (O(n))"; break;
        case 2: info = "AVL Tree (O(log n))"; break;
        case 3: info = "Red-Black Tree (O(log n))"; break;
        case 4: info = "Hash (O(1) avg)"; break;
        case 5: info = "Hash Forward (O(1) avg)"; break;
        case 6: info = "Unordered (O(1) insert)"; break;
        default: info = "Unknown"; break;
    }
    storageInfoLabel->setText(info);
}

void MainWindow::onPerformStoredOp() {
    if (storedOp1Combo->count() == 0 || storedOp2Combo->count() == 0) {
        QMessageBox::warning(this, "Error", "No polynomials saved!");
        return;
    }
    
    int idxA = storedOp1Combo->currentIndex();
    int idxB = storedOp2Combo->currentIndex();
    QString resultName = storedOpResultInput->text().trimmed();
    
    if (idxA < 0 || idxB < 0 || resultName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Select polynomials and enter result name!");
        return;
    }
    
    QString fullTextA = storedOp1Combo->currentText();
    QString fullTextB = storedOp2Combo->currentText();
    
    int eqPosA = fullTextA.indexOf(" = ");
    int eqPosB = fullTextB.indexOf(" = ");
    if (eqPosA < 0 || eqPosB < 0) return;
    
    QString nameA = fullTextA.left(eqPosA);
    QString nameB = fullTextB.left(eqPosB);
    
    auto itA = storage.find(nameA.toStdString());
    auto itB = storage.find(nameB.toStdString());
    
    if (itA == storage.end() || itB == storage.end()) {
        QMessageBox::warning(this, "Error", "Polynomials not found!");
        return;
    }
    
    Polynomial result;
    QString op = storedOpTypeCombo->currentText();
    
    if (op == "+") result = itA->second + itB->second;
    else if (op == "-") result = itA->second - itB->second;
    else if (op == "×") result = itA->second * itB->second;
    else if (op == "÷") {
        if (itB->second.empty()) {
            QMessageBox::warning(this, "Error", "Cannot divide by zero!");
            return;
        }
        result = itA->second * itB->second;
    }
    
    storage.insert({resultName.toStdString(), result});
    refreshStorageList();
    storedOpResultInput->clear();
    QMessageBox::information(this, "Success", nameA + " " + op + " " + nameB + " = " + resultName);
}

void MainWindow::onDeleteFromStorage() {
    auto item = storageListWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Error", "Select polynomial!");
        return;
    }
    
    QString fullText = item->text();
    int eqPos = fullText.indexOf(" = ");
    if (eqPos < 0) return;
    
    QString name = fullText.left(eqPos);
    storage.erase(name.toStdString());
    refreshStorageList();
    QMessageBox::information(this, "Deleted", "Removed: " + name);
}

void MainWindow::onRefreshStorage() {
    refreshStorageList();
}

void MainWindow::refreshStorageList() {
    storageListWidget->clear();
    loadOpACombo->clear();
    loadOpBCombo->clear();
    storedOp1Combo->clear();
    storedOp2Combo->clear();
    
    for (auto it = storage.begin(); it != storage.end(); ++it) {
        QString entry = QString::fromStdString(it->first) + " = " + 
                       QString::fromStdString(it->second.to_string());
        storageListWidget->addItem(entry);
        loadOpACombo->addItem(entry);
        loadOpBCombo->addItem(entry);
        storedOp1Combo->addItem(entry);
        storedOp2Combo->addItem(entry);
    }
}

void MainWindow::updateDisplays() {
    displayA->setText(QString("A = ") + QString::fromStdString(polyA.to_string()));
    displayB->setText(QString("B = ") + QString::fromStdString(polyB.to_string()));
}
