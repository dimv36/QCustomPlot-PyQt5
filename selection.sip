/** PyQt5 binding for QCustomPlot v2.0.0
 *
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */

class QCPDataRange
{
%TypeHeaderCode
#include <QCustomPlot/src/selection.h>
%End
public:
  QCPDataRange();
  QCPDataRange(int begin, int end);

  bool operator==(const QCPDataRange& other) const;
  bool operator!=(const QCPDataRange& other) const;

  // getters:
  int begin() const;
  int end() const;
  int size() const;
  int length() const;

  // setters:
  void setBegin(int begin);
  void setEnd(int end) ;

  // non-property methods:
  bool isValid() const;
  bool isEmpty() const;
  QCPDataRange bounded(const QCPDataRange &other) const;
  QCPDataRange expanded(const QCPDataRange &other) const;
  QCPDataRange intersection(const QCPDataRange &other) const;
  QCPDataRange adjusted(int changeBegin, int changeEnd) const;
  bool intersects(const QCPDataRange &other) const;
  bool contains(const QCPDataRange &other) const;
};


class QCPDataSelection
{
%TypeHeaderCode
#include <QCustomPlot/src/plottable.h>
%End
public:
  explicit QCPDataSelection();
  explicit QCPDataSelection(const QCPDataRange &range);

  bool operator==(const QCPDataSelection& other) const;
  bool operator!=(const QCPDataSelection& other) const;
  QCPDataSelection &operator+=(const QCPDataSelection& other);
  QCPDataSelection &operator+=(const QCPDataRange& other);
  QCPDataSelection &operator-=(const QCPDataSelection& other);
  QCPDataSelection &operator-=(const QCPDataRange& other);

  // getters:
  int dataRangeCount() const;
  int dataPointCount() const;
  QCPDataRange dataRange(int index=0) const;
  QList<QCPDataRange> dataRanges() const;
  QCPDataRange span() const;

  // non-property methods:
  void addDataRange(const QCPDataRange &dataRange, bool simplify=true);
  void clear();
  bool isEmpty() const;
  void simplify();
  void enforceType(QCP::SelectionType type);
  bool contains(const QCPDataSelection &other) const;
  QCPDataSelection intersection(const QCPDataRange &other) const;
  QCPDataSelection intersection(const QCPDataSelection &other) const;
  QCPDataSelection inverse(const QCPDataRange &outerRange) const;
};
