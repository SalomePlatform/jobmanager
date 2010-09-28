Managing SALOME resource with the JOBAMANAGER
=============================================

The JOBAMANAGER provides a GUI to manage SALOME resources.
Later in the chapter, we use resource for SALOME resource.

Definition of a SALOME resource
+++++++++++++++++++++++++++++++

A resource is the SALOME abstraction for managing computers.
A resource contains three different kinds of informations:

- A name.
- A physical description of the computer.
- A description of SALOME installation in this computer.

A resource's name could be different from the computer name since you could
have different SALOME installation in the computer.

Physical description of the computer
------------------------------------

A resource contains a physical description of the computer.
These informations are used by the resource manager (service provided
by the KERNEL) to choose and to user a resource when a container (in YACS)
or a job (in JOBMANAGER) has to be launched.

- **hostname:** It's the network name of the computer. If the computer is a cluster, you
  have to give the frontal computer name.

- **protocol:**
- **username:**
- **batch:**
- **iprotocol:**
- **mpiImpl:**

- **OS:** It's the operating system name, ex: Linux, Windows, Debian.
- **nb_node:** It's the amount of node of the computer.
- **nb_proc_per_node:** It's the amount or processor or core of your computer.
- **mem_mb:** It's the amount of memory in megabytes per node.
- **cpu_clock:** It's the frequency in gigahertz of the computer's processors.


SALOME installation description
-------------------------------

- **componentList:**
- **applipath:**

Two scenarios with SALOME resource
++++++++++++++++++++++++++++++++++

Use an interactive resource
---------------------------

Use a cluster managed by batch system
-------------------------------------

JOBMANAGER resource management GUI
++++++++++++++++++++++++++++++++++
