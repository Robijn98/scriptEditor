#include "highlighter.h"
#include <list>
#include <cstdio>
#include <string>
#include <fstream>

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    //different lists for python highlighting
    std::list<QString> pythonKeywords = {
        QStringLiteral("\\band\\b"), QStringLiteral("\\bassert\\b"), QStringLiteral("\\bbreak\\b"),
        QStringLiteral("\\bclass\\b"), QStringLiteral("\\bcontinue\\b"), QStringLiteral("\\bdef\\b"),
        QStringLiteral("\\bdel\\b"), QStringLiteral("\\belif\\b"), QStringLiteral("\\belse\\b"),
        QStringLiteral("\\bexcept\\b"), QStringLiteral("\\bexec\\b"), QStringLiteral("\\bfinally\\b"),
        QStringLiteral("\\bfor\\b"), QStringLiteral("\\bfrom\\b"), QStringLiteral("\\bglobal\\b"),
        QStringLiteral("\\bif\\b"), QStringLiteral("\\bimport\\b"), QStringLiteral("\\bin\\b"),
        QStringLiteral("\\bis\\b"), QStringLiteral("\\blambda\\b"), QStringLiteral("\\bnot\\b"),
        QStringLiteral("\\bor\\b"), QStringLiteral("\\bpass\\b"), QStringLiteral("\\bprint\\b"),
        QStringLiteral("\\braise\\b"), QStringLiteral("\\breturn\\b"), QStringLiteral("\\btry\\b"),
        QStringLiteral("\\bwhile\\b"), QStringLiteral("\\byield\\b"), QStringLiteral("\\bNone\\b"),
        QStringLiteral("\\bTrue\\b"), QStringLiteral("\\bFalse\\b")
    };


    keywordFormat.setForeground(QColor(197, 134, 192));
    keywordFormat.setFontWeight(QFont::Bold);


    for (const QString &pattern : pythonKeywords) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    //highlight cmds words
    keywordFormat.setForeground(QColor(255, 134, 50));
    keywordFormat.setFontWeight(QFont::Bold);

    std::list<QString> CMDS;
    CMDS = convertToList("C:/Users/robin/OneDrive/Documents/ScriptEditor/syntaxLists/maya_cmds_list.txt", "listName");


    for (const QString &pattern : CMDS) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    //quotation highlighting aka string
    quotationFormat.setForeground(QColor(206, 131, 77));
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    //quotation highlighting aka string
    quotationFormat.setForeground(QColor(206, 131, 77));
    rule.pattern = QRegularExpression(QStringLiteral("\'.*\'"));
    rule.format = quotationFormat;
    highlightingRules.append(rule);


    //single commented out
    singleLineCommentFormat.setForeground(QColor(106, 153, 85));
    rule.pattern = QRegularExpression(QStringLiteral("#[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);


    multiLineCommentFormat.setForeground(Qt::red);

    commentStartExpression = QRegularExpression(QStringLiteral("'''"));
    commentEndExpression = QRegularExpression(QStringLiteral("\'''"));


}

void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : std::as_const(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }



}





//read txt files into lists
std::list<QString> Highlighter::convertToList(std::string filename, std::string listname)
{
    std::list<QString> listOut;


    std::string line;
    std::ifstream file(filename);

    while (std::getline(file, line))
    {
        QString qline = QString::fromStdString(line);
        if (!qline.trimmed().isEmpty())
        {
            QString wrapped = QStringLiteral("\\b%1\\b").arg(qline);
            listOut.push_back(wrapped);
        }
    }

    return listOut;
}









