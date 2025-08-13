//header file that encapsulates the highlighter class, which is used to highlight syntax in the script editor tab
//author: Robin van den Eerenbeemd
//version: 1.0
//date: 13 August 2025 updated to NCCA coding standards


#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H


#include <QSyntaxHighlighter>
#include <QRegularExpression>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    // Constructor for the Highlighter class
    Highlighter(QTextDocument *parent = nullptr);

protected:
    // function to highlight user-defined blocks of text; commented out 
    void highlightBlock(const QString &text) override;
    std::list<QString> convertToList(std::string filename, std::string listname);

private:
    // Function to initialize the highlighting rules
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QList<HighlightingRule> highlightingRules;
    // Regular expressions for different syntax elements
    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;


};

#endif // HIGHLIGHTER_H
