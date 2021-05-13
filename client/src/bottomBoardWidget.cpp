#include "bottomBoardWidget.h"

BottomBoardWidget::BottomBoardWidget() {


    //====================================

     bottomBoardWidget = std::make_unique<QWidget>();

//     leftBoardLayout->addWidget(bottomBoardWidget.get(), 2);
//     bottomBoardWidget->hide();


     //    ------------------------------

        bottomBoardLayout = std::make_unique<QHBoxLayout>(bottomBoardWidget.get());
         bottomBoardWidget->setLayout(bottomBoardLayout.get());

         messageTextBrowser = std::make_unique<TextEdit>(bottomBoardWidget.get(), this);
         bottomBoardLayout->addWidget(messageTextBrowser.get(), 10);
         messageTextBrowser->setReadOnly(false);

         buttonsWidget = std::make_unique<QWidget>(bottomBoardWidget.get());
         bottomBoardLayout->addWidget(buttonsWidget.get(), 1);

         buttonsLayout = std::make_unique<QVBoxLayout>(buttonsWidget.get());
         buttonsWidget->setLayout(buttonsLayout.get());

         sendButton = std::make_unique<QPushButton>(buttonsWidget.get());
         buttonsLayout->addWidget(sendButton.get());
         sendButton->setText("&Send");

         connect(sendButton.get(), SIGNAL(clicked()),
                 this, SLOT(sendMessage())
                 );

         recordButton = std::make_unique<QPushButton>(buttonsWidget.get());
         buttonsLayout->addWidget(recordButton.get());
         recordButton->setText("&Record");

         connect(recordButton.get(), SIGNAL(clicked()),
                 this, SLOT(recordAudio())
                 );

         stopButton = std::make_unique<QPushButton>(buttonsWidget.get());
         buttonsLayout->addWidget(stopButton.get());
         stopButton->setText("&Stop");
         stopButton->hide();

         connect(stopButton.get(), SIGNAL(clicked()),
                 this, SLOT(stopRecordAudio())
                 );


         connect(recordButton.get(), SIGNAL(clicked()), recordButton.get(), SLOT(hide()));
         connect(recordButton.get(), SIGNAL(clicked()), stopButton.get(), SLOT(show()));

         connect(stopButton.get(), SIGNAL(clicked()), stopButton.get(), SLOT(hide()));
         connect(stopButton.get(), SIGNAL(clicked()), recordButton.get(), SLOT(show()));


    //     -------------------------------------

    //     ===============================

}

BottomBoardWidget::~BottomBoardWidget() {

}
