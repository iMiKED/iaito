#include "omnibar.h"
#include "mainwindow.h"

#include <QStringListModel>
#include <QCompleter>
#include <QShortcut>

Omnibar::Omnibar(MainWindow *main, QWidget *parent) :
    QLineEdit(parent)
{
    // Radare core found in:
    this->main = main;

    // QLineEdit basic features
    this->setMinimumHeight(16);
    this->setMaximumHeight(16);
    this->setFrame(false);
    //this->setPlaceholderText("Go to address...");
    this->setStyleSheet("border-radius: 5px;");
    this->setTextMargins(10, 0, 0, 0);
    this->setClearButtonEnabled(true);

    this->commands << ": Comments toggle"
                   << ": Dashboard toggle"
                   << ": Flags toggle"
                   << ": Functions toggle"
                   << ": Imports toggle"
                   << ": Notepad toggle"
                   << ": Relocs toggle"
                   << ": Run Script"
                   << ": Sections toggle"
                   << ": Strings toggle"
                   << ": Symbols toggle"
                   << ": Tabs up/down"
                   << ": Theme switch"
                   << ": Lock/Unlock interface"
                   << ": Web server start/stop";

    connect(this, SIGNAL(returnPressed()), this, SLOT(on_gotoEntry_returnPressed()));

    // Esc clears omnibar
    QShortcut* clear_shortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(clear_shortcut, SIGNAL(activated()), this, SLOT(clearContents()));
}

void Omnibar::setupCompleter() {
    // Set gotoEntry completer for jump history
    QStringList flagsList = this->getFlags();
    QCompleter *completer = new QCompleter(flagsList, this);
    completer->setMaxVisibleItems(20);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);

    QStringListModel *completerModel = (QStringListModel*)(completer->model());
    completerModel->setStringList(completerModel->stringList() << this->commands);

    this->setCompleter(completer);
}

void Omnibar::restoreCompleter()
{
    QCompleter *completer = this->completer();
    completer->setFilterMode(Qt::MatchContains);
}

void Omnibar::showCommands() {
    this->setFocus();
    this->setText(": ");

    QCompleter *completer = this->completer();
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchStartsWith);

    completer->setMaxVisibleItems(20);

    completer->setCompletionPrefix(": ");
    completer->complete();
}

void Omnibar::clearContents() {
    this->setText("");
    this->clearFocus();
}

void Omnibar::on_gotoEntry_returnPressed()
{
    QString str = this->text();
    if (str.length()>0)
        if (str.contains(": ")) {
            if (str.contains("Lock")){
                this->main->on_actionLock_triggered();
            } else if (str.contains("Functions")) {
                this->main->on_actionFunctions_triggered();
            } else if (str.contains("Flags")) {
                this->main->on_actionFlags_triggered();
            } else if (str.contains("Sections")) {
                this->main->on_actionSections_triggered();
            } else if (str.contains("Strings")) {
                this->main->on_actionStrings_triggered();
            } else if (str.contains("Imports")) {
                this->main->on_actionImports_triggered();
            } else if (str.contains("Symbols")) {
                this->main->on_actionSymbols_triggered();
            } else if (str.contains("Relocs")) {
                this->main->on_actionReloc_triggered();
            } else if (str.contains("Comments")) {
                this->main->on_actionComents_triggered();
            } else if (str.contains("Notepad")) {
                this->main->on_actionNotepad_triggered();
            } else if (str.contains("Dashboard")) {
                this->main->on_actionDashboard_triggered();
            } else if (str.contains("Theme")) {
                this->main->sideBar->themesButtonToggle();
            } else if (str.contains("Script")) {
                this->main->on_actionRun_Script_triggered();
            } else if (str.contains("Tabs")) {
                this->main->on_actionTabs_triggered();
            }
        } else {
            //this->main->seek(this->main->core->cmd("?v " + this->text()), this->text());
            QString off = this->main->core->cmd("afo " + this->text());
            this->main->seek(off.trimmed(), this->text());
        }
    // check which tab is open? update all tabs? hex, graph?
    //refreshMem( this->gotoEntry->text() );
    this->setText("");
    this->clearFocus();
    this->restoreCompleter();
}

void Omnibar::fillFlags(QString flag) {
    this->flags << flag;
}

void Omnibar::clearFlags() {
    this->flags.clear();
}

QStringList Omnibar::getFlags() {
    return this->flags;
}
