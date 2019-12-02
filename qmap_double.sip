// QMap<double, T> is implemented as a Python dictionary.

template<int, _TYPE_>
%MappedType QMap<double, _TYPE_> /TypeHint="Dict[float, _TYPE_]", TypeHintValue="{}"/
{
%TypeHeaderCode
#include <qmap.h>
%End

%ConvertFromTypeCode
    // Create the dictionary.
    PyObject *d = PyDict_New();

    if (!d)
        return NULL;

    // Set the dictionary elements.
    QMap<double, _TYPE_>::const_iterator it = sipCpp->constBegin();

    while (it != sipCpp->constEnd())
    {
        PyObject *kobj = PyFloat_FromDouble(it.key());
        if (!kobj) {
            Py_DECREF(d);
            return NULL;
        }

        _TYPE_ *v = new _TYPE_(it.value());
        PyObject *vobj = sipConvertFromNewType(v, sipType__TYPE_, sipTransferObj);
        if (!vobj) {
            delete v;
            Py_DECREF(kobj);
            Py_DECREF(d);
            return NULL;
        }

        int rc = PyDict_SetItem(d, kobj, vobj);
        Py_DECREF(vobj);
        Py_DECREF(kobj);
        if (rc < 0) {
            Py_DECREF(d);
            return NULL;
        }
        ++it;
    }
    return d;
%End

%ConvertToTypeCode
    if (!sipIsErr)
        return PyDict_Check(sipPy);

    QMap<double, _TYPE_> *qm = new QMap<double, _TYPE_>;

    Py_ssize_t pos = 0;
    PyObject *kobj, *vobj;

    while (PyDict_Next(sipPy, &pos, &kobj, &vobj)) {
        double k {PyFloat_AS_DOUBLE(kobj)};

        if (PyErr_Occurred())
        {
            if (PyErr_ExceptionMatches(PyExc_TypeError))
                PyErr_Format(PyExc_TypeError,
                        "a dict key has type '%s' but 'double' is expected",
                        sipPyTypeName(Py_TYPE(kobj)));

            delete qm;
            *sipIsErr = 1;

            return 0;
        }

        int vstate;
        _TYPE_ *v = reinterpret_cast<_TYPE_ *>(
                sipForceConvertToType(vobj, sipType__TYPE_, sipTransferObj,
                        SIP_NOT_NONE, &vstate, sipIsErr));

        if (*sipIsErr)
        {
            PyErr_Format(PyExc_TypeError,
                    "a dict value has type '%s' but '_TYPE_' is expected",
                    sipPyTypeName(Py_TYPE(vobj)));

            delete qm;

            return 0;
        }

        qm->insert(k, *v);

        sipReleaseType(v, sipType__TYPE_, vstate);
    }

    *sipCppPtr = qm;

    return sipGetState(sipTransferObj);
%End
};
