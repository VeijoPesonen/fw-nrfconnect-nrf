.. _lib_nrf91_lwm2m_app:

nRF91 LwM2M client application
##############################

.. contents::
   :local:
   :depth: 2

The :ref:`lwm2m_client` demonstrates how to use Lightweight Machine to Machine (`LwM2M`_) protocol to connect an
nRF91 based device to an LwM2M server such as `Leshan Demo Server`_ via LTE. Once connected, the device can be queried
to obtain location data, and to retrieve information about the modem.

This library provides a basic set of LwM2M resources combined together to avoid the need to duplicate these
in all of the applications planning to communicate with an LwM2M server. Following fixed set of readily initialized
resources are at users exposal.

* Firmware object
* Connection monitoring object
* Device object
* Location object
* Security object

Here given items are not an exhaustive list of resources a device is expected to support. A user application can, and is
expected to, define their additional set of resources based on the device's capabilities and on the use case in question.

.. figure:: /images/lib_nrf91_lwm2m_app.png
      :alt: nRF91 LwM2M application software stack

Requirements
************
The library supports the following development kit:

.. table-from-rows:: /includes/sample_board_rows.txt
   :header: heading
   :rows: nrf9160dk_nrf9160ns, thingy91_nrf9160ns

Configuration and implementation
********************************
The nRF91 LwM2M client application library is used with the :ref:`lwm2m_client` sample. The sample could be used as a
starting for creating your own application.

By default, the library uses LTE-M for connecting and doesn't utilize a bootstrap server. LwM2M Queue mode is not in use.
To use NB-IoT, a bootstrap server, or the queue mode please refer to the forementioned :ref:`lwm2m_client` sample to
see how these features can be taken into use.

API documentation
*****************

| Header files: :file:`include/net/nrf91_lwm2m_app.h`, :file:`include/net/nrf91_lwm2m_fota.h`
| Source files: :file:`lib/nrf91_lwm2m_app/fota`, :file:`lib/nrf91_lwm2m_app/lwm2m`

.. doxygengroup:: nrf91_lwm2m_app
   :project: nrf
   :inner:
