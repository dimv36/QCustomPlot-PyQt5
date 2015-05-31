/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2015 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 25.04.15                                             **
**          Version: 1.3.1                                                **
****************************************************************************/

/*! \file */

#include "core.h"

#include "painter.h"
#include "layer.h"
#include "axis.h"
#include "layoutelements/layoutelement-axisrect.h"
#include "layoutelements/layoutelement-legend.h"
#include "layoutelements/layoutelement-plottitle.h"
#include "plottable.h"
#include "plottables/plottable-graph.h"
#include "item.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCustomPlot
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCustomPlot
  
  \brief The central class of the library. This is the QWidget which displays the plot and
  interacts with the user.
  
  For tutorials on how to use QCustomPlot, see the website\n
  http://www.qcustomplot.com/
*/

/* start of documentation of inline functions */

/*! \fn QRect QCustomPlot::viewport() const
  
  Returns the viewport rect of this QCustomPlot instance. The viewport is the area the plot is
  drawn in, all mechanisms, e.g. margin caluclation take the viewport to be the outer border of the
  plot. The viewport normally is the rect() of the QCustomPlot widget, i.e. a rect with top left
  (0, 0) and size of the QCustomPlot widget.
  
  Don't confuse the viewport with the axis rect (QCustomPlot::axisRect). An axis rect is typically
  an area enclosed by four axes, where the graphs/plottables are drawn in. The viewport is larger
  and contains also the axes themselves, their tick numbers, their labels, the plot title etc.
  
  Only when saving to a file (see \ref savePng, \ref savePdf etc.) the viewport is temporarily
  modified to allow saving plots with sizes independent of the current widget size.
*/

/*! \fn QCPLayoutGrid *QCustomPlot::plotLayout() const
  
  Returns the top level layout of this QCustomPlot instance. It is a \ref QCPLayoutGrid, initially containing just
  one cell with the main QCPAxisRect inside.
*/

/* end of documentation of inline functions */
/* start of documentation of signals */

/*! \fn void QCustomPlot::mouseDoubleClick(QMouseEvent *event)

  This signal is emitted when the QCustomPlot receives a mouse double click event.
*/

/*! \fn void QCustomPlot::mousePress(QMouseEvent *event)

  This signal is emitted when the QCustomPlot receives a mouse press event.
  
  It is emitted before QCustomPlot handles any other mechanism like range dragging. So a slot
  connected to this signal can still influence the behaviour e.g. with \ref QCPAxisRect::setRangeDrag or \ref
  QCPAxisRect::setRangeDragAxes.
*/

/*! \fn void QCustomPlot::mouseMove(QMouseEvent *event)

  This signal is emitted when the QCustomPlot receives a mouse move event.
  
  It is emitted before QCustomPlot handles any other mechanism like range dragging. So a slot
  connected to this signal can still influence the behaviour e.g. with \ref QCPAxisRect::setRangeDrag or \ref
  QCPAxisRect::setRangeDragAxes.
  
  \warning It is discouraged to change the drag-axes with \ref QCPAxisRect::setRangeDragAxes here,
  because the dragging starting point was saved the moment the mouse was pressed. Thus it only has
  a meaning for the range drag axes that were set at that moment. If you want to change the drag
  axes, consider doing this in the \ref mousePress signal instead.
*/

/*! \fn void QCustomPlot::mouseRelease(QMouseEvent *event)

  This signal is emitted when the QCustomPlot receives a mouse release event.
  
  It is emitted before QCustomPlot handles any other mechanisms like object selection. So a
  slot connected to this signal can still influence the behaviour e.g. with \ref setInteractions or
  \ref QCPAbstractPlottable::setSelectable.
*/

/*! \fn void QCustomPlot::mouseWheel(QMouseEvent *event)

  This signal is emitted when the QCustomPlot receives a mouse wheel event.
  
  It is emitted before QCustomPlot handles any other mechanisms like range zooming. So a slot
  connected to this signal can still influence the behaviour e.g. with \ref QCPAxisRect::setRangeZoom, \ref
  QCPAxisRect::setRangeZoomAxes or \ref QCPAxisRect::setRangeZoomFactor.
*/

/*! \fn void QCustomPlot::plottableClick(QCPAbstractPlottable *plottable, QMouseEvent *event)
  
  This signal is emitted when a plottable is clicked.

  \a event is the mouse event that caused the click and \a plottable is the plottable that received
  the click.
  
  \see plottableDoubleClick
*/

/*! \fn void QCustomPlot::plottableDoubleClick(QCPAbstractPlottable *plottable, QMouseEvent *event)
  
  This signal is emitted when a plottable is double clicked.
  
  \a event is the mouse event that caused the click and \a plottable is the plottable that received
  the click.
  
  \see plottableClick
*/

/*! \fn void QCustomPlot::itemClick(QCPAbstractItem *item, QMouseEvent *event)
  
  This signal is emitted when an item is clicked.

  \a event is the mouse event that caused the click and \a item is the item that received the
  click.
  
  \see itemDoubleClick
*/

/*! \fn void QCustomPlot::itemDoubleClick(QCPAbstractItem *item, QMouseEvent *event)
  
  This signal is emitted when an item is double clicked.
  
  \a event is the mouse event that caused the click and \a item is the item that received the
  click.
  
  \see itemClick
*/

/*! \fn void QCustomPlot::axisClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event)
  
  This signal is emitted when an axis is clicked.
  
  \a event is the mouse event that caused the click, \a axis is the axis that received the click and
  \a part indicates the part of the axis that was clicked.
  
  \see axisDoubleClick
*/

/*! \fn void QCustomPlot::axisDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part, QMouseEvent *event)

  This signal is emitted when an axis is double clicked.
  
  \a event is the mouse event that caused the click, \a axis is the axis that received the click and
  \a part indicates the part of the axis that was clicked.
  
  \see axisClick
*/

/*! \fn void QCustomPlot::legendClick(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event)

  This signal is emitted when a legend (item) is clicked.
  
  \a event is the mouse event that caused the click, \a legend is the legend that received the
  click and \a item is the legend item that received the click. If only the legend and no item is
  clicked, \a item is 0. This happens for a click inside the legend padding or the space between
  two items.
  
  \see legendDoubleClick
*/

/*! \fn void QCustomPlot::legendDoubleClick(QCPLegend *legend,  QCPAbstractLegendItem *item, QMouseEvent *event)

  This signal is emitted when a legend (item) is double clicked.
  
  \a event is the mouse event that caused the click, \a legend is the legend that received the
  click and \a item is the legend item that received the click. If only the legend and no item is
  clicked, \a item is 0. This happens for a click inside the legend padding or the space between
  two items.
  
  \see legendClick
*/

/*! \fn void QCustomPlot:: titleClick(QMouseEvent *event, QCPPlotTitle *title)

  This signal is emitted when a plot title is clicked.
  
  \a event is the mouse event that caused the click and \a title is the plot title that received
  the click.
  
  \see titleDoubleClick
*/

/*! \fn void QCustomPlot::titleDoubleClick(QMouseEvent *event, QCPPlotTitle *title)

  This signal is emitted when a plot title is double clicked.
  
  \a event is the mouse event that caused the click and \a title is the plot title that received
  the click.
  
  \see titleClick
*/

/*! \fn void QCustomPlot::selectionChangedByUser()
  
  This signal is emitted after the user has changed the selection in the QCustomPlot, e.g. by
  clicking. It is not emitted when the selection state of an object has changed programmatically by
  a direct call to setSelected() on an object or by calling \ref deselectAll.
  
  In addition to this signal, selectable objects also provide individual signals, for example
  QCPAxis::selectionChanged or QCPAbstractPlottable::selectionChanged. Note that those signals are
  emitted even if the selection state is changed programmatically.
  
  See the documentation of \ref setInteractions for details about the selection mechanism.
  
  \see selectedPlottables, selectedGraphs, selectedItems, selectedAxes, selectedLegends
*/

/*! \fn void QCustomPlot::beforeReplot()
  
  This signal is emitted immediately before a replot takes place (caused by a call to the slot \ref
  replot).
  
  It is safe to mutually connect the replot slot with this signal on two QCustomPlots to make them
  replot synchronously, it won't cause an infinite recursion.
  
  \see replot, afterReplot
*/

/*! \fn void QCustomPlot::afterReplot()
  
  This signal is emitted immediately after a replot has taken place (caused by a call to the slot \ref
  replot).
  
  It is safe to mutually connect the replot slot with this signal on two QCustomPlots to make them
  replot synchronously, it won't cause an infinite recursion.
  
  \see replot, beforeReplot
*/

/* end of documentation of signals */
/* start of documentation of public members */

/*! \var QCPAxis *QCustomPlot::xAxis

  A pointer to the primary x Axis (bottom) of the main axis rect of the plot.
  
  QCustomPlot offers convenient pointers to the axes (\ref xAxis, \ref yAxis, \ref xAxis2, \ref
  yAxis2) and the \ref legend. They make it very easy working with plots that only have a single
  axis rect and at most one axis at each axis rect side. If you use \link thelayoutsystem the
  layout system\endlink to add multiple axis rects or multiple axes to one side, use the \ref
  QCPAxisRect::axis interface to access the new axes. If one of the four default axes or the
  default legend is removed due to manipulation of the layout system (e.g. by removing the main
  axis rect), the corresponding pointers become 0.
*/

/*! \var QCPAxis *QCustomPlot::yAxis

  A pointer to the primary y Axis (left) of the main axis rect of the plot.
  
  QCustomPlot offers convenient pointers to the axes (\ref xAxis, \ref yAxis, \ref xAxis2, \ref
  yAxis2) and the \ref legend. They make it very easy working with plots that only have a single
  axis rect and at most one axis at each axis rect side. If you use \link thelayoutsystem the
  layout system\endlink to add multiple axis rects or multiple axes to one side, use the \ref
  QCPAxisRect::axis interface to access the new axes. If one of the four default axes or the
  default legend is removed due to manipulation of the layout system (e.g. by removing the main
  axis rect), the corresponding pointers become 0.
*/

/*! \var QCPAxis *QCustomPlot::xAxis2

  A pointer to the secondary x Axis (top) of the main axis rect of the plot. Secondary axes are
  invisible by default. Use QCPAxis::setVisible to change this (or use \ref
  QCPAxisRect::setupFullAxesBox).
  
  QCustomPlot offers convenient pointers to the axes (\ref xAxis, \ref yAxis, \ref xAxis2, \ref
  yAxis2) and the \ref legend. They make it very easy working with plots that only have a single
  axis rect and at most one axis at each axis rect side. If you use \link thelayoutsystem the
  layout system\endlink to add multiple axis rects or multiple axes to one side, use the \ref
  QCPAxisRect::axis interface to access the new axes. If one of the four default axes or the
  default legend is removed due to manipulation of the layout system (e.g. by removing the main
  axis rect), the corresponding pointers become 0.
*/

/*! \var QCPAxis *QCustomPlot::yAxis2

  A pointer to the secondary y Axis (right) of the main axis rect of the plot. Secondary axes are
  invisible by default. Use QCPAxis::setVisible to change this (or use \ref
  QCPAxisRect::setupFullAxesBox).
  
  QCustomPlot offers convenient pointers to the axes (\ref xAxis, \ref yAxis, \ref xAxis2, \ref
  yAxis2) and the \ref legend. They make it very easy working with plots that only have a single
  axis rect and at most one axis at each axis rect side. If you use \link thelayoutsystem the
  layout system\endlink to add multiple axis rects or multiple axes to one side, use the \ref
  QCPAxisRect::axis interface to access the new axes. If one of the four default axes or the
  default legend is removed due to manipulation of the layout system (e.g. by removing the main
  axis rect), the corresponding pointers become 0.
*/

/*! \var QCPLegend *QCustomPlot::legend

  A pointer to the default legend of the main axis rect. The legend is invisible by default. Use
  QCPLegend::setVisible to change this.
  
  QCustomPlot offers convenient pointers to the axes (\ref xAxis, \ref yAxis, \ref xAxis2, \ref
  yAxis2) and the \ref legend. They make it very easy working with plots that only have a single
  axis rect and at most one axis at each axis rect side. If you use \link thelayoutsystem the
  layout system\endlink to add multiple legends to the plot, use the layout system interface to
  access the new legend. For example, legends can be placed inside an axis rect's \ref
  QCPAxisRect::insetLayout "inset layout", and must then also be accessed via the inset layout. If
  the default legend is removed due to manipulation of the layout system (e.g. by removing the main
  axis rect), the corresponding pointer becomes 0.
*/

/* end of documentation of public members */

/*!
  Constructs a QCustomPlot and sets reasonable default values.
*/
QCustomPlot::QCustomPlot(QWidget *parent) :
  QWidget(parent),
  xAxis(0),
  yAxis(0),
  xAxis2(0),
  yAxis2(0),
  legend(0),
  mPlotLayout(0),
  mAutoAddPlottableToLegend(true),
  mAntialiasedElements(QCP::aeNone),
  mNotAntialiasedElements(QCP::aeNone),
  mInteractions(0),
  mSelectionTolerance(8),
  mNoAntialiasingOnDrag(false),
  mBackgroundBrush(Qt::white, Qt::SolidPattern),
  mBackgroundScaled(true),
  mBackgroundScaledMode(Qt::KeepAspectRatioByExpanding),
  mCurrentLayer(0),
  mPlottingHints(QCP::phCacheLabels|QCP::phForceRepaint),
  mMultiSelectModifier(Qt::ControlModifier),
  mPaintBuffer(size()),
  mMouseEventElement(0),
  mReplotting(false)
{
  setAttribute(Qt::WA_NoMousePropagation);
  setAttribute(Qt::WA_OpaquePaintEvent);
  setMouseTracking(true);
  QLocale currentLocale = locale();
  currentLocale.setNumberOptions(QLocale::OmitGroupSeparator);
  setLocale(currentLocale);
  
  // create initial layers:
  mLayers.append(new QCPLayer(this, QLatin1String("background")));
  mLayers.append(new QCPLayer(this, QLatin1String("grid")));
  mLayers.append(new QCPLayer(this, QLatin1String("main")));
  mLayers.append(new QCPLayer(this, QLatin1String("axes")));
  mLayers.append(new QCPLayer(this, QLatin1String("legend")));
  updateLayerIndices();
  setCurrentLayer(QLatin1String("main"));
  
  // create initial layout, axis rect and legend:
  mPlotLayout = new QCPLayoutGrid;
  mPlotLayout->initializeParentPlot(this);
  mPlotLayout->setParent(this); // important because if parent is QWidget, QCPLayout::sizeConstraintsChanged will call QWidget::updateGeometry
  mPlotLayout->setLayer(QLatin1String("main"));
  QCPAxisRect *defaultAxisRect = new QCPAxisRect(this, true);
  mPlotLayout->addElement(0, 0, defaultAxisRect);
  xAxis = defaultAxisRect->axis(QCPAxis::atBottom);
  yAxis = defaultAxisRect->axis(QCPAxis::atLeft);
  xAxis2 = defaultAxisRect->axis(QCPAxis::atTop);
  yAxis2 = defaultAxisRect->axis(QCPAxis::atRight);
  legend = new QCPLegend;
  legend->setVisible(false);
  defaultAxisRect->insetLayout()->addElement(legend, Qt::AlignRight|Qt::AlignTop);
  defaultAxisRect->insetLayout()->setMargins(QMargins(12, 12, 12, 12));
  
  defaultAxisRect->setLayer(QLatin1String("background"));
  xAxis->setLayer(QLatin1String("axes"));
  yAxis->setLayer(QLatin1String("axes"));
  xAxis2->setLayer(QLatin1String("axes"));
  yAxis2->setLayer(QLatin1String("axes"));
  xAxis->grid()->setLayer(QLatin1String("grid"));
  yAxis->grid()->setLayer(QLatin1String("grid"));
  xAxis2->grid()->setLayer(QLatin1String("grid"));
  yAxis2->grid()->setLayer(QLatin1String("grid"));
  legend->setLayer(QLatin1String("legend"));
  
  setViewport(rect()); // needs to be called after mPlotLayout has been created
  
  replot();
}

QCustomPlot::~QCustomPlot()
{
  clearPlottables();
  clearItems();

  if (mPlotLayout)
  {
    delete mPlotLayout;
    mPlotLayout = 0;
  }
  
  mCurrentLayer = 0;
  qDeleteAll(mLayers); // don't use removeLayer, because it would prevent the last layer to be removed
  mLayers.clear();
}

/*!
  Sets which elements are forcibly drawn antialiased as an \a or combination of QCP::AntialiasedElement.
  
  This overrides the antialiasing settings for whole element groups, normally controlled with the
  \a setAntialiasing function on the individual elements. If an element is neither specified in
  \ref setAntialiasedElements nor in \ref setNotAntialiasedElements, the antialiasing setting on
  each individual element instance is used.
  
  For example, if \a antialiasedElements contains \ref QCP::aePlottables, all plottables will be
  drawn antialiased, no matter what the specific QCPAbstractPlottable::setAntialiased value was set
  to.
  
  if an element in \a antialiasedElements is already set in \ref setNotAntialiasedElements, it is
  removed from there.
  
  \see setNotAntialiasedElements
*/
void QCustomPlot::setAntialiasedElements(const QCP::AntialiasedElements &antialiasedElements)
{
  mAntialiasedElements = antialiasedElements;
  
  // make sure elements aren't in mNotAntialiasedElements and mAntialiasedElements simultaneously:
  if ((mNotAntialiasedElements & mAntialiasedElements) != 0)
    mNotAntialiasedElements |= ~mAntialiasedElements;
}

/*!
  Sets whether the specified \a antialiasedElement is forcibly drawn antialiased.
  
  See \ref setAntialiasedElements for details.
  
  \see setNotAntialiasedElement
*/
void QCustomPlot::setAntialiasedElement(QCP::AntialiasedElement antialiasedElement, bool enabled)
{
  if (!enabled && mAntialiasedElements.testFlag(antialiasedElement))
    mAntialiasedElements &= ~antialiasedElement;
  else if (enabled && !mAntialiasedElements.testFlag(antialiasedElement))
    mAntialiasedElements |= antialiasedElement;
  
  // make sure elements aren't in mNotAntialiasedElements and mAntialiasedElements simultaneously:
  if ((mNotAntialiasedElements & mAntialiasedElements) != 0)
    mNotAntialiasedElements |= ~mAntialiasedElements;
}

/*!
  Sets which elements are forcibly drawn not antialiased as an \a or combination of
  QCP::AntialiasedElement.
  
  This overrides the antialiasing settings for whole element groups, normally controlled with the
  \a setAntialiasing function on the individual elements. If an element is neither specified in
  \ref setAntialiasedElements nor in \ref setNotAntialiasedElements, the antialiasing setting on
  each individual element instance is used.
  
  For example, if \a notAntialiasedElements contains \ref QCP::aePlottables, no plottables will be
  drawn antialiased, no matter what the specific QCPAbstractPlottable::setAntialiased value was set
  to.
  
  if an element in \a notAntialiasedElements is already set in \ref setAntialiasedElements, it is
  removed from there.
  
  \see setAntialiasedElements
*/
void QCustomPlot::setNotAntialiasedElements(const QCP::AntialiasedElements &notAntialiasedElements)
{
  mNotAntialiasedElements = notAntialiasedElements;
  
  // make sure elements aren't in mNotAntialiasedElements and mAntialiasedElements simultaneously:
  if ((mNotAntialiasedElements & mAntialiasedElements) != 0)
    mAntialiasedElements |= ~mNotAntialiasedElements;
}

/*!
  Sets whether the specified \a notAntialiasedElement is forcibly drawn not antialiased.
  
  See \ref setNotAntialiasedElements for details.
  
  \see setAntialiasedElement
*/
void QCustomPlot::setNotAntialiasedElement(QCP::AntialiasedElement notAntialiasedElement, bool enabled)
{
  if (!enabled && mNotAntialiasedElements.testFlag(notAntialiasedElement))
    mNotAntialiasedElements &= ~notAntialiasedElement;
  else if (enabled && !mNotAntialiasedElements.testFlag(notAntialiasedElement))
    mNotAntialiasedElements |= notAntialiasedElement;
  
  // make sure elements aren't in mNotAntialiasedElements and mAntialiasedElements simultaneously:
  if ((mNotAntialiasedElements & mAntialiasedElements) != 0)
    mAntialiasedElements |= ~mNotAntialiasedElements;
}

/*!
  If set to true, adding a plottable (e.g. a graph) to the QCustomPlot automatically also adds the
  plottable to the legend (QCustomPlot::legend).
  
  \see addPlottable, addGraph, QCPLegend::addItem
*/
void QCustomPlot::setAutoAddPlottableToLegend(bool on)
{
  mAutoAddPlottableToLegend = on;
}

/*!
  Sets the possible interactions of this QCustomPlot as an or-combination of \ref QCP::Interaction
  enums. There are the following types of interactions:
  
  <b>Axis range manipulation</b> is controlled via \ref QCP::iRangeDrag and \ref QCP::iRangeZoom. When the
  respective interaction is enabled, the user may drag axes ranges and zoom with the mouse wheel.
  For details how to control which axes the user may drag/zoom and in what orientations, see \ref
  QCPAxisRect::setRangeDrag, \ref QCPAxisRect::setRangeZoom, \ref QCPAxisRect::setRangeDragAxes,
  \ref QCPAxisRect::setRangeZoomAxes.
  
  <b>Plottable selection</b> is controlled by \ref QCP::iSelectPlottables. If \ref QCP::iSelectPlottables is
  set, the user may select plottables (graphs, curves, bars,...) by clicking on them or in their
  vicinity (\ref setSelectionTolerance). Whether the user can actually select a plottable can
  further be restricted with the \ref QCPAbstractPlottable::setSelectable function on the specific
  plottable. To find out whether a specific plottable is selected, call
  QCPAbstractPlottable::selected(). To retrieve a list of all currently selected plottables, call
  \ref selectedPlottables. If you're only interested in QCPGraphs, you may use the convenience
  function \ref selectedGraphs.
  
  <b>Item selection</b> is controlled by \ref QCP::iSelectItems. If \ref QCP::iSelectItems is set, the user
  may select items (QCPItemLine, QCPItemText,...) by clicking on them or in their vicinity. To find
  out whether a specific item is selected, call QCPAbstractItem::selected(). To retrieve a list of
  all currently selected items, call \ref selectedItems.
  
  <b>Axis selection</b> is controlled with \ref QCP::iSelectAxes. If \ref QCP::iSelectAxes is set, the user
  may select parts of the axes by clicking on them. What parts exactly (e.g. Axis base line, tick
  labels, axis label) are selectable can be controlled via \ref QCPAxis::setSelectableParts for
  each axis. To retrieve a list of all axes that currently contain selected parts, call \ref
  selectedAxes. Which parts of an axis are selected, can be retrieved with QCPAxis::selectedParts().
  
  <b>Legend selection</b> is controlled with \ref QCP::iSelectLegend. If this is set, the user may
  select the legend itself or individual items by clicking on them. What parts exactly are
  selectable can be controlled via \ref QCPLegend::setSelectableParts. To find out whether the
  legend or any of its child items are selected, check the value of QCPLegend::selectedParts. To
  find out which child items are selected, call \ref QCPLegend::selectedItems.
  
  <b>All other selectable elements</b> The selection of all other selectable objects (e.g.
  QCPPlotTitle, or your own layerable subclasses) is controlled with \ref QCP::iSelectOther. If set, the
  user may select those objects by clicking on them. To find out which are currently selected, you
  need to check their selected state explicitly.
  
  If the selection state has changed by user interaction, the \ref selectionChangedByUser signal is
  emitted. Each selectable object additionally emits an individual selectionChanged signal whenever
  their selection state has changed, i.e. not only by user interaction.
  
  To allow multiple objects to be selected by holding the selection modifier (\ref
  setMultiSelectModifier), set the flag \ref QCP::iMultiSelect.
  
  \note In addition to the selection mechanism presented here, QCustomPlot always emits
  corresponding signals, when an object is clicked or double clicked. see \ref plottableClick and
  \ref plottableDoubleClick for example.
  
  \see setInteraction, setSelectionTolerance
*/
void QCustomPlot::setInteractions(const QCP::Interactions &interactions)
{
  mInteractions = interactions;
}

/*!
  Sets the single \a interaction of this QCustomPlot to \a enabled.
  
  For details about the interaction system, see \ref setInteractions.
  
  \see setInteractions
*/
void QCustomPlot::setInteraction(const QCP::Interaction &interaction, bool enabled)
{
  if (!enabled && mInteractions.testFlag(interaction))
    mInteractions &= ~interaction;
  else if (enabled && !mInteractions.testFlag(interaction))
    mInteractions |= interaction;
}

/*!
  Sets the tolerance that is used to decide whether a click selects an object (e.g. a plottable) or
  not.
  
  If the user clicks in the vicinity of the line of e.g. a QCPGraph, it's only regarded as a
  potential selection when the minimum distance between the click position and the graph line is
  smaller than \a pixels. Objects that are defined by an area (e.g. QCPBars) only react to clicks
  directly inside the area and ignore this selection tolerance. In other words, it only has meaning
  for parts of objects that are too thin to exactly hit with a click and thus need such a
  tolerance.
  
  \see setInteractions, QCPLayerable::selectTest
*/
void QCustomPlot::setSelectionTolerance(int pixels)
{
  mSelectionTolerance = pixels;
}

/*!
  Sets whether antialiasing is disabled for this QCustomPlot while the user is dragging axes
  ranges. If many objects, especially plottables, are drawn antialiased, this greatly improves
  performance during dragging. Thus it creates a more responsive user experience. As soon as the
  user stops dragging, the last replot is done with normal antialiasing, to restore high image
  quality.
  
  \see setAntialiasedElements, setNotAntialiasedElements
*/
void QCustomPlot::setNoAntialiasingOnDrag(bool enabled)
{
  mNoAntialiasingOnDrag = enabled;
}

/*!
  Sets the plotting hints for this QCustomPlot instance as an \a or combination of QCP::PlottingHint.
  
  \see setPlottingHint
*/
void QCustomPlot::setPlottingHints(const QCP::PlottingHints &hints)
{
  mPlottingHints = hints;
}

/*!
  Sets the specified plotting \a hint to \a enabled.
  
  \see setPlottingHints
*/
void QCustomPlot::setPlottingHint(QCP::PlottingHint hint, bool enabled)
{
  QCP::PlottingHints newHints = mPlottingHints;
  if (!enabled)
    newHints &= ~hint;
  else
    newHints |= hint;
  
  if (newHints != mPlottingHints)
    setPlottingHints(newHints);
}

/*!
  Sets the keyboard modifier that will be recognized as multi-select-modifier.
  
  If \ref QCP::iMultiSelect is specified in \ref setInteractions, the user may select multiple objects
  by clicking on them one after the other while holding down \a modifier.
  
  By default the multi-select-modifier is set to Qt::ControlModifier.
  
  \see setInteractions
*/
void QCustomPlot::setMultiSelectModifier(Qt::KeyboardModifier modifier)
{
  mMultiSelectModifier = modifier;
}

/*!
  Sets the viewport of this QCustomPlot. The Viewport is the area that the top level layout
  (QCustomPlot::plotLayout()) uses as its rect. Normally, the viewport is the entire widget rect.
  
  This function is used to allow arbitrary size exports with \ref toPixmap, \ref savePng, \ref
  savePdf, etc. by temporarily changing the viewport size.
*/
void QCustomPlot::setViewport(const QRect &rect)
{
  mViewport = rect;
  if (mPlotLayout)
    mPlotLayout->setOuterRect(mViewport);
}

/*!
  Sets \a pm as the viewport background pixmap (see \ref setViewport). The pixmap is always drawn
  below all other objects in the plot.

  For cases where the provided pixmap doesn't have the same size as the viewport, scaling can be
  enabled with \ref setBackgroundScaled and the scaling mode (whether and how the aspect ratio is
  preserved) can be set with \ref setBackgroundScaledMode. To set all these options in one call,
  consider using the overloaded version of this function.
  
  If a background brush was set with \ref setBackground(const QBrush &brush), the viewport will
  first be filled with that brush, before drawing the background pixmap. This can be useful for
  background pixmaps with translucent areas.

  \see setBackgroundScaled, setBackgroundScaledMode
*/
void QCustomPlot::setBackground(const QPixmap &pm)
{
  mBackgroundPixmap = pm;
  mScaledBackgroundPixmap = QPixmap();
}

/*!
  Sets the background brush of the viewport (see \ref setViewport).

  Before drawing everything else, the background is filled with \a brush. If a background pixmap
  was set with \ref setBackground(const QPixmap &pm), this brush will be used to fill the viewport
  before the background pixmap is drawn. This can be useful for background pixmaps with translucent
  areas.
  
  Set \a brush to Qt::NoBrush or Qt::Transparent to leave background transparent. This can be
  useful for exporting to image formats which support transparency, e.g. \ref savePng.

  \see setBackgroundScaled, setBackgroundScaledMode
*/
void QCustomPlot::setBackground(const QBrush &brush)
{
  mBackgroundBrush = brush;
}

/*! \overload
  
  Allows setting the background pixmap of the viewport, whether it shall be scaled and how it
  shall be scaled in one call.

  \see setBackground(const QPixmap &pm), setBackgroundScaled, setBackgroundScaledMode
*/
void QCustomPlot::setBackground(const QPixmap &pm, bool scaled, Qt::AspectRatioMode mode)
{
  mBackgroundPixmap = pm;
  mScaledBackgroundPixmap = QPixmap();
  mBackgroundScaled = scaled;
  mBackgroundScaledMode = mode;
}

/*!
  Sets whether the viewport background pixmap shall be scaled to fit the viewport. If \a scaled is
  set to true, control whether and how the aspect ratio of the original pixmap is preserved with
  \ref setBackgroundScaledMode.
  
  Note that the scaled version of the original pixmap is buffered, so there is no performance
  penalty on replots. (Except when the viewport dimensions are changed continuously.)
  
  \see setBackground, setBackgroundScaledMode
*/
void QCustomPlot::setBackgroundScaled(bool scaled)
{
  mBackgroundScaled = scaled;
}

/*!
  If scaling of the viewport background pixmap is enabled (\ref setBackgroundScaled), use this
  function to define whether and how the aspect ratio of the original pixmap is preserved.
  
  \see setBackground, setBackgroundScaled
*/
void QCustomPlot::setBackgroundScaledMode(Qt::AspectRatioMode mode)
{
  mBackgroundScaledMode = mode;
}

/*!
  Returns the plottable with \a index. If the index is invalid, returns 0.
  
  There is an overloaded version of this function with no parameter which returns the last added
  plottable, see QCustomPlot::plottable()
  
  \see plottableCount, addPlottable
*/
QCPAbstractPlottable *QCustomPlot::plottable(int index)
{
  if (index >= 0 && index < mPlottables.size())
  {
    return mPlottables.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return 0;
  }
}

/*! \overload
  
  Returns the last plottable that was added with \ref addPlottable. If there are no plottables in
  the plot, returns 0.
  
  \see plottableCount, addPlottable
*/
QCPAbstractPlottable *QCustomPlot::plottable()
{
  if (!mPlottables.isEmpty())
  {
    return mPlottables.last();
  } else
    return 0;
}

/*!
  Adds the specified plottable to the plot and, if \ref setAutoAddPlottableToLegend is enabled, to
  the legend (QCustomPlot::legend). QCustomPlot takes ownership of the plottable.
  
  Returns true on success, i.e. when \a plottable isn't already in the plot and the parent plot of
  \a plottable is this QCustomPlot (the latter is controlled by what axes were passed in the
  plottable's constructor).
  
  \see plottable, plottableCount, removePlottable, clearPlottables
*/
bool QCustomPlot::addPlottable(QCPAbstractPlottable *plottable)
{
  if (mPlottables.contains(plottable))
  {
    qDebug() << Q_FUNC_INFO << "plottable already added to this QCustomPlot:" << reinterpret_cast<quintptr>(plottable);
    return false;
  }
  if (plottable->parentPlot() != this)
  {
    qDebug() << Q_FUNC_INFO << "plottable not created with this QCustomPlot as parent:" << reinterpret_cast<quintptr>(plottable);
    return false;
  }
  
  mPlottables.append(plottable);
  // possibly add plottable to legend:
  if (mAutoAddPlottableToLegend)
    plottable->addToLegend();
  // special handling for QCPGraphs to maintain the simple graph interface:
  if (QCPGraph *graph = qobject_cast<QCPGraph*>(plottable))
    mGraphs.append(graph);
  if (!plottable->layer()) // usually the layer is already set in the constructor of the plottable (via QCPLayerable constructor)
    plottable->setLayer(currentLayer());
  return true;
}

/*!
  Removes the specified plottable from the plot and, if necessary, from the legend (QCustomPlot::legend).
  
  Returns true on success.
  
  \see addPlottable, clearPlottables
*/
bool QCustomPlot::removePlottable(QCPAbstractPlottable *plottable)
{
  if (!mPlottables.contains(plottable))
  {
    qDebug() << Q_FUNC_INFO << "plottable not in list:" << reinterpret_cast<quintptr>(plottable);
    return false;
  }
  
  // remove plottable from legend:
  plottable->removeFromLegend();
  // special handling for QCPGraphs to maintain the simple graph interface:
  if (QCPGraph *graph = qobject_cast<QCPGraph*>(plottable))
    mGraphs.removeOne(graph);
  // remove plottable:
  delete plottable;
  mPlottables.removeOne(plottable);
  return true;
}

/*! \overload
  
  Removes the plottable by its \a index.
*/
bool QCustomPlot::removePlottable(int index)
{
  if (index >= 0 && index < mPlottables.size())
    return removePlottable(mPlottables[index]);
  else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return false;
  }
}

/*!
  Removes all plottables from the plot (and the QCustomPlot::legend, if necessary).
  
  Returns the number of plottables removed.
  
  \see removePlottable
*/
int QCustomPlot::clearPlottables()
{
  int c = mPlottables.size();
  for (int i=c-1; i >= 0; --i)
    removePlottable(mPlottables[i]);
  return c;
}

/*!
  Returns the number of currently existing plottables in the plot
  
  \see plottable, addPlottable
*/
int QCustomPlot::plottableCount() const
{
  return mPlottables.size();
}

/*!
  Returns a list of the selected plottables. If no plottables are currently selected, the list is empty.
  
  There is a convenience function if you're only interested in selected graphs, see \ref selectedGraphs.
  
  \see setInteractions, QCPAbstractPlottable::setSelectable, QCPAbstractPlottable::setSelected
*/
QList<QCPAbstractPlottable*> QCustomPlot::selectedPlottables() const
{
  QList<QCPAbstractPlottable*> result;
  foreach (QCPAbstractPlottable *plottable, mPlottables)
  {
    if (plottable->selected())
      result.append(plottable);
  }
  return result;
}

/*!
  Returns the plottable at the pixel position \a pos. Plottables that only consist of single lines
  (like graphs) have a tolerance band around them, see \ref setSelectionTolerance. If multiple
  plottables come into consideration, the one closest to \a pos is returned.
  
  If \a onlySelectable is true, only plottables that are selectable
  (QCPAbstractPlottable::setSelectable) are considered.
  
  If there is no plottable at \a pos, the return value is 0.
  
  \see itemAt, layoutElementAt
*/
QCPAbstractPlottable *QCustomPlot::plottableAt(const QPointF &pos, bool onlySelectable) const
{
  QCPAbstractPlottable *resultPlottable = 0;
  double resultDistance = mSelectionTolerance; // only regard clicks with distances smaller than mSelectionTolerance as selections, so initialize with that value
  
  foreach (QCPAbstractPlottable *plottable, mPlottables)
  {
    if (onlySelectable && !plottable->selectable()) // we could have also passed onlySelectable to the selectTest function, but checking here is faster, because we have access to QCPabstractPlottable::selectable
      continue;
    if ((plottable->keyAxis()->axisRect()->rect() & plottable->valueAxis()->axisRect()->rect()).contains(pos.toPoint())) // only consider clicks inside the rect that is spanned by the plottable's key/value axes
    {
      double currentDistance = plottable->selectTest(pos, false);
      if (currentDistance >= 0 && currentDistance < resultDistance)
      {
        resultPlottable = plottable;
        resultDistance = currentDistance;
      }
    }
  }
  
  return resultPlottable;
}

/*!
  Returns whether this QCustomPlot instance contains the \a plottable.
  
  \see addPlottable
*/
bool QCustomPlot::hasPlottable(QCPAbstractPlottable *plottable) const
{
  return mPlottables.contains(plottable);
}

/*!
  Returns the graph with \a index. If the index is invalid, returns 0.
  
  There is an overloaded version of this function with no parameter which returns the last created
  graph, see QCustomPlot::graph()
  
  \see graphCount, addGraph
*/
QCPGraph *QCustomPlot::graph(int index) const
{
  if (index >= 0 && index < mGraphs.size())
  {
    return mGraphs.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return 0;
  }
}

/*! \overload
  
  Returns the last graph, that was created with \ref addGraph. If there are no graphs in the plot,
  returns 0.
  
  \see graphCount, addGraph
*/
QCPGraph *QCustomPlot::graph() const
{
  if (!mGraphs.isEmpty())
  {
    return mGraphs.last();
  } else
    return 0;
}

/*!
  Creates a new graph inside the plot. If \a keyAxis and \a valueAxis are left unspecified (0), the
  bottom (xAxis) is used as key and the left (yAxis) is used as value axis. If specified, \a
  keyAxis and \a valueAxis must reside in this QCustomPlot.
  
  \a keyAxis will be used as key axis (typically "x") and \a valueAxis as value axis (typically
  "y") for the graph.
  
  Returns a pointer to the newly created graph, or 0 if adding the graph failed.
  
  \see graph, graphCount, removeGraph, clearGraphs
*/
QCPGraph *QCustomPlot::addGraph(QCPAxis *keyAxis, QCPAxis *valueAxis)
{
  if (!keyAxis) keyAxis = xAxis;
  if (!valueAxis) valueAxis = yAxis;
  if (!keyAxis || !valueAxis)
  {
    qDebug() << Q_FUNC_INFO << "can't use default QCustomPlot xAxis or yAxis, because at least one is invalid (has been deleted)";
    return 0;
  }
  if (keyAxis->parentPlot() != this || valueAxis->parentPlot() != this)
  {
    qDebug() << Q_FUNC_INFO << "passed keyAxis or valueAxis doesn't have this QCustomPlot as parent";
    return 0;
  }
  
  QCPGraph *newGraph = new QCPGraph(keyAxis, valueAxis);
  if (addPlottable(newGraph))
  {
    newGraph->setName(QLatin1String("Graph ")+QString::number(mGraphs.size()));
    return newGraph;
  } else
  {
    delete newGraph;
    return 0;
  }
}

/*!
  Removes the specified \a graph from the plot and, if necessary, from the QCustomPlot::legend. If
  any other graphs in the plot have a channel fill set towards the removed graph, the channel fill
  property of those graphs is reset to zero (no channel fill).
  
  Returns true on success.
  
  \see clearGraphs
*/
bool QCustomPlot::removeGraph(QCPGraph *graph)
{
  return removePlottable(graph);
}

/*! \overload
  
  Removes the graph by its \a index.
*/
bool QCustomPlot::removeGraph(int index)
{
  if (index >= 0 && index < mGraphs.size())
    return removeGraph(mGraphs[index]);
  else
    return false;
}

/*!
  Removes all graphs from the plot (and the QCustomPlot::legend, if necessary).

  Returns the number of graphs removed.
  
  \see removeGraph
*/
int QCustomPlot::clearGraphs()
{
  int c = mGraphs.size();
  for (int i=c-1; i >= 0; --i)
    removeGraph(mGraphs[i]);
  return c;
}

/*!
  Returns the number of currently existing graphs in the plot
  
  \see graph, addGraph
*/
int QCustomPlot::graphCount() const
{
  return mGraphs.size();
}

/*!
  Returns a list of the selected graphs. If no graphs are currently selected, the list is empty.
  
  If you are not only interested in selected graphs but other plottables like QCPCurve, QCPBars,
  etc., use \ref selectedPlottables.
  
  \see setInteractions, selectedPlottables, QCPAbstractPlottable::setSelectable, QCPAbstractPlottable::setSelected
*/
QList<QCPGraph*> QCustomPlot::selectedGraphs() const
{
  QList<QCPGraph*> result;
  foreach (QCPGraph *graph, mGraphs)
  {
    if (graph->selected())
      result.append(graph);
  }
  return result;
}

/*!
  Returns the item with \a index. If the index is invalid, returns 0.
  
  There is an overloaded version of this function with no parameter which returns the last added
  item, see QCustomPlot::item()
  
  \see itemCount, addItem
*/
QCPAbstractItem *QCustomPlot::item(int index) const
{
  if (index >= 0 && index < mItems.size())
  {
    return mItems.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return 0;
  }
}

/*! \overload
  
  Returns the last item, that was added with \ref addItem. If there are no items in the plot,
  returns 0.
  
  \see itemCount, addItem
*/
QCPAbstractItem *QCustomPlot::item() const
{
  if (!mItems.isEmpty())
  {
    return mItems.last();
  } else
    return 0;
}

/*!
  Adds the specified item to the plot. QCustomPlot takes ownership of the item.
  
  Returns true on success, i.e. when \a item wasn't already in the plot and the parent plot of \a
  item is this QCustomPlot.
  
  \see item, itemCount, removeItem, clearItems
*/
bool QCustomPlot::addItem(QCPAbstractItem *item)
{
  if (!mItems.contains(item) && item->parentPlot() == this)
  {
    mItems.append(item);
    return true;
  } else
  {
    qDebug() << Q_FUNC_INFO << "item either already in list or not created with this QCustomPlot as parent:" << reinterpret_cast<quintptr>(item);
    return false;
  }
}

/*!
  Removes the specified item from the plot.
  
  Returns true on success.
  
  \see addItem, clearItems
*/
bool QCustomPlot::removeItem(QCPAbstractItem *item)
{
  if (mItems.contains(item))
  {
    delete item;
    mItems.removeOne(item);
    return true;
  } else
  {
    qDebug() << Q_FUNC_INFO << "item not in list:" << reinterpret_cast<quintptr>(item);
    return false;
  }
}

/*! \overload
  
  Removes the item by its \a index.
*/
bool QCustomPlot::removeItem(int index)
{
  if (index >= 0 && index < mItems.size())
    return removeItem(mItems[index]);
  else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return false;
  }
}

/*!
  Removes all items from the plot.
  
  Returns the number of items removed.
  
  \see removeItem
*/
int QCustomPlot::clearItems()
{
  int c = mItems.size();
  for (int i=c-1; i >= 0; --i)
    removeItem(mItems[i]);
  return c;
}

/*!
  Returns the number of currently existing items in the plot
  
  \see item, addItem
*/
int QCustomPlot::itemCount() const
{
  return mItems.size();
}

/*!
  Returns a list of the selected items. If no items are currently selected, the list is empty.
  
  \see setInteractions, QCPAbstractItem::setSelectable, QCPAbstractItem::setSelected
*/
QList<QCPAbstractItem*> QCustomPlot::selectedItems() const
{
  QList<QCPAbstractItem*> result;
  foreach (QCPAbstractItem *item, mItems)
  {
    if (item->selected())
      result.append(item);
  }
  return result;
}

/*!
  Returns the item at the pixel position \a pos. Items that only consist of single lines (e.g. \ref
  QCPItemLine or \ref QCPItemCurve) have a tolerance band around them, see \ref
  setSelectionTolerance. If multiple items come into consideration, the one closest to \a pos is
  returned.
  
  If \a onlySelectable is true, only items that are selectable (QCPAbstractItem::setSelectable) are
  considered.
  
  If there is no item at \a pos, the return value is 0.
  
  \see plottableAt, layoutElementAt
*/
QCPAbstractItem *QCustomPlot::itemAt(const QPointF &pos, bool onlySelectable) const
{
  QCPAbstractItem *resultItem = 0;
  double resultDistance = mSelectionTolerance; // only regard clicks with distances smaller than mSelectionTolerance as selections, so initialize with that value
  
  foreach (QCPAbstractItem *item, mItems)
  {
    if (onlySelectable && !item->selectable()) // we could have also passed onlySelectable to the selectTest function, but checking here is faster, because we have access to QCPAbstractItem::selectable
      continue;
    if (!item->clipToAxisRect() || item->clipRect().contains(pos.toPoint())) // only consider clicks inside axis cliprect of the item if actually clipped to it
    {
      double currentDistance = item->selectTest(pos, false);
      if (currentDistance >= 0 && currentDistance < resultDistance)
      {
        resultItem = item;
        resultDistance = currentDistance;
      }
    }
  }
  
  return resultItem;
}

/*!
  Returns whether this QCustomPlot contains the \a item.
  
  \see addItem
*/
bool QCustomPlot::hasItem(QCPAbstractItem *item) const
{
  return mItems.contains(item);
}

/*!
  Returns the layer with the specified \a name. If there is no layer with the specified name, 0 is
  returned.
  
  Layer names are case-sensitive.
  
  \see addLayer, moveLayer, removeLayer
*/
QCPLayer *QCustomPlot::layer(const QString &name) const
{
  foreach (QCPLayer *layer, mLayers)
  {
    if (layer->name() == name)
      return layer;
  }
  return 0;
}

/*! \overload
  
  Returns the layer by \a index. If the index is invalid, 0 is returned.
  
  \see addLayer, moveLayer, removeLayer
*/
QCPLayer *QCustomPlot::layer(int index) const
{
  if (index >= 0 && index < mLayers.size())
  {
    return mLayers.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "index out of bounds:" << index;
    return 0;
  }
}

/*!
  Returns the layer that is set as current layer (see \ref setCurrentLayer).
*/
QCPLayer *QCustomPlot::currentLayer() const
{
  return mCurrentLayer;
}

/*!
  Sets the layer with the specified \a name to be the current layer. All layerables (\ref
  QCPLayerable), e.g. plottables and items, are created on the current layer.
  
  Returns true on success, i.e. if there is a layer with the specified \a name in the QCustomPlot.
  
  Layer names are case-sensitive.
  
  \see addLayer, moveLayer, removeLayer, QCPLayerable::setLayer
*/
bool QCustomPlot::setCurrentLayer(const QString &name)
{
  if (QCPLayer *newCurrentLayer = layer(name))
  {
    return setCurrentLayer(newCurrentLayer);
  } else
  {
    qDebug() << Q_FUNC_INFO << "layer with name doesn't exist:" << name;
    return false;
  }
}

/*! \overload
  
  Sets the provided \a layer to be the current layer.
  
  Returns true on success, i.e. when \a layer is a valid layer in the QCustomPlot.
  
  \see addLayer, moveLayer, removeLayer
*/
bool QCustomPlot::setCurrentLayer(QCPLayer *layer)
{
  if (!mLayers.contains(layer))
  {
    qDebug() << Q_FUNC_INFO << "layer not a layer of this QCustomPlot:" << reinterpret_cast<quintptr>(layer);
    return false;
  }
  
  mCurrentLayer = layer;
  return true;
}

/*!
  Returns the number of currently existing layers in the plot
  
  \see layer, addLayer
*/
int QCustomPlot::layerCount() const
{
  return mLayers.size();
}

/*!
  Adds a new layer to this QCustomPlot instance. The new layer will have the name \a name, which
  must be unique. Depending on \a insertMode, it is positioned either below or above \a otherLayer.
  
  Returns true on success, i.e. if there is no other layer named \a name and \a otherLayer is a
  valid layer inside this QCustomPlot.
  
  If \a otherLayer is 0, the highest layer in the QCustomPlot will be used.
  
  For an explanation of what layers are in QCustomPlot, see the documentation of \ref QCPLayer.
  
  \see layer, moveLayer, removeLayer
*/
bool QCustomPlot::addLayer(const QString &name, QCPLayer *otherLayer, QCustomPlot::LayerInsertMode insertMode)
{
  if (!otherLayer)
    otherLayer = mLayers.last();
  if (!mLayers.contains(otherLayer))
  {
    qDebug() << Q_FUNC_INFO << "otherLayer not a layer of this QCustomPlot:" << reinterpret_cast<quintptr>(otherLayer);
    return false;
  }
  if (layer(name))
  {
    qDebug() << Q_FUNC_INFO << "A layer exists already with the name" << name;
    return false;
  }
    
  QCPLayer *newLayer = new QCPLayer(this, name);
  mLayers.insert(otherLayer->index() + (insertMode==limAbove ? 1:0), newLayer);
  updateLayerIndices();
  return true;
}

/*!
  Removes the specified \a layer and returns true on success.
  
  All layerables (e.g. plottables and items) on the removed layer will be moved to the layer below
  \a layer. If \a layer is the bottom layer, the layerables are moved to the layer above. In both
  cases, the total rendering order of all layerables in the QCustomPlot is preserved.
  
  If \a layer is the current layer (\ref setCurrentLayer), the layer below (or above, if bottom
  layer) becomes the new current layer.
  
  It is not possible to remove the last layer of the plot.
  
  \see layer, addLayer, moveLayer
*/
bool QCustomPlot::removeLayer(QCPLayer *layer)
{
  if (!mLayers.contains(layer))
  {
    qDebug() << Q_FUNC_INFO << "layer not a layer of this QCustomPlot:" << reinterpret_cast<quintptr>(layer);
    return false;
  }
  if (mLayers.size() < 2)
  {
    qDebug() << Q_FUNC_INFO << "can't remove last layer";
    return false;
  }
  
  // append all children of this layer to layer below (if this is lowest layer, prepend to layer above)
  int removedIndex = layer->index();
  bool isFirstLayer = removedIndex==0;
  QCPLayer *targetLayer = isFirstLayer ? mLayers.at(removedIndex+1) : mLayers.at(removedIndex-1);
  QList<QCPLayerable*> children = layer->children();
  if (isFirstLayer) // prepend in reverse order (so order relative to each other stays the same)
  {
    for (int i=children.size()-1; i>=0; --i)
      children.at(i)->moveToLayer(targetLayer, true);
  } else  // append normally
  {
    for (int i=0; i<children.size(); ++i)
      children.at(i)->moveToLayer(targetLayer, false);
  }
  // if removed layer is current layer, change current layer to layer below/above:
  if (layer == mCurrentLayer)
    setCurrentLayer(targetLayer);
  // remove layer:
  delete layer;
  mLayers.removeOne(layer);
  updateLayerIndices();
  return true;
}

/*!
  Moves the specified \a layer either above or below \a otherLayer. Whether it's placed above or
  below is controlled with \a insertMode.
  
  Returns true on success, i.e. when both \a layer and \a otherLayer are valid layers in the
  QCustomPlot.
  
  \see layer, addLayer, moveLayer
*/
bool QCustomPlot::moveLayer(QCPLayer *layer, QCPLayer *otherLayer, QCustomPlot::LayerInsertMode insertMode)
{
  if (!mLayers.contains(layer))
  {
    qDebug() << Q_FUNC_INFO << "layer not a layer of this QCustomPlot:" << reinterpret_cast<quintptr>(layer);
    return false;
  }
  if (!mLayers.contains(otherLayer))
  {
    qDebug() << Q_FUNC_INFO << "otherLayer not a layer of this QCustomPlot:" << reinterpret_cast<quintptr>(otherLayer);
    return false;
  }
  
  mLayers.move(layer->index(), otherLayer->index() + (insertMode==limAbove ? 1:0));
  updateLayerIndices();
  return true;
}

/*!
  Returns the number of axis rects in the plot.
  
  All axis rects can be accessed via QCustomPlot::axisRect().
  
  Initially, only one axis rect exists in the plot.
  
  \see axisRect, axisRects
*/
int QCustomPlot::axisRectCount() const
{
  return axisRects().size();
}

/*!
  Returns the axis rect with \a index.
  
  Initially, only one axis rect (with index 0) exists in the plot. If multiple axis rects were
  added, all of them may be accessed with this function in a linear fashion (even when they are
  nested in a layout hierarchy or inside other axis rects via QCPAxisRect::insetLayout).
  
  \see axisRectCount, axisRects
*/
QCPAxisRect *QCustomPlot::axisRect(int index) const
{
  const QList<QCPAxisRect*> rectList = axisRects();
  if (index >= 0 && index < rectList.size())
  {
    return rectList.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "invalid axis rect index" << index;
    return 0;
  }
}

/*!
  Returns all axis rects in the plot.
  
  \see axisRectCount, axisRect
*/
QList<QCPAxisRect*> QCustomPlot::axisRects() const
{
  QList<QCPAxisRect*> result;
  QStack<QCPLayoutElement*> elementStack;
  if (mPlotLayout)
    elementStack.push(mPlotLayout);
  
  while (!elementStack.isEmpty())
  {
    foreach (QCPLayoutElement *element, elementStack.pop()->elements(false))
    {
      if (element)
      {
        elementStack.push(element);
        if (QCPAxisRect *ar = qobject_cast<QCPAxisRect*>(element))
          result.append(ar);
      }
    }
  }
  
  return result;
}

/*!
  Returns the layout element at pixel position \a pos. If there is no element at that position,
  returns 0.
  
  Only visible elements are used. If \ref QCPLayoutElement::setVisible on the element itself or on
  any of its parent elements is set to false, it will not be considered.
  
  \see itemAt, plottableAt
*/
QCPLayoutElement *QCustomPlot::layoutElementAt(const QPointF &pos) const
{
  QCPLayoutElement *currentElement = mPlotLayout;
  bool searchSubElements = true;
  while (searchSubElements && currentElement)
  {
    searchSubElements = false;
    foreach (QCPLayoutElement *subElement, currentElement->elements(false))
    {
      if (subElement && subElement->realVisibility() && subElement->selectTest(pos, false) >= 0)
      {
        currentElement = subElement;
        searchSubElements = true;
        break;
      }
    }
  }
  return currentElement;
}

/*!
  Returns the axes that currently have selected parts, i.e. whose selection state is not \ref
  QCPAxis::spNone.
  
  \see selectedPlottables, selectedLegends, setInteractions, QCPAxis::setSelectedParts,
  QCPAxis::setSelectableParts
*/
QList<QCPAxis*> QCustomPlot::selectedAxes() const
{
  QList<QCPAxis*> result, allAxes;
  foreach (QCPAxisRect *rect, axisRects())
    allAxes << rect->axes();
  
  foreach (QCPAxis *axis, allAxes)
  {
    if (axis->selectedParts() != QCPAxis::spNone)
      result.append(axis);
  }
  
  return result;
}

/*!
  Returns the legends that currently have selected parts, i.e. whose selection state is not \ref
  QCPLegend::spNone.
  
  \see selectedPlottables, selectedAxes, setInteractions, QCPLegend::setSelectedParts,
  QCPLegend::setSelectableParts, QCPLegend::selectedItems
*/
QList<QCPLegend*> QCustomPlot::selectedLegends() const
{
  QList<QCPLegend*> result;
  
  QStack<QCPLayoutElement*> elementStack;
  if (mPlotLayout)
    elementStack.push(mPlotLayout);
  
  while (!elementStack.isEmpty())
  {
    foreach (QCPLayoutElement *subElement, elementStack.pop()->elements(false))
    {
      if (subElement)
      {
        elementStack.push(subElement);
        if (QCPLegend *leg = qobject_cast<QCPLegend*>(subElement))
        {
          if (leg->selectedParts() != QCPLegend::spNone)
            result.append(leg);
        }
      }
    }
  }
  
  return result;
}

/*!
  Deselects all layerables (plottables, items, axes, legends,...) of the QCustomPlot.
  
  Since calling this function is not a user interaction, this does not emit the \ref
  selectionChangedByUser signal. The individual selectionChanged signals are emitted though, if the
  objects were previously selected.
  
  \see setInteractions, selectedPlottables, selectedItems, selectedAxes, selectedLegends
*/
void QCustomPlot::deselectAll()
{
  foreach (QCPLayer *layer, mLayers)
  {
    foreach (QCPLayerable *layerable, layer->children())
      layerable->deselectEvent(0);
  }
}

/*!
  Causes a complete replot into the internal buffer. Finally, update() is called, to redraw the
  buffer on the QCustomPlot widget surface. This is the method that must be called to make changes,
  for example on the axis ranges or data points of graphs, visible.
  
  Under a few circumstances, QCustomPlot causes a replot by itself. Those are resize events of the
  QCustomPlot widget and user interactions (object selection and range dragging/zooming).
  
  Before the replot happens, the signal \ref beforeReplot is emitted. After the replot, \ref
  afterReplot is emitted. It is safe to mutually connect the replot slot with any of those two
  signals on two QCustomPlots to make them replot synchronously, it won't cause an infinite
  recursion.
*/
void QCustomPlot::replot(QCustomPlot::RefreshPriority refreshPriority)
{
  if (mReplotting) // incase signals loop back to replot slot
    return;
  mReplotting = true;
  emit beforeReplot();
  
  mPaintBuffer.fill(mBackgroundBrush.style() == Qt::SolidPattern ? mBackgroundBrush.color() : Qt::transparent);
  QCPPainter painter;
  painter.begin(&mPaintBuffer);
  if (painter.isActive())
  {
    painter.setRenderHint(QPainter::HighQualityAntialiasing); // to make Antialiasing look good if using the OpenGL graphicssystem
    if (mBackgroundBrush.style() != Qt::SolidPattern && mBackgroundBrush.style() != Qt::NoBrush)
      painter.fillRect(mViewport, mBackgroundBrush);
    draw(&painter);
    painter.end();
    if ((refreshPriority == rpHint && mPlottingHints.testFlag(QCP::phForceRepaint)) || refreshPriority==rpImmediate)
      repaint();
    else
      update();
  } else // might happen if QCustomPlot has width or height zero
    qDebug() << Q_FUNC_INFO << "Couldn't activate painter on buffer. This usually happens because QCustomPlot has width or height zero.";
  
  emit afterReplot();
  mReplotting = false;
}

/*!
  Rescales the axes such that all plottables (like graphs) in the plot are fully visible.
  
  if \a onlyVisiblePlottables is set to true, only the plottables that have their visibility set to true
  (QCPLayerable::setVisible), will be used to rescale the axes.
  
  \see QCPAbstractPlottable::rescaleAxes, QCPAxis::rescale
*/
void QCustomPlot::rescaleAxes(bool onlyVisiblePlottables)
{
  QList<QCPAxis*> allAxes;
  foreach (QCPAxisRect *rect, axisRects())
    allAxes << rect->axes();
  
  foreach (QCPAxis *axis, allAxes)
    axis->rescale(onlyVisiblePlottables);
}

/*!
  Saves a PDF with the vectorized plot to the file \a fileName. The axis ratio as well as the scale
  of texts and lines will be derived from the specified \a width and \a height. This means, the
  output will look like the normal on-screen output of a QCustomPlot widget with the corresponding
  pixel width and height. If either \a width or \a height is zero, the exported image will have the
  same dimensions as the QCustomPlot widget currently has.

  \a noCosmeticPen disables the use of cosmetic pens when drawing to the PDF file. Cosmetic pens
  are pens with numerical width 0, which are always drawn as a one pixel wide line, no matter what
  zoom factor is set in the PDF-Viewer. For more information about cosmetic pens, see the QPainter
  and QPen documentation.
  
  The objects of the plot will appear in the current selection state. If you don't want any
  selected objects to be painted in their selected look, deselect everything with \ref deselectAll
  before calling this function.

  Returns true on success.
  
  \warning
  \li If you plan on editing the exported PDF file with a vector graphics editor like
  Inkscape, it is advised to set \a noCosmeticPen to true to avoid losing those cosmetic lines
  (which might be quite many, because cosmetic pens are the default for e.g. axes and tick marks).
  \li If calling this function inside the constructor of the parent of the QCustomPlot widget
  (i.e. the MainWindow constructor, if QCustomPlot is inside the MainWindow), always provide
  explicit non-zero widths and heights. If you leave \a width or \a height as 0 (default), this
  function uses the current width and height of the QCustomPlot widget. However, in Qt, these
  aren't defined yet inside the constructor, so you would get an image that has strange
  widths/heights.
  
  \a pdfCreator and \a pdfTitle may be used to set the according metadata fields in the resulting
  PDF file.
  
  \note On Android systems, this method does nothing and issues an according qDebug warning
  message. This is also the case if for other reasons the define flag QT_NO_PRINTER is set.
  
  \see savePng, saveBmp, saveJpg, saveRastered
*/
bool QCustomPlot::savePdf(const QString &fileName, bool noCosmeticPen, int width, int height, const QString &pdfCreator, const QString &pdfTitle)
{
  bool success = false;
#ifdef QT_NO_PRINTER
  Q_UNUSED(fileName)
  Q_UNUSED(noCosmeticPen)
  Q_UNUSED(width)
  Q_UNUSED(height)
  Q_UNUSED(pdfCreator)
  Q_UNUSED(pdfTitle)
  qDebug() << Q_FUNC_INFO << "Qt was built without printer support (QT_NO_PRINTER). PDF not created.";
#else
  int newWidth, newHeight;
  if (width == 0 || height == 0)
  {
    newWidth = this->width();
    newHeight = this->height();
  } else
  {
    newWidth = width;
    newHeight = height;
  }
  
  QPrinter printer(QPrinter::ScreenResolution);
  printer.setOutputFileName(fileName);
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setColorMode(QPrinter::Color);
  printer.printEngine()->setProperty(QPrintEngine::PPK_Creator, pdfCreator);
  printer.printEngine()->setProperty(QPrintEngine::PPK_DocumentName, pdfTitle);
  QRect oldViewport = viewport();
  setViewport(QRect(0, 0, newWidth, newHeight));
#if QT_VERSION < QT_VERSION_CHECK(5, 3, 0)
  printer.setFullPage(true);
  printer.setPaperSize(viewport().size(), QPrinter::DevicePixel);
#else
  QPageLayout pageLayout;
  pageLayout.setMode(QPageLayout::FullPageMode);
  pageLayout.setOrientation(QPageLayout::Portrait);
  pageLayout.setMargins(QMarginsF(0, 0, 0, 0));
  pageLayout.setPageSize(QPageSize(viewport().size(), QPageSize::Point, QString(), QPageSize::ExactMatch));
  printer.setPageLayout(pageLayout);
#endif
  QCPPainter printpainter;
  if (printpainter.begin(&printer))
  {
    printpainter.setMode(QCPPainter::pmVectorized);
    printpainter.setMode(QCPPainter::pmNoCaching);
    printpainter.setMode(QCPPainter::pmNonCosmetic, noCosmeticPen);
    printpainter.setWindow(mViewport);
    if (mBackgroundBrush.style() != Qt::NoBrush &&
        mBackgroundBrush.color() != Qt::white &&
        mBackgroundBrush.color() != Qt::transparent &&
        mBackgroundBrush.color().alpha() > 0) // draw pdf background color if not white/transparent
      printpainter.fillRect(viewport(), mBackgroundBrush);
    draw(&printpainter);
    printpainter.end();
    success = true;
  }
  setViewport(oldViewport);
#endif // QT_NO_PRINTER
  return success;
}

/*!
  Saves a PNG image file to \a fileName on disc. The output plot will have the dimensions \a width
  and \a height in pixels. If either \a width or \a height is zero, the exported image will have
  the same dimensions as the QCustomPlot widget currently has. Line widths and texts etc. are not
  scaled up when larger widths/heights are used. If you want that effect, use the \a scale parameter.

  For example, if you set both \a width and \a height to 100 and \a scale to 2, you will end up with an
  image file of size 200*200 in which all graphical elements are scaled up by factor 2 (line widths,
  texts, etc.). This scaling is not done by stretching a 100*100 image, the result will have full
  200*200 pixel resolution.
  
  If you use a high scaling factor, it is recommended to enable antialiasing for all elements via
  temporarily setting \ref QCustomPlot::setAntialiasedElements to \ref QCP::aeAll as this allows
  QCustomPlot to place objects with sub-pixel accuracy.

  \warning If calling this function inside the constructor of the parent of the QCustomPlot widget
  (i.e. the MainWindow constructor, if QCustomPlot is inside the MainWindow), always provide
  explicit non-zero widths and heights. If you leave \a width or \a height as 0 (default), this
  function uses the current width and height of the QCustomPlot widget. However, in Qt, these
  aren't defined yet inside the constructor, so you would get an image that has strange
  widths/heights.
  
  The objects of the plot will appear in the current selection state. If you don't want any selected
  objects to be painted in their selected look, deselect everything with \ref deselectAll before calling
  this function.

  If you want the PNG to have a transparent background, call \ref setBackground(const QBrush
  &brush) with no brush (Qt::NoBrush) or a transparent color (Qt::transparent), before saving.

  PNG compression can be controlled with the \a quality parameter which must be between 0 and 100 or
  -1 to use the default setting.
  
  Returns true on success. If this function fails, most likely the PNG format isn't supported by
  the system, see Qt docs about QImageWriter::supportedImageFormats().

  \see savePdf, saveBmp, saveJpg, saveRastered
*/
bool QCustomPlot::savePng(const QString &fileName, int width, int height, double scale, int quality)
{
  return saveRastered(fileName, width, height, scale, "PNG", quality);
}

/*!
  Saves a JPG image file to \a fileName on disc. The output plot will have the dimensions \a width
  and \a height in pixels. If either \a width or \a height is zero, the exported image will have
  the same dimensions as the QCustomPlot widget currently has. Line widths and texts etc. are not
  scaled up when larger widths/heights are used. If you want that effect, use the \a scale parameter.

  For example, if you set both \a width and \a height to 100 and \a scale to 2, you will end up with an
  image file of size 200*200 in which all graphical elements are scaled up by factor 2 (line widths,
  texts, etc.). This scaling is not done by stretching a 100*100 image, the result will have full
  200*200 pixel resolution.
  
  If you use a high scaling factor, it is recommended to enable antialiasing for all elements via
  temporarily setting \ref QCustomPlot::setAntialiasedElements to \ref QCP::aeAll as this allows
  QCustomPlot to place objects with sub-pixel accuracy.

  \warning If calling this function inside the constructor of the parent of the QCustomPlot widget
  (i.e. the MainWindow constructor, if QCustomPlot is inside the MainWindow), always provide
  explicit non-zero widths and heights. If you leave \a width or \a height as 0 (default), this
  function uses the current width and height of the QCustomPlot widget. However, in Qt, these
  aren't defined yet inside the constructor, so you would get an image that has strange
  widths/heights.

  The objects of the plot will appear in the current selection state. If you don't want any selected
  objects to be painted in their selected look, deselect everything with \ref deselectAll before calling
  this function.

  JPG compression can be controlled with the \a quality parameter which must be between 0 and 100 or
  -1 to use the default setting.
  
  Returns true on success. If this function fails, most likely the JPG format isn't supported by
  the system, see Qt docs about QImageWriter::supportedImageFormats().

  \see savePdf, savePng, saveBmp, saveRastered
*/
bool QCustomPlot::saveJpg(const QString &fileName, int width, int height, double scale, int quality)
{
  return saveRastered(fileName, width, height, scale, "JPG", quality);
}

/*!
  Saves a BMP image file to \a fileName on disc. The output plot will have the dimensions \a width
  and \a height in pixels. If either \a width or \a height is zero, the exported image will have
  the same dimensions as the QCustomPlot widget currently has. Line widths and texts etc. are not
  scaled up when larger widths/heights are used. If you want that effect, use the \a scale parameter.

  For example, if you set both \a width and \a height to 100 and \a scale to 2, you will end up with an
  image file of size 200*200 in which all graphical elements are scaled up by factor 2 (line widths,
  texts, etc.). This scaling is not done by stretching a 100*100 image, the result will have full
  200*200 pixel resolution.
  
  If you use a high scaling factor, it is recommended to enable antialiasing for all elements via
  temporarily setting \ref QCustomPlot::setAntialiasedElements to \ref QCP::aeAll as this allows
  QCustomPlot to place objects with sub-pixel accuracy.

  \warning If calling this function inside the constructor of the parent of the QCustomPlot widget
  (i.e. the MainWindow constructor, if QCustomPlot is inside the MainWindow), always provide
  explicit non-zero widths and heights. If you leave \a width or \a height as 0 (default), this
  function uses the current width and height of the QCustomPlot widget. However, in Qt, these
  aren't defined yet inside the constructor, so you would get an image that has strange
  widths/heights.

  The objects of the plot will appear in the current selection state. If you don't want any selected
  objects to be painted in their selected look, deselect everything with \ref deselectAll before calling
  this function.
  
  Returns true on success. If this function fails, most likely the BMP format isn't supported by
  the system, see Qt docs about QImageWriter::supportedImageFormats().

  \see savePdf, savePng, saveJpg, saveRastered
*/
bool QCustomPlot::saveBmp(const QString &fileName, int width, int height, double scale)
{
  return saveRastered(fileName, width, height, scale, "BMP");
}

/*! \internal
  
  Returns a minimum size hint that corresponds to the minimum size of the top level layout
  (\ref plotLayout). To prevent QCustomPlot from being collapsed to size/width zero, set a minimum
  size (setMinimumSize) either on the whole QCustomPlot or on any layout elements inside the plot.
  This is especially important, when placed in a QLayout where other components try to take in as
  much space as possible (e.g. QMdiArea).
*/
QSize QCustomPlot::minimumSizeHint() const
{
  return mPlotLayout->minimumSizeHint();
}

/*! \internal
  
  Returns a size hint that is the same as \ref minimumSizeHint.
  
*/
QSize QCustomPlot::sizeHint() const
{
  return mPlotLayout->minimumSizeHint();
}

/*! \internal
  
  Event handler for when the QCustomPlot widget needs repainting. This does not cause a \ref replot, but
  draws the internal buffer on the widget surface.
*/
void QCustomPlot::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);
  QPainter painter(this);
  painter.drawPixmap(0, 0, mPaintBuffer);
}

/*! \internal
  
  Event handler for a resize of the QCustomPlot widget. Causes the internal buffer to be resized to
  the new size. The viewport (which becomes the outer rect of mPlotLayout) is resized
  appropriately. Finally a \ref replot is performed.
*/
void QCustomPlot::resizeEvent(QResizeEvent *event)
{
  // resize and repaint the buffer:
  mPaintBuffer = QPixmap(event->size());
  setViewport(rect());
  replot(rpQueued); // queued update is important here, to prevent painting issues in some contexts
}

/*! \internal
  
 Event handler for when a double click occurs. Emits the \ref mouseDoubleClick signal, then emits
 the specialized signals when certain objecs are clicked (e.g. \ref plottableDoubleClick, \ref
 axisDoubleClick, etc.). Finally determines the affected layout element and forwards the event to
 it.
 
 \see mousePressEvent, mouseReleaseEvent
*/
void QCustomPlot::mouseDoubleClickEvent(QMouseEvent *event)
{
  emit mouseDoubleClick(event);
  
  QVariant details;
  QCPLayerable *clickedLayerable = layerableAt(event->pos(), false, &details);
  
  // emit specialized object double click signals:
  if (QCPAbstractPlottable *ap = qobject_cast<QCPAbstractPlottable*>(clickedLayerable))
    emit plottableDoubleClick(ap, event);
  else if (QCPAxis *ax = qobject_cast<QCPAxis*>(clickedLayerable))
    emit axisDoubleClick(ax, details.value<QCPAxis::SelectablePart>(), event);
  else if (QCPAbstractItem *ai = qobject_cast<QCPAbstractItem*>(clickedLayerable))
    emit itemDoubleClick(ai, event);
  else if (QCPLegend *lg = qobject_cast<QCPLegend*>(clickedLayerable))
    emit legendDoubleClick(lg, 0, event);
  else if (QCPAbstractLegendItem *li = qobject_cast<QCPAbstractLegendItem*>(clickedLayerable))
    emit legendDoubleClick(li->parentLegend(), li, event);
  else if (QCPPlotTitle *pt = qobject_cast<QCPPlotTitle*>(clickedLayerable))
    emit titleDoubleClick(event, pt);
  
  // call double click event of affected layout element:
  if (QCPLayoutElement *el = layoutElementAt(event->pos()))
    el->mouseDoubleClickEvent(event);
  
  // call release event of affected layout element (as in mouseReleaseEvent, since the mouseDoubleClick replaces the second release event in double click case):
  if (mMouseEventElement)
  {
    mMouseEventElement->mouseReleaseEvent(event);
    mMouseEventElement = 0;
  }
  
  //QWidget::mouseDoubleClickEvent(event); don't call base class implementation because it would just cause a mousePress/ReleaseEvent, which we don't want.
}

/*! \internal
  
  Event handler for when a mouse button is pressed. Emits the mousePress signal. Then determines
  the affected layout element and forwards the event to it.
  
  \see mouseMoveEvent, mouseReleaseEvent
*/
void QCustomPlot::mousePressEvent(QMouseEvent *event)
{
  emit mousePress(event);
  mMousePressPos = event->pos(); // need this to determine in releaseEvent whether it was a click (no position change between press and release)
  
  // call event of affected layout element:
  mMouseEventElement = layoutElementAt(event->pos());
  if (mMouseEventElement)
    mMouseEventElement->mousePressEvent(event);
  
  QWidget::mousePressEvent(event);
}

/*! \internal
  
  Event handler for when the cursor is moved. Emits the \ref mouseMove signal.

  If a layout element has mouse capture focus (a mousePressEvent happened on top of the layout
  element before), the mouseMoveEvent is forwarded to that element.
  
  \see mousePressEvent, mouseReleaseEvent
*/
void QCustomPlot::mouseMoveEvent(QMouseEvent *event)
{
  emit mouseMove(event);

  // call event of affected layout element:
  if (mMouseEventElement)
    mMouseEventElement->mouseMoveEvent(event);
  
  QWidget::mouseMoveEvent(event);
}

/*! \internal
  
  Event handler for when a mouse button is released. Emits the \ref mouseRelease signal.
  
  If the mouse was moved less than a certain threshold in any direction since the \ref
  mousePressEvent, it is considered a click which causes the selection mechanism (if activated via
  \ref setInteractions) to possibly change selection states accordingly. Further, specialized mouse
  click signals are emitted (e.g. \ref plottableClick, \ref axisClick, etc.)
  
  If a layout element has mouse capture focus (a \ref mousePressEvent happened on top of the layout
  element before), the \ref mouseReleaseEvent is forwarded to that element.
  
  \see mousePressEvent, mouseMoveEvent
*/
void QCustomPlot::mouseReleaseEvent(QMouseEvent *event)
{
  emit mouseRelease(event);
  bool doReplot = false;
  
  if ((mMousePressPos-event->pos()).manhattanLength() < 5) // determine whether it was a click operation
  {
    if (event->button() == Qt::LeftButton)
    {
      // handle selection mechanism:
      QVariant details;
      QCPLayerable *clickedLayerable = layerableAt(event->pos(), true, &details);
      bool selectionStateChanged = false;
      bool additive = mInteractions.testFlag(QCP::iMultiSelect) && event->modifiers().testFlag(mMultiSelectModifier);
      // deselect all other layerables if not additive selection:
      if (!additive)
      {
        foreach (QCPLayer *layer, mLayers)
        {
          foreach (QCPLayerable *layerable, layer->children())
          {
            if (layerable != clickedLayerable && mInteractions.testFlag(layerable->selectionCategory()))
            {
              bool selChanged = false;
              layerable->deselectEvent(&selChanged);
              selectionStateChanged |= selChanged;
            }
          }
        }
      }
      if (clickedLayerable && mInteractions.testFlag(clickedLayerable->selectionCategory()))
      {
        // a layerable was actually clicked, call its selectEvent:
        bool selChanged = false;
        clickedLayerable->selectEvent(event, additive, details, &selChanged);
        selectionStateChanged |= selChanged;
      }
      if (selectionStateChanged)
      {
        doReplot = true;
        emit selectionChangedByUser();
      }
    }
    
    // emit specialized object click signals:
    QVariant details;
    QCPLayerable *clickedLayerable = layerableAt(event->pos(), false, &details); // for these signals, selectability is ignored, that's why we call this again with onlySelectable set to false
    if (QCPAbstractPlottable *ap = qobject_cast<QCPAbstractPlottable*>(clickedLayerable))
      emit plottableClick(ap, event);
    else if (QCPAxis *ax = qobject_cast<QCPAxis*>(clickedLayerable))
      emit axisClick(ax, details.value<QCPAxis::SelectablePart>(), event);
    else if (QCPAbstractItem *ai = qobject_cast<QCPAbstractItem*>(clickedLayerable))
      emit itemClick(ai, event);
    else if (QCPLegend *lg = qobject_cast<QCPLegend*>(clickedLayerable))
      emit legendClick(lg, 0, event);
    else if (QCPAbstractLegendItem *li = qobject_cast<QCPAbstractLegendItem*>(clickedLayerable))
      emit legendClick(li->parentLegend(), li, event);
    else if (QCPPlotTitle *pt = qobject_cast<QCPPlotTitle*>(clickedLayerable))
      emit titleClick(event, pt);
  }
  
  // call event of affected layout element:
  if (mMouseEventElement)
  {
    mMouseEventElement->mouseReleaseEvent(event);
    mMouseEventElement = 0;
  }
  
  if (doReplot || noAntialiasingOnDrag())
    replot();
  
  QWidget::mouseReleaseEvent(event);
}

/*! \internal
  
  Event handler for mouse wheel events. First, the \ref mouseWheel signal is emitted. Then
  determines the affected layout element and forwards the event to it.
  
*/
void QCustomPlot::wheelEvent(QWheelEvent *event)
{
  emit mouseWheel(event);
  
  // call event of affected layout element:
  if (QCPLayoutElement *el = layoutElementAt(event->pos()))
    el->wheelEvent(event);
  
  QWidget::wheelEvent(event);
}

/*! \internal
  
  This is the main draw function. It draws the entire plot, including background pixmap, with the
  specified \a painter. Note that it does not fill the background with the background brush (as the
  user may specify with \ref setBackground(const QBrush &brush)), this is up to the respective
  functions calling this method (e.g. \ref replot, \ref toPixmap and \ref toPainter).
*/
void QCustomPlot::draw(QCPPainter *painter)
{
  // run through layout phases:
  mPlotLayout->update(QCPLayoutElement::upPreparation);
  mPlotLayout->update(QCPLayoutElement::upMargins);
  mPlotLayout->update(QCPLayoutElement::upLayout);
  
  // draw viewport background pixmap:
  drawBackground(painter);

  // draw all layered objects (grid, axes, plottables, items, legend,...):
  foreach (QCPLayer *layer, mLayers)
  {
    foreach (QCPLayerable *child, layer->children())
    {
      if (child->realVisibility())
      {
        painter->save();
        painter->setClipRect(child->clipRect().translated(0, -1));
        child->applyDefaultAntialiasingHint(painter);
        child->draw(painter);
        painter->restore();
      }
    }
  }
  
  /* Debug code to draw all layout element rects
  foreach (QCPLayoutElement* el, findChildren<QCPLayoutElement*>())
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(0, 0, 0, 100), 0, Qt::DashLine));
    painter->drawRect(el->rect());
    painter->setPen(QPen(QColor(255, 0, 0, 100), 0, Qt::DashLine));
    painter->drawRect(el->outerRect());
  }
  */
}

/*! \internal
  
  Draws the viewport background pixmap of the plot.
  
  If a pixmap was provided via \ref setBackground, this function buffers the scaled version
  depending on \ref setBackgroundScaled and \ref setBackgroundScaledMode and then draws it inside
  the viewport with the provided \a painter. The scaled version is buffered in
  mScaledBackgroundPixmap to prevent expensive rescaling at every redraw. It is only updated, when
  the axis rect has changed in a way that requires a rescale of the background pixmap (this is
  dependent on the \ref setBackgroundScaledMode), or when a differend axis background pixmap was
  set.
  
  Note that this function does not draw a fill with the background brush (\ref setBackground(const
  QBrush &brush)) beneath the pixmap.
  
  \see setBackground, setBackgroundScaled, setBackgroundScaledMode
*/
void QCustomPlot::drawBackground(QCPPainter *painter)
{
  // Note: background color is handled in individual replot/save functions

  // draw background pixmap (on top of fill, if brush specified):
  if (!mBackgroundPixmap.isNull())
  {
    if (mBackgroundScaled)
    {
      // check whether mScaledBackground needs to be updated:
      QSize scaledSize(mBackgroundPixmap.size());
      scaledSize.scale(mViewport.size(), mBackgroundScaledMode);
      if (mScaledBackgroundPixmap.size() != scaledSize)
        mScaledBackgroundPixmap = mBackgroundPixmap.scaled(mViewport.size(), mBackgroundScaledMode, Qt::SmoothTransformation);
      painter->drawPixmap(mViewport.topLeft(), mScaledBackgroundPixmap, QRect(0, 0, mViewport.width(), mViewport.height()) & mScaledBackgroundPixmap.rect());
    } else
    {
      painter->drawPixmap(mViewport.topLeft(), mBackgroundPixmap, QRect(0, 0, mViewport.width(), mViewport.height()));
    }
  }
}


/*! \internal
  
  This method is used by \ref QCPAxisRect::removeAxis to report removed axes to the QCustomPlot
  so it may clear its QCustomPlot::xAxis, yAxis, xAxis2 and yAxis2 members accordingly.
*/
void QCustomPlot::axisRemoved(QCPAxis *axis)
{
  if (xAxis == axis)
    xAxis = 0;
  if (xAxis2 == axis)
    xAxis2 = 0;
  if (yAxis == axis)
    yAxis = 0;
  if (yAxis2 == axis)
    yAxis2 = 0;
  
  // Note: No need to take care of range drag axes and range zoom axes, because they are stored in smart pointers
}

/*! \internal
  
  This method is used by the QCPLegend destructor to report legend removal to the QCustomPlot so
  it may clear its QCustomPlot::legend member accordingly.
*/
void QCustomPlot::legendRemoved(QCPLegend *legend)
{
  if (this->legend == legend)
    this->legend = 0;
}

/*! \internal
  
  Assigns all layers their index (QCPLayer::mIndex) in the mLayers list. This method is thus called
  after every operation that changes the layer indices, like layer removal, layer creation, layer
  moving.
*/
void QCustomPlot::updateLayerIndices() const
{
  for (int i=0; i<mLayers.size(); ++i)
    mLayers.at(i)->mIndex = i;
}

/*! \internal
  
  Returns the layerable at pixel position \a pos. If \a onlySelectable is set to true, only those
  layerables that are selectable will be considered. (Layerable subclasses communicate their
  selectability via the QCPLayerable::selectTest method, by returning -1.)

  \a selectionDetails is an output parameter that contains selection specifics of the affected
  layerable. This is useful if the respective layerable shall be given a subsequent
  QCPLayerable::selectEvent (like in \ref mouseReleaseEvent). \a selectionDetails usually contains
  information about which part of the layerable was hit, in multi-part layerables (e.g.
  QCPAxis::SelectablePart).
*/
QCPLayerable *QCustomPlot::layerableAt(const QPointF &pos, bool onlySelectable, QVariant *selectionDetails) const
{
  for (int layerIndex=mLayers.size()-1; layerIndex>=0; --layerIndex)
  {
    const QList<QCPLayerable*> layerables = mLayers.at(layerIndex)->children();
    double minimumDistance = selectionTolerance()*1.1;
    QCPLayerable *minimumDistanceLayerable = 0;
    for (int i=layerables.size()-1; i>=0; --i)
    {
      if (!layerables.at(i)->realVisibility())
        continue;
      QVariant details;
      double dist = layerables.at(i)->selectTest(pos, onlySelectable, &details);
      if (dist >= 0 && dist < minimumDistance)
      {
        minimumDistance = dist;
        minimumDistanceLayerable = layerables.at(i);
        if (selectionDetails) *selectionDetails = details;
      }
    }
    if (minimumDistance < selectionTolerance())
      return minimumDistanceLayerable;
  }
  return 0;
}

/*!
  Saves the plot to a rastered image file \a fileName in the image format \a format. The plot is
  sized to \a width and \a height in pixels and scaled with \a scale. (width 100 and scale 2.0 lead
  to a full resolution file with width 200.) If the \a format supports compression, \a quality may
  be between 0 and 100 to control it.
  
  Returns true on success. If this function fails, most likely the given \a format isn't supported
  by the system, see Qt docs about QImageWriter::supportedImageFormats().
  
  \see saveBmp, saveJpg, savePng, savePdf
*/
bool QCustomPlot::saveRastered(const QString &fileName, int width, int height, double scale, const char *format, int quality)
{
  QPixmap buffer = toPixmap(width, height, scale);
  if (!buffer.isNull())
    return buffer.save(fileName, format, quality);
  else
    return false;
}

/*!
  Renders the plot to a pixmap and returns it.
  
  The plot is sized to \a width and \a height in pixels and scaled with \a scale. (width 100 and
  scale 2.0 lead to a full resolution pixmap with width 200.)
  
  \see toPainter, saveRastered, saveBmp, savePng, saveJpg, savePdf
*/
QPixmap QCustomPlot::toPixmap(int width, int height, double scale)
{
  // this method is somewhat similar to toPainter. Change something here, and a change in toPainter might be necessary, too.
  int newWidth, newHeight;
  if (width == 0 || height == 0)
  {
    newWidth = this->width();
    newHeight = this->height();
  } else
  {
    newWidth = width;
    newHeight = height;
  }
  int scaledWidth = qRound(scale*newWidth);
  int scaledHeight = qRound(scale*newHeight);

  QPixmap result(scaledWidth, scaledHeight);
  result.fill(mBackgroundBrush.style() == Qt::SolidPattern ? mBackgroundBrush.color() : Qt::transparent); // if using non-solid pattern, make transparent now and draw brush pattern later
  QCPPainter painter;
  painter.begin(&result);
  if (painter.isActive())
  {
    QRect oldViewport = viewport();
    setViewport(QRect(0, 0, newWidth, newHeight));
    painter.setMode(QCPPainter::pmNoCaching);
    if (!qFuzzyCompare(scale, 1.0))
    {
      if (scale > 1.0) // for scale < 1 we always want cosmetic pens where possible, because else lines might disappear for very small scales
        painter.setMode(QCPPainter::pmNonCosmetic);
      painter.scale(scale, scale);
    }
    if (mBackgroundBrush.style() != Qt::SolidPattern && mBackgroundBrush.style() != Qt::NoBrush) // solid fills were done a few lines above with QPixmap::fill
      painter.fillRect(mViewport, mBackgroundBrush);
    draw(&painter);
    setViewport(oldViewport);
    painter.end();
  } else // might happen if pixmap has width or height zero
  {
    qDebug() << Q_FUNC_INFO << "Couldn't activate painter on pixmap";
    return QPixmap();
  }
  return result;
}

/*!
  Renders the plot using the passed \a painter.
  
  The plot is sized to \a width and \a height in pixels. If the \a painter's scale is not 1.0, the resulting plot will
  appear scaled accordingly.
  
  \note If you are restricted to using a QPainter (instead of QCPPainter), create a temporary QPicture and open a QCPPainter
  on it. Then call \ref toPainter with this QCPPainter. After ending the paint operation on the picture, draw it with
  the QPainter. This will reproduce the painter actions the QCPPainter took, with a QPainter.
  
  \see toPixmap
*/
void QCustomPlot::toPainter(QCPPainter *painter, int width, int height)
{
  // this method is somewhat similar to toPixmap. Change something here, and a change in toPixmap might be necessary, too.
  int newWidth, newHeight;
  if (width == 0 || height == 0)
  {
    newWidth = this->width();
    newHeight = this->height();
  } else
  {
    newWidth = width;
    newHeight = height;
  }

  if (painter->isActive())
  {
    QRect oldViewport = viewport();
    setViewport(QRect(0, 0, newWidth, newHeight));
    painter->setMode(QCPPainter::pmNoCaching);
    if (mBackgroundBrush.style() != Qt::NoBrush) // unlike in toPixmap, we can't do QPixmap::fill for Qt::SolidPattern brush style, so we also draw solid fills with fillRect here
      painter->fillRect(mViewport, mBackgroundBrush);
    draw(painter);
    setViewport(oldViewport);
  } else
    qDebug() << Q_FUNC_INFO << "Passed painter is not active";
}
