#ifndef TEXTEDIT_H
#define TEXTEDIT_H


#include <QList>
#include <QMainWindow>
#include <QtWidgets>

#include "highlightStyle.h"
#include "highlighter.h"


class KexEdit;
class TemplateTextEdit;

class QUILIBSHARED_EXPORT SearchWidget : public QWidget {
    Q_OBJECT
   public:
    SearchWidget(QWidget *parent = nullptr) : QWidget(parent) {
        line = new QLineEdit(this);

        auto newWidth = line->width() + line->height() * 6;
        resize(newWidth, line->height() + hihghShift * 2);

        QRect cr = contentsRect();

        crossButton = newButton(0, ":/icons/cross.png");
        downButton = newButton(1, ":/icons/downArrow.png");
        upButton = newButton(2, ":/icons/upArrow.png");

        connect(crossButton, SIGNAL(clicked()), this->parent(), SLOT(hideSearchWidgetAndSetFocus()));
        connect(downButton, SIGNAL(clicked()), this->parent(), SLOT(findDown()));
        connect(upButton, SIGNAL(clicked()), this->parent(), SLOT(findUp()));


        this->setStyleSheet("QWidget{background-color: #F0F0F0;}");
        line->setStyleSheet("QWidget{background-color: #F0F0F0;border: 1px solid black;}");
        line->setGeometry(cr.left() + buttonShift, cr.top() + hihghShift,
                          newWidth - line->height() * 3 - buttonShift * 2, line->height());

        connect(line, SIGNAL(textChanged(QString)), this->parent(), SLOT(highlitText(QString)));
        // connect(line, SIGNAL (returnPressed()),this->parent(), SLOT (findDown( )));
    }

    ~SearchWidget() {
        delete line;
        delete upButton;
        delete downButton;
        delete crossButton;
    }

   private:
    QPushButton *newButton(int index, QString icon) {
        QPushButton *button = new QPushButton(this);

        auto bstyle = QString(
            "QPushButton:pressed {"
            //"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1,   stop:0 rgba(60, 186, 162,
            // 255), stop:1 rgba(98, 211, 162, 255))"
            "     background-color: #F0F0F0; border: 1px solid black;"
            "}"
            "QPushButton {"
            "     background-color: #F0F0F0;"  // border: 1px solid black;"
            "     border-radius: 0px;"
            "}"
            "QPushButton:disabled {"
            "    background-color: rgb(170, 170, 127)"
            "}");

        QRect cr = contentsRect();
        button->setGeometry(cr.right() - line->height() + buttonShift - index * line->height(),
                            cr.top() + hihghShift + buttonShift, line->height() - buttonShift * 2,
                            line->height() - buttonShift * 2);
        button->setStyleSheet(bstyle);
        button->setIcon(QIcon(icon));
        return button;
    }

   public:
    inline void setFocus() { line->setFocus(); }
    inline QString text() { return line->text(); }
    inline bool hasFocus() { return line->hasFocus() || QWidget::hasFocus(); }

   private:
    const int hihghShift = 2;
    const int buttonShift = 2;

    QLineEdit *line;
    QPushButton *upButton;
    QPushButton *downButton;
    QPushButton *crossButton;
};



class LineNumberArea : public QWidget {
   public:
    LineNumberArea(KexEdit *editor);
    QSize sizeHint() const override;

   protected:
    void paintEvent(QPaintEvent *event) override;

   private:
    KexEdit *editor;
};


class QUILIBSHARED_EXPORT TemplateTextEdit : public QPlainTextEdit {
    Q_OBJECT
   public:
    TemplateTextEdit() : QPlainTextEdit() {
        run_output_highlighter_ = new SearchHighLight(document());
        QRect cr = contentsRect();
        search_widget_ = new SearchWidget(this);
        search_widget_->hide();
        auto rightOffset = verticalScrollBar()->isVisible() ? verticalScrollBar()->width() : 0;

        search_widget_->setGeometry(QRect(cr.right() - search_widget_->width() - rightOffset, cr.top(),
                                          search_widget_->width(), search_widget_->height()));

        setLineWrapMode(NoWrap);
    }
    ~TemplateTextEdit() {
        delete run_output_highlighter_;
        delete search_widget_;
    }

    bool isSelectd() { return hasFocus() || search_widget_->hasFocus(); }

   protected:
    /*
    void contextMenuEvent(QContextMenuEvent *e) override { //example. do nothing
        QMenu *menu = createStandardContextMenu();
        menu->addAction(tr("hide"),this, SLOT(showSearchWidget()), QKeySequence::Find);
        menu->addAction(tr("show"),this, SLOT(showSearchWidget()));
        menu->exec(e->globalPos());
        delete menu;
    }*/

    void keyPressEvent(QKeyEvent *event) override {
        if (!hasFocus()) {
            auto key = event->key();

            if (((key == Qt::Key_Enter) || (key == Qt::Key_Return))) findDown();
            else if (key == Qt::Key_Escape) hideSearchWidgetAndSetFocus();
            else return;
        } else QPlainTextEdit::keyPressEvent(event);
    }


    void resizeEvent(QResizeEvent *e) override {
        QPlainTextEdit::resizeEvent(e);
        QRect cr = contentsRect();

        auto rightOffset = verticalScrollBar()->isVisible() ? verticalScrollBar()->width() : 0;

        search_widget_->setGeometry(QRect(cr.right() - search_widget_->width() - rightOffset, cr.top(),
                                          search_widget_->width(), search_widget_->height()));
    }

   public Q_SLOTS:
    void showSearchWidget() {
        search_widget_->show();
        this->clearFocus();
        search_widget_->setFocus();
        highlitText(search_widget_->text());
    }

    void hideSearchWidget() { search_widget_->hide(); }

    void hideSearchWidgetAndSetFocus() {
        search_widget_->hide();
        setFocus();
        highlitText("");
    }

    virtual void highlitText(const QString &text) { run_output_highlighter_->searchText(text); }



    void findDown() {
        search_widget_->text();
        findString(search_widget_->text(), false, false, false);
    }

    void findUp() {
        search_widget_->text();
        findString(search_widget_->text(), true, false, false);
    }

   private:
    void findString(QString s, bool reverse, bool casesens, bool words) {
        QTextDocument::FindFlags flag;
        if (reverse) flag |= QTextDocument::FindBackward;
        if (casesens) flag |= QTextDocument::FindCaseSensitively;
        if (words) flag |= QTextDocument::FindWholeWords;

        QTextCursor cursor = this->textCursor();
        // here , you save the cursor position
        QTextCursor cursorSaved = cursor;

        if (!find(s, flag)) {
            // nothing is found | jump to start/end
            cursor.movePosition(reverse ? QTextCursor::End : QTextCursor::Start);

            /* following line :
            - the cursor is set at the beginning/end of the document (if search is reverse or not)
            - in the next "find", if the word is found, now you will change the cursor position
            */
            setTextCursor(cursor);

            if (!find(s, flag)) {
                // setTextCursor(cursorSaved);
            }
        }
    }

   protected:
    SearchHighLight *run_output_highlighter_;

   private:
    SearchWidget *search_widget_;
};


class Namespace;

class QUILIBSHARED_EXPORT KexEdit : public TemplateTextEdit {
    Q_OBJECT
   public:
    KexEdit();
    ~KexEdit();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void setHighlightStyle(HighlightStyle *);

   public Q_SLOTS:

    void rehighlight();

    void updateNamespace(QList<QString> &sources);
    void clearNamespace();
    void addToNamespace(const std::string &text);
    virtual void highlitText(const QString &text) override;

   private Q_SLOTS:

    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

   protected:
    // void changeEvent(QEvent *event);
    void resizeEvent(QResizeEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void changeEvent(QEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

   Q_SIGNALS:
    void uncorrect_syntax(QString);
    void correct_syntax();
    void start_rehighlight();

   private:
    bool rehighlight_in_progress_ = false;
    bool spaces_instead_of_tabs_ = true;
    bool is_invalid_syntax_ = false;
    int tab_width_ = 2;

    Highlighter *highlighter__;
    HighlightStyle *highlight_style_;
    QWidget *line_number_area_;
    std::vector<Namespace *> namespase_list_;
};

class QUILIBSHARED_EXPORT RunOutputBrowser : public TemplateTextEdit {
    Q_OBJECT
   public:
    RunOutputBrowser() : TemplateTextEdit() {}
};
/*
a = param()
b = param()

function loop(x, b):

    y = loop[-20] + x - b[20]
    y = y ** 6
    return y
end


return loop(a, b) * 100
 */
#endif
