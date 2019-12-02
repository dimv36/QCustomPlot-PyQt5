/** PyQt5 binding for QCustomPlot v2.0.0
 *
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */

class QCPAbstractLegendItem : public QCPLayoutElement /Abstract/
{
%TypeHeaderCode
#include <QCustomPlot/src/qcp.h>
%End
public:
  explicit QCPAbstractLegendItem(QCPLegend *parent /TransferThis/);

  // getters:
  QCPLegend *parentLegend() const;
  QFont font() const;
  QColor textColor() const;
  QFont selectedFont() const;
  QColor selectedTextColor() const;
  bool selectable() const;
  bool selected() const;

  // setters:
  void setFont(const QFont &font);
  void setTextColor(const QColor &color);
  void setSelectedFont(const QFont &font);
  void setSelectedTextColor(const QColor &color);
  Q_SLOT void setSelectable(bool selectable);
  Q_SLOT void setSelected(bool selected);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

signals:
  void selectionChanged(bool selected);
  void selectableChanged(bool selectable);
};


class QCPPlottableLegendItem : public QCPAbstractLegendItem
{
%TypeHeaderCode
#include <QCustomPlot/src/qcp.h>
%End
public:
  QCPPlottableLegendItem(QCPLegend *parent /TransferThis/, QCPAbstractPlottable *plottable);

  // getters:
  QCPAbstractPlottable *plottable();
};


class QCPLegend : public QCPLayoutGrid
{
%TypeHeaderCode
#include <QCustomPlot/src/qcp.h>
%End
public:
  enum SelectablePart { spNone        = 0x000 ///< <tt>0x000</tt> None
                        ,spLegendBox  = 0x001 ///< <tt>0x001</tt> The legend box (frame)
                        ,spItems      = 0x002 ///< <tt>0x002</tt> Legend items individually (see \ref selectedItems)
                      };
  typedef QFlags<QCPLegend::SelectablePart> SelectableParts;

  explicit QCPLegend();
  virtual ~QCPLegend();

  // getters:
  QPen borderPen() const;
  QBrush brush() const;
  QFont font() const;
  QColor textColor() const;
  QSize iconSize() const;
  int iconTextPadding() const;
  QPen iconBorderPen() const;
  SelectableParts selectableParts() const;
  SelectableParts selectedParts() const;
  QPen selectedBorderPen() const;
  QPen selectedIconBorderPen() const;
  QBrush selectedBrush() const;
  QFont selectedFont() const;
  QColor selectedTextColor() const;

  // setters:
  void setBorderPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setFont(const QFont &font);
  void setTextColor(const QColor &color);
  void setIconSize(const QSize &size);
  void setIconSize(int width, int height);
  void setIconTextPadding(int padding);
  void setIconBorderPen(const QPen &pen);
  Q_SLOT void setSelectableParts(const SelectableParts &selectableParts);
  Q_SLOT void setSelectedParts(const SelectableParts &selectedParts);
  void setSelectedBorderPen(const QPen &pen);
  void setSelectedIconBorderPen(const QPen &pen);
  void setSelectedBrush(const QBrush &brush);
  void setSelectedFont(const QFont &font);
  void setSelectedTextColor(const QColor &color);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

  // non-virtual methods:
  QCPAbstractLegendItem *item(int index) const;
  QCPPlottableLegendItem *itemWithPlottable(const QCPAbstractPlottable *plottable) const;
  int itemCount() const;
  bool hasItem(QCPAbstractLegendItem *item) const;
  bool hasItemWithPlottable(const QCPAbstractPlottable *plottable) const;
  bool addItem(QCPAbstractLegendItem *item);
  bool removeItem(int index);
  bool removeItem(QCPAbstractLegendItem *item);
  void clearItems();
  QList<QCPAbstractLegendItem*> selectedItems() const;

signals:
  void selectionChanged(QCPLegend::SelectableParts parts);
  void selectableChanged(QCPLegend::SelectableParts parts);
};
