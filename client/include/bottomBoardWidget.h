#ifndef BOTTOMBOARDWIDGET_H
#define BOTTOMBOARDWIDGET_H

#include <memory>
#include <QTextEdit>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>


class TextEdit: public QTextEdit {
private:
    void* mainWindow = nullptr;
public:
    TextEdit(QWidget* parent = nullptr, void* mainWindow = nullptr) {
        this->mainWindow = mainWindow;
        this->setParent(parent);
    }
protected:
    void keyPressEvent(QKeyEvent *e);
};


class BottomBoardWidget: public QWidget {
private:
    std::unique_ptr<QWidget> bottomBoardWidget;
    std::unique_ptr<QHBoxLayout> bottomBoardLayout;
    std::unique_ptr<TextEdit> messageTextBrowser;
    std::unique_ptr<QWidget> buttonsWidget;
    std::unique_ptr<QVBoxLayout> buttonsLayout;
    std::unique_ptr<QPushButton> sendButton;
    std::unique_ptr<QPushButton> recordButton;
    std::unique_ptr<QPushButton> stopButton;

public:
    BottomBoardWidget();
    ~BottomBoardWidget();
};

#endif // BOTTOMBOARDWIDGET_H

