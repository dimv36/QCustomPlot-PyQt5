/** PyQt5 binding for QCustomPlot v2.0.0
 *  
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *  
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */


class QCPMarginGroup : public QObject
{
%TypeHeaderCode
#include <QCustomPlot/src/layout.h>
%End

public:
  explicit QCPMarginGroup(QCustomPlot *parentPlot /TransferThis/);
  virtual ~QCPMarginGroup();
  
  // non-virtual methods:
  QList<QCPLayoutElement*> elements(QCP::MarginSide side) const;
  bool isEmpty() const;
  void clear();
};


class QCPLayoutElement : public QCPLayerable
{
%TypeHeaderCode
#include <QCustomPlot/src/layout.h>
%End

public:
  enum UpdatePhase { upPreparation ///< Phase used for any type of preparation that needs to be done before margin calculation and layout
                     ,upMargins    ///< Phase in which the margins are calculated and set
                     ,upLayout     ///< Final phase in which the layout system places the rects of the elements
                   };
  
  enum SizeConstraintRect { scrInnerRect ///< Minimum/Maximum size constraints apply to inner rect
                            , scrOuterRect ///< Minimum/Maximum size constraints apply to outer rect, thus include layout element margins
                          };

  explicit QCPLayoutElement(QCustomPlot *parentPlot /TransferThis/ = 0);
  virtual ~QCPLayoutElement();
  
  // getters:
  QCPLayout *layout() const;
  QRect rect() const;
  QRect outerRect() const;
  QMargins margins() const;
  QMargins minimumMargins() const;
  QCP::MarginSides autoMargins() const;
  QSize minimumSize() const;
  QSize maximumSize() const;
  SizeConstraintRect sizeConstraintRect() const;
  QCPMarginGroup *marginGroup(QCP::MarginSide side) const;
  QHash<QCP::MarginSide, QCPMarginGroup*> marginGroups() const;
  
  // setters:
  void setOuterRect(const QRect &rect);
  void setMargins(const QMargins &margins);
  void setMinimumMargins(const QMargins &margins);
  void setAutoMargins(QCP::MarginSides sides);
  void setMinimumSize(const QSize &size);
  void setMinimumSize(int width, int height);
  void setMaximumSize(const QSize &size);
  void setMaximumSize(int width, int height);
  void setSizeConstraintRect(SizeConstraintRect constraintRect);
  void setMarginGroup(QCP::MarginSides sides, QCPMarginGroup *group);
  
  // introduced virtual methods:
  virtual void update(UpdatePhase phase);
  virtual QSize minimumOuterSizeHint() const;
  virtual QSize maximumOuterSizeHint() const;
  virtual QList<QCPLayoutElement*> elements(bool recursive) const;
  
  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
};


class QCPLayout : public QCPLayoutElement
{
%TypeHeaderCode
#include <QCustomPlot/src/layout.h>
%End

public:
  explicit QCPLayout();
  
  // reimplemented virtual methods:
  virtual void update(UpdatePhase phase);
  virtual QList<QCPLayoutElement*> elements(bool recursive) const;
  
  // introduced virtual methods:
  virtual int elementCount() const = 0;
  virtual QCPLayoutElement* elementAt(int index) const = 0;
  virtual QCPLayoutElement* takeAt(int index) = 0;
  virtual bool take(QCPLayoutElement* element) = 0;
  virtual void simplify();
  
  // non-virtual methods:
  bool removeAt(int index);
  bool remove(QCPLayoutElement* element);
  void clear();
};


class QCPLayoutGrid : public QCPLayout
{
%TypeHeaderCode
#include <QCustomPlot/src/layout.h>
%End
 
public:
  enum FillOrder { foRowsFirst    ///< Rows are filled first, and a new element is wrapped to the next column if the row count would exceed \ref setWrap.
                  ,foColumnsFirst ///< Columns are filled first, and a new element is wrapped to the next row if the column count would exceed \ref setWrap.
                };
  
  explicit QCPLayoutGrid();
  virtual ~QCPLayoutGrid();
  
  // getters:
  int rowCount() const;
  int columnCount() const;
  QList<double> columnStretchFactors() const;
  QList<double> rowStretchFactors() const;
  int columnSpacing() const;
  int rowSpacing() const;
  int wrap() const;
  FillOrder fillOrder() const;
  
  // setters:
  void setColumnStretchFactor(int column, double factor);
  void setColumnStretchFactors(const QList<double> &factors);
  void setRowStretchFactor(int row, double factor);
  void setRowStretchFactors(const QList<double> &factors);
  void setColumnSpacing(int pixels);
  void setRowSpacing(int pixels);
  void setWrap(int count);
  void setFillOrder(FillOrder order, bool rearrange=true);
  
  // reimplemented virtual methods:
  virtual void updateLayout() ;
  virtual int elementCount() const;
  virtual QCPLayoutElement* elementAt(int index) const;
  virtual QCPLayoutElement* takeAt(int index);
  virtual bool take(QCPLayoutElement* element);
  virtual QList<QCPLayoutElement*> elements(bool recursive) const;
  virtual void simplify();
  virtual QSize minimumOuterSizeHint() const;
  virtual QSize maximumOuterSizeHint() const;
  
  // non-virtual methods:
  QCPLayoutElement *element(int row, int column) const;
  bool addElement(int row, int column, QCPLayoutElement *element);
  bool addElement(QCPLayoutElement *element);
  bool hasElement(int row, int column);
  void expandTo(int newRowCount, int newColumnCount);
  void insertRow(int newIndex);
  void insertColumn(int newIndex);
  int rowColToIndex(int row, int column) const;
  void indexToRowCol(int index, int &row, int &column) const;
};


class QCPLayoutInset : public QCPLayout
{
%TypeHeaderCode
#include <QCustomPlot/src/layout.h>
%End

public:
  enum InsetPlacement { ipFree            ///< The element may be positioned/sized arbitrarily, see \ref setInsetRect
                        ,ipBorderAligned  ///< The element is aligned to one of the layout sides, see \ref setInsetAlignment
                      };
  
  explicit QCPLayoutInset();
  virtual ~QCPLayoutInset();
  
  // getters:
  InsetPlacement insetPlacement(int index) const;
  Qt::Alignment insetAlignment(int index) const;
  QRectF insetRect(int index) const;
  
  // setters:
  void setInsetPlacement(int index, InsetPlacement placement);
  void setInsetAlignment(int index, Qt::Alignment alignment);
  void setInsetRect(int index, const QRectF &rect);
  
  // reimplemented virtual methods:
  virtual void updateLayout();
  virtual int elementCount() const;
  virtual QCPLayoutElement* elementAt(int index) const;
  virtual QCPLayoutElement* takeAt(int index);
  virtual bool take(QCPLayoutElement* element);
  virtual void simplify();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  // non-virtual methods:
  void addElement(QCPLayoutElement *element, Qt::Alignment alignment);
  void addElement(QCPLayoutElement *element, const QRectF &rect);
};
