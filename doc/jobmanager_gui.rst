Using the JOBMANAGER GUI
========================

Managing a job with the GUI
---------------------------

Job creation workflow
---------------------

This section describes the workflow when a job is created. The jobmanager uses a wizard to create a job.

The first page (see :ref:`figure_jobmanager_job_workflow_1`) of the wizard is to define the **job name** 
and the **job type**. For each job type, the page provides an explanation of what job type refers.

.. _figure_jobmanager_job_workflow_1:

.. figure:: images/jobmanager_job_workflow_1.png
  :align: center

  **Page 1**

The second page (see :ref:`figure_jobmanager_job_workflow_2`) permits to add two files, the main job file 
(in this example a command file). You could also add an environment file, it is not mandatory.

.. _figure_jobmanager_job_workflow_2:

.. figure:: images/jobmanager_job_workflow_2.png
  :align: center

  **Page 2**

The third page (see :ref:`figure_jobmanager_job_workflow_3`) permits to define the batch parameters related
to your job. You have to define the *Remote work directory*. It's the directory where the job will be executed.
Input files defined in page 4 will be copied in this directory. You have define how many times the job will be running,
the amount of memory needed and the number of processors/cores the job need.

.. _figure_jobmanager_job_workflow_3:

.. figure:: images/jobmanager_job_workflow_3.png
  :align: center

  **Page 3**

The fourth page (see :ref:`figure_jobmanager_job_workflow_4`) permits to add the input and output files. 
Input files are files located into the user
computer that have to be transfered into the execution resource. In this page, you also the result 
directory where job results and logs will be copied.

.. _figure_jobmanager_job_workflow_4:

.. figure:: images/jobmanager_job_workflow_4.png
  :align: center

  **Page 4**

The fith page (see :ref:`figure_jobmanager_job_workflow_5`) permits to choose the resource in which the job 
will executed. You can also define the batch queue that you want to use.

.. _figure_jobmanager_job_workflow_5:

.. figure:: images/jobmanager_job_workflow_5.png
  :align: center

  **Page 5**

The last page (see :ref:`figure_jobmanager_job_workflow_6`) finalize the job creation. You could choose 
if you want or not start the job at the end of the wizard.

.. _figure_jobmanager_job_workflow_6:

.. figure:: images/jobmanager_job_workflow_6.png
  :align: center

  **Page 6**

Loading and saving job list
---------------------------

The JOBMANAGER permits to save and load a list of jobs.
For some jobs, this feature permits to follow the execution of a job
into different SALOME session.

The figure :ref:`figure_jobmanager_load_save_buttons` shows where are located 
the load and save buttons in the JOBMANAGER GUI.

.. _figure_jobmanager_load_save_buttons:

.. figure:: images/jobmanager_load_save_buttons.png
  :align: center

  **Location of load and save job list**

All jobs cannot be followed between to SALOME session. Indeed, It's the *batch* type
of the resource that allows to know if you can or not follow a job. Currently, resources
that use **ssh** for batch configuration cannot be followed. In this case, when the JOBMANAGER
load the job, it will set this kind job in the **Error** state.

