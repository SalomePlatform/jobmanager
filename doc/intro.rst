Introduction: What is the JOBMANAGER ?
======================================

The JOBMANAGER module allows creating, launching and following 
calculation jobs on different types of computers. 

.. figure:: images/jobmanager_overview.png
  :scale: 75 %
  :align: center

  **Overview of the JOBMANAGER module**


Main functionalities
++++++++++++++++++++

The JOBMANAGER module permits to define three types of jobs:

- User scripts
- Python scripts launched in a SALOME session
- YACS schemas

The module handle different types of computers:

- Interactive computers (rsh, ssh)
- Clusters managed by batch systems like PBS, LSF or SGE

User's job list could be saved and loaded. Finally, the module provides
an editor for managing user's SALOME resource (See resource in the KERNEL documentation).
