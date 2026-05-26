#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QTableWidget>
#include <QListWidget>
#include <QTabWidget>
#include "polynomial.h"
#include "Ordered_tab.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Create tab
    void onAddMonomToA();
    void onAddMonomToB();
    void onClearA();
    void onClearB();
    void onSavePolyA();
    void onSavePolyB();
    
    // Operations tab
    void onAddition();
    void onSubtraction();
    void onMultiplication();
    void onDivision();
    void onSaveResultOp();
    void onLoadPolyAOp();
    void onLoadPolyBOp();
    
    // Storage tab
    void onStorageTypeChanged(int index);
    void onPerformStoredOp();
    void onDeleteFromStorage();
    void onRefreshStorage();
    
    // General
    void updateDisplays();

private:
    void setupUI();
    void createCreateTab();
    void createOperationsTab();
    void createStorageTab();
    void setupStylesheet();
    void refreshStorageList();
    std::string getStorageDescription(int index);

    // Data
    Polynomial polyA;
    Polynomial polyB;
    Polynomial polyResult;
    Ordered_tab<std::string, Polynomial> storage;

    // Main tab widget
    QTabWidget* tabWidget;

    // === CREATE TAB ===
    QDoubleSpinBox* coeffSpinBoxA;
    QSpinBox* powerXSpinBoxA;
    QSpinBox* powerYSpinBoxA;
    QSpinBox* powerZSpinBoxA;
    QPushButton* addMonomButtonA;
    QPushButton* clearButtonA;
    QPushButton* savePolyAButton;
    QLineEdit* saveNameInputA;
    QTextEdit* displayA;

    QDoubleSpinBox* coeffSpinBoxB;
    QSpinBox* powerXSpinBoxB;
    QSpinBox* powerYSpinBoxB;
    QSpinBox* powerZSpinBoxB;
    QPushButton* addMonomButtonB;
    QPushButton* clearButtonB;
    QPushButton* savePolyBButton;
    QLineEdit* saveNameInputB;
    QTextEdit* displayB;

    // === OPERATIONS TAB ===
    QTextEdit* displayOpA;
    QTextEdit* displayOpB;
    QTextEdit* displayOpResult;
    
    QComboBox* loadOpACombo;
    QComboBox* loadOpBCombo;
    QPushButton* loadOpAButton;
    QPushButton* loadOpBButton;
    
    QPushButton* opAddButton;
    QPushButton* opSubButton;
    QPushButton* opMulButton;
    QPushButton* opDivButton;
    
    QLineEdit* opResultNameInput;
    QPushButton* opSaveButton;

    // === STORAGE TAB ===
    QComboBox* storageTypeCombo;
    QLabel* storageInfoLabel;
    
    QListWidget* storageListWidget;
    
    QComboBox* storedOp1Combo;
    QComboBox* storedOp2Combo;
    QComboBox* storedOpTypeCombo;
    QLineEdit* storedOpResultInput;
    QPushButton* storedOpExecuteButton;
    
    QPushButton* deleteStorageButton;
    QPushButton* refreshStorageButton;
};

#endif // MAINWINDOW_H
