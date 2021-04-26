.. _lib_nrf91_lwm2m_app:

nRF91 LwM2M client application
##############################

.. contents::
   :local:
   :depth: 2

The :ref:`lwm2m_client` demonstrates how to use Lightweight Machine to Machine (`LwM2M`_) protocol to connect an
nRF91 based device to an LwM2M server such as `Leshan Demo Server`_ via LTE. Once connected, the device can be queried
to obtain location data, and to retrieve information about the modem.

This library provides a basic set of LwM2M objects combined together to avoid the need to duplicate these
in all of the applications planning to communicate with an LwM2M server. Following fixed set of readily initialized
objects are at users exposal.

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

Defining your own resources
***************************

This section will walk through a process of creating a custom LwM2M resource. For most application, the
business logic may be implemented inside custom resources, which are either proprietary to this application
or may be following some external specification, like IPSO objects. Either way, they extend the resource tree
by defining own objects on top of what LwM2M already defines.

Before defining a proprietary resource, it is a good practice to check whether similar functionality is
already defined in `LwM2M Object and Resource Registry`_.
If there is already such definition you might want to use that, instead of defining your own.

Please note that Zephyr's LwM2M library has only a limited support for some IPSO objects, so extending beyound those,
requires changes to the internal engine as well.

For this example, we are going to demonstrate how do we define a resource that follow Generic Sensor definition from IPSO.
First, we look for "Generic Sensor" from the `LwM2M Object and Resource Registry`_ list
which would give us that the resource object ID would be `3300`. Within that list, we could open a `raw XML`_ file
or open a bit more human readable view from `the LwM2M editor`_ to see what resources are defined under the object ID 3300.

.. _LwM2M Object and Resource Registry: http://openmobilealliance.org/wp/OMNA/LwM2M/LwM2MRegistry.html
.. _raw XML: https://raw.githubusercontent.com/OpenMobileAlliance/lwm2m-registry/prod/version_history/3300-1_0.xml
.. _the LwM2M editor: http://devtoolkit.openmobilealliance.org/OEditor/LWMOView?url=https%3A%2F%2Fraw.githubusercontent.com%2FOpenMobileAlliance%2Flwm2m-registry%2Fprod%2Fversion_history%2F3300-1_0.xml

.. list-table:: Resource Definitions
   :header-rows: 1
   :widths: auto

   * - ID
     - Name
     - Operations
     - Instances
     - Mandatory
     - Type
     - Description
   * - 5700
     - Sensor Value
     - R
     - Single
     - Mandatory
     - Float
     - Last or Current Measured Value from the Sensor.
   * - 5701
     - Sensor Units
     - R
     - Single
     - Optional
     - String
     - Measurement Units Definition.
   * - 5601
     - Min Measured Value
     - R
     - Single
     - Optional
     - Float
     - The minimum value measured by the sensor since power ON or reset.
   * - 5602
     - Max Measured Value
     - R
     - Single
     - Optional
     - Float
     - The maximum value measured by the sensor since power ON or reset.

From the table above, we can see that resource ID of our sensor value would be `5700`.
For the purpose of the tutorial, we only demonstrate that one resource, which is mandatory.
As the LwM2M uses resource paths in form of `object ID/instance/resource ID` and our object ID is 3300
and of course this is our first instance. Therefore our full path for the sensor value would be
`3300/0/5700`. You need to use this path in the LwM2M library API.

First thing we do, is to have read-function for our data. It is out of scope for this tutorial
how the values are exactly generated, so we are only interested how LwM2M library uses those values. The data
type of our resource is float, so we need to use following struct to store our float values:

.. code:: c

   typedef struct float32_value {
      int32_t val1;
      int32_t val2;
   } float32_value_t;

Where `val1` would be our integer part of the value. Then `val2` would be our decimal part, multiplied
by constant `LWM2M_FLOAT32_DEC_MAX` which is equal to `1000000`.

Then we can define our function that responds to read requests coming from server:

.. code:: c

   static struct float32_value sensor_value = { 1, 0 };

   static void *read_cb(uint16_t obj_inst_id, uint16_t res_id, uint16_t res_inst_id,
               size_t *data_len)
   {
      /* Only object instance 0 is currently used */
      if (obj_inst_id != 0) {
         *data_len = 0;
         return NULL;
      }

      /* Demo: change the sensor value */
      ++sensor_value.val2;

      /* Return sensor value for the LwM2M library */
      lwm2m_engine_set_float32("3300/0/5700", &sensor_value);
      *data_len = sizeof(sensor_value);
      return &sensor_value;
   }

And to get that function called and our sensor published, we introduce our resource to LwM2M library:

.. code:: c

   int init_resource(void)
   {
      lwm2m_engine_create_obj_inst("3300/0");
      lwm2m_engine_register_read_callback("3300/0/5700", read_cb);
      return 0;
   }

As show above, first we need to create an object and then register callbacks for specific resources. This causes internal engine to allocate memory and
create all the resources that are defined for such IPSO object ID. For this to work, this sensor type should be already
supported by the library. For generic sensor, the support is controlled by Kconfig option `CONFIG_LWM2M_IPSO_GENERIC_SENSOR=y`.

That should be all that there is required to get our generic sensor working. For defining outputs,
process is very much similar, but instead of read callback, we would be defining write callback as well.

API documentation
*****************

| Header files: :file:`include/net/nrf91_lwm2m_app.h`, :file:`include/net/nrf91_lwm2m_fota.h`
| Source files: :file:`lib/nrf91_lwm2m_app/fota`, :file:`lib/nrf91_lwm2m_app/lwm2m`

.. doxygengroup:: nrf91_lwm2m_app
   :project: nrf
   :members:
   :inner:
