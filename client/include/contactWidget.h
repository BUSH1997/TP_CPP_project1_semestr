#ifndef CONTACTWIDGET_H
#define CONTACTWIDGET_H

#include "parser.h"

#include <QWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <memory>


class ContactWidget: public QWidget {
private:
    std::unique_ptr<QWidget> contactWidget;
    std::unique_ptr<QHBoxLayout> contactLayout;
    std::unique_ptr<QWidget> contactRightPanel;
    std::unique_ptr<QVBoxLayout> contactRightPanelLayout;

    std::unique_ptr<QWidget> contactTopPanel;
    std::unique_ptr<QHBoxLayout> contactTopPanelLayout;
    std::unique_ptr<QLabel> contactNameLabel;
    std::unique_ptr<QLabel> timeLabel;

    std::unique_ptr<QWidget> contactBottomPanel;
    std::unique_ptr<QHBoxLayout> contactBottomPanelLayout;
    std::unique_ptr<QLabel> prewMessageLabel;

    std::unique_ptr<QListWidgetItem> item;

public:
    ContactWidget(JsonData data, QListWidget* parent = nullptr);
    ~ContactWidget() = default;

    void update(JsonData data);
};

#endif // CONTACTWIDGET_H
