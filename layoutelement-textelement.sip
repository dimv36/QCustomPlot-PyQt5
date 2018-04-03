/** PyQt5 binding for QCustomPlot v2.0.0
 *
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */


class QCPTextElement : public QCPLayoutElement /NoDefaultCtors/
{
%TypeHeaderCode
#include <QCustomPlot/src/layoutelements/layoutelement-textelement.h>
%End
public:
  explicit QCPTextElement(QCustomPlot *parentPlot);
  QCPTextElement(QCustomPlot *parentPlot, const QString &text);
  QCPTextElement(QCustomPlot *parentPlot, const QString &text, double pointSize);
  QCPTextElement(QCustomPlot *parentPlot, const QString &text, const QString &fontFamily, double pointSize);
  QCPTextElement(QCustomPlot *parentPlot, const QString &text, const QFont &font);

  // getters:
  QString text() const;
  int textFlags() const;
  QFont font() const;
  QColor textColor() const;
  QFont selectedFont() const;
  QColor selectedTextColor() const;
  bool selectable() const;
  bool selected() const;

  // setters:
  void setText(const QString &text);
  void setTextFlags(int flags);
  void setFont(const QFont &font);
  void setTextColor(const QColor &color);
  void setSelectedFont(const QFont &font);
  void setSelectedTextColor(const QColor &color);
  Q_SLOT void setSelectable(bool selectable);
  Q_SLOT void setSelected(bool selected);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  virtual void mousePressEvent(QMouseEvent *event, const QVariant &details);
  virtual void mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos);
  virtual void mouseDoubleClickEvent(QMouseEvent *event, const QVariant &details);

signals:
  void selectionChanged(bool selected);
  void selectableChanged(bool selectable);
  void clicked(QMouseEvent *event);
  void doubleClicked(QMouseEvent *event);
};
