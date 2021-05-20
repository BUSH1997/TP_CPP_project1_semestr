#include "contactWidget.h"

ContactWidget::ContactWidget(JsonData data, QListWidget* parent) {
    contactWidget = std::make_unique<QWidget>(parent);
    contactLayout = std::make_unique<QHBoxLayout>(contactWidget.get());
    contactWidget->setLayout(contactLayout.get());
    contactLayout->setMargin(0);

    // TODO add avatar
    contactLayout->addWidget(new QLabel("avatar", contactWidget.get()), 1);

    contactRightPanel = std::make_unique<QWidget>(contactWidget.get());
    contactRightPanelLayout = std::make_unique<QVBoxLayout>();
    contactRightPanelLayout->setMargin(5);

    contactRightPanel->setLayout(contactRightPanelLayout.get());
    contactLayout->addWidget(contactRightPanel.get(), 9);

//    ----------------
    contactTopPanel = std::make_unique<QWidget>(contactRightPanel.get());
    contactTopPanelLayout = std::make_unique<QHBoxLayout>(contactTopPanel.get());
    contactTopPanelLayout->setMargin(0);
    contactTopPanel->setLayout(contactTopPanelLayout.get());
    contactRightPanelLayout->addWidget(contactTopPanel.get(), 7);
    contactNameLabel = std::make_unique<QLabel>(QString::fromStdString(data.name),
                                                contactRightPanel.get());
    contactTopPanelLayout->addWidget(contactNameLabel.get());

    auto t = std::time_t(std::stol(data.date));
    char timestr[100];
    std::strftime(timestr, sizeof(timestr), "%F %T", std::localtime(&t));

    timeLabel = std::make_unique<QLabel>(QString::fromStdString(timestr),
                                         contactTopPanel.get());
    contactTopPanelLayout->addWidget(timeLabel.get(), 1);

//------------------
    contactBottomPanel = std::make_unique<QLabel>(contactRightPanel.get());
    contactBottomPanelLayout = std::make_unique<QHBoxLayout>(contactBottomPanel.get());
    contactBottomPanelLayout.get()->setMargin(0);
    contactBottomPanel->setLayout(contactBottomPanelLayout.get());
    contactRightPanelLayout->addWidget(contactBottomPanel.get(), 7);

    prewMessageLabel = std::make_unique<QLabel>(QString::fromStdString(data.text),
                                                contactWidget.get());
    contactBottomPanelLayout->addWidget(prewMessageLabel.get(), 1);
    contactBottomPanelLayout->addWidget(new QLabel("readCheck"));

    item = std::make_unique<QListWidgetItem>(parent);
    item->setSizeHint(contactWidget->sizeHint());
    parent->setItemWidget(item.get(), contactWidget.get());
}


void ContactWidget::update(JsonData data) {
    auto t = std::time_t(std::stol(data.date));
    char timestr[100];
    std::strftime(timestr, sizeof(timestr), "%F %T", std::localtime(&t));
    timeLabel->setText(QString::fromStdString(timestr));

    prewMessageLabel->setText(QString::fromStdString(data.text));
}

