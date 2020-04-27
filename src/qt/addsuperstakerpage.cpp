#include "addsuperstakerpage.h"
#include "qt/forms/ui_addsuperstakerpage.h"

#include <miner.h>
#include <wallet/wallet.h>
#include <qt/walletmodel.h>
#include <QMessageBox>

AddSuperStakerPage::AddSuperStakerPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSuperStakerPage),
    m_model(nullptr)
{
    ui->setupUi(this);

    setWindowTitle(tr("Add super staker"));

    connect(ui->lineEditStakerName, &QLineEdit::textChanged, this, &AddSuperStakerPage::on_updateAddStakerButton);
    connect(ui->lineEditStakerAddress, &QComboBox::currentTextChanged, this, &AddSuperStakerPage::on_updateAddStakerButton);
}

AddSuperStakerPage::~AddSuperStakerPage()
{
    delete ui;
}

void AddSuperStakerPage::setModel(WalletModel *_model)
{
    m_model = _model;
    if(m_model)
    {
        ui->lineEditStakerAddress->setWalletModel(m_model);
    }
}

void AddSuperStakerPage::clearAll()
{
    ui->lineEditStakerName->setText("");
    ui->lineEditStakerAddress->setCurrentIndex(-1);
}

void AddSuperStakerPage::accept()
{
    clearAll();
    QDialog::accept();
}

void AddSuperStakerPage::reject()
{
    clearAll();
    QDialog::reject();
}

void AddSuperStakerPage::show()
{
    ui->lineEditStakerName->setFocus();
    QDialog::show();
}

void AddSuperStakerPage::on_cancelButton_clicked()
{
    reject();
}

void AddSuperStakerPage::on_updateAddStakerButton()
{
    bool enabled = true;
    QString stakerName = ui->lineEditStakerName->text().trimmed();
    QString stakerAddress = ui->lineEditStakerAddress->currentText();
    if(stakerName.isEmpty())
    {
        enabled = false;
    }
    if(stakerAddress.isEmpty() || !ui->lineEditStakerAddress->isValidAddress())
    {
        enabled = false;
    }

    ui->addSuperStakerButton->setEnabled(enabled);
}

void AddSuperStakerPage::on_addSuperStakerButton_clicked()
{
    if(m_model)
    {
        bool fSuperStake = gArgs.GetBoolArg("-superstaking", DEFAULT_SUPER_STAKE);
        if(!fSuperStake)
        {
            QMessageBox::information(this, tr("Super staking"), tr("Enable super staking from the option menu in order to start the super staker."));
        }

        QString stakerAddress = ui->lineEditStakerAddress->currentText();
        QString stakerName = ui->lineEditStakerName->text().trimmed();
        interfaces::SuperStakerInfo superStaker;
        superStaker.staker_address = stakerAddress.toStdString();
        superStaker.staker_name = stakerName.toStdString();
        m_model->wallet().addSuperStakerEntry(superStaker);
        accept();
    }
}