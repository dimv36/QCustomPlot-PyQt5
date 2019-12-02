/** PyQt5 binding for QCustomPlot v2.0.0
 *  
 *  Authors: Dmitry Voronin, Giuseppe Corbelli
 *  License: MIT
 *  
 *  QCustomPlot author: Emanuel Eichhammer
 *  QCustomPlot Website/Contact: http://www.qcustomplot.com
 */


%MappedType QMap<double, QColor>
{
%TypeHeaderCode
// Include the library interface to the type being mapped.
#include <QMap>
#include <QColor>
%End

%ConvertFromTypeCode
    PyObject *d = PyDict_New();

    if (!d)
        return 0;

    QMap<double, QColor>::const_iterator it = sipCpp->constBegin();
    QMap<double, QColor>::const_iterator end = sipCpp->constEnd();

    while (it != end)
    {
        PyObject *kobj = PyFloat_FromDouble(it.key());

        if (!kobj)
        {
            Py_DECREF(d);
            return 0;
        }

        QColor *v = new QColor(it.value());
        PyObject *vobj = sipConvertFromNewType(v, sipType_QColor,
                sipTransferObj);

        if (!vobj)
        {
            delete v;
            Py_DECREF(kobj);
            Py_DECREF(d);
            return 0;
        }

        int rc = PyDict_SetItem(d, kobj, vobj);

        Py_DECREF(vobj);
        Py_DECREF(kobj);

        if (rc < 0)
        {
            Py_DECREF(d);
            return 0;
        }

        ++it;
    }

    return d;
%End

%ConvertToTypeCode
    if (!sipIsErr)
        return PyDict_Check(sipPy);

    QMap<double, QColor> *qm = new QMap<double, QColor>;

    SIP_SSIZE_T pos = 0;
    PyObject *kobj, *vobj;

    while (PyDict_Next(sipPy, &pos, &kobj, &vobj))
    {
        PyErr_Clear();
        double k = PyFloat_AsDouble(kobj);

        if (PyErr_Occurred())
        {
            PyErr_Format(PyExc_TypeError,
                         "a key has type '%s' but 'float' is expected",
                         Py_TYPE(kobj)->tp_name);

            delete qm;
            *sipIsErr = 1;

            return 0;
        }

        int vstate;
        QColor* v = reinterpret_cast<QColor*>(
            sipForceConvertToType(
                vobj, sipType_QColor, sipTransferObj, SIP_NOT_NONE, &vstate, sipIsErr
            )
        );

        if (*sipIsErr)
        {
            PyErr_Format(PyExc_TypeError,
                         "a value has type '%s' but 'QColor' is expected",
                         Py_TYPE(vobj)->tp_name);
            sipReleaseType(v, sipType_QColor, vstate);
            delete qm;
            return 0;
        }

        qm->insert(k, *v);

        sipReleaseType(v, sipType_QColor, vstate);
    }

    *sipCppPtr = qm;

    return sipGetState(sipTransferObj);
%End
};


class QCPColorGradient
{
%TypeHeaderCode
#include <QCustomPlot/src/colorgradient.h>
%End
public:
  enum ColorInterpolation { ciRGB  ///< Color channels red, green and blue are linearly interpolated
                            ,ciHSV ///< Color channels hue, saturation and value are linearly interpolated (The hue is interpolated over the shortest angle distance)
                          };
  
  enum GradientPreset { gpGrayscale  ///< Continuous lightness from black to white (suited for non-biased data representation)
                        ,gpHot       ///< Continuous lightness from black over firey colors to white (suited for non-biased data representation)
                        ,gpCold      ///< Continuous lightness from black over icey colors to white (suited for non-biased data representation)
                        ,gpNight     ///< Continuous lightness from black over weak blueish colors to white (suited for non-biased data representation)
                        ,gpCandy     ///< Blue over pink to white
                        ,gpGeography ///< Colors suitable to represent different elevations on geographical maps
                        ,gpIon       ///< Half hue spectrum from black over purple to blue and finally green (creates banding illusion but allows more precise magnitude estimates)
                        ,gpThermal   ///< Colors suitable for thermal imaging, ranging from dark blue over purple to orange, yellow and white
                        ,gpPolar     ///< Colors suitable to emphasize polarity around the center, with blue for negative, black in the middle and red for positive values
                        ,gpSpectrum  ///< An approximation of the visible light spectrum (creates banding illusion but allows more precise magnitude estimates)
                        ,gpJet       ///< Hue variation similar to a spectrum, often used in numerical visualization (creates banding illusion but allows more precise magnitude estimates)
                        ,gpHues      ///< Full hue cycle, with highest and lowest color red (suitable for periodic data, such as angles and phases, see \ref setPeriodic)
                      };
  
  QCPColorGradient();
  QCPColorGradient(GradientPreset preset);
  bool operator==(const QCPColorGradient &other) const;
  bool operator!=(const QCPColorGradient &other) const;
  
  // getters:
  int levelCount() const;
  QMap<double, QColor> colorStops() const;
  ColorInterpolation colorInterpolation() const;
  bool periodic() const;
  
  // setters:
  void setLevelCount(int n);
  void setColorStops(const QMap<double, QColor> &colorStops);
  void setColorStopAt(double position, const QColor &color);
  void setColorInterpolation(ColorInterpolation interpolation);
  void setPeriodic(bool enabled);
  
  // non-property methods:
  void colorize(const double *data, const QCPRange &range, QRgb *scanLine, int n, int dataIndexFactor=1, bool logarithmic=false);
  void colorize(const double *data, const unsigned char *alpha, const QCPRange &range, QRgb *scanLine, int n, int dataIndexFactor=1, bool logarithmic=false);
  QRgb color(double position, const QCPRange &range, bool logarithmic=false);
  void loadPreset(GradientPreset preset);
  void clearColorStops();
  QCPColorGradient inverted() const;
};
