#include "mainwidget.h"

void MainWidget::makeContact(JsonData data) {

    QWidget* contactWidget;
    contactWidget = new QWidget(contactListWidget);

    QHBoxLayout* contactLayout;
    contactLayout = new QHBoxLayout(contactWidget);
    contactWidget->setLayout(contactLayout);
    contactLayout->setMargin(0);

    // TODO add avatar
    contactLayout->addWidget(new QLabel("avatar", contactWidget), 1);

    QWidget* contactRightPanel;
    contactRightPanel = new QWidget(contactWidget);
    QVBoxLayout* contactRightPanelLayout;
    contactRightPanelLayout = new QVBoxLayout;
    contactRightPanelLayout->setMargin(5);

    contactRightPanel->setLayout(contactRightPanelLayout);
    contactLayout->addWidget(contactRightPanel, 9);


//    ----------------

    QWidget* contactTopPanel;
    contactTopPanel = new QWidget(contactRightPanel);
    QHBoxLayout* contactTopPanelLayout;
    contactTopPanelLayout = new QHBoxLayout(contactTopPanel);
    contactTopPanelLayout->setMargin(0);
    contactTopPanel->setLayout(contactTopPanelLayout);
    contactRightPanelLayout->addWidget(contactTopPanel, 7);

    QLabel* contactNameLabel;
    contactNameLabel = new QLabel(QString::fromStdString(data.name), contactWidget);
    contactTopPanelLayout->addWidget(contactNameLabel);


//    std::string timestr = data.date;
    auto t = std::time_t(std::stol(data.date));
    char timestr[100];
    std::strftime(timestr, sizeof(timestr), "%F %T", std::localtime(&t));


    QLabel* timeLabel;
//    timeLabel = new QLabel(QString(timestr), contactTopPanel);
    timeLabel = new QLabel(QString::fromStdString(timestr), contactTopPanel);
    contactTopPanelLayout->addWidget(timeLabel, 1);


//------------------
//    QWidget* contactBottomPanel;
    contactBottomPanel = new QWidget(contactRightPanel);
    QHBoxLayout* contactBottomPanelLayout;
    contactBottomPanelLayout = new QHBoxLayout(contactBottomPanel);
    contactBottomPanelLayout->setMargin(0);
    contactBottomPanel->setLayout(contactBottomPanelLayout);
    contactRightPanelLayout->addWidget(contactBottomPanel, 7);

    QLabel* prewMessageLabel;
    prewMessageLabel = new QLabel(QString::fromStdString(data.text), contactWidget);
    contactBottomPanelLayout->addWidget(prewMessageLabel, 1);

    contactBottomPanelLayout->addWidget(new QLabel("readCheck"));


    QListWidgetItem* item = new QListWidgetItem(contactListWidget);

    item->setSizeHint(contactWidget->sizeHint());

    contactListWidget->setItemWidget(item, contactWidget);

}

void MainWidget::makeContactList() {
    for (auto it = contactList->list.begin(); it != contactList->list.end(); ++it) {
        makeContact(*it);
    }
}
